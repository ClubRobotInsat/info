//
// Created by paul on 18/08/16.
//

#include "CanListener.h"
#include "Monitor.h"
#include "../../commun/Units/TimePoint.h"

CanListener::CanListener(std::string& port) : _shallStopListening(false), _refreshRate(20_ms), _buffer(10000) {

    if (port.substr(0,5) == "TCPIP") {
        _can.reset(new Commun::CAN(std::make_unique<Commun::TCPIP>(port.substr(7,port.length()-12), 1234)));
        std::cout << "CAN listening on : " << port.substr(7,port.length()-12) << ":1234" <<std::endl;
    }
    else {
        std::cout << "CAN listening on : " << "/dev" + port << std::endl;
        _can.reset(new Commun::CAN(std::make_unique<Commun::RS232>("/dev/" + port)));
    }

    _can->setTemporisation(10_ms);

}

void CanListener::start(Monitor* caller) {

    this->mainLoop(caller);

}

void CanListener::mainLoop(Monitor* caller) {

    while(!this->shallStopListening()) {
        auto now = Units::TimePoint::now();
        while (Units::TimePoint::now() < now + _refreshRate) {
            auto Trame = this->waitForMessage();
            std::lock_guard<std::mutex> lock(_mutex);
            _buffer.addMessage(Trame);
        }
        caller->notify();
    }
}

Trame CanListener::waitForMessage() {

    auto Trame = _can->recevoirTrame();
    return Trame;

}

Trame CanListener::getOldestMessage() {
    std::lock_guard<std::mutex> lock(_mutex);
    Trame result = _buffer.retrieveMessageMatchingFilter(_filter, true);
    return result;
}

bool CanListener::shallStopListening() {
    std::lock_guard<std::mutex> lock(_mutex);
    return _shallStopListening;
}

void CanListener::setFilter(std::string newfilter) {

    _filter = newfilter;

}
