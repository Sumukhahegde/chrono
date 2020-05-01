%{

/* Includes additional C++ in the wrapper code */

#include <string>
#include <vector>

#include "chrono/physics/ChSystem.h"
#include "chrono/physics/ChBodyAuxRef.h"
#include "chrono/physics/ChShaft.h"
#include "chrono/physics/ChShaftsBody.h"
#include "chrono/assets/ChCylinderShape.h"

#include "chrono_vehicle/ChApiVehicle.h"
#include "chrono_vehicle/ChPart.h"

#include "chrono_vehicle/wheeled_vehicle/ChSuspension.h"

#include "chrono_vehicle/wheeled_vehicle/suspension/ChDoubleWishbone.h"
#include "chrono_vehicle/wheeled_vehicle/suspension/ChMacPhersonStrut.h"
#include "chrono_vehicle/wheeled_vehicle/suspension/ChLeafspringAxle.h"
#include "chrono_vehicle/wheeled_vehicle/suspension/ChHendricksonPRIMAXX.h"
#include "chrono_vehicle/wheeled_vehicle/suspension/ChDoubleWishboneReduced.h"
#include "chrono_vehicle/wheeled_vehicle/suspension/ChMultiLink.h"
#include "chrono_vehicle/wheeled_vehicle/suspension/ChRigidPinnedAxle.h"
#include "chrono_vehicle/wheeled_vehicle/suspension/ChSemiTrailingArm.h"
#include "chrono_vehicle/wheeled_vehicle/suspension/ChRigidSuspension.h"
#include "chrono_vehicle/wheeled_vehicle/suspension/ChSolidAxle.h"
#include "chrono_vehicle/wheeled_vehicle/suspension/ChThreeLinkIRS.h"
#include "chrono_vehicle/wheeled_vehicle/suspension/ChToeBarLeafspringAxle.h"
#include "chrono_vehicle/wheeled_vehicle/suspension/ChSolidBellcrankThreeLinkAxle.h"
#include "chrono_vehicle/wheeled_vehicle/suspension/ChSolidThreeLinkAxle.h"

#include "chrono_vehicle/wheeled_vehicle/suspension/DoubleWishbone.h"
#include "chrono_vehicle/wheeled_vehicle/suspension/DoubleWishboneReduced.h"
#include "chrono_vehicle/wheeled_vehicle/suspension/HendricksonPRIMAXX.h"
#include "chrono_vehicle/wheeled_vehicle/suspension/LeafspringAxle.h"
#include "chrono_vehicle/wheeled_vehicle/suspension/MacPhersonStrut.h"
#include "chrono_vehicle/wheeled_vehicle/suspension/MultiLink.h"
#include "chrono_vehicle/wheeled_vehicle/suspension/SemiTrailingArm.h"
#include "chrono_vehicle/wheeled_vehicle/suspension/SolidAxle.h"
#include "chrono_vehicle/wheeled_vehicle/suspension/ThreeLinkIRS.h"
#include "chrono_vehicle/wheeled_vehicle/suspension/ToeBarLeafspringAxle.h"
#include "chrono_vehicle/wheeled_vehicle/suspension/SolidBellcrankThreeLinkAxle.h"
#include "chrono_vehicle/wheeled_vehicle/suspension/SolidThreeLinkAxle.h"

#include "chrono_thirdparty/rapidjson/document.h"

%}


%shared_ptr(chrono::vehicle::ChSuspension)
%shared_ptr(chrono::vehicle::ChDoubleWishbone)
%shared_ptr(chrono::vehicle::ChMacPhersonStrut)
%shared_ptr(chrono::vehicle::MacPhersonStrut)
%shared_ptr(chrono::vehicle::ChLeafspringAxle)
%shared_ptr(chrono::vehicle::LeafspringAxle)
%shared_ptr(chrono::vehicle::ChHendricksonPRIMAXX)
%shared_ptr(chrono::vehicle::ChDoubleWishboneReduced)
%shared_ptr(chrono::vehicle::ChMultiLink)
%shared_ptr(chrono::vehicle::MultiLink)
%shared_ptr(chrono::vehicle::ChRigidPinnedAxle)
%shared_ptr(chrono::vehicle::ChSemiTrailingArm)
%shared_ptr(chrono::vehicle::SemiTrailingArm)
%shared_ptr(chrono::vehicle::ChRigidSuspension)
%shared_ptr(chrono::vehicle::ChSolidAxle)
%shared_ptr(chrono::vehicle::ChThreeLinkIRS)
%shared_ptr(chrono::vehicle::ChToeBarLeafspringAxle)
%shared_ptr(chrono::vehicle::DoubleWishbone)
%shared_ptr(chrono::vehicle::DoubleWishboneReduced)
%shared_ptr(chrono::vehicle::HendricksonPRIMAXX)
%shared_ptr(chrono::vehicle::SolidAxle)
%shared_ptr(chrono::vehicle::ThreeLinkIRS)
%shared_ptr(chrono::vehicle::ToeBarLeafspringAxle)
%shared_ptr(chrono::vehicle::ChSolidBellcrankThreeLinkAxle)
%shared_ptr(chrono::vehicle::ChSolidThreeLinkAxle)
%shared_ptr(chrono::vehicle::SolidBellcrankThreeLinkAxle)
%shared_ptr(chrono::vehicle::SolidThreeLinkAxle)



%import(module = "pychrono.core") "chrono_python/core/ChShaft.i"
%import "../../chrono_vehicle/ChPart.h"


//%shared_ptr(chrono::vehicle::RigidTerrain::Patch)

/* Parse the header file to generate wrappers */
%include "../../chrono_vehicle/wheeled_vehicle/ChSuspension.h"
%ignore chrono::vehicle::ChDoubleWishbone::getLocation;
%include "../../chrono_vehicle/wheeled_vehicle/suspension/ChDoubleWishbone.h"
%include "../../chrono_vehicle/wheeled_vehicle/suspension/ChMacPhersonStrut.h"
%include "../../chrono_vehicle/wheeled_vehicle/suspension/ChLeafspringAxle.h"
%include "../../chrono_vehicle/wheeled_vehicle/suspension/ChHendricksonPRIMAXX.h"
%include "../../chrono_vehicle/wheeled_vehicle/suspension/ChDoubleWishboneReduced.h"
%include "../../chrono_vehicle/wheeled_vehicle/suspension/ChMultiLink.h"
%ignore chrono::vehicle::ChRigidPinnedAxle::getLocation;
%include "../../chrono_vehicle/wheeled_vehicle/suspension/ChRigidPinnedAxle.h"
%include "../../chrono_vehicle/wheeled_vehicle/suspension/ChSemiTrailingArm.h"
%ignore chrono::vehicle::ChRigidSuspension::getLocation;
%include "../../chrono_vehicle/wheeled_vehicle/suspension/ChRigidSuspension.h"
%include "../../chrono_vehicle/wheeled_vehicle/suspension/ChSolidAxle.h"
%include "../../chrono_vehicle/wheeled_vehicle/suspension/ChThreeLinkIRS.h"
%include "../../chrono_vehicle/wheeled_vehicle/suspension/ChToeBarLeafspringAxle.h"
%include "../../chrono_vehicle/wheeled_vehicle/suspension/ChSolidBellcrankThreeLinkAxle.h"
%include "../../chrono_vehicle/wheeled_vehicle/suspension/ChSolidThreeLinkAxle.h"

%include "../../chrono_vehicle/wheeled_vehicle/suspension/DoubleWishbone.h"
%include "../../chrono_vehicle/wheeled_vehicle/suspension/DoubleWishboneReduced.h"
%include "../../chrono_vehicle/wheeled_vehicle/suspension/HendricksonPRIMAXX.h"
%include "../../chrono_vehicle/wheeled_vehicle/suspension/LeafspringAxle.h"
%include "../../chrono_vehicle/wheeled_vehicle/suspension/MacPhersonStrut.h"
%include "../../chrono_vehicle/wheeled_vehicle/suspension/MultiLink.h"
%include "../../chrono_vehicle/wheeled_vehicle/suspension/SemiTrailingArm.h"
%include "../../chrono_vehicle/wheeled_vehicle/suspension/SolidAxle.h"
%include "../../chrono_vehicle/wheeled_vehicle/suspension/ThreeLinkIRS.h"
%include "../../chrono_vehicle/wheeled_vehicle/suspension/ToeBarLeafspringAxle.h"
%include "../../chrono_vehicle/wheeled_vehicle/suspension/SolidBellcrankThreeLinkAxle.h"
%include "../../chrono_vehicle/wheeled_vehicle/suspension/SolidThreeLinkAxle.h"

%include "chrono_python/models/SuspensionModels.i"


//
// C- DOWNCASTING OF SHARED POINTERS
//
%DefSharedPtrDynamicDowncast(chrono::vehicle,ChSuspension, ChDoubleWishbone)
%DefSharedPtrDynamicDowncast(chrono::vehicle,ChSuspension, ChMacPhersonStrut)
//%DefSharedPtrDynamicDowncast(chrono::vehicle,ChSuspension, MacPhersonStrut)
%DefSharedPtrDynamicDowncast(chrono::vehicle,ChSuspension, ChLeafspringAxle)
//%DefSharedPtrDynamicDowncast(chrono::vehicle,ChSuspension, LeafspringAxle)
%DefSharedPtrDynamicDowncast(chrono::vehicle,ChSuspension, ChHendricksonPRIMAXX)
%DefSharedPtrDynamicDowncast(chrono::vehicle,ChSuspension, ChDoubleWishboneReduced)
%DefSharedPtrDynamicDowncast(chrono::vehicle,ChSuspension, ChMultiLink)
//%DefSharedPtrDynamicDowncast(chrono::vehicle,ChSuspension, MultiLink)
%DefSharedPtrDynamicDowncast(chrono::vehicle,ChSuspension, ChRigidPinnedAxle)
%DefSharedPtrDynamicDowncast(chrono::vehicle,ChSuspension, ChSemiTrailingArm)
//%DefSharedPtrDynamicDowncast(chrono::vehicle,ChSuspension, SemiTrailingArm)
%DefSharedPtrDynamicDowncast(chrono::vehicle,ChSuspension, ChRigidSuspension)
%DefSharedPtrDynamicDowncast(chrono::vehicle,ChSuspension, ChSolidAxle)
%DefSharedPtrDynamicDowncast(chrono::vehicle,ChSuspension, ChThreeLinkIRS)
%DefSharedPtrDynamicDowncast(chrono::vehicle,ChSuspension, ChToeBarLeafspringAxle)
//%DefSharedPtrDynamicDowncast(chrono::vehicle,ChSuspension, DoubleWishbone)
//%DefSharedPtrDynamicDowncast(chrono::vehicle,ChSuspension, DoubleWishboneReduced)
//%DefSharedPtrDynamicDowncast(chrono::vehicle,ChSuspension, HendricksonPRIMAXX)
//%DefSharedPtrDynamicDowncast(chrono::vehicle,ChSuspension, SolidAxle)
//%DefSharedPtrDynamicDowncast(chrono::vehicle,ChSuspension, ThreeLinkIRS)
//%DefSharedPtrDynamicDowncast(chrono::vehicle,ChSuspension, ToeBarLeafspringAxle)
%DefSharedPtrDynamicDowncast(chrono::vehicle,ChSuspension, ChSolidBellcrankThreeLinkAxle)
%DefSharedPtrDynamicDowncast(chrono::vehicle,ChSuspension, ChSolidThreeLinkAxle)
//%DefSharedPtrDynamicDowncast(chrono::vehicle,ChSuspension, SolidBellcrankThreeLinkAxle)
//%DefSharedPtrDynamicDowncast(chrono::vehicle,ChSuspension, SolidThreeLinkAxle)