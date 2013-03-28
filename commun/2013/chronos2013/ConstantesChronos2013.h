// Constantes2010.h

#ifndef CONSTANTES_CHRONOS_2013_H
#define CONSTANTES_CHRONOS_2013_H

#include <iostream>

namespace ConstantesChronos2013
{

	// Numeros des contacteurs et les roles associes
	enum NumContacteur	{
		CONTACTEUR_TIRETTE 			= 1,
		CONTACTEUR_CHOIX_EQUIPE     = 0
	};

	enum NumContacteurAsc{
		CONTACTEUR_GAUCHE_ASCG	= 0,
		CONTACTEUR_DROIT_ASCG	= 1,
		CONTACTEUR_GAUCHE_ASCD 	= 2,
		CONTACTEUR_DROIT_ASCD	= 3
	};

	// Indice des différents servo de ramassage dans le tableau de positions
	enum ServosAsc {
		SERVO_G_ASC_D        	= 0,
		SERVO_G_ASC_G     		= 1,
		SERVO_D_ASC_D      		= 2,
		SERVO_D_ASC_G        	= 3,
		SERVO_ASC_NBR         	= 4
	};

	// Indice des différents servo de ramassage dans le tableau de positions
	enum ServosBras {
		SERVO_BRAS_G_HAUT       	= 0,
		SERVO_BRAS_D_HAUT     		= 1,
		SERVO_BRAS_G_BAS      		= 2,
		SERVO_BRAS_D_BAS        	= 3,
		SERVO_BRAS_NBR  	       	= 4
	};

	// Numeros des positions des servos et moteur asservi et les roles associes
	enum PositionServosPinceAsc {
        POSITION_PINCE_ASC_OUVERTE = 0,
        POSITION_PINCE_ASC_BLOCAGE,
        POSITION_PINCE_ASC_RANGE,
        POSITION_PINCE_ASC_NBR};
    
	enum PositionServosBras {
        POSITION_BRAS_HAUT = 0,
        POSITION_BRAS_SOUFFLE_BOUGIE,
        POSITION_BRAS_RANGE,
        POSITION_BRAS_NBR};

	// angles des positions des servos et moteur asservi (en valeur interne du servo)
	static const int positionServoGAscD[POSITION_PINCE_ASC_NBR] = {900, 640, 1480};
	static const int positionServoGAscG[POSITION_PINCE_ASC_NBR] = {1290, 1000, 600};
	static const int positionServoDAscD[POSITION_PINCE_ASC_NBR] = {1420, 1710, 2160};
	static const int positionServoDAscG[POSITION_PINCE_ASC_NBR] = {1270, 1000, 1750};

	// angles des positions des servos et moteur asservi (en valeur interne du servo)
	static const int positionServoBrasGHaut[POSITION_BRAS_NBR] = {1400, 1597, 2390};
	static const int positionServoBrasDHaut[POSITION_BRAS_NBR] = {1400, 1542,  2372};
	static const int positionServoBrasGBas[POSITION_BRAS_NBR] = {1407, 1637,  2387};
	static const int positionServoBrasDBas[POSITION_BRAS_NBR] = {952, 1162,  1962};

	//////////////////////////////////////////// Moteurs ///////////////////////////////////////////////////////
    
	// Positions possibles pour l'ascenseur gauche et droit
	enum PositionsAsc {ASC_BAS = 0, ASC_MILIEU, ASC_HAUT, NBR_POSITIONS_ASC};
	static const int tableauPositionsAscGauche[NBR_POSITIONS_ASC] = {0,20000, 40000}; // TDB
	static const int tableauPositionsAscDroit[NBR_POSITIONS_ASC] = {0,20000, 40000}; // TDB


}
#endif

