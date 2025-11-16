#ifndef CONFIG_H
#define CONFIG_H

#include "struct.h"

// Charge la configuration depuis le fichier config.cfg
bool load_map_config(MapConfig* config);

// Affiche la configuration chargée (pour debug)
void print_map_config(MapConfig* config);

#endif
