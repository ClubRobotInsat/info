//
// Created by terae on 8/21/18.
//

#include "Servos.h"
#include <log/Log.h>

namespace PhysicalRobot {
	void Servos::add_servo(uint8_t id, BlockingMode mode) {
		std::lock_guard<std::mutex> lk(_mutex_variables);

		if(id >= ID_MAX_SERVOS) {
			throw std::runtime_error("ID du servo trop grand ("s + std::to_string(id) + " > " + std::to_string(ID_MAX_SERVOS) + ") !");
		} else if(id == 0) {
			throw std::runtime_error("L'ID 0 des servos est réservé !");
		} else if(_servos[id]) {
			throw std::runtime_error("Double assignation du servo "s + std::to_string(id) + " !");
		}

		_servos[id] = std::make_unique<Servo>(id, mode);
	}

	uint8_t Servos::get_nbr_servos() const {
		uint8_t count = 0;
		for(uint8_t index = 0; index < ID_MAX_SERVOS; ++index) {
			count += (_servos[index] != nullptr);
		}
		return count;
	}

	uint8_t Servos::get_index_of(uint8_t id) const {
		const uint8_t INDEX_BAD_ID = ID_MAX_SERVOS;
		std::lock_guard<std::mutex> lk(_mutex_variables);

		// 'id == 0' veut dire qu'il n'y a pas de servo-moteur dans la représentation C
		if(id == 0)
			return INDEX_BAD_ID;

		for(uint8_t index = 0; index < ID_MAX_SERVOS; ++index) {
			if(_servos[index] != nullptr && _servos[index]->id == id)
				return index;
		}

		return INDEX_BAD_ID;
	}

	void Servos::set_position(uint8_t id, Angle angle) {
		uint8_t index = get_index_of(id);

		if(index >= ID_MAX_SERVOS) {
			throw std::runtime_error("Numéro du servo demandé invalide : "s + std::to_string(id));
		}

		lock_variables();
		_servos[index]->blocked = false;
		_servos[index]->command = angle;
		_servos[index]->command_type = CommandType::Position;
		_state_changed.exchange(true);
		unlock_variables();
	}

	void Servos::set_speed(uint8_t id, uint16_t speed, Rotation rotation) {
		uint8_t index = get_index_of(id);

		if(index >= ID_MAX_SERVOS) {
			throw std::runtime_error("Numéro du servo demandé invalide : "s + std::to_string(id));
		}

		lock_variables();
		_servos[index]->command = Servo::CommandSpeed(speed, rotation);
		_servos[index]->command_type = CommandType::Speed;
		_state_changed.exchange(true);
		unlock_variables();
	}

	Angle Servos::read_position(uint8_t id) const {
		uint8_t index = get_index_of(id);

		if(index >= ID_MAX_SERVOS) {
			throw std::runtime_error("Numéro du servo demandé invalide : "s + std::to_string(id));
		}

		std::lock_guard<std::mutex> lk(_mutex_variables);
		return _servos[index]->position;
	}

	void Servos::set_color(uint8_t id, Color color) {
		uint8_t index = get_index_of(id);

		if(index >= ID_MAX_SERVOS) {
			throw std::runtime_error("Numéro du servo demandé invalide : "s + std::to_string(id));
		}

		lock_variables();
		_servos[index]->color = color;
		_state_changed.exchange(true);
		unlock_variables();
	}

	void Servos::set_blocking_mode(uint8_t id, BlockingMode mode) {
		uint8_t index = get_index_of(id);

		if(index >= ID_MAX_SERVOS) {
			throw std::runtime_error("Numéro du servo demandé invalide : "s + std::to_string(id));
		}

		lock_variables();
		_servos[index]->blocking_mode = mode;
		_state_changed.exchange(true);
		unlock_variables();
	}

	bool Servos::is_blocking(uint8_t id) const {
		uint8_t index = get_index_of(id);

		if(index >= ID_MAX_SERVOS) {
			throw std::runtime_error("Numéro du servo demandé invalide : "s + std::to_string(id));
		}

		std::lock_guard<std::mutex> lk(_mutex_variables);
		return _servos[index]->blocked;
	}

	bool Servos::is_moving_done(uint8_t id) const {
		uint8_t index = get_index_of(id);

		if(index >= ID_MAX_SERVOS) {
			throw std::runtime_error("Numéro du servo demandé invalide : "s + std::to_string(id));
		}

		std::lock_guard<std::mutex> lk(_mutex_variables);
		if(_servos[index]->command_type == CommandType::Speed) {
			return true;
		}

		// TODO : voir s'il faut mettre un seuil pour considérer le mouvement terminé, ou si les élecs nous le disent
		return _servos[index]->position == std::get<Angle>(_servos[id]->command);
	}

	uint16_t Servos::angle_to_uint16t(Angle angle) {
		uint16_t pos = static_cast<uint16_t>((angle.toMinusPiPi().toDeg() + 166.65) * 1023 / 333.35);
		if(pos < 21 || pos > 1002) {
			logWarn("L'angle ", angle, " est en-dehors de l'intervalle [-159.8°; 159.8°].");
		}
		pos = static_cast<uint16_t>(pos < 21 ? 21 : (pos > 1023 ? 1023 : pos));
		return pos;
	};

	Angle Servos::uint16t_to_angle(uint16_t pos) {
		return Angle::makeFromDeg((333.3 * pos) / 1023 - 166.650);
	};

	std::vector<JSON> Servos::generate_list_jsons() const {
		std::vector<JSON> result;

		for(uint8_t index = 0; index < ID_MAX_SERVOS; ++index) {
			if(_servos[index] != nullptr) {
				JSON servo;
				servo["id"] = _servos[index]->id;
				servo["known_position"] = angle_to_uint16t(_servos[index]->position);
				servo["control"] = toString(_servos[index]->command_type);

				if(_servos[index]->command_type == CommandType::Position) {
					servo["data"] = angle_to_uint16t(std::get<Angle>(_servos[index]->command));
					// FIXME: Ce champ est utile seulement tant que le code élec n'est pas fix et qu'il en a besoin
					servo["rotation"] = "CounterClockwise";
				} else {
					Servo::CommandSpeed command = std::get<Servo::CommandSpeed>(_servos[index]->command);
					servo["data"] = command.first;
					servo["rotation"] = toString(command.second);
				}

				servo["blocked"] = _servos[index]->blocked.load();
				servo["mode"] = toString(_servos[index]->blocking_mode);
				servo["color"] = toString(_servos[index]->color.load());

				result.push_back(servo);
			}
		}

		return result;
	}

	void Servos::message_processing(const JSON& servo) {
		uint8_t index = get_index_of(servo["id"]);

		if(_servos[index] != nullptr) {
			// Les données de commande (position ou vitesse) ne sont pas prises en compte ici
			// Seule l'informatique a le droit d'écriture dessus
			_servos[index]->position = uint16t_to_angle(servo["known_position"]);
			_servos[index]->blocked.exchange(servo["blocked"]);

			_servos[index]->color.exchange(Color::Red);
			for(auto ee : getEnumValues<Color>()) {
				if(toString(ee) == servo["color"]) {
					_servos[index]->color.exchange(ee);
					break;
				}
			}
		}
	}


	void Servos::deactivation() {
		lock_variables();

		for(uint8_t index = 0; index < ID_MAX_SERVOS; ++index) {
			if(_servos[index] != nullptr) {
				_servos[index]->command = Servo::CommandSpeed(0, Rotation::CounterClockwise);
				_servos[index]->command_type = CommandType::Speed;
			}
		}

		_state_changed.exchange(true);
		unlock_variables();
	}
} // namespace PhysicalRobot
