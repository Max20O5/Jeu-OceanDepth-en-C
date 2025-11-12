#ifndef STRUCT_H
#define STRUCT_H

#include "include.h"

typedef enum {
    EFFET_AUCUN,
    EFFET_PARALYSIE,
    EFFET_BRULURE,
    EFFET_CHARME,
    EFFET_BOUCLIER
} EffectType;

typedef enum {
    SKILL_EFFECT_NONE,
    SKILL_EFFECT_HEAL_OXYGEN,
    SKILL_EFFECT_DAMAGE_AOE,
    SKILL_EFFECT_PACIFY,
    SKILL_EFFECT_DEBUFF_SPEED
} SkillEffectType;


typedef struct {
    EffectType type;
    int duration;
    int potency;
} StatusEffect;

typedef struct {
    int id;
    char nom[30];
    int cost_oxygene;
    int cost_fatigue;
    SkillEffectType effect_type;
    int potency;
    int cooldown;
} CompetenceAquatique;

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
    EffectType special_attack_effect;
    int special_attack_chance;   
    int special_attack_potency;
    int special_attack_duration;
    bool est_vivant;
    char effet_special[20];
    StatusEffect active_effects[5];
    int active_effect_count;
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
    EffectType effect_type_to_apply;
    int effect_chance;
    int effect_potency;
    int effect_duration;
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
    Consommable inventaire[8];
    StatusEffect active_effects[5];
    int active_effect_count;
    CompetenceAquatique competences_apprises[4];
    int competences_cooldowns[4];
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

#endif