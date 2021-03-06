//
// Created by terae on 27/01/18.
//

#ifndef ROOT_ACTION_H
#define ROOT_ACTION_H

#include "../../../Principal/librobot/libRobot.h"
#include "../../Principal/petri/src/IA2018.h"
#include "Element.hpp"

// TODO: include generated petri file (as "info/src/robot/Principal/petri/IA2017.h")
//#include <memory>
#include <petrilab/Cpp/MemberPetriDynamicLib.h>
#include <petrilab/Cpp/PetriLab.h>

namespace StrategyGenerator {
	class Element;

	/**
	 * @brief Abstraction d'une action d'un match
	 *
	 * Cette classe possède toutes les informations d'une action (points, durée, coords de départ, nom)
	 * Lorsque l'acion est choisie pour être exécutée, elle lance le réseau pétri avec les bons arguments
	 *
	 * @code
	 * // Ici, l'élément "bee" apparaît après l'appel de la fonction "cube"
	 * Element next_element(ElementType::BEE, Coordonnees({1_m, 1_m}, 90_deg));
	 * Action action(20_s, 30, Element(ElementType::CUBE, Coordonnees({50_cm, 1_m}), {next_element}, "cube");
	 * // La durée d'exécution totale d'une action vaut la durée du petri + du déplacement (à vol d'oiseau)
	 * assert(action.get_execution_time(Coordonnees({50_cm, 0_m}) == 20_s + 1_m / LINEAR_SPEED_DEFAULT);
	 *
	 * _dep.allerA(action.get_coordonnees().getPos2D());
	 * _dep.tournerAbsolu(action.get_coordonnees().getAngle());
	 *
	 * // L'action va finir 5_s en avance (fin du chrono par exemple)
	 * action.execute_petri(..., 5_s);
	 * std::cout << "L'action '" << action.get_name() << "' s'est temrinée." << std::endl;
	 * @endcode
	 */
	class Action {
	protected:
		// Execution time since the robot is at the good position & in the good angle (approximation)
		Duration _execution_time;
		// TODO: allow user to calculate the point while action is performing?
		int _nr_points;

		Element _element;

		// return a list of all new `Element`s generated by this action
		// return an empty list if the action is the last one
		std::vector<Element> _elements_created_after_call;

		std::string _name;

	public:
		Action(Duration time, int points, Element element, std::vector<Element> elements_created_after_call)
		        : _execution_time(time)
		        , _nr_points(points)
		        , _element(element)
		        , _elements_created_after_call(std::move(elements_created_after_call))
		        , _name(toString(element.get_type())) {}

		inline int get_nr_points() const {
			return _nr_points;
		}

		inline repere::Coordonnees get_coordonnees() const {
			return _element.get_coordonnees();
		}

		inline Duration get_execution_time(const repere::Coordonnees& initial_coords) const {
			Distance dist{Distance::makeFromCm(
			    std::sqrt(std::pow(initial_coords.getX().toCm() - _element.get_coordonnees().getX().toCm(), 2) +
			              std::pow(initial_coords.getY().toCm() - _element.get_coordonnees().getY().toCm(), 2)))};
			return _execution_time + dist / Constantes::LINEAR_SPEED_DEFAULT;
		}

		inline std::vector<Element> get_next_elements() const {
			return _elements_created_after_call;
		}

		virtual void execute_petri(Petri::PetriNet& petri, Duration timeout) const {
			// FIXME : impossible d'appeler 'petri', il y a une segfault
			logDebug("MagicStrategy::Action::execute_petri called for action ", _name, " with timeout = ", timeout);
			std::cout << &petri << std::endl;
			StopWatch start;
			petri.variables()[Petri::Generated::IA2018::Petri_Param_Enum_Vaction].value() =
			    static_cast<std::int64_t>(_element.get_type());
			petri.run();

			while(petri.running() && start.getElapsedTime() < timeout) {
				sleep(50_ms);
			}

			petri.stop();
		}

		std::string get_name() const {
			return _name;
		}

		Element get_associated_element() const {
			return _element;
		}

		friend bool operator==(const Action& a1, const Action& a2) {
			return a1._element.get_coordonnees() == a2._element.get_coordonnees() && a1.get_nr_points() == a2.get_nr_points() &&
			       a1.get_next_elements() == a2.get_next_elements() && a1._execution_time == a2._execution_time;
		}

		friend bool operator!=(const Action& a1, const Action& a2) {
			return !(a1 == a2);
		}

		friend std::ostream& operator<<(std::ostream& os, const Action& action) {
			return os << action._name;
		}
	};

	class ActionWait : public Action {
	public:
		explicit ActionWait(Duration time)
		        : Action(time, 0, Element(ElementType::NOTHING, repere::Coordonnees()), {}) {}

		void execute_petri(Petri::PetriNet&, Duration remainingTime) const override {
			logDebug("MagicStrategy::Action::execute_petri called for ActionWait(", remainingTime, ")");
			sleep(std::min(_execution_time, remainingTime));
		}
	};
} // namespace StrategyGenerator

#endif // ROOT_ACTION_H
