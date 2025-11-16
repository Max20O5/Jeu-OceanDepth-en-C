#ifndef JOUEUR_H
#define JOUEUR_H

#include "struct.h"

Plongeur create_player();

void display_player_stats(Plongeur player);

// Système d'XP et de progression
void gagner_experience(Plongeur* player, int xp);
void monter_niveau(Plongeur* player);

#endif