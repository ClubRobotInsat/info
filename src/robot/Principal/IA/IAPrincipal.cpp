#include "IAPrincipal.h"
#include "../petri/Deplacement.h"
#include "../petri/Servo.h"
#include "../petri/src/IA2018.h"
#include "Parsing.h"
#include <petrilab/Cpp/PetriLab.h>

#define CHECK_LIST 0
int main(int argc, char* argv[]) {

	// Couleur du robot
	RobotColor color = ia_parsing::parseColor(argc, argv);
	bool debugMode = ia_parsing::getDebugMode(argc, argv);

	Log::open(argc, argv, false);

	logDebug6("Initialisation du robot...Veuillez patienter...");
	IAPrincipal strategie({argv, argv + argc}, color, debugMode);

	logDebug6("Démarre la stratégie");
	if(!debugMode) {
		strategie.demarrer(MATCH_DURATION);
	} else {
		strategie.demarrer(MATCH_DURATION * 100);
	}
	logDebug9("fin initialisation");
	return 0;
}

IAPrincipal::IAPrincipal(std::vector<std::string> const& args, Constantes::RobotColor color, bool debugMode)
        : StrategiePrincipal(args, color), _debugMode(debugMode) {
	// main --> IAPrincipal() --> initialisation --> attendre tirette -> démarrer
	this->initialisation();
#ifdef HOMOLOGATION
	_petri = Petri::Generated::Homologation_createLib(".");
#else
	_petri = Petri::Generated::IA2018::createLib(".");
#endif
	_petri->load();
}

void IAPrincipal::initialisation() {
#if CHECK_LIST
	logCyan("-----------------------");
	logCyan("Carte pinguées ?");
	getchar();
	logCyan("La tourelle tourne-t-elle ? ");
	getchar();
	logCyan("Cadre recalages placés ?");
	getchar();
	logCyan("Couleur robot : ", this->getCouleurEquipe());
	getchar();
	logCyan("-----------------------");
#endif
	//_robot->getCAN().setDebug(true);
	// Recallage du robot (manuel)
	// Test pour voir si on a tout ping
	logDebug9("Initialisation");

	if(!_debugMode) {
		// se mettre en position de départ : sortirBrasGaucheMax(); ...

		//		sortirParasol();
		//		sleep(50_ms);
		//		rentrerParasol();
	}


	logCyan("Couleur robot : ", this->getCouleurEquipe());
	logDebug("Recalage du robot");

	const repere::Repere& repere_match = (getCouleurEquipe() == RobotColor::Orange ? REFERENCE_ORANGE : REFERENCE_GREEN);
	repere::Coordonnees robot_coords(START_ROBOT_POSITION, START_ROBOT_ANGLE, repere_match);

	getDeplacement().setRepere(robot_coords);


	logDebug("StrategyGenerator");
	// Action definitions
	auto action_bee = [](repere::Coordonnees coords) -> StrategyGenerator::Action {
		return StrategyGenerator::Action(5_s, 50, StrategyGenerator::Element(StrategyGenerator::ElementType::BEE, coords), {}, "bee");
	};

	auto action_cube = [](repere::Coordonnees coords) -> StrategyGenerator::Action {
		return StrategyGenerator::Action(20_s, 30, StrategyGenerator::Element(StrategyGenerator::ElementType::CUBE, coords), {}, "cube");
	};

	auto action_sphere = [&repere_match](repere::Coordonnees coords) -> StrategyGenerator::Action {
		return StrategyGenerator::Action(20_s,
		                                 80,
		                                 StrategyGenerator::Element(StrategyGenerator::ElementType::SPHERE, coords),
		                                 {StrategyGenerator::Element(StrategyGenerator::ElementType::CUBE,
		                                                             Coordonnees({1.5_m, 0_m}, 0_deg, repere_match))},
		                                 "sphere");
	};

	auto action_switch = [](repere::Coordonnees coords) -> StrategyGenerator::Action {
		return StrategyGenerator::Action(3_s, 50, StrategyGenerator::Element(StrategyGenerator::ElementType::SWITCH, coords), {}, "switch");
	};

	// Element definitions
	std::function<bool()> always_possible = []() -> bool { return true; };

	_strategy.associate_element(StrategyGenerator::ElementType::BEE, action_bee, always_possible);
	_strategy.associate_element(StrategyGenerator::ElementType::SWITCH, action_switch, always_possible);
	_strategy.associate_element(StrategyGenerator::ElementType::CUBE, action_cube, always_possible);
	_strategy.associate_element(StrategyGenerator::ElementType::SPHERE, action_sphere, always_possible);


	// Table generation
	StrategyGenerator::Table table;
	table.set_robot_coords(robot_coords);

	table.emplace(std::make_shared<StrategyGenerator::Element>(
	    StrategyGenerator::Element(StrategyGenerator::ElementType::BEE, Coordonnees({1_m, 1.5_m}, 0_deg, repere_match))));
	table.emplace(std::make_shared<StrategyGenerator::Element>(
	    StrategyGenerator::Element(StrategyGenerator::ElementType::SPHERE, Coordonnees({10_cm, 50_cm}, 0_deg, repere_match))));
	table.emplace(std::make_shared<StrategyGenerator::Element>(
	    StrategyGenerator::Element(StrategyGenerator::ElementType::SPHERE, Coordonnees({50_cm, 2_m}, 0_deg, repere_match))));
	table.emplace(std::make_shared<StrategyGenerator::Element>(
	    StrategyGenerator::Element(StrategyGenerator::ElementType::SWITCH, Coordonnees({20_cm, 1.9_m}, 0_deg, repere_match))));
	_strategy.initialize(table);


/// Recalage
/*_dep->avancer(3_cm, SensAvance::Avant, 500_ms);
sleep(1_s);
_dep->avancer(3_cm, SensAvance::Arriere, 500_ms);*/
#if CHECK_LIST
	logCyan("-----------------------");
	logCyan("Cadre décalage enlevé ?");
	getchar();
	logCyan("-----------------------");
#endif

	// Attente pour le départ
	logDebug0("Tirette");

	if(!_debugMode) {
		if(_robot->getModeConnexion() == Commun::ModeConnexion::LOCAL) {
			// Attente en mode simu
			// TODO faire une tirette en mode simu ?
			std::cout << "Mode local : Appuyez sur une touche." << std::endl;
			getchar();
		} else {
			attendreTirette();
		}
	}
}

void IAPrincipal::executer() {
	_dep->setDebugState(true);

	logDebug5("Debut exécuter");
#ifdef HOMOLOGATION
	_robot->setAngleDetectionAdv(0.5_PI);
#else
	_robot->setAngleDetectionAdv(0.4_PI);
#endif

#ifdef USE_MAGIC_STRATEGY
	_strategy.run(*_petri, 500_ms);
#else
	lancerPetri(_debugMode);
#endif

	/*std::thread([=]() {
	    logDebug(_robot->getPositionAdversaire());
	});*/
}

void IAPrincipal::funnyAction() {
	//_meca->lancerEnginSpatial();
}

ResultatAction IAPrincipal::lancerPetri(bool debug) {
	if(debug) {
		Petri::DebugServer debugSession(*_petri);
		debugSession.start();
		debugSession.join();
	} else {
		auto ia = _petri->createPetriNet();
		ia->run();

		// Garde fou pour les 90s de match
		while(getTempsRestant() > 0_s) {
			sleep(50_ms);
		}

		ia->stop();
	}

	return ResultatAction::REUSSI;
}
