#ifndef COMPETENCE_H
#define COMPETENCE_H

#include "struct.h"

CompetenceAquatique get_skill_by_id(int id);

void reduce_all_skill_cooldowns(Plongeur* player);
bool handle_skill_use(Plongeur* player, CreatureMarine* enemies, int enemy_count);

#endif