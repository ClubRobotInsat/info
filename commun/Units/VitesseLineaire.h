//
//  VitesseLineaire.h
//
//  Created by Rémi on 17/01/2015.
//

#ifndef Club_Robot_VitesseLineaire_h
#define Club_Robot_VitesseLineaire_h

#include "Unit.h"

using VitesseLineaire = Unit<0, 1, -1, true>;

/**
 * Classe représentant une grandeur de vitesse linéaire.
 */
template<>
class Unit<0, 1, -1, true> : public Unit<0, 1, -1, false> {
	friend class Unit<0, 1, -1, false>;
public:
	using Unit<0, 1, -1, false>::ValueType;
	using Type = Unit<0, 1, -1, true>;

	/**
	 * Écrit la vitesse linéaire sur le flux.
	 * @param s le flux sur lequel écrire la vitesse linéaire.
	 * @param v la vitesse linéaire à écrire
	 * @return le flux
	 */
	friend std::ostream &operator<<(std::ostream &s, Type const &v);

	/**
	 * Créé une vitesse linéaire avec la valeur en mètres par secondes spécifiée.
	 * @param m_s la valeur de vitesse linéaire en mètres par secondes
	 * @return une grandeur de vitesse linéaire avec la valeur spécifiée.
	 */
	static constexpr Type makeFromM_s(long double m_s) { return Type(m_s); }

	/**
	 * Créé une vitesse linéaire avec la valeur en décimètres par secondes spécifiée.
	 * @param dm_s la valeur de vitesse linéaire en décimètres par secondes
	 * @return une grandeur de vitesse linéaire avec la valeur spécifiée.
	 */
	static constexpr Type makeFromDm_s(long double dm_s) { return Type(dm_s / 10); }

	/**
	 * Créé une vitesse linéaire avec la valeur en centimètres par secondes spécifiée.
	 * @param cm_s la valeur de vitesse linéaire en centimètres par secondes
	 * @return une grandeur de vitesse linéaire avec la valeur spécifiée.
	 */
	static constexpr Type makeFromCm_s(long double cm_s) { return Type(cm_s / 100); }

	/**
	 * Créé une vitesse linéaire avec la valeur en millimètres par secondes spécifiée.
	 * @param mm_s la valeur de vitesse linéaire en millimètres par secondes
	 * @return une grandeur de vitesse linéaire avec la valeur spécifiée.
	 */
	static constexpr Type makeFromMm_s(long double mm_s) { return Type(mm_s / 1000); }

	/**
	 * Retourne la valeur stockée sous forme de millimètres par seconde.
	 * Attention, une erreur sera affichée si le type vers lequel est convertie la valeur
	 * ne peut pas représenter cette dernière. Voir la documentation de NumericValue::value() pour plus d'informations.
	 * @return la valeur en millimètres par seconde de la vitesse linéaire.
	 */
	template<typename Rep = ValueType>
	constexpr Rep toMm_s() const {
		return (*this * 1000).value<Rep>();
	}

	/**
	 * Retourne la valeur stockée sous forme de mètres par seconde.
	 * Attention, une erreur sera affichée si le type vers lequel est convertie la valeur
	 * ne peut pas représenter cette dernière. Voir la documentation de NumericValue::value() pour plus d'informations.
	 * @return la valeur en mètres par seconde de la vitesse linéaire.
	 */
	template<typename Rep = ValueType>
	constexpr Rep toM_s() const {
		return (*this).value<Rep>();
	}

	/**
	 * Retourne la valeur stockée sous forme de décimètres par seconde.
	 * Attention, une erreur sera affichée si le type vers lequel est convertie la valeur
	 * ne peut pas représenter cette dernière. Voir la documentation de NumericValue::value() pour plus d'informations.
	 * @return la valeur en décimètres par seconde de la vitesse linéaire.
	 */
	template<typename Rep = ValueType>
	constexpr Rep toDm_s() const {
		return (*this * 10).value<Rep>();
	}

private:
	using Unit<0, 1, -1, false>::Unit;
};

/**
 * Permet de créer une vitesse linéaire à partir d'une valeur littérale en millimètres par seconde : 1_mm_s, 2_mm_s, 0.5_mm_s …
 */
inline constexpr VitesseLineaire operator"" _mm_s(long double v) { return VitesseLineaire::makeFromMm_s(v); }
inline constexpr VitesseLineaire operator"" _mm_s(unsigned long long v) { return VitesseLineaire::makeFromMm_s(v); }

/**
 * Permet de créer une vitesse linéaire à partir d'une valeur littérale en centimètres par seconde : 1_cm_s, 2_cm_s, 0.5_cm_s …
 */
inline constexpr VitesseLineaire operator"" _cm_s(long double v) { return VitesseLineaire::makeFromCm_s(v); }
inline constexpr VitesseLineaire operator"" _cm_s(unsigned long long v) { return VitesseLineaire::makeFromCm_s(v); }

/**
 * Permet de créer une vitesse linéaire à partir d'une valeur littérale en décimètres par seconde : 1_dm_s, 2_dm_s, 0.5_dm_s …
 */
inline constexpr VitesseLineaire operator"" _dm_s(long double v) { return VitesseLineaire::makeFromDm_s(v); }
inline constexpr VitesseLineaire operator"" _dm_s(unsigned long long v) { return VitesseLineaire::makeFromDm_s(v); }

/**
 * Permet de créer une vitesse linéaire à partir d'une valeur littérale en mètres par seconde : 1_m_s, 2_m_s, 0.5_m_s …
 */
inline constexpr VitesseLineaire operator"" _m_s(long double v) { return VitesseLineaire::makeFromM_s(v); }
inline constexpr VitesseLineaire operator"" _m_s(unsigned long long v) { return VitesseLineaire::makeFromM_s(v); }

/**
 * Permet de considérer une VitesseLineaire comme un nombre
 */
namespace std {
	template<>
	struct is_scalar<VitesseLineaire> : public std::integral_constant<bool, true> {};
}

#endif
