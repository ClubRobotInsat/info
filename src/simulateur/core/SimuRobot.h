#ifndef ROOT_SIMUROBOT_H
#define ROOT_SIMUROBOT_H

#include <memory>

#include "Object3D.h"
#include "../communication/SimuCommunicator.h"
#include "../../robot/Modules/ModuleManager.h"
#include "../../robot/Communication/Communicator.h"

namespace Simu {

class SimuRobot {
public:
    explicit SimuRobot(const std::string &name, Object3D &robotObject);

    void connect(const std::shared_ptr<Communication::Protocol> &protocol);

    void update();

private:
    std::string _name;
    Object3D &_robotObject;

    std::shared_ptr<PhysicalRobot::ModuleManager> _moduleMgr;
    std::shared_ptr<SimuCommunicator> _communicator;

    void assignModules();
};

};

#endif //ROOT_SIMUROBOT_H
