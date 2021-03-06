//
// Created by terae on 05/02/19.
//

#include "NavigationInterfacer.h"

namespace Strategy::Interfacer {

	// Constructor
	NavigationInterfacer::NavigationInterfacer(std::shared_ptr<PhysicalRobot::Robot> robot, Environment& env, AvoidanceInterfacer& avoidance)
	        : _module(robot->get_module<interfaced_type>()), _avoidance(avoidance), _env(env) {

		logInfo("name: ", robot->name);
		push_linear_speed(GLOBAL_CONSTANTS()[robot->name].get_linear_speed());
		push_angular_speed(GLOBAL_CONSTANTS()[robot->name].get_angular_speed());
		push_linear_accuracy(GLOBAL_CONSTANTS()[robot->name].get_linear_accuracy());
		push_angular_accuracy(GLOBAL_CONSTANTS()[robot->name].get_angular_accuracy());
	}

	NavigationInterfacer::interfaced_type* NavigationInterfacer::operator->() {
		return &_module;
	}

	// States
	repere::Coordinates NavigationInterfacer::get_origin_coordinates() const {
		return _origin_coordinates;
	}

	void NavigationInterfacer::activate_asserv() {
		_module.set_asserv_enabled(true);
	}

	void NavigationInterfacer::deactivate_asserv() {
		_module.set_asserv_enabled(false);
	}

	Speed NavigationInterfacer::get_linear_speed() const {
		return peek_stack(_linear_speed);
	}

	void NavigationInterfacer::push_linear_speed(Speed speed) {
		push_stack(_linear_speed, speed);
		_module.update_linear_speed(speed);
	}

	Speed NavigationInterfacer::pop_linear_speed() {
		auto speed = pop_stack(_linear_speed);
		_module.update_linear_speed(speed);
		return speed;
	}

	AngularSpeed NavigationInterfacer::get_angular_speed() const {
		return peek_stack(_angular_speed);
	}

	void NavigationInterfacer::push_angular_speed(AngularSpeed speed) {
		push_stack(_angular_speed, speed);
		_module.update_angular_speed(speed);
	}

	AngularSpeed NavigationInterfacer::pop_angular_speed() {
		auto speed = pop_stack(_angular_speed);
		_module.update_angular_speed(speed);
		return speed;
	}

	Distance NavigationInterfacer::get_linear_accuracy() const {
		return peek_stack(_linear_accuracy);
	}

	void NavigationInterfacer::push_linear_accuracy(Distance accuracy) {
		push_stack(_linear_accuracy, accuracy);
		_module.update_linear_accuracy(accuracy);
	}

	Distance NavigationInterfacer::pop_linear_accuracy() {
		auto accuracy = pop_stack(_linear_accuracy);
		_module.update_linear_accuracy(accuracy);
		return accuracy;
	}

	Angle NavigationInterfacer::get_angular_accuracy() const {
		return peek_stack(_angular_accuracy);
	}

	void NavigationInterfacer::push_angular_accuracy(Angle accuracy) {
		push_stack(_angular_accuracy, accuracy);
		_module.update_angular_accuracy(accuracy);
	}

	Angle NavigationInterfacer::pop_angular_accuracy() {
		auto accuracy = pop_stack(_angular_accuracy);
		_module.update_angular_accuracy(accuracy);
		return accuracy;
	}

	// States
	Outcome NavigationInterfacer::move_to(repere::Coordinates destination, Duration timeout) {
		return move_to(destination, SensAdvance::Forward, timeout);
	}

	Outcome NavigationInterfacer::move_to(repere::Coordinates destination, SensAdvance sens, Duration timeout) {
		auto deadline = TimePoint::now() + timeout;

		// On récupère la Coordinates par rapport au robot
		Outcome res = this->follow_trajectory(this->compute_trajectory(destination, sens), deadline);

		if(res != Outcome::BLOCKED_BY_ADV) {
			if(res == Outcome::SUCCESS && _env.isForbidden(destination.getPos2D()))
				res = Outcome::POSITION_MODIFIED;
		} else {
#ifndef HOMOLOGATION
			sleep(1_s);
			logDebug("Adversary detected! Try to get around...");

			// Pour pouvoir se déplacer (ne plus avoir l'adversaire dans la tronche)
			// On tente de reculer.
			SensAdvance escapeSens = (sens == SensAdvance::Forward) ? SensAdvance::Backward : SensAdvance::Forward;
			Distance escapeRadius = this->compute_backup_distance(escapeSens, 20_cm);

			// Si on ne peut pas reculer assez : on se considère bloqué par adv.
			if(escapeRadius <= 1_cm) {
				return Outcome::BLOCKED_BY_ADV;
			}

			// Si on peut reculer : on le fait.
			res = this->forward(escapeRadius, escapeSens, timeout);


			if(res != Outcome::SUCCESS) {
				logDebug("Can't go backward.");
				return res;
			}

			// On retente d'aller à la destination avec le contournement.
			res = this->move_to(destination, sens, deadline - TimePoint::now());
#endif
		}

		return res;
	}

	Outcome NavigationInterfacer::internal_forward(Distance distance, SensAdvance sens, Duration timeout) {
		auto threshold = std::min(distance + GLOBAL_CONSTANTS()["primary"].get_radius_rotation() / 2, 30_cm);
		if(_avoidance.adversary_detected(threshold, sens)) {
			return Outcome::BLOCKED_BY_ADV;
		}

		TimePoint date_timeout = TimePoint::now() + timeout;
		Outcome result = Outcome::FAILURE;

		auto lock = get_lock_for_action(date_timeout);
		if(lock.owns_lock()) {
			_module.forward(distance, sens);
			result = wait_end_trajectory(get_check_moving_done(), date_timeout, true, true, sens);
		} else {
			logDebug3("Module not available");
		}

		return result;
	}

	Outcome NavigationInterfacer::forward(Distance distance, SensAdvance sens, Duration timeout) {
		// Attente que la position soit actualisée à coup sûr
		sleep(GLOBAL_CONSTANTS().get_frame_period());

		auto begin = TimePoint::now();

		auto initial_coords = _module.get_coordinates();
		auto initial_pos = initial_coords.getPos2D();
		auto initial_angle = initial_coords.getAngle();

		int reverser = (sens == SensAdvance::Forward ? 1 : -1);
		auto final_coords = initial_pos + reverser * distance * Vector2d{cos(initial_angle), sin(initial_angle)};

		// Le robot essaie d'avancer. S'il se retrouve bloqué par l'adversaire, il retente l'action
		// jusqu'à ce que le timer ait expiré.
		Outcome result;
		do {
			sleep(GLOBAL_CONSTANTS().get_frame_period());
			auto current_pos = _module.get_coordinates().getPos2D();
			distance = (final_coords - current_pos).norm();
			logDebug("Current pos: ", current_pos, " | dest: ", final_coords, " | left distance: ", distance);
			result = internal_forward(distance, sens, timeout - (TimePoint::now() - begin));

			if(result != Outcome::SUCCESS) {
				logWarn("Failed to forward to ", final_coords, ", sens: ", sens, ", result: ", result);
			}

			if(result == Outcome::BLOCKED) {
				break;
			}
		} while(result == Outcome::BLOCKED_BY_ADV && (TimePoint::now() - begin) <= timeout);

		if(result != Outcome::SUCCESS) {
			logWarn("Failed to forward to ", final_coords, ", sens: ", sens, ", result: ", result);
			_module.stop();
			sleep(100_ms);
		}

		logDebug("END OF NavigationInterfacer::forward(); STATUS = ", result);
		return result;
	}

	Outcome NavigationInterfacer::forward_infinity(SensAdvance sens, Duration timeout) {
		const static Distance infinity = 3_m; // Distance::makeFromMm(std::numeric_limits<int32_t>::max() / 10.0);
		return forward(infinity, sens, timeout);
	}

	Outcome NavigationInterfacer::turn_absolute(const repere::Orientation& angle, Duration timeout) {
		SensRotation sens = optimal_rotation_sens(_module.get_orientation(), angle);

		return turn_absolute(angle, sens, timeout);
	}

	Outcome NavigationInterfacer::turn_absolute(const repere::Orientation& angle,
	                                            Strategy::Interfacer::NavigationInterfacer::SensRotation sens,
	                                            Duration timeout) {
		TimePoint date_timeout = TimePoint::now() + timeout;
		auto lock = get_lock_for_action(date_timeout);
		if(lock.owns_lock()) {
			logDebug("Turn absolute ", angle.getAngle().toDeg(), " deg");
			_module.turn_absolute(angle.getAngle(), sens);
			return wait_end_trajectory(get_check_moving_done(), date_timeout, true, false);
		}
		logDebug3("Module not available");
		return Outcome::FAILURE;
	}

	Outcome NavigationInterfacer::turn_relative(Angle angle, Duration timeout, const repere::Repere& repere) {
		TimePoint date_timeout = TimePoint::now() + timeout;
		auto lock = get_lock_for_action(date_timeout);

		Angle real_angle = repere::ABSOLUTE_REFERENCE.convertDeltaAngle(repere, angle);

		if(lock.owns_lock()) {
			_module.turn_relative(real_angle);
			return wait_end_trajectory(get_check_moving_done(), date_timeout, true, false);
		}
		logDebug3("Module not available");
		return Outcome::FAILURE;
	}

	Outcome NavigationInterfacer::stop() {
		_module.stop();
		sleep(100_ms);
		return Outcome::SUCCESS;
	}

	Outcome NavigationInterfacer::emergency_stop() {
		_module.emergency_stop();
		sleep(100_ms);
		return Outcome::TIMEOUT;
	}

	NavigationInterfacer::SensRotation
	    NavigationInterfacer::optimal_rotation_sens(const repere::Orientation& from, const repere::Orientation& to) {
		return (to.getAngle() - from.getAngle()).toMinusPiPi() >= 0_rad ? SensRotation::Trigo : SensRotation::Clockwise;
	}

	Outcome NavigationInterfacer::recaling_top(SensAdvance sens, Distance y) {
		return recaling_helper(sens, y, std::make_pair(90_deg, -90_deg), false);
	}

	Outcome NavigationInterfacer::recaling_bottom(SensAdvance sens, Distance y) {
		return recaling_helper(sens, y, std::make_pair(-90_deg, 90_deg), false);
	}

	Outcome NavigationInterfacer::recaling_right(SensAdvance sens, Distance x) {
		return recaling_helper(sens, x, std::make_pair(0_deg, 180_deg), true);
	}

	Outcome NavigationInterfacer::recaling_left(SensAdvance sens, Distance x) {
		return recaling_helper(sens, x, std::make_pair(180_deg, 0_deg), true);
	}

	std::unique_lock<std::recursive_timed_mutex> NavigationInterfacer::try_get_lock_for_action() {
		auto lock = std::unique_lock<std::recursive_timed_mutex>(_mutex_action, std::defer_lock);
		lock.try_lock();
		return lock;
	}

	std::unique_lock<std::recursive_timed_mutex> NavigationInterfacer::get_lock_for_action(const TimePoint& try_until) {
		return std::unique_lock<std::recursive_timed_mutex>(_mutex_action, try_until.value());
	}

	std::unique_lock<std::recursive_timed_mutex> NavigationInterfacer::get_lock_for_action(std::defer_lock_t) {
		return std::unique_lock<std::recursive_timed_mutex>(_mutex_action, std::defer_lock);
	}

	std::function<bool()> NavigationInterfacer::get_check_moving_done() {
		return [this]() { return _module.is_moving_done(); };
	}

	std::function<bool()> NavigationInterfacer::get_check_accuracy_reached() {
		return [this]() { return _module.is_precision_reached(); };
	};

	Outcome NavigationInterfacer::wait_end_trajectory(const std::function<bool()>& condition_end_trajectory,
	                                                  const TimePoint& date_timeout,
	                                                  bool stop,
	                                                  bool check_adversary,
	                                                  SensAdvance sens) {
		for(;;) {
			// Timeout
			if(TimePoint::now() >= date_timeout) {
				return Outcome::TIMEOUT;
			}

			// Adversary
			if(check_adversary && _avoidance.adversary_detected(sens)) {
				this->stop();
				return Outcome::BLOCKED_BY_ADV;
			}

			auto lock = try_get_lock_for_action();
			// Module unavailable: trajectory not valid
			if(!lock.owns_lock()) {
				return Outcome::FAILURE;
			}

			// Blocking
			if(_module.is_physically_blocked()) {
				return Outcome::BLOCKED;
			}

			// Trajectory ended
			if(condition_end_trajectory()) {
				if(stop) {
					this->stop();
				}
				return Outcome::SUCCESS;
			}

			sleep(10_ms);
		}
	}

	NavigationInterfacer::Trajectory NavigationInterfacer::compute_trajectory(const repere::Coordinates& destination, SensAdvance sens) {

		using namespace repere;
		const Repere& ref_astar = GLOBAL_CONSTANTS().REFERENCE_ASTAR;

		// On récupère la trajectoire depuis l'environnement
		Coordinates origin = _module.get_coordinates();

		logDebug3("[compute_trajectory] from ", origin.getPos2D(), " to ", destination.getPos2D());

		// Récupération de la trajectoire calculée par l'A* en prenant en compte les adversaires
		auto adversary_shapes = _avoidance.update_environment(_env);

		if(!adversary_shapes.empty()) {
			std::stringstream sstream;

			for(auto& position : _avoidance.get_adversary_positions()) {
				sstream << position.getPos2D() << ", ";
			}
			logDebug4("[compute_trajectory] adversaries detected: ", sstream.str());
		}

		std::vector<Vector2m> trajectoryAstar =
		    _env.getTrajectory(origin.getPos2D(ref_astar), destination.getPos2D(ref_astar), 3_s);

		for(int adv_shape : adversary_shapes) {
			_env.removeDynamicShape(adv_shape);
		}

		// Dessin en TGA
		static int fileIndex = 0;
		fileIndex++;

		if(this->_write_astar) {
			// logDebug("Displaying trajectory for debug");
			// Ça flood le CAN ça, que en simu plz !
			// this->afficherTrajectoire(start.getPosition(), destination, trajectoryAstar);
			auto trajcpy = std::vector<Vector2m>(trajectoryAstar);
			trajcpy.emplace_back(origin.getPos2D(ref_astar)); // rajoute la position initiale dans le dessin
			_env.saveToTGA(("calcul" + std::to_string(fileIndex) + ".tga").c_str(), trajcpy);
		}

		// On calcule les angles (pour créer la trajectoire).
		Trajectory trajectory;

		for(int i = static_cast<int>(trajectoryAstar.size()) - 1; i >= 0; i--) {
			Vector2m pos;

			if(i == 0) {
				pos = origin.getPos2D(ref_astar);
			} else {
				pos = trajectoryAstar[i - 1];
			}

			Vector2m nextPos = trajectoryAstar[i];
			Angle nextAngle = i == 0 ? destination.getAngle() :
			                           Angle::makeFromValue(std::atan2((nextPos.y - pos.y).toM(), (nextPos.x - pos.x).toM()));

			Coordinates coords(nextPos, nextAngle, ref_astar);

			trajectory.push_back({coords, sens});
		}

		// Affichage de la trajectoire
		std::stringstream sstream;
		for(auto& traj_point : trajectory) {
			sstream << traj_point.coordinates.getPos2D() << " -> ";
		}
		sstream << "fin" << std::endl;
		logDebug4("Trajectoire : ", sstream.str());

		return trajectory;
	}

	Outcome NavigationInterfacer::follow_trajectory(Trajectory&& trajectory, TimePoint const& timeoutDate) {

		Outcome result = trajectory.empty() ? Outcome::FAILURE : Outcome::SUCCESS;

		while(!trajectory.empty()) {
			TrajectoryPoint nextPoint = trajectory.front();
			trajectory.pop_front();

			repere::Coordinates nextPos = nextPoint.coordinates;

			// repère ?
			logDebug("Passer par ", trajectory.empty() ? "(LAST): " : ": ", nextPos.getPos2D());
			result = go_to_point_straight(nextPos, nextPoint.sens, timeoutDate);

			// Si une erreur s'est produite, on abandonne
			if(result != Outcome::SUCCESS) {
				logWarn("failed : ", result);
				return result;
			}

			if(trajectory.empty()) {
				// Le robot tourne pour se positionner correctement
				logDebug("Repositionnement final: ", nextPos.getAngle().toDeg(), " deg");
				result = turn_absolute(nextPos.getAngle(), timeoutDate - TimePoint::now());
			}
		}

		return result;
	}

	Outcome NavigationInterfacer::go_to_point_straight(const repere::Coordinates& destination, SensAdvance sens, TimePoint const& date_timeout) {
		repere::Coordinates origin = _module.get_coordinates();
		Vector2m diff = destination.getPos2D() - origin.getPos2D();
		Angle direction = atan2(diff.y, diff.x);

		logDebug4("Position actuelle: ", origin);
		// Turn to direction
		auto lock = get_lock_for_action(date_timeout);

		Outcome result;
		if(lock.owns_lock()) {
			logDebug("Repositionnement vers ", direction.toDeg(), " deg");
			_module.turn_absolute(direction, optimal_rotation_sens(origin.getAngle(), direction));
			result = wait_end_trajectory(get_check_moving_done(), date_timeout);
		} else {
			logDebug6("Module is unavailable");
			result = Outcome::FAILURE;
		}

		logDebug4("Angle actuel: ", _module.get_coordinates().getAngle().toDeg(), " deg");
		if(result == Outcome::SUCCESS) {
			logDebug("Avancée de ", diff.norm());
			result = internal_forward(diff.norm(), sens, date_timeout - TimePoint::now());
		}

		return result;
	}

	Distance NavigationInterfacer::compute_backup_distance(SensAdvance escapeSens, Distance escapeRadius) {

		Distance escapeStep = 2_cm;
		repere::Coordinates coords = _module.get_coordinates();
		Angle currentAngle = coords.getAngle();
		Vector2m direction = Vector2m(Distance::makeFromM(cos(currentAngle)), Distance::makeFromM(sin(currentAngle)));

		if(escapeSens == SensAdvance::Backward)
			direction = -direction;

		// On teste plusieurs positions possibles.
		while(escapeRadius > 0_cm) {
			// On recule
			Vector2m escapePosition = coords.getPos2D() + direction * escapeRadius.toM();
			if(!_env.isForbidden(escapePosition)) {
				break;
			}
			escapeRadius -= escapeStep;
		}
		return escapeRadius;
	}

	Outcome NavigationInterfacer::recaling_helper(SensAdvance sens, Distance D, std::pair<Angle, Angle> angles, bool isX) {
		// Ces constantes permettent aux recallages d'etre immunisés à un palet farceur qui se glisserait entre le robot et un mur
		static constexpr Angle OFFSET_MAX_RECALING_ANGLE = 45_deg;
		static constexpr Distance OFFSET_MAX_RECALING_DISTANCE = 20_cm;

		Angle angle = (sens == SensAdvance::Forward ? angles.first : angles.second);
		Outcome result = turn_absolute(angle);
		if(result != Outcome::SUCCESS) {
			logError("Failed to turn at the good angle");
			return result;
		}

		push_linear_speed(get_linear_speed() / 2);
		result = forward_infinity(sens, 4_s);
		pop_linear_speed();

		/*if(result != Outcome::SUCCESS && result != Outcome::BLOCKED) {
		    logError("Failed to reach the recaling wall, result = ", result);
		    stop();
		    return result;
		}*/

		Vector2m old_pos = _module.get_position().getPos2D();
		Angle old_angle = _module.get_orientation().getAngle();

		Distance old_distance = (isX ? old_pos.x : old_pos.y);

		Distance variation_distance = abs(old_distance - D);
		Angle variation_angle = abs(old_angle - angle).toMinusPiPi();

		if(variation_distance > OFFSET_MAX_RECALING_DISTANCE) {
			logWarn("Problem while recaling the robot: the variation between the actual position and the expected "
			        "position in ",
			        (isX ? "X" : "Y"),
			        " is too important: ",
			        variation_distance);
			return Outcome::BLOCKED;
		}

		if(variation_angle > OFFSET_MAX_RECALING_ANGLE) {
			logWarn("Problem while recaling the robot: the variation between the actual angle and the expected "
			        "angle is too important: ",
			        variation_angle);
			return Outcome::BLOCKED;
		}

		if(isX) {
			_module.set_coordinates(repere::Coordinates({D, old_pos.y}, angle));
		} else {
			_module.set_coordinates(repere::Coordinates({old_pos.x, D}, angle));
		}
		logDebug4("Recalage de ", repere::Coordinates(old_pos, old_angle), " vers ", _module.get_coordinates());
		return Outcome::SUCCESS;
	}

} // namespace Strategy::Interfacer
