#ifndef MAP_H
#define MAP_H

#include "struct.h"
#include "config.h"

// ===== NOUVEAU SYSTÈME DE MAP PAR ZONES =====

// Initialisation de la carte avec configuration
Carte* init_carte();

// Libération de la mémoire
void free_carte(Carte* carte);

// Génération d'une zone
Zone* generer_zone(int zone_number, MapConfig* config);

// Génération d'une tuile selon les probabilités
void generer_tuile(Tile* tile, int x, int y, int zone_number, MapConfig* config);

// Affichage de la carte actuelle
void afficher_carte(Carte* carte);

// Navigation dans la carte
void naviguer_carte(Carte* carte, Plongeur* joueur);

// Déplacement du joueur
bool deplacer_joueur(Carte* carte, Plongeur* joueur, int dx, int dy);

// Explorer la tuile actuelle
void explorer_tuile(Carte* carte, Plongeur* joueur);

// Obtenir la profondeur actuelle
int get_profondeur_actuelle(Carte* carte);

// Passer à la zone suivante (après avoir battu le boss)
void passer_zone_suivante(Carte* carte);

// Libérer une zone de la mémoire
void free_zone(Zone* zone);

// Obtenir l'icône d'une tuile
const char* get_tile_icon(TileType type);

#endif
