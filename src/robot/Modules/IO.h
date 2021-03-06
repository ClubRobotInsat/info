/**
 * Cette classe permets d'accéder aux états de contacteurs sur le robot
 * Actuellement, seule la tirette est présente
 * L'IO permets donc de savoir quand commence le match
 */

#ifndef ROOT_MODULEIO_H
#define ROOT_MODULEIO_H

#include "Module.hpp"

namespace PhysicalRobot {

	ENUM_CLASS_NS(PhysicalRobot, TriggerState, Triggered, Waiting);
	ENUM_CLASS_NS(PhysicalRobot, IOState, On, Off, Exception);
	ENUM_CLASS_NS(PhysicalRobot, BuzzerState, Rest, PlayErrorSound, PlaySuccessSound);

	class IO final : public Module {
	public:
		explicit IO(uint8_t id);

		TriggerState read_tirette() const;
		void set_sound(BuzzerState);

		IOState limit_switches(std::string limit_switch_name) const;

	private:
		std::vector<JSON> generate_list_jsons() const override;
		void message_processing(const JSON&) override;

		void deactivation() override {}

		std::atomic<TriggerState> _tirette;
		std::atomic<BuzzerState> _buzzer;

		std::atomic<IOState> _limit_left_down;
		std::atomic<IOState> _limit_left_middle;
		std::atomic<IOState> _limit_left_high;
		std::atomic<IOState> _limit_right_down;
		std::atomic<IOState> _limit_right_middle;
		std::atomic<IOState> _limit_right_high;
	};

} // namespace PhysicalRobot

#endif // ROOT_MODULEIO_H
