#include <algorithm>

// not used but prevents compilation errors with cuda 7 RC
#include <thrust/transform.h>

#include "collision/ChCCollisionModel.h"
#include "chrono_parallel/math/ChParallelMath.h"
#include "chrono_parallel/collision/ChCNarrowphaseDispatch.h"
#include "chrono_parallel/collision/ChCNarrowphaseMPRUtils.h"
#include "chrono_parallel/collision/ChCNarrowphaseMPR.h"
#include "chrono_parallel/collision/ChCNarrowphaseR.h"

namespace chrono {
namespace collision {

void ChCNarrowphaseDispatch::Process() {
  //======== Collision output data for rigid contacts
  custom_vector<real3>& norm_data = data_container->host_data.norm_rigid_rigid;
  custom_vector<real3>& cpta_data = data_container->host_data.cpta_rigid_rigid;
  custom_vector<real3>& cptb_data = data_container->host_data.cptb_rigid_rigid;
  custom_vector<real>& dpth_data = data_container->host_data.dpth_rigid_rigid;
  custom_vector<real>& erad_data = data_container->host_data.erad_rigid_rigid;
  custom_vector<int2>& bids_data = data_container->host_data.bids_rigid_rigid;

  //======== Body state information
  custom_vector<bool>& obj_active = data_container->host_data.active_data;
  custom_vector<real3>& body_pos = data_container->host_data.pos_data;
  custom_vector<real4>& body_rot = data_container->host_data.rot_data;
  //======== Broadphase information
  custom_vector<long long>& potentialCollisions = data_container->host_data.pair_rigid_rigid;
  //======== Indexing variables and other information
  collision_envelope = data_container->settings.collision.collision_envelope;
  uint& number_of_contacts = data_container->num_contacts;
  narrowphase_algorithm = data_container->settings.collision.narrowphase_algorithm;
  system_type = data_container->settings.system_type;
  // The number of possible contacts based on the broadphase pair list
  num_potentialCollisions = potentialCollisions.size();

  // Return now if no potential collisions.
  if (num_potentialCollisions == 0) {
    norm_data.resize(0);
    cpta_data.resize(0);
    cptb_data.resize(0);
    dpth_data.resize(0);
    erad_data.resize(0);
    bids_data.resize(0);
    number_of_contacts = 0;
    return;
  }

  obj_data_A_global = data_container->host_data.ObA_rigid;
  obj_data_B_global = data_container->host_data.ObB_rigid;
  obj_data_C_global = data_container->host_data.ObC_rigid;
  obj_data_R_global = data_container->host_data.ObR_rigid;
  // Transform to global coordinate system
  PreprocessLocalToParent();

  contact_index.resize(num_potentialCollisions);

  // Count Number of Contacts
  PreprocessCount();
  // scan to find starting index
  int num_potentialContacts = contact_index.back();
  thrust::exclusive_scan(thrust_parallel, contact_index.begin(), contact_index.end(), contact_index.begin());
  num_potentialContacts += contact_index.back();

  // This counter will keep track of which pairs are actually in contact
  contact_active.resize(num_potentialContacts);
  // Fill the counter with 1, if the contact is active set the value to zero
  // POSSIBLE PERF IMPROVEMENT:, use bool for this?
  thrust::fill(contact_active.begin(), contact_active.end(), 1);
  // Create storage to hold maximum number of contacts in worse case
  norm_data.resize(num_potentialContacts);
  cpta_data.resize(num_potentialContacts);
  cptb_data.resize(num_potentialContacts);
  dpth_data.resize(num_potentialContacts);
  erad_data.resize(num_potentialContacts);
  bids_data.resize(num_potentialContacts);

  Dispatch();

  number_of_contacts = num_potentialContacts - thrust::count(contact_active.begin(), contact_active.end(), 1);

  // remove any entries where the counter is equal to one, these are contacts that do not exist
  thrust::remove_if(norm_data.begin(), norm_data.end(), contact_active.begin(), thrust::identity<int>());
  thrust::remove_if(cpta_data.begin(), cpta_data.end(), contact_active.begin(), thrust::identity<int>());
  thrust::remove_if(cptb_data.begin(), cptb_data.end(), contact_active.begin(), thrust::identity<int>());
  thrust::remove_if(dpth_data.begin(), dpth_data.end(), contact_active.begin(), thrust::identity<int>());
  thrust::remove_if(erad_data.begin(), erad_data.end(), contact_active.begin(), thrust::identity<int>());
  thrust::remove_if(bids_data.begin(), bids_data.end(), contact_active.begin(), thrust::identity<int>());
  thrust::remove_if(potentialCollisions.begin(), potentialCollisions.end(), contact_active.begin(), thrust::identity<int>());
  // Resize all lists so that we don't access invalid contacts
  potentialCollisions.resize(number_of_contacts);
  norm_data.resize(number_of_contacts);
  cpta_data.resize(number_of_contacts);
  cptb_data.resize(number_of_contacts);
  dpth_data.resize(number_of_contacts);
  erad_data.resize(number_of_contacts);
  bids_data.resize(number_of_contacts);
  data_container->erad_is_set = true;

  // std::cout << num_potentialContacts << " " << number_of_contacts << std::endl;
}

void ChCNarrowphaseDispatch::PreprocessCount() {

  // shape type (per shape)
  const shape_type* obj_data_T = data_container->host_data.typ_rigid.data();
  // encoded shape IDs (per collision pair)
  const long long* collision_pair = data_container->host_data.pair_rigid_rigid.data();
  uint* max_contacts = contact_index.data();

#pragma omp parallel for
  for (int index = 0; index < num_potentialCollisions; index++) {

    // Identify the two candidate shapes and get their types.
    int2 pair = I2(int(collision_pair[index] >> 32), int(collision_pair[index] & 0xffffffff));
    shape_type type1 = obj_data_T[pair.x];
    shape_type type2 = obj_data_T[pair.y];

    if (narrowphase_algorithm == NARROWPHASE_MPR) {
      max_contacts[index] = 1;
      continue;
      //} else if (narrowphase_algorithm == NARROWPHASE_GJK) {
      //   max_contacts[index] = 1;
      //   return;
    } else if (narrowphase_algorithm == NARROWPHASE_HYBRID_MPR) {
      max_contacts[index] = 1;
      if (type1 == SPHERE || type2 == SPHERE) {
        max_contacts[index] = 1;
      } else if (type1 == CAPSULE || type2 == CAPSULE) {
        max_contacts[index] = 2;
      }
      continue;
      //} else if (narrowphase_algorithm == NARROWPHASE_HYBRID_GJK) {
      //   max_contacts[index] = 1;
      //   if (type1 == SPHERE || type2 == SPHERE) {
      //      max_contacts[index] = 1;
      //   } else if (type1 == CAPSULE || type2 == CAPSULE) {
      //      max_contacts[index] = 2;
      //   }
    }

    // Set the maximum number of possible contacts for this particular pair
    if (type1 == SPHERE || type2 == SPHERE) {
      max_contacts[index] = 1;
    } else if (type1 == CAPSULE || type2 == CAPSULE) {
      max_contacts[index] = 2;
    } else {
      max_contacts[index] = 4;
    }
  }
}

void ChCNarrowphaseDispatch::PreprocessLocalToParent() {

  const uint& num_shapes = data_container->num_shapes;
  const shape_type* obj_data_T = data_container->host_data.typ_rigid.data();

  custom_vector<real3>& obj_data_A = data_container->host_data.ObA_rigid;
  custom_vector<real3>& obj_data_B = data_container->host_data.ObB_rigid;
  custom_vector<real3>& obj_data_C = data_container->host_data.ObC_rigid;
  custom_vector<real4>& obj_data_R = data_container->host_data.ObR_rigid;
  custom_vector<uint>& obj_data_ID = data_container->host_data.id_rigid;

  custom_vector<real3>& body_pos = data_container->host_data.pos_data;
  custom_vector<real4>& body_rot = data_container->host_data.rot_data;

#pragma omp parallel for
  for (int index = 0; index < num_shapes; index++) {

    shape_type T = obj_data_T[index];

    // Get the identifier for the object associated with this collision shape
    uint ID = obj_data_ID[index];

    real3 pos = body_pos[ID];    // Get the global object position
    real4 rot = body_rot[ID];    // Get the global object rotation

    obj_data_A_global[index] = TransformLocalToParent(pos, rot, obj_data_A[index]);
    if (T == TRIANGLEMESH) {
      obj_data_B_global[index] = TransformLocalToParent(pos, rot, obj_data_B[index]);
      obj_data_C_global[index] = TransformLocalToParent(pos, rot, obj_data_C[index]);
    }
    obj_data_R_global[index] = mult(rot, obj_data_R[index]);
  }
}

void ChCNarrowphaseDispatch::Dispatch_Init(uint index, uint& icoll, uint& ID_A, uint& ID_B, ConvexShape& shapeA, ConvexShape& shapeB) {

  const shape_type* obj_data_T = data_container->host_data.typ_rigid.data();
  const custom_vector<uint>& obj_data_ID = data_container->host_data.id_rigid;
  const custom_vector<long long>& contact_pair = data_container->host_data.pair_rigid_rigid;

  real3* convex_data = data_container->host_data.convex_data.data();

  long long p = contact_pair[index];
  int2 pair = I2(int(p >> 32), int(p & 0xffffffff));    // Get the identifiers for the two shapes involved in this collision

  ID_A = obj_data_ID[pair.x];
  ID_B = obj_data_ID[pair.y];    // Get the identifiers of the two associated objects (bodies)

  shapeA.type = obj_data_T[pair.x];
  shapeB.type = obj_data_T[pair.y];    // Load the type data for each object in the collision pair

  shapeA.A = obj_data_A_global[pair.x];
  shapeB.A = obj_data_A_global[pair.y];
  shapeA.B = obj_data_B_global[pair.x];
  shapeB.B = obj_data_B_global[pair.y];
  shapeA.C = obj_data_C_global[pair.x];
  shapeB.C = obj_data_C_global[pair.y];
  shapeA.R = obj_data_R_global[pair.x];
  shapeB.R = obj_data_R_global[pair.y];
  shapeA.convex = convex_data;
  shapeB.convex = convex_data;

  //// TODO: what is the best way to dispatch this?
  icoll = contact_index[index];
}

void ChCNarrowphaseDispatch::Dispatch_Finalize(uint icoll, uint ID_A, uint ID_B, int nC) {

  custom_vector<int2>& body_ids = data_container->host_data.bids_rigid_rigid;

  // Mark the active contacts and set their body IDs
  for (int i = 0; i < nC; i++) {
    contact_active[icoll + i] = 0;
    body_ids[icoll + i] = I2(ID_A, ID_B);
  }
}

void ChCNarrowphaseDispatch::DispatchMPR() {

  custom_vector<real3>& norm = data_container->host_data.norm_rigid_rigid;
  custom_vector<real3>& ptA = data_container->host_data.cpta_rigid_rigid;
  custom_vector<real3>& ptB = data_container->host_data.cptb_rigid_rigid;
  custom_vector<real>& contactDepth = data_container->host_data.dpth_rigid_rigid;
  custom_vector<real>& effective_radius = data_container->host_data.erad_rigid_rigid;

#pragma omp parallel for
  for (int index = 0; index < num_potentialCollisions; index++) {
    uint ID_A, ID_B, icoll;
    ConvexShape shapeA, shapeB;

    Dispatch_Init(index, icoll, ID_A, ID_B, shapeA, shapeB);

    if (MPRCollision(shapeA, shapeB, collision_envelope, norm[icoll], ptA[icoll], ptB[icoll], contactDepth[icoll])) {
      effective_radius[icoll] = edge_radius;
      // The number of contacts reported by MPR is always 1.
      Dispatch_Finalize(icoll, ID_A, ID_B, 1);
    }
  }
}

void ChCNarrowphaseDispatch::DispatchR() {

  real3* norm = data_container->host_data.norm_rigid_rigid.data();
  real3* ptA = data_container->host_data.cpta_rigid_rigid.data();
  real3* ptB = data_container->host_data.cptb_rigid_rigid.data();
  real* contactDepth = data_container->host_data.dpth_rigid_rigid.data();
  real* effective_radius = data_container->host_data.erad_rigid_rigid.data();

#pragma omp parallel for
  for (int index = 0; index < num_potentialCollisions; index++) {
    uint ID_A, ID_B, icoll;
    ConvexShape shapeA, shapeB;
    int nC;

    Dispatch_Init(index, icoll, ID_A, ID_B, shapeA, shapeB);

    if (RCollision(shapeA, shapeB, 2 * collision_envelope, &norm[icoll], &ptA[icoll], &ptB[icoll], &contactDepth[icoll], &effective_radius[icoll], nC)) {
      Dispatch_Finalize(icoll, ID_A, ID_B, nC);
    }
  }
}

void ChCNarrowphaseDispatch::DispatchHybridMPR() {

  real3* norm = data_container->host_data.norm_rigid_rigid.data();
  real3* ptA = data_container->host_data.cpta_rigid_rigid.data();
  real3* ptB = data_container->host_data.cptb_rigid_rigid.data();
  real* contactDepth = data_container->host_data.dpth_rigid_rigid.data();
  real* effective_radius = data_container->host_data.erad_rigid_rigid.data();

#pragma omp parallel for
  for (int index = 0; index < num_potentialCollisions; index++) {
    uint ID_A, ID_B, icoll;
    ConvexShape shapeA, shapeB;
    int nC;

    Dispatch_Init(index, icoll, ID_A, ID_B, shapeA, shapeB);

    if (RCollision(shapeA, shapeB, 2 * collision_envelope, &norm[icoll], &ptA[icoll], &ptB[icoll], &contactDepth[icoll], &effective_radius[icoll], nC)) {
      Dispatch_Finalize(icoll, ID_A, ID_B, nC);
    } else if (MPRCollision(shapeA, shapeB, collision_envelope, norm[icoll], ptA[icoll], ptB[icoll], contactDepth[icoll])) {
      effective_radius[icoll] = edge_radius;
      Dispatch_Finalize(icoll, ID_A, ID_B, 1);
    }
  }
}

void ChCNarrowphaseDispatch::Dispatch() {

  switch (narrowphase_algorithm) {
    case NARROWPHASE_MPR:
      DispatchMPR();
      break;
    case NARROWPHASE_R:
      DispatchR();
      break;
    case NARROWPHASE_HYBRID_MPR:
      DispatchHybridMPR();
      break;
  }
}

} // end namespace collision
} // end namespace chrono
