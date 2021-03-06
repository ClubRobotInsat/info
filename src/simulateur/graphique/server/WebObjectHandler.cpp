//
// Created by louis on 30/11/17.
//

#include "WebObjectHandler.h"

#include "../../core/Object3D.h"
#include "../../physique/box2d/PhysicalToSimulationUnits.h"
#include "SimulationToWebserver.h"


// *************** APPROXIMATIONS ********************

// Retourne true si 2 distances sont à peu près égales (0.1mm)
inline bool almostEquals(Distance d1, Distance d2) {
	return abs(d2 - d1) <= 0.1_mm;
}

// Retourne true si 2 vecteurs sont à peu près égaux (0.1mm)
inline bool vecAlmostEquals(Vector3m v1, Vector3m v2) {
	return almostEquals(v1.x, v2.x) && almostEquals(v1.y, v2.y) && almostEquals(v1.z, v2.z);
}

inline bool angAlmostEquals(Angle a1, Angle a2) {
	return abs(a1 - a2) <= 0.1_deg;
}

// Retourne true si 2 vecteurs sont à peu près égaux (0.1mm)
inline bool vecAngAlmostEquals(Vector3ang v1, Vector3ang v2) {
	return angAlmostEquals(v1.x, v2.x) && angAlmostEquals(v1.y, v2.y) && angAlmostEquals(v1.z, v2.z);
}

// **************************************************


WebObjectHandler::WebObjectHandler(WebGraphicalContext& context, int id)
        : _context(context), _id(id), _scale({1, 1, 1}), _created(true), _changed(false) {}

void WebObjectHandler::setAdditionalContent(JSON content) {
	_additionalContent = content;
}

Color3f WebObjectHandler::getColor() {
	return _color;
}

void WebObjectHandler::setColor(const Color3f& color) {
	_color = color;
}

void WebObjectHandler::setScale(const Vector3f& scale) {
	_scale = scale;
}

JSON WebObjectHandler::getCreationMessage() {
	// Partie qui envoie au serveur un nouvel objet
	JSON message = _additionalContent;
	message["id"] = _id;
	message["matrix"] = Json::toJSONMatrix(generateTransformationMatrix());
	message["angle"] = _rotation.z.toDeg();
	message["color"] = _color.toHexStr();
	return message;
}

JSON WebObjectHandler::getUpdateMessage() {
	JSON message;

	message["type"] = "move";
	message["matrix"] = Json::toJSONMatrix(generateTransformationMatrix());
	message["id"] = _id;

	return message;
}

JSON WebObjectHandler::getRemoveMessage() {
	JSON message;
	message["type"] = "remove";
	message["id"] = _id;
	return message;
}

void WebObjectHandler::update(Object3D& parent) {
	if(!vecAlmostEquals(parent.getPosition(), _position)) {
		_changed = true;
		_position = parent.getPosition();
	}
	if(!vecAngAlmostEquals(parent.getRotation(), _rotation)) {
		_changed = true;
		_rotation = parent.getRotation();
	}
}

Matrix4f WebObjectHandler::generateTransformationMatrix() {
	// Le webserver traite les cylindres de manière bizarre...
	if(_additionalContent["type"] == "newcylinder") {
		Vector3ang rotation = {90_deg, _rotation.z, 0_deg};
		return getTransformationMatrix(toWebServer(_position), toWebServer(rotation), _scale);
	}

	// Et les modèles aussi !
	if(_additionalContent["type"] == "newmodel") {
		Vector3ang rotation = {90_deg, _rotation.z + 90_deg, 0_deg};
		return getTransformationMatrix(toWebServer(_position), toWebServer(rotation), _scale);
	}

	return getTransformationMatrix(toWebServer(_position), toWebServer(_rotation), _scale);
}
