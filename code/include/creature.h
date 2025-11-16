#ifndef CREATURES_H
#define CREATURES_H

#include "struct.h"

CreatureMarine createCreature();

CreatureMarine get_creature_by_id(int id);
void print_creature(CreatureMarine creature);

#endif
