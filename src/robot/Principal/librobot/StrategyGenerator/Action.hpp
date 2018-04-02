//
// Created by terae on 27/01/18.
//

#ifndef ROOT_ACTION_H
#define ROOT_ACTION_H

//#include "Table.hpp"
#include "../../librobot/libRobot.h"
#include "Element.hpp"

// TODO: include generated petri file (as "info/src/robot/Principal/petri/IA2017.h")
//#include <memory>
//#include <petri/Runtime/Cpp/MemberPetriDynamicLib.h>

namespace StrategyGenerator {
	class Element;

	/**
	* @brief Abstraction of an action in the Cup
	*/
	class Action {
	protected:
		// Execution time since the robot is at the good position & in the good angle (approximation)
		Duration _execution_time;
		// TODO: allow user to calculate the point while action is performing?
		int _nr_points;

		// in order to call AllerA() + TournerAbsolu() functions
		Vector2m _start_position;
		Angle _start_angle;

		// return a list of all new `Element`s generated by this action
		// return an empty list if the action is the last one
		std::vector<Element> _elements_created_after_call;

		// Associated Petri network to launch
		// std::shared_ptr<Petri::PetriDynamicLib> _petri;

	public:
		Action() : Action(0_s, 0, {0_m, 0_m}, 0_deg, {}) {}

		Action(Duration time, int points, Vector2m pos, Angle angle, std::vector<Element> elements_created_after_call /*, std::shared_ptr<Petri::PetriDynamicLib> petri*/)
		        : _execution_time(time)
		        , _nr_points(points)
		        , _start_position(pos)
		        , _start_angle(angle)
		        , _elements_created_after_call(std::move(elements_created_after_call)) /*, _petri(petri)*/ {}

		inline int get_nr_points() const {
			return _nr_points;
		}

		inline Vector2m get_start_position() const {
			return _start_position;
		}

		inline Angle get_start_angle() const {
			return _start_angle;
		}

		// TODO: add deplacement time
		inline Duration get_execution_time(Vector2m initial_pos) const {
			Distance dist{Distance::makeFromCm(std::sqrt(std::pow(initial_pos.x.toCm() - _start_position.x.toCm(), 2) +
			                                             std::pow(initial_pos.y.toCm() - _start_position.y.toCm(), 2)))};
			return _execution_time + dist / 10_cm_s;
		}

		inline std::vector<Element> get_next_elements() const {
			return _elements_created_after_call;
		}

		virtual void execute() {
			/*bool debug{true};
			if (debug) {
			    Petri::DebugServer debugSession(*_petri);
			    debugSession.start();
			    debugSession.join();
			} else {
			    auto ia = _petri->createPetriNet();
			    ia->run();

			    while (getTempsRestant() > 0_s) {
			        sleep(50_ms);
			    }

			    ia->stop();
			}*/
		}
	};

	class ActionWait : public Action {
	public:
		explicit ActionWait(Duration time) : Action(time, 0, {0_m, 0_m}, 0_deg, {} /*, nullptr*/) {}

		void execute() override {
			sleep(_execution_time);
		}
	};
}

namespace std {
	// namespace StrategyGenerator {
	template <>
	struct numeric_limits<StrategyGenerator::Action> {
		static constexpr bool is_specialized = true;
		static bool has_infinity() {
			return false;
		}
		static StrategyGenerator::Action max() {
			return StrategyGenerator::Action(9000_s,
			                                 std::numeric_limits<int>::max(),
			                                 {5000_m, 5000_m},
			                                 0_deg,
			                                 std::vector<StrategyGenerator::Element>() /*, nullptr*/);
		}
		static StrategyGenerator::Action infinity() {
			return StrategyGenerator::Action(9000_s,
			                                 std::numeric_limits<int>::infinity(),
			                                 {5000_m, 5000_m},
			                                 0_deg,
			                                 std::vector<StrategyGenerator::Element>() /*, nullptr*/);
		}
	};
	//}
}

#endif // ROOT_ACTION_H
