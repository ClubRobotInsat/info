//
// Created by paul on 18/07/16.
//

// Lib nécessaires pour la compilation : libgtkmm-3.0-dev

#include "Application.h"
#include "Header.h"

int main(int argc, char** argv) {
	if(!Glib::thread_supported())
		Glib::thread_init();
	Application app(argc, argv, "club.robot.can");
}
