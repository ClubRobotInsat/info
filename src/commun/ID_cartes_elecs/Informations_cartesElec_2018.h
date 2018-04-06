/**
 * @file 	mon_header.h
 * @brief 	Header commun aux codes info et elec pour partager des informations communes
 *
 * @details     Permet par exemple d'avoir dans un même fichier les ID des différentes cartes
 *              ou bien encore leur types de messages.
 */

/**
 * Date historique de fondation de ce fichier légendaire : le 2 avril 2015.
 *
 */

#ifndef HEADER_COMMUN
#define HEADER_COMMUN

////////////////////////
// ID DES CARTES ELEC //
////////////////////////

/// IDs des cartes

#define ID_CARTE_ELEC_DEPLACEMENT 1
// La nucléo gère directement l'IO, les servos et les moteurs
#define ID_CARTE_ELEC_SERVOS 2
#define ID_CARTE_ELEC_IO 3
#define ID_CARTE_ELEC_EVITEMENT 4
// FIXME : 2 IDs pour une 'carte' moteurs asservis et une 'carte' moteurs non asservis + ID unique pour chaque moteur?
#define ID_CARTE_ELEC_MOTEUR_ASCENSEUR_GAUCHE 5
#define ID_CARTE_ELEC_MOTEUR_ASCENSEUR_DROIT 6
#define ID_CARTE_ELEC_MOTEUR_AVALEUR_GAUCHE 7
#define ID_CARTE_ELEC_MOTEUR_AVALEUR_DROIT 8
// FIXME #define ID_CARTE_ELEC_MOTEURS_ASSERVIS 5
// FIXME #define ID_CARTE_ELEC_MOTEURS_ASSERVIS 6

/// IDs des servos
#define ID_SERVO_OUVERTURE_PORTE_SOUTE_DROIT 0
#define ID_SERVO_LOQUET 1
#define ID_SERVO_OUVERTURE_PORTE_SOUTE_GAUCHE 2

#define ID_SERVO_ABEILLE_GAUCHE 3
#define ID_SERVO_ABEILLE_DROIT 4

/// IDs des contacteurs
#define ID_CONTACTEUR_TIRETTE 2

/* constantes dans le cas où on a 2 interfaces moteur côté élec
/// IDs des moteurs
// asservis
#define ID_MOTEUR_ASCENSEUR_GAUCHE 0
#define ID_MOTEUR_ASCENSEUR_DROIT  1

// non asservis
#define ID_MOTEUR_AVALEUR_GAUCHE 0
#define ID_MOTEUR_AVALEUR_DROIT  1
*/

#endif /* HEADER_COMMUN */