#ifndef EQUIPEMENT_H
#define EQUIPEMENT_H

#include "struct.h"

Equipement get_equipement_by_id(int id);

int get_defense_equipement(Equipement* equipement);
int get_reduction_oxygene(Equipement* equipement);

#endif