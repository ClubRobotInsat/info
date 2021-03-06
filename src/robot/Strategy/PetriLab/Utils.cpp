//
// Created by terae on 01/02/19.
//

#include "Utils.h"
#include "../AbstractStrategy.h"

namespace {
	Strategy::AbstractStrategy* _strategy;
} // namespace

void init_petri_utils(Strategy::AbstractStrategy& strategy) {
	_strategy = &strategy;
}

bool color_purple() {
	return _strategy->get_color() == Constants::RobotColor::Purple;
}

bool color_yellow() {
	return _strategy->get_color() == Constants::RobotColor::Yellow;
}

bool color_undef() {
	return _strategy->get_color() == Constants::RobotColor::Undef;
}

Outcome set_points(int points) {
	_strategy->set_points(points);
	return Outcome::SUCCESS;
}

Outcome add_points(int points) {
	_strategy->add_points(points);
	return Outcome::SUCCESS;
}

int get_points() {
	return _strategy->get_points();
}

Duration get_left_time() {
	return _strategy->get_left_time();
}
