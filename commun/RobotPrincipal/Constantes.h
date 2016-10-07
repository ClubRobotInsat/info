// Constantes.h

#ifndef CONSTANTES_ROBOT_PRINCIPAL__H
#define CONSTANTES_ROBOT_PRINCIPAL__H


#ifdef TARGET_SIMULATEUR

#include "../../simulateur/cartes/CarteAsservissement2009.h"
#include "../../simulateur/cartes/CarteCAN_USB.h"
#include "../../simulateur/cartes/CarteDebugDessin.h"
#include "../../simulateur/cartes/CarteDeplacement2009.h"
#include "../../simulateur/cartes/CarteDetectAdv2009.h"
#include "../../simulateur/cartes/CarteIO2014.h"
#include "../../simulateur/cartes/CarteServosNova2009.h"
#include "../../simulateur/robot/Carte.h"

#else

#include "../../robot/Cartes/Carte.h"
#include "../../robot/Cartes/CarteAsservissement2009.h"
#include "../../robot/Cartes/CarteCAN_USB.h"
#include "../../robot/Cartes/CarteDebugDessin.h"
#include "../../robot/Cartes/CarteDeplacementSymetrieY.h"
#include "../../robot/Cartes/CarteIO2014.h"
#include "../robot/Cartes/CarteDetectAdv2009.h"

#endif

#include "../../../CodeCommun/Informations_cartesElec.h"

#include "../Commun.h"
#include "../ConstantesCommunes.h"
#include <type_traits>

namespace IDCartesPrincipal {
	enum {
		ID_CARTE_CAN_USB = 0,
		ID_CARTE_DEPLACEMENT = ID_CARTE_CHUCK_DEPLACEMENT,
		ID_CARTE_SERVO_G = ID_CARTE_CHUCK_SERVOS_GAUCHE,
		ID_CARTE_SERVO_D = ID_CARTE_CHUCK_SERVOS_DROIT,
		ID_CARTE_IO = ID_CARTE_CHUCK_IO,
		ID_CARTE_PNEUMATIQUE_HAUTE = ID_CARTE_CHUCK_PNEUMATIQUE_HAUT,
		ID_CARTE_PNEUMATIQUE_BAS = ID_CARTE_CHUCK_PNEUMATIQUE_BAS,
		ID_CARTE_EVITEMENT = ID_CARTE_CHUCK_EVITEMENT,
		ID_CARTE_ELECTROVANNES = ID_CARTE_CHUCK_ELECTROVANNES,
		ID_CARTE_DEBUG_DESSIN = 9
	};

	ENUM_NS(IDCartesPrincipal, IDCartes, CAN_USB, DEPLACEMENT, SERVO_G, SERVO_D, IO, PNEUMATIQUE_HAUTE, PNEUMATIQUE_BAS, EVITEMENT, ELECTROVANNES);

	ENUM_CLASS_NS(IDCartesPrincipal, IDCartesServo, ASCENSEUR, AUTRES);
}

namespace ConstantesPrincipal {
	using IDCartes = IDCartesPrincipal::IDCartes;
	using IDCartesServo = IDCartesPrincipal::IDCartesServo;

	/////////////////// CONTACTEURS ///////////////////
	// Numéros des contacteurs et les rôles associés
	enum class Contacteur {
		TIRETTE = 2,
		BACK_CONTACT_1 = 5,
		BACK_CONTACT_2 = 6,
		BACK_CONTACT_3 = 7,
		BACK_CONTACT_4 = 8,
		BACK_CONTACT_5 = 9,
		BACK_CONTACT_6 = 10,
	};

	/////////////////// SERVOS ///////////////////
	// Durée attendue lors de l'envoi d'un message aux cartes élecs dans les fonctions blonquantes autre que les servos.
	auto const TIMEOUT_ELEC = 100_ms;

	// On reprend par défaut les mêmes valeur que la partie commune aux 2 robots, à changer si nécessaire
	extern Duration const TIMEOUT_DEPLACEMENT_DEFAUT;
	extern Duration const TIMEOUT_BLOCAGE_ADVERSAIRE_DEFAUT;
	extern Speed const VITESSE_LINEAIRE_DEFAUT;
	extern AngularSpeed const VITESSE_ANGULAIRE_DEFAUT;
	extern Distance const PRECISION_LINEAIRE_DEFAUT;
	extern Angle const PRECISION_ANGULAIRE_DEFAUT;

	extern Distance const RAYON_ROTATION_ROBOT;
	extern Vector3m const TAILLE_ROBOT;
	extern uint16_t const TCPIP_PORT_SIMU_DEFAULT;
	// Cet offset sert à aligner le 0 de la balise avec le 0 de l'info
	// NB : 0 devant, PI/-PI derrière, PI/2 à gauche et -PI/2 à droite
	extern Angle const OFFSET_ANGLE_ADV;

	extern int const IDRobot;

	/////////////////// CARTES ///////////////////

	template <IDCartesPrincipal::IDCartes ID_CARTE>
	struct CarteInfo {};

	template <IDCartesPrincipal::IDCartesServo ID_CARTE>
	struct CarteServoInfo {};

	template <>
	struct CarteInfo<IDCartesPrincipal::CAN_USB> {
		using typeCarte = CarteCAN_USB;
		enum : std::uint8_t { idCarte = IDCartesPrincipal::ID_CARTE_CAN_USB };
	};
	template <>
	struct CarteInfo<IDCartesPrincipal::DEPLACEMENT> {
#ifdef TARGET_SIMULATEUR
		using typeCarte = CarteDeplacement2009;
#else
		using typeCarte = CarteDeplacementSymetrieY;
#endif
		enum : std::uint8_t { idCarte = IDCartesPrincipal::ID_CARTE_DEPLACEMENT };
	};
	template <>
	struct CarteInfo<IDCartesPrincipal::IO> {
		using typeCarte = CarteIO2014;
		enum : std::uint8_t { idCarte = IDCartesPrincipal::ID_CARTE_IO };
	};
	template <>
	struct CarteInfo<IDCartesPrincipal::EVITEMENT> {
		using typeCarte = CarteDetectAdv2009;
		enum : std::uint8_t { idCarte = IDCartesPrincipal::ID_CARTE_EVITEMENT };
	};
};

struct ConstantesRobotPrincipal : public Commun::ConstantesRobot {
	virtual Duration getTimeoutMoveDefault() const override;
	virtual Duration getTimeoutOpponentBlockingDefault() const override;
	virtual Speed getLinearSpeedDefault() const override;
	virtual AngularSpeed getAngularSpeedDefault() const override;
	virtual Distance getLinearPrecisionDefault() const override;
	virtual Angle getAngularPrecisionDefault() const override;

	virtual Distance getRayRotation() const override;
	virtual Vector3m getRobotSize() const override;
	virtual Angle getOffsetAngleAdv() const override;

	virtual uint16_t getPortTCPIPDefault() const override;

	virtual int getIDRobot() const override;
};

#endif
