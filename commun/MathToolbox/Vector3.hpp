// Vector3.hpp
// Vector en 3 dimensions
// 2008 par Funto
// Version 1.0

#include <cmath>

template <class T>
Vector3<T>::Vector3() : x(T(0)), y(T(0)), z(T(0)) {
	
}

template <class T>
Vector3<T>::Vector3(const T& x, const T& y, const T& z) : x(x), y(y), z(z) {
	
}

template <class T>
template <class T_scalar>
Vector3<T>::Vector3(const T_scalar& s) : x(s), y(s), z(s) {
	
}

template <class T>
template <class T_scalar>
Vector3<T>::Vector3(T_scalar const *s) : x(s[0]), y(s[1]), z(s[2]) {
}

template <class T>
Vector3<T>::Vector3(const Vector3<T>& ref) : x(ref.x), y(ref.y), z(ref.z) {
	
}

template <class T>
template <class T_2>
Vector3<T>::Vector3(const Vector3<T_2>& ref) : x(ref.x), y(ref.y), z(ref.z) {
	
}

// Affectation suivant un autre vecteur
template <class T>
template <class T_2>
Vector3<T>& Vector3<T>::operator=(const Vector3<T_2>& ref) {
	this->x = ref.x;
	this->y = ref.y;
	this->z = ref.z;
	return *this;
}

// Affectation suivant un autre vecteur du même type (sinon, ça compile pas...)
template <class T>
Vector3<T>& Vector3<T>::operator=(const Vector3<T>& ref) {
	this->x = ref.x;
	this->y = ref.y;
	this->z = ref.z;
	return *this;
}

// Affectation suivant un scalaire
template <class T>
template <class T_scalar>
Vector3<T>& Vector3<T>::operator=(const T_scalar& s) {
	this->x = s;
	this->y = s;
	this->z = s;
	return *this;
}

// Affectation-addition
template <class T>
template <class T_2>
Vector3<T>& Vector3<T>::operator+=(const Vector3<T_2>& v) {
	this->x += v.x;
	this->y += v.y;
	this->z += v.z;
	return *this;
}

// Affectation-soustraction
template <class T>
template <class T_2>
Vector3<T>& Vector3<T>::operator-=(const Vector3<T_2>& v) {
	this->x -= v.x;
	this->y -= v.y;
	this->z -= v.z;
	return *this;
}

// Affectation-produit vectoriel
template <class T>
template <class T_2>
Vector3<T>& Vector3<T>::operator^=(const Vector3<T_2>& v) {
	T old_x = this->x;
	T old_y = this->y;
	
	this->x = this->y * v.z - this->z * v.y;
	this->y = - (old_x * v.z - this->z * v.x);
	this->z = old_x * v.y - old_y * v.x;
	
	return *this;
}

// Affectation-multiplication par un scalaire
template <class T>
template <class T_scalar>
Vector3<T>& Vector3<T>::operator*=(const T_scalar& s) {
	this->x *= s;
	this->y *= s;
	this->z *= s;
	return *this;
}

// Affectation-division par un scalaire
template <class T>
template <class T_scalar>
Vector3<T>& Vector3<T>::operator/=(const T_scalar& s) {
	this->x /= s;
	this->y /= s;
	this->z /= s;
	return *this;
}

// Négation
template <class T>
inline Vector3<T> Vector3<T>::operator-() const {
	return Vector3<T>(-this->x, -this->y, -this->z);
}

// Addition
template <class T>
template <class T_2>
Vector3<T> Vector3<T>::operator+(const Vector3<T_2>& v) const {
	return Vector3<T>(this->x + v.x, this->y + v.y, this->z + v.z);
}

// Soustraction
template <class T>
template <class T_2>
Vector3<T> Vector3<T>::operator-(const Vector3<T_2>& v) const {
	return Vector3<T>(this->x - v.x, this->y - v.y, this->z - v.z);
}

// Produit vectoriel
template <class T>
template <class T_2>
Vector3<T> Vector3<T>::operator^(const Vector3<T_2>& v) const {
	Vector3<T> resultat;
	resultat.x = this->y * v.z - this->z * v.y;
	resultat.y = - (this->x * v.z - this->z * v.x);
	resultat.z = this->x * v.y - this->y * v.x;
	return resultat;
}

// Multiplication par un scalaire
template <class T>
template <class T_scalar>
Vector3<T> Vector3<T>::operator*(const T_scalar& s) const {
	return Vector3<T>(this->x * s, this->y * s, this->z * s);
}

// Division par un scalaire
template <class T>
template <class T_scalar>
Vector3<T> Vector3<T>::operator/(const T_scalar& s) const {
	return Vector3<T>(this->x / s, this->y / s, this->z / s);
}

// Produit scalaire
template <class T>
T Vector3<T>::operator*(const Vector3<T>& v) const {
	return this->x * v.x + this->y * v.y + this->z * v.z;
}

// Normalisation + renvoi de la valeur de la norme
template <class T>
void Vector3<T>::normalize() {
	double norm = sqrtf(x*x + y*y + z*z);
	x = T(double(x) / norm);
	y = T(double(y) / norm);
	z = T(double(z) / norm);
}

// Calcul de la norme
template <class T>
double Vector3<T>::norm() const {
	return sqrtf(x*x + y*y + z*z);
}

// Calcul de la norme au carré (plus rapide)
template <class T>
T Vector3<T>::squaredNorm() const {
	return x*x + y*y + z*z;
}

// Multiplication par un scalaire (s * v)
template <class T, class T_scalar>
Vector3<T> operator*(const T_scalar& s, const Vector3<T>& v) {
	return Vector3<T>(v.x * s, v.y * s, v.z * s);
}

// Division par un scalaire (s / v)
template <class T, class T_scalar>
Vector3<T> operator/(const T_scalar& s, const Vector3<T>& v) {
	return Vector3<T>(v.x / s, v.y / s, v.z / s);
}
