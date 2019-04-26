//
// Created by terae on 05/02/19.
//

#include "NavigationInterfacer.h"

namespace Strategy {
	namespace Interfacer {

		// Constructor
		NavigationInterfacer::NavigationInterfacer(std::shared_ptr<PhysicalRobot::Robot> robot, Environment& env, AvoidanceInterfacer& avoidance)
		        : _module(robot->get_module<interfaced_type>()), _avoidance(avoidance), _env(env) {
			push_linear_speed(GLOBAL_CONSTANTS()[Constants::name(robot->name)].get_linear_speed());
			push_angular_speed(GLOBAL_CONSTANTS()[Constants::name(robot->name)].get_angular_speed());
			push_linear_accuracy(GLOBAL_CONSTANTS()[Constants::name(robot->name)].get_linear_accuracy());
			push_angular_accuracy(GLOBAL_CONSTANTS()[Constants::name(robot->name)].get_angular_accuracy());
		}

		// States
		repere::Coordinates NavigationInterfacer::get_origin_coordinates() const {
			return _origin_coordinates;
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
		ActionResult NavigationInterfacer::move_to(repere::Coordinates destination, Duration timeout) {
			return move_to(destination, SensAdvance::Forward, timeout);
		}

		ActionResult NavigationInterfacer::move_to(repere::Coordinates destination, SensAdvance sens, Duration timeout) {
			auto deadline = TimePoint::now() + timeout;

			// On récupère la Coordinates par rapport au robot
			ActionResult res = this->follow_trajectory(this->compute_trajectory(destination, sens), deadline);

			if(res != ActionResult::BLOCKED_BY_ADV) {
				if(res == ActionResult::SUCCESS && _env.isForbidden(destination.getPos2D()))
					res = ActionResult::POSITION_MODIFIED;
			} else {
#ifndef HOMOLOGATION
				sleep(1_s);
				logDebug("Adversary detected! Try to get around...");

				// Pour pouvoir se déplacer (ne plus avoir l'adversaire dans la tronche)
				// On tente de reculer.
				SensAdvance escapeSens = (sens == SensAdvance::Forward) ? SensAdvance::Backward : SensAdvance::Forward;
				Distance escapeRadius = this->compute_backup_distance(escapeSens, 20_cm);

				// Si pn ne peut pas reculer assez : on se considère bloqué par adv.
				if(escapeRadius <= 1_cm) {
					return ActionResult::BLOCKED_BY_ADV;
				}

				// Si on peut reculer : on le fait.
				res = this->forward(escapeRadius, escapeSens, timeout);


				if(res != ActionResult::SUCCESS) {
					logDebug("Can't go backward.");
					return res;
				}

				// On retente d'aller à la destination avec le contournement.
				res = this->move_to(destination, sens, deadline - TimePoint::now());
#endif
			}

			return res;
		}

		ActionResult NavigationInterfacer::internal_forward(Distance distance, SensAdvance sens, Duration timeout) {
			auto threshold = std::min(distance + GLOBAL_CONSTANTS()["primary"].get_radius_rotation() / 2, 30_cm);
			if(_avoidance.adversary_detected(threshold, sens)) {
				return ActionResult::BLOCKED_BY_ADV;
			}

			TimePoint date_timeout = TimePoint::now() + timeout;
			ActionResult result = ActionResult::FAILURE;

			auto lock = get_lock_for_action(date_timeout);
			if(lock.owns_lock()) {
				_module.forward(distance, sens);
				result = wait_end_trajectory(get_check_moving_done(), date_timeout, true, true, sens);
			} else {
				logDebug3("Module not available");
			}

			return result;
		}

		ActionResult NavigationInterfacer::forward(Distance distance, SensAdvance sens, Duration timeout) {
			// Attente que la position soit actualisée à coup sûr
			sleep(GLOBAL_CONSTANTS().get_frame_period());

			auto begin = TimePoint::now();

			auto initial_coords = _module.get_coordinates();
			auto ref = _module.get_reference();
			auto initial_pos = initial_coords.getPos2D(ref);
			auto initial_angle = initial_coords.getAngle(ref);

			int reverser = (sens == SensAdvance::Forward ? 1 : -1);
			auto final_coords = initial_pos + reverser * distance * Vector2d{cos(initial_angle), sin(initial_angle)};

			// Le robot essaie d'avancer. S'il se retrouve bloqué par l'adversaire, il retente l'action
			// jusqu'à ce que le timer ait expiré.
			ActionResult result;
			do {
				sleep(GLOBAL_CONSTANTS().get_frame_period());
				auto current_pos = _module.get_coordinates().getPos2D(ref);
				distance = (final_coords - current_pos).norm();
				logDebug("Current pos: ", current_pos, " | dest: ", final_coords, " | left distance: ", distance);
				result = internal_forward(distance, sens, timeout - (TimePoint::now() - begin));

				if(result != ActionResult::SUCCESS) {
					logWarn("Failed to forward to ", final_coords, ", sens: ", sens, ", result: ", result);
				}

				if(result == ActionResult::BLOCKED) {
					break;
				}
			} while(result == ActionResult::BLOCKED_BY_ADV && (TimePoint::now() - begin) <= timeout);

			if(result != ActionResult::SUCCESS) {
				logWarn("Failed to forward to ", final_coords, ", sens: ", sens, ", result: ", result);
			}

			logDebug("END OF NavigationInterfacer::forward(); STATUS = ", result);
			return result;
		}

		ActionResult NavigationInterfacer::turn_absolute(repere::Orientation angle, Duration timeout) {
			SensRotation sens = optimal_rotation_sens(_module.get_orientation(), angle);

			return turn_absolute(angle, sens, timeout);
		}

		ActionResult NavigationInterfacer::turn_absolute(repere::Orientation angle,
		                                                 Strategy::Interfacer::NavigationInterfacer::SensRotation sens,
		                                                 Duration timeout) {
			TimePoint date_timeout = TimePoint::now() + timeout;
			auto lock = get_lock_for_action(date_timeout);
			if(lock.owns_lock()) {
				_module.turn_absolute(angle.getAngle(), sens);
				return wait_end_trajectory(get_check_moving_done(), date_timeout, true, false);
			}
			logDebug3("Module not available");
			return ActionResult::FAILURE;
		}

		ActionResult NavigationInterfacer::turn_relative(Angle angle, Duration timeout) {
			TimePoint date_timeout = TimePoint::now() + timeout;
			auto lock = get_lock_for_action(date_timeout);
			if(lock.owns_lock()) {
				// FIXME: elecs get a relative value or an absolute one?
				_module.turn_relative(angle);
				return wait_end_trajectory(get_check_moving_done(), date_timeout, true, false);
			}
			logDebug3("Module not available");
			return ActionResult::FAILURE;
		}

		ActionResult NavigationInterfacer::stop() {
			_module.stop();
			sleep(100_ms);
			return ActionResult::SUCCESS;
		}

		ActionResult NavigationInterfacer::emergency_stop() {
			_module.emergency_stop();
			sleep(100_ms);
			return ActionResult::TIMEOUT;
		}

		NavigationInterfacer::SensRotation
		    NavigationInterfacer::optimal_rotation_sens(repere::Orientation from, repere::Orientation to) {
			return (to.getAngle() - from.getAngle()).toMinusPiPi() >= 0_rad ? SensRotation::Trigo : SensRotation::Clockwise;
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

		ActionResult NavigationInterfacer::wait_end_trajectory(const std::function<bool()>& condition_end_trajectory,
		                                                       TimePoint date_timeout,
		                                                       bool stop,
		                                                       bool check_adversary,
		                                                       SensAdvance sens) {
			for(;;) {
				// Timeout
				if(TimePoint::now() >= date_timeout) {
					return ActionResult::TIMEOUT;
				}

				// Adversary
				if(check_adversary && _avoidance.adversary_detected(sens)) {
					this->emergency_stop();
					return ActionResult::BLOCKED_BY_ADV;
				}

				auto lock = try_get_lock_for_action();
				// Module unavailable: trajectory not valid
				if(!lock.owns_lock()) {
					return ActionResult::FAILURE;
				}

				// Blocking
				if(_module.is_physically_blocked()) {
					return ActionResult::BLOCKED;
				}

				// Trajectory ended
				if(condition_end_trajectory()) {
					if(stop) {
						this->stop();
					}
					return ActionResult::SUCCESS;
				}

				sleep(10_ms);
			}
		}

		NavigationInterfacer::Trajectory
		    NavigationInterfacer::compute_trajectory(const repere::Coordinates& destination, SensAdvance sens) {

			using namespace repere;
			const Repere& ref_astar = GLOBAL_CONSTANTS().REFERENCE_ASTAR;
			const Repere& ref = _module.get_reference();

			// On récupère la trajectoire depuis l'environnement
			Coordinates origin = _module.get_coordinates();

			logDebug3("[compute_trajectory] from ", origin.getPos2D(ref), " to ", destination.getPos2D(ref));

			// Récupération de la trajectoire calculée par l'A* en prenant en compte les adversaires
			auto adversary_shapes = _avoidance.update_environment(_env);

			if(!adversary_shapes.empty()) {
				std::stringstream sstream;

				for(auto& position : _avoidance.get_adversary_positions()) {
					sstream << position.getPos2D(ref) << ", ";
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

			for(int i = (int)trajectoryAstar.size() - 1; i >= 0; i--) {
				Vector2m pos;

				if(i == 0) {
					pos = origin.getPos2D(ref_astar);
				} else {
					pos = trajectoryAstar[i - 1];
				}

				Vector2m nextPos = trajectoryAstar[i];

				Coordinates coords(nextPos,
				                   Angle::makeFromValue(std::atan2((nextPos.y - pos.y).toM(), (nextPos.x - pos.x).toM())),
				                   ref_astar);

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

		ActionResult NavigationInterfacer::follow_trajectory(Trajectory&& trajectory, TimePoint const& timeoutDate) {

			ActionResult result = trajectory.empty() ? ActionResult::FAILURE : ActionResult::SUCCESS;

			while(!trajectory.empty()) {
				TrajectoryPoint nextPoint = trajectory.front();
				trajectory.pop_front();

				repere::Coordinates nextPos = nextPoint.coordinates;

				// repère ?
				logDebug("Passer par ", trajectory.empty() ? "(LAST): " : ": ", nextPos.getPos2D());
				result = go_to_point_straight(nextPos, nextPoint.sens, timeoutDate);

				// Si une erreur s'est produite, on abandonne
				if(result != ActionResult::SUCCESS) {
					logWarn("[follow_trajectory] failed : ", result);
					return result;
				}

				if(trajectory.empty()) {
					// Le robot tourne pour se positionner correctement
					result = turn_absolute(nextPos.getAngle(), timeoutDate - TimePoint::now());
				}
			}

			return result;
		}

		ActionResult NavigationInterfacer::go_to_point_straight(const repere::Coordinates& destination,
		                                                        SensAdvance sens,
		                                                        TimePoint const& date_timeout) {
			repere::Coordinates origin = _module.get_coordinates();
			auto ref = _module.get_reference();
			Vector2m diff = destination.getPos2D(ref) - origin.getPos2D(ref);

			// FIXME cette valeur peut être fausse suivant le repère qu'on utilise.
			Angle direction = atan2(diff.y, diff.x);

			// Turn to direction
			auto lock = get_lock_for_action(date_timeout);

			ActionResult result;
			if(lock.owns_lock()) {
				_module.turn_absolute(direction, optimal_rotation_sens(origin.getAngle(ref), direction));
				result = wait_end_trajectory(get_check_moving_done(), date_timeout);
			} else {
				logDebug6("Module is unavailable");
				result = ActionResult::FAILURE;
			}

			if(result == ActionResult::SUCCESS) {
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
	} // namespace Interfacer
} // namespace Strategy
