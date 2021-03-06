//
//  Servo.h
//  Club Robot
//
//  Created by Rémi on 12/01/2015.
//

#ifndef Club_Robot_Servo_h
#define Club_Robot_Servo_h

#include "../../../../src/robot/Robot.h"
#include "../../Commun/Modules/Servos2019.h"
#include <functional>

namespace Commun {
	class ServoManager {
	public:
		using interfaced_type = Servos2019;
		ServoManager(Servos2019& module_servo, std::function<Angle(uint8_t, uint8_t)> getPositionServo);
		virtual ~ServoManager() = default;

		/**
		 * Positionne un servo en gérant les blocages
		 */
		ResultatAction positionnerServoBloquant(uint8_t servo, Angle pos);

		ResultatAction positionnerServoBloquant(uint8_t servo, uint8_t pos) {
			auto result = this->positionnerServoBloquant(servo, _getPositionServo(servo, pos));
			sleep(500_ms);
			return result;
		}

		/**
		 * Lit la position actuelle du servo
		 */
		Angle lirePosition(uint8_t servo);

		/**
		 * Retourne vrai si le servo est à la position donnée
		 */
		bool estALaPosition(uint8_t servo, Angle position);

		/**
		 * Règle la vitesse du servo en pourcentage
		 * Attention: comme on ne peut pas régler le couple des servo, ça va aller moins vite mais en faisant des "pas"
		 */
		void reglerVitesse(uint8_t servo, uint8_t vitesse);

		/**
		 * Ajoute un offset à la valeur théorique ou doit aller le servo
		 */
		void setOffset(Angle offset, uint8_t servo) {
			_offset[servo] = offset;
		}
		Angle getOffset(uint8_t servo) {
			return _offset[servo];
		}

		/**
		 * Change la couleur des leds des servos
		 */
		void setCouleur(uint8_t servo, Servos2019::Color);

		/**
		 * Change le mode de blocage d'un servo
		 */
		void reglerModeBlocage(uint8_t servo, Servos2019::BlockingMode);

	private:
		/// VARIABLES ///
		Servos2019& _module;
		std::function<Angle(uint8_t, uint8_t)> _getPositionServo;
		Angle _offset[Servos2019::NB_MAX_SERVOS];
	};
} // namespace Commun
#endif
