#include "PanelConnect.h"

#include <sstream>
#include <vector>

#include "resources/EmbeddedFiles.h"

PanelConnect::PanelConnect() {
	_builder = Gtk::Builder::create_from_string(EmbeddedFiles::readText("glade/connection-panel.glade"));
	_builder->get_widget("connection-type", _connectionType);
	_builder->get_widget("connection-arguments", _connectionArguments);
	_builder->get_widget("connect-button", _connectButton);
	Widget *widget;
	_builder->get_widget("root", widget);
	add(*widget);
}

Gtk::Button& PanelConnect::getConnectButton() {
	return *_connectButton;
}

std::string PanelConnect::getConnectionType() {
	return _connectionType->get_active_text();
}

// TODO faire de cette fonction une fonction utilitaire
std::vector<std::string> PanelConnect::getConnectionArguments() {
	std::vector<std::string> args;

	std::string parsed = _connectionArguments->get_text();
	std::ostringstream outputStream;
	int state = 0;

	for(char c : parsed) {
		switch(state) {
			case 0: // reading normally
				if(c == ' ') {
					if(outputStream.str().length()) {
						args.push_back(outputStream.str());
						outputStream = std::ostringstream();
					}
				} else if(c == '"') {
					state = 1;
				} else if(c == '\\') {
					state = 2;
				} else {
					outputStream << c;
				}
				break;
			case 1: // reading quoted
				if(c == '"') {
					state = 0;
				} else {
					outputStream << c;
				}
				break;
			case 2: // reading escaped
				outputStream << c;
				state = 0;
			default:
				break;
		}
	}

	if(outputStream.str().length() != 0)
		args.push_back(outputStream.str());

	return args;
}