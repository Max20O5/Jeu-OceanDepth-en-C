#include "struct.h"

Consommable *tab_consommable;

Map_section *map;// choix si on décide de faire un tableau simple pour la map et non pas une liste chainée

char **inventaire;

char **combinaison_effet;

typedef struct Node_map {
    Map_section section;
    struct Node_map *nord;
    struct Node_map *sud;
    struct Node_map *est;
    struct Node_map *ouest;
} Node_map;

typedef struct {
    Node_map *debut;  // Pointeur vers le premier nœud
    Node_map *actuel; // Position actuelle du joueur
    int taille;       // Nombre de sections
} Map;//map en liste chainée
