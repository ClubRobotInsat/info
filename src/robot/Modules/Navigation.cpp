//
// Created by terae on 16/09/18.
//

#include "Navigation.h"

namespace PhysicalRobot {

	void Navigation::forward(Distance distance) {
		lock_variables();
		set_command(MovingCommand::GoForward);
		_args_cmd[0] = distance_to_u16(distance);
		_state_changed.exchange(true);
		unlock_variables();
	}

	void Navigation::backward(Distance distance) {
		lock_variables();
		set_command(MovingCommand::GoBackward);
		_args_cmd[0] = distance_to_u16(distance);
		_state_changed.exchange(true);
		unlock_variables();
	}

	void Navigation::turn_absolute(Angle angle) {
		lock_variables();
		set_command(MovingCommand::TurnAbsolute);
		_args_cmd[0] = angle_to_u16(angle);
		_state_changed.exchange(true);
		unlock_variables();
	}

	void Navigation::turn_relative(Angle angle) {
		lock_variables();
		set_command(MovingCommand::TurnRelative);
		_args_cmd[0] = angle_to_u16(angle);
		_state_changed.exchange(true);
		unlock_variables();
	}

	void Navigation::stop() {
		lock_variables();
		set_command(MovingCommand::Stop);
		_state_changed.exchange(true);
		unlock_variables();
	}

	repere::Coordinates Navigation::get_coordinates() const {
		std::lock_guard<std::mutex> lk(_mutex_variables);
		return _coords;
	}

	std::vector<JSON> Navigation::generate_list_jsons() const {
		JSON moving;
		moving["x"] = distance_to_u16(_coords.getX());
		moving["y"] = distance_to_u16(_coords.getY());
		moving["angle"] = angle_to_u16(_coords.getAngle());
		moving["blocked"] = _blocked.load();
		moving["asserv_on_off"] = _asserv_on_off.load();
		moving["led"] = _leds.load();
		moving["reset"] = _reset.load();

		moving["command"] = toString(_command);
		moving["args_cmd1"] = _args_cmd[0];
		moving["args_cmd2"] = _args_cmd[1];
		moving["counter"] = _counter;
		moving["moving_done"] = _moving_done.load();
		return {moving};
	}

	void Navigation::message_processing(const JSON& json) {
		_coords = repere::Coordinates({u16_to_distance(json["x"]), u16_to_distance(json["y"])}, u16_to_angle(json["angle"]));

		_blocked = json["blocked"];

		// moving_done est valide seulement si ils parlent
		// de la commande actuelle.
		if(json["counter"] == _counter) {
			_moving_done = json["moving_done"];
		}
	}

	void Navigation::deactivation() {
		lock_variables();
		set_command(MovingCommand::EmergencyStop);
		_state_changed.exchange(true);
		// TODO : arrêt d'urgence + arrêter l'asservissement
		unlock_variables();
	}

	void Navigation::set_command(MovingCommand command) {
		_command = command;
		++_counter;
		_moving_done = false;
	}

	uint16_t Navigation::distance_to_u16(Distance distance) const {
		// TODO controller overflow et underflow
		return static_cast<uint16_t>(distance.toMm() * 10);
	}

	Distance Navigation::u16_to_distance(uint16_t distance) const {
		return Distance::makeFromMm(distance / 10.0);
	}

	uint16_t Navigation::angle_to_u16(Angle angle) const {
		return static_cast<uint16_t>(angle.toMilliRad() * 10);
	}

	Angle Navigation::u16_to_angle(uint16_t angle) const {
		return Angle::makeFromMilliRad(angle / 10.0);
	}
} // namespace PhysicalRobot