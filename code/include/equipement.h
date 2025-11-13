#ifndef EQUIPEMENT_H
#define EQUIPEMENT_H

#include "struct.h"

// Créer les différentes combinaisons
Equipement creer_neoprene_basic();
Equipement creer_combinaison_renforcee();
Equipement creer_combinaison_profondeur();
Equipement creer_armure_titanium();

// Obtenir la défense totale d'un équipement
int get_defense_equipement(Equipement* equipement);

// Obtenir la réduction de consommation d'oxygène
int get_reduction_oxygene(Equipement* equipement);

#endif
