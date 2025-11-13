#ifndef COMBAT_H
#define COMBAT_H

#include "struct.h"

void start_combat(Plongeur* player);
void start_combat_profondeur(Plongeur* player, int profondeur);

// Combat avec créatures déjà générées (pour le système de map par zones)
// Retourne true si victoire, false si défaite
bool combat(Plongeur* player, CreatureMarine* creatures, int nb_creatures, int profondeur);

#endif 