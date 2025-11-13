#include "save.h"
#include "map.h"
#include "weapon.h"
#include "consommable.h"
#include "equipement.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

// Vérifier si le dossier saves existe, sinon le créer
void ensure_saves_directory() {
    struct stat st = {0};
    if (stat("saves", &st) == -1) {
        #ifdef _WIN32
            mkdir("saves");
        #else
            mkdir("saves", 0700);
        #endif
    }
}

// Vérifier si une sauvegarde existe
bool sauvegarde_existe() {
    FILE* file = fopen(SAVE_FILE, "r");
    if (file) {
        fclose(file);
        return true;
    }
    return false;
}

// Supprimer la sauvegarde
void supprimer_sauvegarde() {
    remove(SAVE_FILE);
}

// Sauvegarder la partie
bool sauvegarder_partie(Plongeur* joueur, Carte* carte) {
    ensure_saves_directory();

    FILE* file = fopen(SAVE_FILE, "w");
    if (!file) {
        printf("Erreur: Impossible de creer le fichier de sauvegarde!\n");
        return false;
    }

    // En-tête
    fprintf(file, "OCEANDEPTHS_SAVE_V1\n");

    // Informations du joueur
    fprintf(file, "JOUEUR_NOM:%s\n", joueur->name);
    fprintf(file, "JOUEUR_STATS:%d:%d:%d:%d:%d:%d\n",
            joueur->points_de_vie,
            joueur->points_de_vie_max,
            joueur->niveau_oxygene,
            joueur->niveau_oxygene_max,
            joueur->niveau_fatigue,
            joueur->perles);
    fprintf(file, "JOUEUR_PROGRESSION:%d:%d:%d:%d:%d\n",
            joueur->niveau,
            joueur->experience,
            joueur->experience_prochain_niveau,
            joueur->creatures_tuees,
            joueur->boss_tues);

    // Arme équipée
    fprintf(file, "ARME:%d:%s:%d:%d:%d:%d:%d:%d:%d:%d:%d:%d:%s\n",
            joueur->arme_equipee.id,
            joueur->arme_equipee.nom,
            joueur->arme_equipee.attaque_minimale,
            joueur->arme_equipee.attaque_maximale,
            joueur->arme_equipee.consommation_oxygene,
            joueur->arme_equipee.special,
            joueur->arme_equipee.nb_utilisations,
            joueur->arme_equipee.nb_utilisations_max,
            joueur->arme_equipee.nb_utilisations_special,
            joueur->arme_equipee.nb_utilisations_special_max,
            joueur->arme_equipee.vitesse_minimale,
            joueur->arme_equipee.vitesse_maximale,
            joueur->arme_equipee.effet_special);

    // Combinaison équipée
    fprintf(file, "COMBINAISON:%d:%s:%d:%d:%d:%d:%d:%s\n",
            joueur->combinaison_equipee.id,
            joueur->combinaison_equipee.nom,
            joueur->combinaison_equipee.defense_minimale,
            joueur->combinaison_equipee.defense_maximale,
            joueur->combinaison_equipee.vitesse_minimale,
            joueur->combinaison_equipee.vitesse_maximale,
            joueur->combinaison_equipee.consommation_oxygene_reduction,
            joueur->combinaison_equipee.effet_special);

    // Inventaire
    fprintf(file, "INVENTAIRE_START\n");
    for (int i = 0; i < 8; i++) {
        Consommable* item = &joueur->inventaire[i];
        fprintf(file, "%d:%s:%s:%d:%d:%d\n",
                item->id,
                item->nom,
                item->effet_special,
                item->soin,
                item->oxygene,
                item->fatigue);
    }
    fprintf(file, "INVENTAIRE_END\n");

    // Position sur la carte (zone, x, y, profondeur)
    int profondeur_actuelle = get_profondeur_actuelle(carte);
    fprintf(file, "POSITION:%d:%d:%d:%d\n",
            carte->current_zone->zone_number,
            carte->current_zone->player_x,
            carte->current_zone->player_y,
            profondeur_actuelle);

    fclose(file);
    printf("\n==============================================================\n");
    printf("           PARTIE SAUVEGARDEE AVEC SUCCES!                    \n");
    printf("==============================================================\n");
    return true;
}

// Charger la partie
bool charger_partie(Plongeur* joueur, Carte** carte) {
    FILE* file = fopen(SAVE_FILE, "r");
    if (!file) {
        printf("Erreur: Aucune sauvegarde trouvee!\n");
        return false;
    }

    char buffer[256];
    bool success = true;

    // Vérifier l'en-tête
    if (fgets(buffer, sizeof(buffer), file) == NULL ||
        strncmp(buffer, "OCEANDEPTHS_SAVE_V1", 19) != 0) {
        printf("Erreur: Format de sauvegarde invalide!\n");
        fclose(file);
        return false;
    }

    // Charger le nom du joueur
    if (fgets(buffer, sizeof(buffer), file)) {
        if (sscanf(buffer, "JOUEUR_NOM:%[^\n]", joueur->name) != 1) {
            success = false;
        }
    }

    // Charger les stats du joueur
    if (fgets(buffer, sizeof(buffer), file)) {
        if (sscanf(buffer, "JOUEUR_STATS:%d:%d:%d:%d:%d:%d",
                   &joueur->points_de_vie,
                   &joueur->points_de_vie_max,
                   &joueur->niveau_oxygene,
                   &joueur->niveau_oxygene_max,
                   &joueur->niveau_fatigue,
                   &joueur->perles) != 6) {
            success = false;
        }
    }

    // Charger la progression
    if (fgets(buffer, sizeof(buffer), file)) {
        if (sscanf(buffer, "JOUEUR_PROGRESSION:%d:%d:%d:%d:%d",
                   &joueur->niveau,
                   &joueur->experience,
                   &joueur->experience_prochain_niveau,
                   &joueur->creatures_tuees,
                   &joueur->boss_tues) != 5) {
            // Si la ligne n'existe pas (anciennes sauvegardes), initialiser les valeurs par défaut
            joueur->niveau = 1;
            joueur->experience = 0;
            joueur->experience_prochain_niveau = 100;
            joueur->creatures_tuees = 0;
            joueur->boss_tues = 0;
        }
    }

    // Charger l'arme
    if (fgets(buffer, sizeof(buffer), file)) {
        if (sscanf(buffer, "ARME:%d:%[^:]:%d:%d:%d:%d:%d:%d:%d:%d:%d:%d:%[^\n]",
                   &joueur->arme_equipee.id,
                   joueur->arme_equipee.nom,
                   &joueur->arme_equipee.attaque_minimale,
                   &joueur->arme_equipee.attaque_maximale,
                   &joueur->arme_equipee.consommation_oxygene,
                   &joueur->arme_equipee.special,
                   &joueur->arme_equipee.nb_utilisations,
                   &joueur->arme_equipee.nb_utilisations_max,
                   &joueur->arme_equipee.nb_utilisations_special,
                   &joueur->arme_equipee.nb_utilisations_special_max,
                   &joueur->arme_equipee.vitesse_minimale,
                   &joueur->arme_equipee.vitesse_maximale,
                   joueur->arme_equipee.effet_special) != 13) {
            success = false;
        }
    }

    // Charger la combinaison
    if (fgets(buffer, sizeof(buffer), file)) {
        if (sscanf(buffer, "COMBINAISON:%d:%[^:]:%d:%d:%d:%d:%d:%[^\n]",
                   &joueur->combinaison_equipee.id,
                   joueur->combinaison_equipee.nom,
                   &joueur->combinaison_equipee.defense_minimale,
                   &joueur->combinaison_equipee.defense_maximale,
                   &joueur->combinaison_equipee.vitesse_minimale,
                   &joueur->combinaison_equipee.vitesse_maximale,
                   &joueur->combinaison_equipee.consommation_oxygene_reduction,
                   joueur->combinaison_equipee.effet_special) != 8) {
            success = false;
        }
    }

    // Charger l'inventaire
    if (fgets(buffer, sizeof(buffer), file)) {
        if (strncmp(buffer, "INVENTAIRE_START", 16) != 0) {
            success = false;
        } else {
            for (int i = 0; i < 8; i++) {
                if (fgets(buffer, sizeof(buffer), file)) {
                    Consommable* item = &joueur->inventaire[i];
                    if (sscanf(buffer, "%d:%[^:]:%[^:]:%d:%d:%d",
                               &item->id,
                               item->nom,
                               item->effet_special,
                               &item->soin,
                               &item->oxygene,
                               &item->fatigue) != 6) {
                        success = false;
                    }
                }
            }
            // Lire INVENTAIRE_END
            fgets(buffer, sizeof(buffer), file);
        }
    }

    // Charger la position (zone, x, y, profondeur)
    int zone_num = 1;
    int player_x = 0;
    int player_y = 0;
    int profondeur = 0;
    if (fgets(buffer, sizeof(buffer), file)) {
        // Nouvelle version avec 4 champs
        int fields = sscanf(buffer, "POSITION:%d:%d:%d:%d", &zone_num, &player_x, &player_y, &profondeur);
        if (fields != 4) {
            // Ancienne version avec 3 champs (compatibilité)
            if (sscanf(buffer, "POSITION:%d:%d:%d", &player_x, &profondeur, &zone_num) != 3) {
                success = false;
            }
            // Convertir l'ancien format
            player_y = 0;
            zone_num = 1;
        }
    }

    // Initialiser la carte (nouvelle génération par zones)
    *carte = init_carte();
    if (!*carte) {
        fclose(file);
        return false;
    }

    // Placer le joueur à la position sauvegardée
    (*carte)->current_zone->player_x = player_x;
    (*carte)->current_zone->player_y = player_y;

    // Note: Pour simplifier, on commence toujours en zone 1
    // On pourrait générer les zones jusqu'à zone_num si nécessaire

    fclose(file);

    if (success) {
        printf("\n==============================================================\n");
        printf("           PARTIE CHARGEE AVEC SUCCES!                        \n");
        printf("==============================================================\n");
        printf("Bienvenue de retour, %s!\n", joueur->name);
        printf("Position: %dm de profondeur\n", get_profondeur_actuelle(*carte));
        printf("==============================================================\n");
    } else {
        printf("Erreur lors du chargement de la sauvegarde!\n");
        if (*carte) {
            free_carte(*carte);
            *carte = NULL;
        }
    }

    return success;
}
