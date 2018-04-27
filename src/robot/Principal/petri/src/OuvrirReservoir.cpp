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

extern "C" Petri_actionResult_t OuvrirReservoir_entryInit(VarSlot& _PETRI_PRIVATE_GET_VARIABLES_) {
	if(!_PETRI_PRIVATE_GET_VARIABLES_.isFirstSlot()) {
		_PETRI_PRIVATE_GET_VARIABLES_.pushVariables(0);
	}
	fillVariables(_PETRI_PRIVATE_GET_VARIABLES_);
	return static_cast<actionResult_t>(Petri::Utility::doNothing());
}

extern "C" Petri_actionResult_t OuvrirReservoir_exitAction(VarSlot& _PETRI_PRIVATE_GET_VARIABLES_) {
	auto _PETRI_PRIVATE_EXEC_RESULT_ = static_cast<actionResult_t>(Petri::Utility::doNothing());
	_PETRI_PRIVATE_GET_VARIABLES_.pop();
	_PETRI_PRIVATE_GET_VARIABLES_.pushReturnValues(0);
	return _PETRI_PRIVATE_EXEC_RESULT_;
}

static Petri_actionResult_t state_3_invocation(VarSlot& _PETRI_PRIVATE_GET_VARIABLES_) {
	return static_cast<actionResult_t>(Petri::Utility::doNothing());
}

static Petri_actionResult_t state_4_invocation(VarSlot& _PETRI_PRIVATE_GET_VARIABLES_) {
	return static_cast<actionResult_t>(tournerAbsolu(static_cast<Angle>(45_deg)));
}

static Petri_actionResult_t state_5_invocation(VarSlot& _PETRI_PRIVATE_GET_VARIABLES_) {
	return static_cast<actionResult_t>(ouvrirSouteD());
}

static Petri_actionResult_t state_6_invocation(VarSlot& _PETRI_PRIVATE_GET_VARIABLES_) {
	return static_cast<actionResult_t>(avancerDe(static_cast<Distance>(20_cm)));
}

static Petri_actionResult_t state_7_invocation(VarSlot& _PETRI_PRIVATE_GET_VARIABLES_) {
	return static_cast<actionResult_t>(Petri::Utility::pause(static_cast<std::chrono::milliseconds>(1s)));
}

static Petri_actionResult_t state_8_invocation(VarSlot& _PETRI_PRIVATE_GET_VARIABLES_) {
	return static_cast<actionResult_t>(avancerDe(static_cast<Distance>(15_cm)));
}

static Petri_actionResult_t state_9_invocation(VarSlot& _PETRI_PRIVATE_GET_VARIABLES_) {
	return static_cast<actionResult_t>(avancerDe(static_cast<Distance>(20_cm)));
}

static Petri_actionResult_t state_10_invocation(VarSlot& _PETRI_PRIVATE_GET_VARIABLES_) {
	return static_cast<actionResult_t>(fermerSouteG());
}

static Petri_actionResult_t state_11_invocation(VarSlot& _PETRI_PRIVATE_GET_VARIABLES_) {
	return static_cast<actionResult_t>(tournerAbsolu(static_cast<Angle>(45_deg)));
}

static Petri_actionResult_t state_12_invocation(VarSlot& _PETRI_PRIVATE_GET_VARIABLES_) {
	return static_cast<actionResult_t>(avancerDe(static_cast<Distance>(20_cm)));
}

static bool transition_23_invocation(VarSlot const& _PETRI_PRIVATE_GET_VARIABLES_, Petri_actionResult_t _PETRI_PRIVATE_GET_ACTION_RESULT_) {
	return true;
}

static bool transition_13_invocation(VarSlot const& _PETRI_PRIVATE_GET_VARIABLES_, Petri_actionResult_t _PETRI_PRIVATE_GET_ACTION_RESULT_) {
	return true;
}

static bool transition_14_invocation(VarSlot const& _PETRI_PRIVATE_GET_VARIABLES_, Petri_actionResult_t _PETRI_PRIVATE_GET_ACTION_RESULT_) {
	return true;
}

static bool transition_15_invocation(VarSlot const& _PETRI_PRIVATE_GET_VARIABLES_, Petri_actionResult_t _PETRI_PRIVATE_GET_ACTION_RESULT_) {
	return true;
}

static bool transition_16_invocation(VarSlot const& _PETRI_PRIVATE_GET_VARIABLES_, Petri_actionResult_t _PETRI_PRIVATE_GET_ACTION_RESULT_) {
	return true;
}

static bool transition_17_invocation(VarSlot const& _PETRI_PRIVATE_GET_VARIABLES_, Petri_actionResult_t _PETRI_PRIVATE_GET_ACTION_RESULT_) {
	return true;
}

static bool transition_18_invocation(VarSlot const& _PETRI_PRIVATE_GET_VARIABLES_, Petri_actionResult_t _PETRI_PRIVATE_GET_ACTION_RESULT_) {
	return true;
}

static bool transition_19_invocation(VarSlot const& _PETRI_PRIVATE_GET_VARIABLES_, Petri_actionResult_t _PETRI_PRIVATE_GET_ACTION_RESULT_) {
	return true;
}

static bool transition_20_invocation(VarSlot const& _PETRI_PRIVATE_GET_VARIABLES_, Petri_actionResult_t _PETRI_PRIVATE_GET_ACTION_RESULT_) {
	return true;
}

static bool transition_21_invocation(VarSlot const& _PETRI_PRIVATE_GET_VARIABLES_, Petri_actionResult_t _PETRI_PRIVATE_GET_ACTION_RESULT_) {
	return true;
}

static bool transition_22_invocation(VarSlot const& _PETRI_PRIVATE_GET_VARIABLES_, Petri_actionResult_t _PETRI_PRIVATE_GET_ACTION_RESULT_) {
	return true;
}


extern "C" FillResult OuvrirReservoir_fill(PetriNet& petriNet,
                                           std::uint64_t entitiesOffset,
                                           bool firstLevel,
                                           Petri_actionResult_t (*initEntryPtr)(VarSlot&),
                                           Petri_actionResult_t (*exitActionPtr)(VarSlot&)) {
	auto& state_0 = petriNet.addAction(Action(0 + entitiesOffset, "Root_Entry", initEntryPtr, 0), firstLevel);
	auto& state_2 = petriNet.addAction(Action(2 + entitiesOffset, "Root_End", exitActionPtr, 1), false);
	auto& state_3 = petriNet.addAction(Action(3 + entitiesOffset, "Root_BEGIN", &state_3_invocation, 0), false);
	auto& state_4 = petriNet.addAction(Action(4 + entitiesOffset, "Root_4", &state_4_invocation, 1), false);
	auto& state_5 = petriNet.addAction(Action(5 + entitiesOffset, "Root_5", &state_5_invocation, 1), false);
	auto& state_6 = petriNet.addAction(Action(6 + entitiesOffset, "Root_6", &state_6_invocation, 1), false);
	auto& state_7 = petriNet.addAction(Action(7 + entitiesOffset, "Root_7", &state_7_invocation, 1), false);
	auto& state_8 = petriNet.addAction(Action(8 + entitiesOffset, "Root_3", &state_8_invocation, 1), false);
	auto& state_9 = petriNet.addAction(Action(9 + entitiesOffset, "Root_8", &state_9_invocation, 1), false);
	auto& state_10 = petriNet.addAction(Action(10 + entitiesOffset, "Root_10", &state_10_invocation, 1), false);
	auto& state_11 = petriNet.addAction(Action(11 + entitiesOffset, "Root_11", &state_11_invocation, 1), false);
	auto& state_12 = petriNet.addAction(Action(12 + entitiesOffset, "Root_12", &state_12_invocation, 1), false);


	state_0.addTransition(23 + entitiesOffset, "Root_Entry_to_BEGIN", state_3, &transition_23_invocation);
	state_3.addTransition(13 + entitiesOffset, "Root_15", state_8, &transition_13_invocation);
	state_8.addTransition(14 + entitiesOffset, "Root_16", state_4, &transition_14_invocation);
	state_4.addTransition(15 + entitiesOffset, "Root_17", state_5, &transition_15_invocation);
	state_5.addTransition(16 + entitiesOffset, "Root_18", state_6, &transition_16_invocation);
	state_6.addTransition(17 + entitiesOffset, "Root_19", state_7, &transition_17_invocation);
	state_7.addTransition(18 + entitiesOffset, "Root_20", state_9, &transition_18_invocation);
	state_9.addTransition(19 + entitiesOffset, "Root_21", state_10, &transition_19_invocation);
	state_10.addTransition(20 + entitiesOffset, "Root_22", state_11, &transition_20_invocation);
	state_11.addTransition(21 + entitiesOffset, "Root_23", state_12, &transition_21_invocation);
	state_12.addTransition(22 + entitiesOffset, "Root_24", state_2, &transition_22_invocation);

	return (FillResult){&state_0, &state_2};
}
namespace Petri {
	namespace Generated {
		namespace OuvrirReservoir {
			std::unique_ptr<::Petri::PetriNet> createPetriNet() {
				auto petriNet = std::make_unique<PetriNet>("OuvrirReservoir", 0);
				petriNet->setLogVerbosity(PetriNet::VerbosityNothing);
				OuvrirReservoir_fill(*petriNet, 0, true, &OuvrirReservoir_entryInit, &OuvrirReservoir_exitAction);
				fillVariables(petriNet->variables());
				return petriNet;
			}

			std::unique_ptr<::Petri::PetriDebug> createDebugPetriNet() {
				auto petriNet = std::make_unique<PetriDebug>("OuvrirReservoir", 0);
				petriNet->setLogVerbosity(PetriNet::VerbosityNothing);
				OuvrirReservoir_fill(*petriNet, 0, true, &OuvrirReservoir_entryInit, &OuvrirReservoir_exitAction);
				fillVariables(petriNet->variables());
				return petriNet;
			}
		}
	}
}

extern "C" void* OuvrirReservoir_create() {
	return Petri::Generated::OuvrirReservoir::createPetriNet().release();
}

extern "C" void* OuvrirReservoir_createDebug() {
	return Petri::Generated::OuvrirReservoir::createDebugPetriNet().release();
}

extern "C" char* OuvrirReservoir_evaluate(void* vars, char const* libPath) {
	return Petri::Utility::loadEvaluateAndInvoke(vars, libPath, "OuvrirReservoir");
}
extern "C" char const* OuvrirReservoir_getHash() {
	return "F33AEF3172041E33680DCBEACDB626BEF7F02097416D2E1E0AEFD801A127355A";
}


extern "C" void* OuvrirReservoir_createLibForEditor() {
	return ::Petri::MemberPetriDynamicLib::libForEditor("OuvrirReservoir", 12346);
}