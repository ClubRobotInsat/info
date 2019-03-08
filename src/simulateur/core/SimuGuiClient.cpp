#include "SimuGuiClient.h"

#include "Simulateur.h"

using Constants::RobotColor;

SimuGuiClient::SimuGuiClient(Simulateur& simulator) : _simu(simulator) {}

SimuGuiClient::~SimuGuiClient() = default;

void SimuGuiClient::connect(const ConnectionData& connectionData) {
	logDebug("Connecting to robot with : ", connectionData.method);

	if(connectionData.method == "UDP") {
		if(connectionData.parameters.size() < 3) {
			throw std::invalid_argument("Usage : UDP [localhost] [local port] [remote port]");
		}
		std::string host = connectionData.parameters[0];
		int localPort = std::stoi(connectionData.parameters[1]);
		int remotePort = std::stoi(connectionData.parameters[2]);

		std::shared_ptr<Communication::Protocol> protocol =
		    std::make_shared<Communication::protocol_udp>(host, localPort, remotePort);
		_simu._robot->connect(protocol);
	}
}

void SimuGuiClient::createIAProcess(const IAProcessData& iaProcessData, const ConnectionData& connectionData) {
	logDebug("Launching process : ", iaProcessData.executablePath);
}

void SimuGuiClient::testNavigationForward(Distance distance) {
	if(_simu._robot != nullptr) {
		auto& controller = _simu._robot->getController();
		controller.forward(distance);
	}
}

void SimuGuiClient::testNavigationTurn(Angle angle) {
	if(_simu._robot != nullptr) {
		auto& controller = _simu._robot->getController();
		controller.turn(angle);
	}
}

void SimuGuiClient::reset(const ResetData& resetData) {
	RobotColor color = fromString<RobotColor>(resetData.color);
	_simu.resetWorld();
}

std::vector<std::string> SimuGuiClient::getRobotColors() const {
	std::vector<std::string> result;

	for(RobotColor robotColor : getEnumValues<RobotColor>()) {
		result.emplace_back(toString(robotColor));
	}

	return result;
}
