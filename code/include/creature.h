#ifndef CREATURES_H
#define CREATURES_H

#include "struct.h"

// Création de créatures spécifiques
CreatureMarine create_kraken();
CreatureMarine create_requin();
CreatureMarine create_meduse();
CreatureMarine create_poisson_epee();
CreatureMarine create_crabe_geant();

// Génération aléatoire
CreatureMarine generer_creature_aleatoire(int profondeur);
int generer_groupe_creatures(CreatureMarine creatures[], int max_creatures, int profondeur);

// Créer une créature par ID (1-6)
CreatureMarine creerCreature(int id, int profondeur);

// Affichage
void print_creature(CreatureMarine creature);

#endif
