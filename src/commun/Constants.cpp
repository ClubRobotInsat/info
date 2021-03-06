//
// Created by terae on 23/09/18.
//

#include "Constants.h"
#include "resources/EmbeddedFiles.h"

#include <log/Log.h>

const Constants::Constants& GLOBAL_CONSTANTS() {
	const static Constants::Constants CONSTANTS(EmbeddedFiles::readText("../robot.ini"));
	return CONSTANTS;
}

namespace Constants {

	int read_field(IniFile& file, const std::string& section, const std::string& field, int default_value) {
		auto it = file[section];
		if(it.has(field)) {
			return it[field].asInt();
		} else {
			return default_value;
		}
	}

	std::string read_field(IniFile& file, const std::string& section, const std::string& field, const std::string& default_value) {
		auto it = file[section];
		if(it.has(field)) {
			return it[field].asString();
		} else {
			return default_value;
		}
	}

	RobotInitializationData::RobotInitializationData(IniFile& reader, std::string name) {
		const std::string section = "robot." + name;
		try {
			_start_angle = Angle::makeFromDeg(reader[section]["angle"].asDouble());
			_start_position = {
			    Distance::makeFromMm(reader[section]["position_x"].asInt()),
			    Distance::makeFromMm(reader[section]["position_y"].asInt()),
			    Distance::makeFromMm(reader[section]["position_z"].asInt()),
			};
		} catch(std::exception&) {
			_start_angle = std::nullopt;
			_start_position = std::nullopt;
		}

		for(auto it : reader) {
			if(it.first == section + ".modules") {
				for(const auto& field : it.second) {
					_modules[field.first] = static_cast<uint8_t>(field.second.asInt());
				}
				break;
			}
		}

		_linear_speed = Speed::makeFromMm_s(read_field(reader, "robot", "linear_speed", 400));
		_angular_speed = AngularSpeed::makeFromRad_s(read_field(reader, "robot", "angular_speed", 4));
		_linear_accuracy = Distance::makeFromMm(read_field(reader, "robot", "linear_accuracy", 40));
		_angular_accuracy = Angle::makeFromDeg(read_field(reader, "robot", "angular_accuracy", 4));
		_angle_adversary_detection = Angle::makeFromDeg(read_field(reader, "robot", "angle_adversary_detection", 70));

		_turret_position = {
		    Distance::makeFromMm(read_field(reader, section, "turret_position_x", 0)),
		    Distance::makeFromMm(read_field(reader, section, "turret_position_y", 0)),
		};

		_radius_rotation = Distance::makeFromMm(read_field(reader, section, "radius_rotation", 250));

		_size = {
		    Distance::makeFromMm(read_field(reader, section, "size_x", 300)),
		    Distance::makeFromMm(read_field(reader, section, "size_y", 300)),
		    Distance::makeFromMm(read_field(reader, section, "size_z", 420)),
		};

		std::string lidar_type = read_field(reader, section, "lidar_type", "any");
		lidar_type = lidar_type.substr(0, lidar_type.find_first_of(' '));
		if(lidar_type == "any") {
			_lidar_type = Lidar::Any;
		} else if(lidar_type == "sick") {
			_lidar_type = Lidar::Sick;
		} else if(lidar_type == "hokuyo") {
			_lidar_type = Lidar::Hokuyo;
		} else {
			if(lidar_type != "none") {
				logWarn("Impossible to parse the 'lidar_type' field: considering that no lidar is conencted.");
			}
			_lidar_type = Lidar::None;
		}

		_protocol_type = read_field(reader, section, "protocol_type", "null");
		_protocol_type = _protocol_type.substr(0, _protocol_type.find_first_of(' '));

		for(auto it : reader) {
			if(it.first == section + ".communication") {
				for(const auto& field : it.second) {
					_communication_arguments[field.first] = field.second.asString();
				}
				break;
			}
		}

		if(_modules.find("servos") != _modules.cend()) {
			const std::string servos = section + ".servos";
			const uint8_t NBR = read_field(reader, servos, "nbr", 0);
			for(uint8_t i = 0; i < NBR; ++i) {
				_list_id_servos.push_back(reader[servos][std::to_string(i) + "_id"].asInt());
			}
		}
	}

	Constants::Constants(const std::string& ini_string) : _reader(IniFile('=', '#')) {
		std::stringstream ss;
		ss << ini_string;
		_reader.decode(ss);

		for(auto it : _reader) {
			if(it.first == "robot.list") {
				for(const auto& robot : it.second) {
					if(robot.second.asBool()) {
						_robots[robot.first] =
						    std::unique_ptr<RobotInitializationData>(new RobotInitializationData(_reader, robot.first));
					}
				}
				break;
			}
		}
		_robots["default"] = std::unique_ptr<RobotInitializationData>(new RobotInitializationData(_reader, "default"));

		const std::string section = "constants";

		_TCPIP_port_simu = static_cast<uint16_t>(read_field(_reader, section, "TCPIP_port", 4321));
		_table_size = {
		    Distance::makeFromMm(read_field(_reader, section, "table_x", 3000)),
		    Distance::makeFromMm(read_field(_reader, section, "table_y", 2000)),
		    0_m,
		};
		_scale_environment = Distance::makeFromMm(read_field(_reader, section, "scale_environment", 10));
		_table_env_grid = {
		    static_cast<uint16_t>(_table_size.x / _scale_environment),
		    static_cast<uint16_t>(_table_size.y / _scale_environment),
		};

		_match_duration = Duration::makeFromS(read_field(_reader, section, "match_duration", 100));

		_communication_delay = Duration::makeFromMs(read_field(_reader, section, "communication_delay", 3));
		_frame_period = Duration::makeFromMs(read_field(_reader, section, "frame_period", 100));
		_lidar_actualization_period = Duration::makeFromMs(read_field(_reader, section, "lidar_actualization_period", 300));
		_threshold_adversary_detection =
		    Distance::makeFromMm(read_field(_reader, section, "threshold_adversary_detection", 500));
	}

	const RobotInitializationData& Constants::operator[](const std::string& name) const {
		auto it = _robots.find(name);
		if(it == _robots.cend()) {
			throw std::runtime_error("Constants of the robot '" + name + "' does not exist.");
		}
		return *it->second;
	}
} // namespace Constants
