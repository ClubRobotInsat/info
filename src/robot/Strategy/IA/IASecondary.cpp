//
// Created by terae on 20/05/19.
//

#include "IASecondary.h"
#include "../PetriLab/src/Secondary.h"
#include <petrilab/Cpp/PetriLab.h>

int main(int argc, char* argv[]) {
	Log::open(argc, argv, false);

	Strategy::IASecondary strategy(Constants::RobotColor::Yellow);

	strategy.start(strategy.debug_mode ? 1_h : GLOBAL_CONSTANTS().get_match_duration());

	return EXIT_SUCCESS;
}

namespace Strategy {
	IASecondary::IASecondary(Constants::RobotColor color) : AbstractStrategy(color, "secondary") {
		if(debug_mode) {
			logInfo("Initialization of the physical robot '", name, "'");
		}

		auto physical_robot = std::make_shared<PhysicalRobot::Robot>(name, true);
		add_robot(physical_robot);

		_petrilab = Petri::Generated::Secondary::createLib(".");
		_petrilab->load();

		logDebug0("Tirette");
		if(!debug_mode) {
			if(get_robot()->get_robot()->get_communication_protocol_type() == typeid(Communication::protocol_local)) {
				// Attente en mode simu
				std::cout << "Local mode: press a key." << std::endl;
				getchar();
			} else {
				wait_for_tirette();
			}
		}
	}

	std::shared_ptr<Interfacer::RobotManager> IASecondary::get_robot() const {
		return _interfacers.find(name)->second;
	}

	void IASecondary::execute() {
		launch_petrilab(debug_mode);
	}

	void IASecondary::funny_action() {
		logDebug0("Funny action here!");
	}

	ActionResult IASecondary::launch_petrilab(bool debug) {
		if(debug) {
			Petri::DebugServer debug_session(*_petrilab);
			debug_session.start();
			debug_session.join();
		} else {
			auto ia = _petrilab->createPetriNet();
			ia->run();

			// Garde fou pour les 100s de match
			while(get_left_time() > 0_ms) {
				sleep(50_ms);
			}

			ia->stop();
		}
		return ActionResult::SUCCESS;
	}
} // namespace Strategy
