// MathToolbox.h

#ifndef MATH_TOOLBOX_H
#define MATH_TOOLBOX_H

#include "MathUtilities.h"
#include "Matrix3.h"
#include "Matrix4.h"
#include "Vector.h"

#include "Units/Units.h"

typedef Vector2<Units::Distance> Vector2m;
typedef Vector3<Units::Distance> Vector3m;
typedef Vector4<Units::Distance> Vector4m;

typedef Vector2<Units::Angle> Vector2ang;
typedef Vector3<Units::Angle> Vector3ang;

/** Retourne la translation qui permet de donner l'impression
 * de faire tourner un élément autour du centreVoulu alors
 * qu'il tourne autour du centreAbsolu pour la rotation décrite
 * par axeRotation et angleRotation.
 * Il faut donc faire tourner la pièce, puis déterminer la translation
 * par cette fonction, et l'appliquer à la pièce.
 */
Vector3m translationPourRotationExcentree(Vector3m centreAbsolu, Vector3m centreVoulu, Vector3d axeRotation, Units::Angle angleRotation);

/** Retourne la matrice de transformation qui applique la
 * translation, la rotation et la scale voulue. */
Matrix4f getTransformationMatrix(const Vector3f& pos, const Vector3f& rot, const Vector3f& scale);

// transforme un vecteur (x, y, z) en vecteur (x, y)
Vector2m toVec2(Vector3m vec3);

Vector3m toVec3(Vector2m vec2);

/**
 * Retourne vrai si la différence angulaire entre l'angle a1
 * et l'angle 'a2' est inférieure à l'angle 'threshold'.
 *
 */
bool equals(Units::Angle a1, Units::Angle a2, Units::Angle threshold);

/*
 * Retourne vrai si la différence linéaire entre les distances a et
 * b est inférieure à la distance 'threshold'
 */
bool distancesEquals(Vector2m a, Vector2m b, Units::Distance threshold);

#endif // MATH_TOOLBOX_H
