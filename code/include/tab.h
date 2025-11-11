#ifndef TAB_H 
#define TAB_H

#include "struct.h"

extern Consommable *tab_consommable;
extern Map_section *map;
extern char **inventaire;
extern char **combinaison_effet;

extern Arme* g_weapon_database;
extern int g_weapon_count;
extern CreatureMarine* g_creature_database;
extern int g_creature_count;

extern Consommable* g_consommable_database;
extern int g_consommable_count;

typedef struct Node_map {
    Map_section section;
    struct Node_map *nord;
    struct Node_map *sud;
    struct Node_map *est;
    struct Node_map *ouest;
} Node_map;

typedef struct {
    Node_map *debut;
    Node_map *actuel;
    int taille;
} Map;

#endif