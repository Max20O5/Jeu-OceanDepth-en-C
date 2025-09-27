#include "include.h"

typedef struct {
    int id;  // identifiant unique pour cibler
    char nom[30];
    int points_de_vie_max;
    int points_de_vie_actuels;
    int attaque_minimale;
    int attaque_maximale;
    int defense;
    int vitesse;
    char effet_special[20]; // "paralysie", "poison", "aucun"
    bool est_vivant;
} CreatureMarine;


typedef struct {
    int points_de_vie;
    int points_de_vie_max;
    int niveau_oxygene;
    int niveau_oxygene_max;
    int niveau_fatigue; // 0 à 5
    int perles;// monnaie du jeu
} Plongeur;

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
    bool tresor;
    bool boss;
    bool big_boss;
    bool inconnu;
    bool save;
} Map;

typedef struct {
    Arme arme;
    Equipement equipement;
    Consommable consommable;
    int nb_consommable;
} Inventaire;