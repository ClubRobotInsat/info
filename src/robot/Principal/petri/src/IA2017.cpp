/*
 * Generated by PetriLab - https://sigilence-technologies.com/petrilab
 * Version 3.0.0-beta1
 */

#include "../Deplacement.h"
#include "../Servo.h"
#include "../Utils.h"
#include <cstdint>
#include <petrilab/Cpp/Action.h>
#include <petrilab/Cpp/MemberPetriDynamicLib.h>
#include <petrilab/Cpp/PetriDebug.h>
#include <petrilab/Cpp/PetriUtils.h>
#include <petrilab/Cpp/VarSlot.h>
#include <petrilab/Cpp/Variable.h>

using namespace Petri;

struct FillResult {
	void* start;
	void* end;
};

static void fillVariables(Petri::VarSlot& variables) {}

extern "C" Petri_actionResult_t IA2017_entryInit(VarSlot& _PETRI_PRIVATE_GET_VARIABLES_) {
	if(!_PETRI_PRIVATE_GET_VARIABLES_.isFirstSlot()) {
		_PETRI_PRIVATE_GET_VARIABLES_.pushVariables(0);
	}
	fillVariables(_PETRI_PRIVATE_GET_VARIABLES_);
	return static_cast<actionResult_t>(Petri::Utility::doNothing());
}

extern "C" Petri_actionResult_t IA2017_exitAction(VarSlot& _PETRI_PRIVATE_GET_VARIABLES_) {
	auto _PETRI_PRIVATE_EXEC_RESULT_ = static_cast<actionResult_t>(Petri::Utility::doNothing());
	_PETRI_PRIVATE_GET_VARIABLES_.pop();
	_PETRI_PRIVATE_GET_VARIABLES_.pushReturnValues(0);
	return _PETRI_PRIVATE_EXEC_RESULT_;
}


extern "C" FillResult IA2017_fill(PetriNet& petriNet,
                                  std::uint64_t entitiesOffset,
                                  bool firstLevel,
                                  Petri_actionResult_t (*initEntryPtr)(VarSlot&),
                                  Petri_actionResult_t (*exitActionPtr)(VarSlot&)) {
	auto& state_0 = petriNet.addAction(Action(0 + entitiesOffset, "Root_Entry", initEntryPtr, 0), firstLevel);
	auto& state_2 = petriNet.addAction(Action(2 + entitiesOffset, "Root_End", exitActionPtr, 0), false);


	return (FillResult){&state_0, &state_2};
}
namespace Petri {
	namespace Generated {
		namespace IA2017 {
			std::unique_ptr<::Petri::PetriNet> createPetriNet() {
				auto petriNet = std::make_unique<PetriNet>("IA2017", 0);
				petriNet->setLogVerbosity(PetriNet::VerbosityNothing);
				IA2017_fill(*petriNet, 0, true, &IA2017_entryInit, &IA2017_exitAction);
				fillVariables(petriNet->variables());
				return petriNet;
			}

			std::unique_ptr<::Petri::PetriDebug> createDebugPetriNet() {
				auto petriNet = std::make_unique<PetriDebug>("IA2017", 0);
				petriNet->setLogVerbosity(PetriNet::VerbosityNothing);
				IA2017_fill(*petriNet, 0, true, &IA2017_entryInit, &IA2017_exitAction);
				fillVariables(petriNet->variables());
				return petriNet;
			}
		}
	}
}

extern "C" void* IA2017_create() {
	return Petri::Generated::IA2017::createPetriNet().release();
}

extern "C" void* IA2017_createDebug() {
	return Petri::Generated::IA2017::createDebugPetriNet().release();
}

extern "C" char* IA2017_evaluate(void* vars, char const* libPath) {
	return Petri::Utility::loadEvaluateAndInvoke(vars, libPath, "IA2017");
}
extern "C" char const* IA2017_getHash() {
	return "BFA70A004A4BB423F4D26C975465633E7B1498A38E40C43BF238F27DEFC4EEC1";
}


extern "C" void* IA2017_createLibForEditor() {
	return ::Petri::MemberPetriDynamicLib::libForEditor("IA2017", 12346);
}
