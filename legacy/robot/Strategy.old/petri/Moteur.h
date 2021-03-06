//
// Created by terae on 4/20/18.
//

#ifndef ROOT_MOTEUR_H
#define ROOT_MOTEUR_H
#pragma once

#include "../../../Principal/librobot/libRobot.h"
#include "PetriCommon.h"
#include <ActionResult.h>

// @PetriLab@ Moteurs
ResultatAction monterAscenseursDe(int nbr_tours);
ResultatAction monterAscenseursDe(Angle value);

// Retourne le nombre de places libres pour les cubes dans l'ascenseur
int cubesLibres();

ResultatAction activerAvaleurs(SensAvaleurs = AVALER);
ResultatAction desactiverAvaleurs();

ResultatAction activerTurbineD();
ResultatAction activerTurbineG();

ResultatAction desactiverTurbineD();
ResultatAction desactiverTurbineG();

#endif // ROOT_MOTEUR_H
