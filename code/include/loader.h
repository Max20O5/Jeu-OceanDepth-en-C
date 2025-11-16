#ifndef LOADER_H
#define LOADER_H

#include "include.h"

bool load_all_game_data(const char* config_path);
void free_game_data(void);

#endif