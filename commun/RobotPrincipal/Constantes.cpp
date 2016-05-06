//
//  ConstantesPrincipal.cpp
//  Club Robot
//
//  Created by Rémi on 28/10/.
//

#include "Constantes.h"

namespace ConstantesPrincipal {
	Distance const ENTRAXE_PINCES = 12_cm;
	Distance const DISTANCE_CENTRE_PINCES = 30_cm;

	Angle const positionPinceD[enumToInt(PositionPince::NBR)] = {180_deg, 135_deg, 55_deg, -5_deg};
	Angle const positionPinceG[enumToInt(PositionPince::NBR)] = {180_deg, 135_deg, 55_deg, -5_deg};
	Angle const positionPelle[enumToInt(PositionPelle::NBR)] = { 0_deg, 185_deg};
	Angle const positionCuillere[enumToInt(PositionCuillere::NBR)] = { 0_deg, 180_deg}; // FIXME : mettre les bonnes valeurs une fois que le servo sera en place
	Angle const positionParasol[enumToInt(PositionParasol::NBR)] = { 180_deg, 0_deg};

}

Duration const ConstantesPrincipal::TIMEOUT_DEPLACEMENT_DEFAUT = Constantes::TIMEOUT_MOVE_DEFAULT;
Duration const ConstantesPrincipal::TIMEOUT_BLOCAGE_ADVERSAIRE_DEFAUT = Constantes::TIMEOUT_OPPONENT_BLOCKING_DEFAULT;
Speed const ConstantesPrincipal::VITESSE_LINEAIRE_DEFAUT = Constantes::LINEAR_SPEED_DEFAULT;
AngularSpeed const ConstantesPrincipal::VITESSE_ANGULAIRE_DEFAUT = Constantes::ANGULAR_SPEED_DEFAULT;
Distance const ConstantesPrincipal::PRECISION_LINEAIRE_DEFAUT = Constantes::LINEAR_PRECISION_DEFAULT;
Angle const ConstantesPrincipal::PRECISION_ANGULAIRE_DEFAUT = Constantes::ANGULAR_PRECISION_DEFAULT;

Distance const ConstantesPrincipal::RAYON_ROTATION_ROBOT = 230.0_mm;
Vector3m const ConstantesPrincipal::TAILLE_ROBOT = {38_cm, 30_cm, 43_cm};
uint16_t const ConstantesPrincipal::TCPIP_PORT_SIMU_DEFAULT = 4321;
Angle const ConstantesPrincipal::OFFSET_ANGLE_ADV = 0_PI; // 0.5_PI

int const ConstantesPrincipal::IDRobot = 1;

Duration ConstantesRobotPrincipal::getTimeoutMoveDefault() const {
	return ConstantesPrincipal::TIMEOUT_DEPLACEMENT_DEFAUT;
}

Duration ConstantesRobotPrincipal::getTimeoutOpponentBlockingDefault() const {
	return ConstantesPrincipal::TIMEOUT_BLOCAGE_ADVERSAIRE_DEFAUT;
}

Speed ConstantesRobotPrincipal::getLinearSpeedDefault() const {
	return ConstantesPrincipal::VITESSE_LINEAIRE_DEFAUT;
}
AngularSpeed ConstantesRobotPrincipal::getAngularSpeedDefault() const {
	return ConstantesPrincipal::VITESSE_ANGULAIRE_DEFAUT;
}

Distance ConstantesRobotPrincipal::getLinearPrecisionDefault() const {
	return ConstantesPrincipal::PRECISION_LINEAIRE_DEFAUT;
}

Angle ConstantesRobotPrincipal::getAngularPrecisionDefault() const {
	return ConstantesPrincipal::PRECISION_ANGULAIRE_DEFAUT;
}

Distance ConstantesRobotPrincipal::getRayRotation() const {
	return ConstantesPrincipal::RAYON_ROTATION_ROBOT;
}

Vector3m ConstantesRobotPrincipal::getRobotSize() const {
	return ConstantesPrincipal::TAILLE_ROBOT;
}

Angle ConstantesRobotPrincipal::getOffsetAngleAdv() const {
	return ConstantesPrincipal::OFFSET_ANGLE_ADV;
}


uint16_t ConstantesRobotPrincipal::getPortTCPIPDefault() const {
	return 1234;
}

int ConstantesRobotPrincipal::getIDRobot() const {
	return ConstantesPrincipal::IDRobot;
}
