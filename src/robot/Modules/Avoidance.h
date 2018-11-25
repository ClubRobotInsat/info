//
// Created by terae on 17/09/18.
//

#ifndef ROOT_AVOIDANCE_H
#define ROOT_AVOIDANCE_H

#include "Module.hpp"

namespace PhysicalRobot {

	class Avoidance final : public Module {
	public:
		explicit Avoidance(uint8_t id);

		Angle get_angle_detection_adversary() const;

		/**
		 * Fonction permettant de modifier l'angle de détection de l'adversaire
		 * Attention: il faut donner la taille de la demi-zone
		 *
		 * ex: pour avoir un cone de +/- 0.25_PI, donner 0.25_PI
		 * default : +/- 0.25_PI
		 */
		void set_angle_detection_adversary(Angle angle);

		void set_position_turret(Vector2m position);

		Vector2m get_position_turret() const;

		bool is_adversary_detected() const;

	private:
		JSON generate_json() const override;
		void message_processing(const JSON&) override;

		void deactivation() override {}

		std::atomic<Angle> _angle_detection_adversary;
		Vector2m _position_turret;
		std::atomic_bool _adversary_detected;
	};
} // namespace PhysicalRobot


#endif // ROOT_AVOIDANCE_H
