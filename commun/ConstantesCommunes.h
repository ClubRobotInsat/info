// ConstantesCommunes2015.h

#ifndef CONSTANTES_COMMUNES_2015_H
#define CONSTANTES_COMMUNES_2015_H

#include "Commun.h"
#include "Enum/Enum.h"
#include "IConstantes.h"
#include <cstdint>
#include <iostream>

namespace Constantes {
	ENUM_CLASS_NS(Constantes,
	              RobotColor,
	              Purple,
	              Green,
	              // Valeur d'initialisation, qui permet de déctecter si la lecture du capteur couleur a déconné.
	              Undef);
}

namespace Constantes {
	using RobotColor = Constantes::RobotColor;

	extern uint16_t const TCPIP_PORT_SIMU_DEFAULT;

	extern int const deltaServoPosition;  // > résolution sur les valeurs de position des servos
	extern int const deltaEnginePosition; // > résolution sur les valeurs de position des moteurs (TODO : mettre la
	                                      // bonne valeur)

	extern Duration const TIMEOUT_MOVE_DEFAULT;
	extern Duration const TIMEOUT_OPPONENT_BLOCKING_DEFAULT;
	extern Speed const LINEAR_SPEED_DEFAULT;
	extern AngularSpeed const ANGULAR_SPEED_DEFAULT; // Unité arbitraire décidée en élec.
	extern Distance const LINEAR_PRECISION_DEFAULT;
	extern Angle const ANGULAR_PRECISION_DEFAULT;

	extern Speed const VITESSE_LINEAIRE_EVITEMENT;
	extern AngularSpeed const VITESSE_ANGULAIRE_EVITEMENT;

	extern Speed const VITESSE_LINEAIRE_RECALAGE;

	extern int const NOMBRE_BLOCAGE_LOGICIEL_MAX;
	extern int const NOMBRE_BLOCAGE_PHYSIQUE_MAX;

	extern Duration const MATCH_DURATION;

	extern Vector2m const TABLE_SIZE;
	extern Vector2u16 const TAILLE_GRILLE;
}

inline Constantes::RobotColor operator!(Constantes::RobotColor const& c) {
	if(c == Constantes::RobotColor::Green)
		return Constantes::RobotColor::Purple;
	else if(c == Constantes::RobotColor::Purple)
		return Constantes::RobotColor::Green;

	return c;
}

struct ConstantesCommunes : Commun::ConstantesCommunes {
	virtual int getNombreBlocageLogicielMax() const override;
	virtual int getNombreBlocagePhysiqueMax() const override;

	virtual Duration getMatchDuration() const override;

	virtual Vector2m getTableSize() const override;
	virtual Vector2u16 getTailleGrille() const override;
};

#endif // CONSTANTES_COMMUNES_2015_H
