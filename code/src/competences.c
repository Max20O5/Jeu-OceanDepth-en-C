#include "competences.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Initialiser les compétences
void init_competences(Competence competences[NB_COMPETENCES]) {
    // Apnée Prolongée
    competences[COMPETENCE_APNEE].id = COMPETENCE_APNEE;
    strcpy(competences[COMPETENCE_APNEE].nom, "Apnee Prolongee");
    competences[COMPETENCE_APNEE].cout_oxygene = 0;
    competences[COMPETENCE_APNEE].cooldown_max = 4;
    competences[COMPETENCE_APNEE].cooldown_actuel = 0;
    competences[COMPETENCE_APNEE].disponible = true;

    // Décharge Électrique
    competences[COMPETENCE_DECHARGE].id = COMPETENCE_DECHARGE;
    strcpy(competences[COMPETENCE_DECHARGE].nom, "Decharge Electrique");
    competences[COMPETENCE_DECHARGE].cout_oxygene = 18;
    competences[COMPETENCE_DECHARGE].cooldown_max = 0;
    competences[COMPETENCE_DECHARGE].cooldown_actuel = 0;
    competences[COMPETENCE_DECHARGE].disponible = true;

    // Communication Marine
    competences[COMPETENCE_COMMUNICATION].id = COMPETENCE_COMMUNICATION;
    strcpy(competences[COMPETENCE_COMMUNICATION].nom, "Communication Marine");
    competences[COMPETENCE_COMMUNICATION].cout_oxygene = 8;
    competences[COMPETENCE_COMMUNICATION].cooldown_max = 0;
    competences[COMPETENCE_COMMUNICATION].cooldown_actuel = 0;
    competences[COMPETENCE_COMMUNICATION].disponible = true;

    // Tourbillon Aquatique
    competences[COMPETENCE_TOURBILLON].id = COMPETENCE_TOURBILLON;
    strcpy(competences[COMPETENCE_TOURBILLON].nom, "Tourbillon Aquatique");
    competences[COMPETENCE_TOURBILLON].cout_oxygene = 22;
    competences[COMPETENCE_TOURBILLON].cooldown_max = 0;
    competences[COMPETENCE_TOURBILLON].cooldown_actuel = 0;
    competences[COMPETENCE_TOURBILLON].disponible = true;
}

// Afficher le menu des compétences
void afficher_menu_competences(Competence competences[NB_COMPETENCES], Plongeur* joueur) {
    printf("\n");
    printf("                    COMPETENCES MARINES\n");
    printf("==============================================================\n");

    for (int i = 0; i < NB_COMPETENCES; i++) {
        printf("[%d] %s\n", i + 1, competences[i].nom);
        printf("    Cout: %d oxygene", competences[i].cout_oxygene);

        if (competences[i].cooldown_max > 0) {
            printf(" | Cooldown: %d tours", competences[i].cooldown_max);
        }

        if (competences[i].cooldown_actuel > 0) {
            printf(" | INDISPONIBLE (%d tours restants)", competences[i].cooldown_actuel);
        } else if (joueur->niveau_oxygene < competences[i].cout_oxygene) {
            printf(" | OXYGENE INSUFFISANT");
        } else {
            printf(" | DISPONIBLE");
        }
        printf("\n\n");
    }

    printf("==============================================================\n");
    printf("[5] Retour\n");
}

// Utiliser une compétence
bool utiliser_competence(int id_competence, Competence competences[NB_COMPETENCES],
                         Plongeur* joueur, CreatureMarine creatures[], int nb_creatures) {
    if (id_competence < 0 || id_competence >= NB_COMPETENCES) {
        return false;
    }

    Competence* comp = &competences[id_competence];

    // Vérifier le cooldown
    if (comp->cooldown_actuel > 0) {
        printf("\nCette competence est en cooldown! (%d tours restants)\n", comp->cooldown_actuel);
        return false;
    }

    // Vérifier l'oxygène
    if (joueur->niveau_oxygene < comp->cout_oxygene) {
        printf("\nOxygene insuffisant! (Requis: %d, Disponible: %d)\n",
               comp->cout_oxygene, joueur->niveau_oxygene);
        return false;
    }

    // Consommer l'oxygène
    joueur->niveau_oxygene -= comp->cout_oxygene;

    printf("\n>>> Vous utilisez: %s!\n", comp->nom);

    // Effets selon la compétence
    switch(id_competence) {
        case COMPETENCE_APNEE:
            joueur->niveau_oxygene += 20;
            if (joueur->niveau_oxygene > joueur->niveau_oxygene_max) {
                joueur->niveau_oxygene = joueur->niveau_oxygene_max;
            }
            printf(">>> Vous recuperez +20 oxygene!\n");
            printf(">>> Oxygene actuel: %d/%d\n", joueur->niveau_oxygene, joueur->niveau_oxygene_max);
            comp->cooldown_actuel = comp->cooldown_max;
            break;

        case COMPETENCE_DECHARGE:
            printf(">>> Decharge electrique sur toutes les creatures!\n");
            for (int i = 0; i < nb_creatures; i++) {
                if (creatures[i].est_vivant) {
                    int degats = 20 + rand() % 11; // 20-30
                    creatures[i].points_de_vie_actuels -= degats;
                    printf(">>> %s subit %d degats!\n", creatures[i].nom, degats);

                    if (creatures[i].points_de_vie_actuels <= 0) {
                        creatures[i].points_de_vie_actuels = 0;
                        creatures[i].est_vivant = false;
                        printf(">>> %s est elimine!\n", creatures[i].nom);
                    }
                }
            }
            break;

        case COMPETENCE_COMMUNICATION:
            printf(">>> Vous tentez de pacifier une creature...\n");
            // Trouver la première créature vivante et la pacifier
            for (int i = 0; i < nb_creatures; i++) {
                if (creatures[i].est_vivant) {
                    creatures[i].est_vivant = false;
                    printf(">>> %s est pacifie et se retire du combat!\n", creatures[i].nom);
                    break;
                }
            }
            break;

        case COMPETENCE_TOURBILLON:
            printf(">>> Tourbillon aquatique! Toutes les creatures sont ralenties!\n");
            for (int i = 0; i < nb_creatures; i++) {
                if (creatures[i].est_vivant) {
                    creatures[i].vitesse -= 2;
                    if (creatures[i].vitesse < 1) creatures[i].vitesse = 1;
                    printf(">>> %s: vitesse reduite a %d\n", creatures[i].nom, creatures[i].vitesse);
                }
            }
            break;
    }

    return true;
}

// Réduire les cooldowns
void reduire_cooldowns(Competence competences[NB_COMPETENCES]) {
    for (int i = 0; i < NB_COMPETENCES; i++) {
        if (competences[i].cooldown_actuel > 0) {
            competences[i].cooldown_actuel--;
        }
    }
}
