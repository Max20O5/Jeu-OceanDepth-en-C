#ifndef STRUCT_H
#define STRUCT_H

#include "include.h"

typedef struct {
    int id;
    char nom[30];
    int points_de_vie_max;
    int points_de_vie_actuels;
    int attaque_minimale;
    int attaque_maximale;
    int defense;
    int vitesse;
    int perles_min;
    int perles_max;
    char effet_special[20]; // "paralysie", "poison", "aucun"
    bool est_vivant;
    int zone; // Zone d'apparition de la créature
} CreatureMarine;

typedef struct {
    int id;
    char nom[30];
    int attaque_minimale;
    int attaque_maximale;
    int consommation_oxygene;
    int special;
    int nb_utilisations;
    int nb_utilisations_max;
    int nb_utilisations_special;
    int nb_utilisations_special_max;
    int vitesse_minimale;
    int vitesse_maximale;
    char effet_special[20];
    int zone; // Zone de disponibilité de l'arme
} Arme;

typedef struct {
    int id;
    char nom[30];
    int defense_minimale;
    int defense_maximale;
    int vitesse_minimale;
    int vitesse_maximale;
    int consommation_oxygene_reduction;
    char effet_special[20];
} Equipement;

typedef struct {
    int id;
    char nom[30];
    char effet_special[20];
    int soin;
    int oxygene;
    int fatigue;
} Consommable;

typedef struct {
    int points_de_vie;
    int points_de_vie_max;
    int niveau_oxygene;
    int niveau_oxygene_max;
    int niveau_fatigue; // 0 à 5
    int perles; // monnaie du jeu
    char name[50];
    Arme arme_equipee;
    Equipement combinaison_equipee;
    Consommable inventaire[8];
    // Système de progression
    int niveau;
    int experience;
    int experience_prochain_niveau;
    int creatures_tuees;
    int boss_tues;
} Plongeur;

typedef struct {
    int id;
    char nom[30];
    int degat_minimale;
    int degat_maximale;
    int vitesse_minimale;
    int vitesse_maximale;
    int defense_minimale;
    int defense_maximale;
    int soin_minimale;
    int soin_maximale;
    int fatigue_minimale;
    int fatigue_maximale;
    int oxygene_minimale;
    int oxygene_maximale;
} Effet;

typedef struct {
    int id;
    char nom[30];
    int x;
    int y;
    int nb_ennemis;
    int *tab_ennemis;
    bool tresor;
    bool boss;
    bool big_boss;
    bool inconnu;
    bool save;
} Map_section;

// ===== NOUVEAU SYSTÈME DE MAP PAR ZONES =====

// Types de tuiles
typedef enum {
    TILE_EMPTY = 0,      // Tuile vide (déplacement libre)
    TILE_COMBAT,         // Tuile de combat (1+ ennemis)
    TILE_TREASURE,       // Tuile de tresor (oxygene/item/rien)
    TILE_MERCHANT,       // Tuile de marchand
    TILE_BOSS,           // Tuile de boss
    TILE_SAVE,           // Tuile de sauvegarde (grotte sécurisée)
    TILE_PLAYER          // Position actuelle du joueur (pour affichage)
} TileType;

// Structure d'une tuile de la carte
typedef struct {
    TileType type;
    bool visited;        // La tuile a-t-elle été visitée?
    bool cleared;        // La tuile a-t-elle été nettoyée (combat terminé, trésor récupéré)?
    int nb_ennemis;      // Nombre d'ennemis (si TILE_COMBAT)
    int* enemy_ids;      // IDs des ennemis présents
    int depth;           // Profondeur à cette tuile (en mètres)
} Tile;

// Structure d'une zone complète
typedef struct Zone {
    int zone_number;              // Numéro de la zone (1, 2, 3...)
    char zone_name[100];          // Nom de la zone
    int width;                    // Largeur de la matrice
    int height;                   // Hauteur de la matrice
    Tile** tiles;                 // Matrice de tuiles [y][x]
    int player_x;                 // Position X du joueur
    int player_y;                 // Position Y du joueur
    int base_depth;               // Profondeur de départ de la zone
    bool boss_defeated;           // Le boss de la zone a-t-il été vaincu?
    struct Zone* next_zone;       // Pointeur vers la zone suivante
} Zone;

// Configuration de génération de carte
typedef struct {
    int map_width;
    int map_height;
    int combat_tile_probability;
    int treasure_tile_probability;
    int merchant_tile_probability;
    int boss_tile_probability;
    int save_tile_probability;    // Probabilité de tuile de sauvegarde
    int enemy_count_min;
    int enemy_count_max;
    int depth_increment_min;
    int depth_increment_max;
    char zone_names[10][100];     // Jusqu'à 10 noms de zones
    int zone_count;               // Nombre de zones définies
    float zone_difficulty_multiplier;
    float zone_enemy_bonus;
    int zone_rare_loot_base;
    int zone_rare_loot_increase;
    int zone_base_depth;
    int zone_depth_increment;
} MapConfig;

// Structure principale de la carte (remplace l'ancien système)
typedef struct {
    Zone* current_zone;           // Zone actuelle
    MapConfig config;             // Configuration chargée depuis le fichier
    int total_zones_generated;   // Nombre total de zones générées
} Carte;

#endif