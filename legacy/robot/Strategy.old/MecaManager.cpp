// MecaManagerPrincipal2015.cpp

#include "MecaManager.h"
//#include "../petri/src/Test.h"
#include <cassert>
#include <petrilab/Cpp/DebugServer.h>

#define ADD(Vector, Function, ...) Vector.push_back(std::bind(&MecaManagerPrincipal::Function, this, ##__VA_ARGS__))
#define NBR_PLACES_STOCKAGE 2

typedef std::function<ResultatAction(void)> fun_ra;

inline ResultatAction combiner_actions(std::vector<fun_ra> actions) {
	ResultatAction result = ResultatAction::REUSSI;

	for(fun_ra action : actions) {
		if(result != ResultatAction::REUSSI && result != ResultatAction::TIMEOUT)
			return result;
		result = action();
	}
	return result;
}

#define SLEEP_TIME_US 100
const Duration MecaManagerPrincipal::TIMEOUT = 4_s;

using namespace Constantes;
using namespace ConstantesPrincipal;
using namespace IDCartesPrincipal;

MecaManagerPrincipal::MecaManagerPrincipal(RobotPrincipal& robot)
        : _robot(robot)
        //, _ascenseur(_robot.getCarte<ASCENSEUR>(), positionAscenseur)
        , _servos(robot.getCarte<SERVOS>(),
                  std::bind(&MecaManagerPrincipal::getPositionServo, this, std::placeholders::_1, std::placeholders::_2))
        , _moteurs(robot.getCarte<MOTEURS>())
/*
        , _servoX(_robot.getCarte<SERVO_X>(),
                                    std::bind(&MecaManagerPrincipal::getPositionServoX, this, std::placeholders::_1,
   std::placeholders::_2))
        , _pompes{&_robot.getCarte<PNEUMATIQUE_1>(), &_robot.getCarte<PNEUMATIQUE_2>() */
{
	_elevator_counter.store(0);

	logDebug("Initialisation de la meca");
	// Test des servos
	// this->setModeBlocageServos(); // pas implémenté côté hard
	this->setCouleurServos(CarteServo::Couleur::JAUNE);

	this->ouvrirAbeille();
	this->fermerAbeille();

	this->ouvrirSouteD();
	this->ouvrirSouteG();

	this->fermerSouteD();
	this->fermerSouteG();

	/*// Test des moteurs
	this->activerAvaleurs(SensAvaleurs::AVALER);
	sleep(1);
	this->desactiverAvaleurs();
	sleep(1);
	this->activerAvaleurs(SensAvaleurs::RECRACHER);
	sleep(1);
	this->desactiverAvaleurs();*/

	// this->monterAscenseursDe(1);

	// this->activerTurbineD();
	// this->activerTurbineG();

	this->ouvrirPorteCube();
	this->fermerPorteCube();

	this->setCouleurServos(CarteServo::Couleur::VERT);

	// _ascenseur.initialiser(SensRotation::Trigo);
	// logDebug4("init de la fusee : ", this->bougerFuseeDe(360_deg));
	// this->initialiserEnginSpatial();

	logDebug("Fin initialisation Méca");
}

MecaManagerPrincipal::~MecaManagerPrincipal() {
	this->couperMeca();
}

void MecaManagerPrincipal::couperMeca() {
	logDebug("couperMeca()");
	this->desactiverAvaleurs();
	// this->ouvrirPince();
}

ResultatAction MecaManagerPrincipal::ouvrirPorteCube() {
	_elevator_counter = 0;
	return this->_servos.positionnerServoBloquant(enumToInt(ConstantesPrincipal::Servo::PORTE_CUBE),
	                                              enumToInt(ConstantesPrincipal::PorteCube::OUVERT));
}

ResultatAction MecaManagerPrincipal::fermerPorteCube() {
	return this->_servos.positionnerServoBloquant(enumToInt(ConstantesPrincipal::Servo::PORTE_CUBE),
	                                              enumToInt(ConstantesPrincipal::PorteCube::FERME));
}

ResultatAction MecaManagerPrincipal::orienterPorteCubeDe(Angle val) {
	return this->_servos.positionnerServoBloquant(enumToInt(ConstantesPrincipal::Servo::PORTE_CUBE), val);
}

ResultatAction MecaManagerPrincipal::monterAscenseursDe(int nbr_tours) {
	_elevator_counter += nbr_tours;

	if(nbr_tours >= 0) {
		auto motor_one =
		    std::make_pair<uint8_t, CarteMoteurs2018::SensRotation>(enumToInt(ConstantesPrincipal::Moteurs::ASCENSEUR_DROIT),
		                                                            CarteMoteurs2018::SensRotation::Horaire);
		auto motor_two =
		    std::make_pair<uint8_t, CarteMoteurs2018::SensRotation>(enumToInt(ConstantesPrincipal::Moteurs::ASCENSEUR_GAUCHE),
		                                                            CarteMoteurs2018::SensRotation::Trigo);
		return this->_moteurs.positionnerNMoteursBloquants({motor_one, motor_two}, nbr_tours);
	} else {
		auto motor_one =
		    std::make_pair<uint8_t, CarteMoteurs2018::SensRotation>(enumToInt(ConstantesPrincipal::Moteurs::ASCENSEUR_DROIT),
		                                                            CarteMoteurs2018::SensRotation::Trigo);
		auto motor_two =
		    std::make_pair<uint8_t, CarteMoteurs2018::SensRotation>(enumToInt(ConstantesPrincipal::Moteurs::ASCENSEUR_GAUCHE),
		                                                            CarteMoteurs2018::SensRotation::Horaire);
		return this->_moteurs.positionnerNMoteursBloquants({motor_one, motor_two}, nbr_tours);
	}
}

ResultatAction MecaManagerPrincipal::monterAscenseursDe(Angle angle) {
	return this->_moteurs.positionnerNMoteursBloquants({enumToInt(ConstantesPrincipal::Moteurs::ASCENSEUR_DROIT),
	                                                    enumToInt(ConstantesPrincipal::Moteurs::ASCENSEUR_GAUCHE)},
	                                                   angle);
}

ResultatAction MecaManagerPrincipal::activerAvaleurs(SensAvaleurs sens) {
	std::pair<uint8_t, CarteMoteurs2018::SensRotation> motor_one;
	std::pair<uint8_t, CarteMoteurs2018::SensRotation> motor_two;
	switch(sens) {
		case AVALER:
			motor_one = std::make_pair<uint8_t, CarteMoteurs2018::SensRotation>(enumToInt(ConstantesPrincipal::Moteurs::AVALEUR_DROIT),
			                                                                    CarteMoteurs2018::SensRotation::Trigo);
			motor_two = std::make_pair<uint8_t, CarteMoteurs2018::SensRotation>(enumToInt(ConstantesPrincipal::Moteurs::AVALEUR_GAUCHE),
			                                                                    CarteMoteurs2018::SensRotation::Horaire);
			break;

		case RECRACHER:
			motor_one = std::make_pair<uint8_t, CarteMoteurs2018::SensRotation>(enumToInt(ConstantesPrincipal::Moteurs::AVALEUR_DROIT),
			                                                                    CarteMoteurs2018::SensRotation::Horaire);
			motor_two = std::make_pair<uint8_t, CarteMoteurs2018::SensRotation>(enumToInt(ConstantesPrincipal::Moteurs::AVALEUR_GAUCHE),
			                                                                    CarteMoteurs2018::SensRotation::Trigo);
			break;
	}

	return this->_moteurs.tournerPlusieursOn({motor_one, motor_two});
}

ResultatAction MecaManagerPrincipal::desactiverAvaleurs() {
	return this->_moteurs.tournerPlusieursOff({enumToInt(ConstantesPrincipal::Moteurs::AVALEUR_GAUCHE),
	                                           enumToInt(ConstantesPrincipal::Moteurs::AVALEUR_DROIT)});
}

int MecaManagerPrincipal::nbrPlaceAscenseur() {
	return MAX_ELEVATOR_SIZE - _elevator_counter;
}

ResultatAction MecaManagerPrincipal::ouvrirSouteD() {
	_turbine_right_free = false;
	return this->_servos.positionnerServoBloquant(enumToInt(ConstantesPrincipal::Servo::PORTE_SOUTE_DROIT),
	                                              enumToInt(ConstantesPrincipal::SouteDroit::OUVERT));
}

ResultatAction MecaManagerPrincipal::fermerSouteD() {
	return this->_servos.positionnerServoBloquant(enumToInt(ConstantesPrincipal::Servo::PORTE_SOUTE_DROIT),
	                                              enumToInt(ConstantesPrincipal::SouteDroit::FERME));
}

ResultatAction MecaManagerPrincipal::orienterSouteDDe(Angle val) {
	return this->_servos.positionnerServoBloquant(enumToInt(ConstantesPrincipal::Servo::PORTE_SOUTE_DROIT), val);
}

ResultatAction MecaManagerPrincipal::activerTurbineD() {
	_turbine_right_free = true;
	return this->_moteurs.tournerOnBrushless(enumToInt(ConstantesPrincipal::Moteurs::BRUSHLESS_DROIT));
}

ResultatAction MecaManagerPrincipal::desactiverTurbineD() {
	return this->_moteurs.tournerOff(enumToInt(ConstantesPrincipal::Moteurs::BRUSHLESS_DROIT));
}

bool MecaManagerPrincipal::turbineDLibre() {
	return _turbine_right_free;
}

ResultatAction MecaManagerPrincipal::ouvrirSouteG() {
	_turbine_left_free = false;
	return this->_servos.positionnerServoBloquant(enumToInt(ConstantesPrincipal::Servo::PORTE_SOUTE_GAUCHE),
	                                              enumToInt(ConstantesPrincipal::SouteGauche::OUVERT));
}

ResultatAction MecaManagerPrincipal::fermerSouteG() {
	return this->_servos.positionnerServoBloquant(enumToInt(ConstantesPrincipal::Servo::PORTE_SOUTE_GAUCHE),
	                                              enumToInt(ConstantesPrincipal::SouteGauche::FERME));
}

ResultatAction MecaManagerPrincipal::orienterSouteGDe(Angle val) {
	return this->_servos.positionnerServoBloquant(enumToInt(ConstantesPrincipal::Servo::PORTE_SOUTE_GAUCHE), val);
}

ResultatAction MecaManagerPrincipal::activerTurbineG() {
	_turbine_left_free = true;
	return this->_moteurs.tournerOnBrushless(enumToInt(ConstantesPrincipal::Moteurs::BRUSHLESS_GAUCHE));
}

ResultatAction MecaManagerPrincipal::desactiverTurbineG() {
	return this->_moteurs.tournerOff(enumToInt(ConstantesPrincipal::Moteurs::BRUSHLESS_GAUCHE));
}

bool MecaManagerPrincipal::turbineGLibre() {
	return _turbine_left_free;
}

ResultatAction MecaManagerPrincipal::ouvrirAbeille() {
	return this->_servos.positionnerServoBloquant(enumToInt(ConstantesPrincipal::Servo::ABEILLE),
	                                              enumToInt(ConstantesPrincipal::Abeille::OUVERT));
}

ResultatAction MecaManagerPrincipal::fermerAbeille() {
	return this->_servos.positionnerServoBloquant(enumToInt(ConstantesPrincipal::Servo::ABEILLE),
	                                              enumToInt(ConstantesPrincipal::Abeille::FERME));
}

ResultatAction MecaManagerPrincipal::orienterAbeilleDe(Angle val) {
	return this->_servos.positionnerServoBloquant(enumToInt(ConstantesPrincipal::Servo::ABEILLE), val);
}

ResultatAction MecaManagerPrincipal::monterLoquet() {
	return this->_servos.positionnerServoBloquant(enumToInt(ConstantesPrincipal::Servo::LOQUET),
	                                              enumToInt(ConstantesPrincipal::Loquet::HAUT));
}

ResultatAction MecaManagerPrincipal::descendreLoquet() {
	return this->_servos.positionnerServoBloquant(enumToInt(ConstantesPrincipal::Servo::LOQUET),
	                                              enumToInt(ConstantesPrincipal::Loquet::BAS));
}

ResultatAction MecaManagerPrincipal::orienterLoquetDe(Angle val) {
	return this->_servos.positionnerServoBloquant(enumToInt(ConstantesPrincipal::Servo::LOQUET), val);
}

/*ResultatAction MecaManagerPrincipal::ascenseurStockageHaut() {
    return this->_ascenseur.allerAPosition(PositionAscenseur::STOCKAGE_HAUT);
}

ResultatAction MecaManagerPrincipal::ascenseurStockageBas() {
    return this->_ascenseur.allerAPosition(PositionAscenseur::STOCKAGE_BAS);
}

ResultatAction MecaManagerPrincipal::ascenseurCylindreSol() {
    auto result = this->_ascenseur.allerAPosition(PositionAscenseur::SAISIE_CYLINDRE_SOL);
    if(result == ResultatAction::REUSSI || result == ResultatAction::BLOQUE)
        return ResultatAction::REUSSI;
    return result;
}

ResultatAction MecaManagerPrincipal::ascenseurAjustagePourSaisieCylindreBas() {
    return this->_ascenseur.allerAPosition(PositionAscenseur::SAISIE_CYLINDRE_STOCKAGE_BAS);
}

ResultatAction MecaManagerPrincipal::ascenseurPoserRails() {
    return this->_ascenseur.allerAPosition(PositionAscenseur::POSER_DANS_RAILS);
}

ResultatAction MecaManagerPrincipal::attraperCylindre() {
    auto action = fermerPince();
    logDebug1("fermerPince = ", action);
    switch(action) {
        case ResultatAction::TIMEOUT:
            return action;
        case ResultatAction::BLOQUE:
            return ResultatAction::REUSSI;
        case ResultatAction::REUSSI:
            return ResultatAction::REUSSI; // à supprimer soon
        default:
            return ResultatAction::RATE;
    }
}

ResultatAction MecaManagerPrincipal::ajouterCylindre(int i) {
    _cylindresStockes[i] = true;
    return ResultatAction::REUSSI;
}

ResultatAction MecaManagerPrincipal::supprimerCylindre(int i) {
    _cylindresStockes[i] = false;
    return ResultatAction::REUSSI;
}

ResultatAction MecaManagerPrincipal::stockerCylindre() {
    std::vector<fun_ra> actions; // toutes les fonctions enchaînées

    for(int i = PositionStockage::NBR - NBR_PLACES_STOCKAGE; i < PositionStockage::NBR; ++i) {
        if(!_cylindresStockes[i]) {
            ADD(actions, ascenseurCylindreSol);
            ADD(actions, orienterPinceAttraper);
            ADD(actions, pencherPinceDevant);
            ADD(actions, attraperCylindre);

            switch((PositionStockage)i) {
                case BAS:
                    ADD(actions, ascenseurStockageBas);
                    // ADD(actions, ouvrirPinceMoitie);
                    ADD(actions, orienterPinceRecalage);
                    ADD(actions, ouvrirPinceMax);
                    ADD(actions, ascenseurCylindreSol);
                    ADD(actions, orienterPinceStocker);
                    ADD(actions, orienterPinceAttraper);
                    ADD(actions, ouvrirPince);
                    break;

                case HAUT:
                    ADD(actions, ascenseurStockageBas);
                    ADD(actions, orienterPinceStocker);
                    ADD(actions, ouvrirPinceMoitie);
                    ADD(actions, ascenseurCylindreSol);
                    ADD(actions, attraperCylindre);
                    ADD(actions, orienterPinceAttraper);
                    ADD(actions, ascenseurStockageHaut);
                    ADD(actions, orienterPinceStocker);
                    // Ajouter des positions de stockage intermédiaires?
                    ADD(actions, ouvrirPinceMax);
                    ADD(actions, orienterPinceAttraper);
                    ADD(actions, ascenseurCylindreSol);
                    ADD(actions, ouvrirPince);
                    break;

                default: // BRAS
                    ADD(actions, ascenseurStockageBas);
                    break;
            }
            ADD(actions, ajouterCylindre, i);
            ADD(actions, printCylindres);
            break;
        }
    }
    return combiner_actions(actions);
}

ResultatAction MecaManagerPrincipal::destockerCylindre() {
    std::vector<fun_ra> actions;

    for(int i = PositionStockage::NBR - 1; i >= PositionStockage::NBR - NBR_PLACES_STOCKAGE; --i) {
        if(_cylindresStockes[i]) {
            ADD(actions, orienterPinceAttraper);
            ADD(actions, pencherPinceDevant);
            switch((PositionStockage)i) {
                case BAS:
                    ADD(actions, ascenseurCylindreSol);
                    ADD(actions, ouvrirPinceMax);
                    ADD(actions, orienterPinceRecalage);
                    ADD(actions, attraperCylindre);
                    ADD(actions, orienterPinceAttraper);
                    break;

                case HAUT:
                    ADD(actions, ascenseurStockageHaut);
                    ADD(actions, ouvrirPinceMax);
                    ADD(actions, orienterPinceStocker);
                    ADD(actions, attraperCylindre);
                    ADD(actions, orienterPinceAttraper);
                    break;

                case BRAS:
                    break;

                default:
                    break;
            }
            ADD(actions, supprimerCylindre, i);
            ADD(actions, printCylindres);
            break;
        }
    }
    return combiner_actions(actions);
}

int MecaManagerPrincipal::getNbrCylindres() {
    int N = 0;
    for(int i = 0; i < PositionStockage::NBR; ++i)
        if(_cylindresStockes[i])
            N++;
    return N - PositionStockage::NBR + NBR_PLACES_STOCKAGE;
}*/

Angle MecaManagerPrincipal::getPositionServo(uint8_t servo, uint8_t pos) {
	switch(static_cast<Servo>(servo)) {
		case Servo::PORTE_CUBE:
			return positionPorteCube[pos];

		case Servo::PORTE_SOUTE_GAUCHE:
			return positionSouteGauche[pos];

		case Servo::PORTE_SOUTE_DROIT:
			return positionSouteDroit[pos];

		case Servo::ABEILLE:
			return positionAbeille[pos];

		case Servo::LOQUET:
			return positionLoquet[pos];

		default:
			throw std::invalid_argument("unimplemented for the servo " + std::to_string(servo));
	}
}

void MecaManagerPrincipal::setModeBlocageServos() {
	auto avec = CarteServo::ModeBlocage::MAINTIEN_SUR_BLOCAGE;
	auto sans = CarteServo::ModeBlocage::DEBLOCAGE;

	// TODO: mettre 'avec' si on a besoin de forcer sur les servos
	_servos.reglerModeBlocage(enumToInt(Servo::PORTE_CUBE), sans);
	_servos.reglerModeBlocage(enumToInt(Servo::ABEILLE), sans);
	_servos.reglerModeBlocage(enumToInt(Servo::PORTE_SOUTE_DROIT), sans);
	_servos.reglerModeBlocage(enumToInt(Servo::PORTE_SOUTE_GAUCHE), sans);
	_servos.reglerModeBlocage(enumToInt(Servo::LOQUET), sans);
}

void MecaManagerPrincipal::setCouleurServos(CarteServo::Couleur couleur) {
	for(uint8_t i = 0; i < enumToInt(Servo::NBR); ++i)
		_servos.setCouleur(i, couleur);
}
