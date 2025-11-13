#ifndef COMPETENCES_H
#define COMPETENCES_H

#include "struct.h"

// Structure pour les compétences
typedef struct {
    int id;
    char nom[50];
    int cout_oxygene;
    int cooldown_max;
    int cooldown_actuel;
    bool disponible;
} Competence;

// Types de compétences
#define COMPETENCE_APNEE 0
#define COMPETENCE_DECHARGE 1
#define COMPETENCE_COMMUNICATION 2
#define COMPETENCE_TOURBILLON 3
#define NB_COMPETENCES 4

// Initialiser les compétences du joueur
void init_competences(Competence competences[NB_COMPETENCES]);

// Afficher le menu des compétences
void afficher_menu_competences(Competence competences[NB_COMPETENCES], Plongeur* joueur);

// Utiliser une compétence
bool utiliser_competence(int id_competence, Competence competences[NB_COMPETENCES],
                         Plongeur* joueur, CreatureMarine creatures[], int nb_creatures);

// Réduire les cooldowns
void reduire_cooldowns(Competence competences[NB_COMPETENCES]);

#endif
