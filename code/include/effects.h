#ifndef EFFECTS_H
#define EFFECTS_H

#include "struct.h"
#include "include.h"


void apply_status_to_enemy(CreatureMarine* enemy, EffectType type, int duration, int potency);
void apply_status_to_player(Plongeur* player, EffectType type, int duration, int potency);

void apply_weapon_effects(Plongeur* player, CreatureMarine* target, Arme weapon, int damage_dealt);

bool process_player_effects(Plongeur* player);
bool process_enemy_effects(CreatureMarine* enemy);

#endif