#include "combat.h"
#include "creature.h"
#include "consommable.h"
#include "competences.h"
#include "equipement.h"
#include "joueur.h"
#include "include.h"
#include "utils.h"
#include "map.h"

#define MAX_CREATURES 4

void wait_for_enter() {
    printf("\nAppuyez sur Entree pour continuer...");
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void print_progress_bar(int current, int max, int length) {
    if (max == 0) max = 1;
    if (current < 0) current = 0;

    float percent = (float)current / max;
    int filled_blocks = (int)(percent * length);
    int empty_blocks = length - filled_blocks;

    printf("[");
    for (int i = 0; i < filled_blocks; i++) printf("#");
    for (int i = 0; i < empty_blocks; i++) printf(".");
    printf("]");
}

// Afficher l'état du combat (joueur + toutes les créatures)
void display_combat_status(Plongeur* player, CreatureMarine creatures[], int nb_creatures, int profondeur) {
    clear_screen();

    printf("OceanDepths - Profondeur: %dm | Niv: %d | XP: %d/%d | Perles: %d\n",
           profondeur, player->niveau, player->experience, player->experience_prochain_niveau, player->perles);

    // Barre de Vie
    printf("Vie     ");
    print_progress_bar(player->points_de_vie, player->points_de_vie_max, 50);
    printf(" %d/%d\n", player->points_de_vie, player->points_de_vie_max);

    // Barre d'Oxygène
    printf("Oxygene ");
    print_progress_bar(player->niveau_oxygene, player->niveau_oxygene_max, 40);
    printf(" %d/%d", player->niveau_oxygene, player->niveau_oxygene_max);

    if (player->niveau_oxygene <= 10) {
        printf(" *** ALERTE CRITIQUE ***");
    }
    printf("\n");

    // Barre de Fatigue
    printf("Fatigue ");
    print_progress_bar(player->niveau_fatigue, 5, 10);
    printf(" %d/5\n", player->niveau_fatigue);

    printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");

    // Afficher toutes les créatures vivantes
    for (int i = 0; i < nb_creatures; i++) {
        if (creatures[i].est_vivant) {
            printf("    [%d] %s \t\t (%d/%d PV) \t Vitesse: %d\n",
                   i,
                   creatures[i].nom,
                   creatures[i].points_de_vie_actuels,
                   creatures[i].points_de_vie_max,
                   creatures[i].vitesse);
        }
    }

    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
}

// Calculer le nombre d'attaques max selon la fatigue
int get_max_attaques_from_fatigue(int fatigue) {
    if (fatigue <= 1) return 3;
    if (fatigue <= 3) return 2;
    return 1; // fatigue 4-5
}

// Appliquer les effets spéciaux des créatures
static int paralysie_actif = 0; // 0 = non, >0 = tours restants

void appliquer_effet_special_creature(CreatureMarine* creature, Plongeur* player __attribute__((unused)), int* degats) {
    if (strcmp(creature->effet_special, "etreinte") == 0) {
        // Kraken: 2 attaques consécutives
        printf(">>> %s utilise Etreinte tentaculaire! (2 attaques consecutives)\n", creature->nom);
        // La deuxième attaque est gérée dans le code d'attaque
    }
    else if (strcmp(creature->effet_special, "paralysie") == 0) {
        // Méduse: Réduit les attaques du joueur de 1 au prochain tour
        printf(">>> %s utilise Piqure paralysante!\n", creature->nom);
        paralysie_actif = 2; // Actif pour 2 tours
        printf(">>> Vous etes paralyse! -1 attaque au prochain tour\n");
    }
    else if (strcmp(creature->effet_special, "frenesie") == 0) {
        // Requin: +30% dégâts si PV < 50%
        float pv_percent = (float)creature->points_de_vie_actuels / creature->points_de_vie_max;
        if (pv_percent < 0.5f) {
            printf(">>> %s entre en Frenesie sanguinaire! (+30%% degats)\n", creature->nom);
            *degats = (int)(*degats * 1.3f);
        }
    }
    else if (strcmp(creature->effet_special, "perforation") == 0) {
        // Poisson-Épée: Ignore 2 points de défense (déjà géré dans le calcul)
        printf(">>> %s utilise Charge perforante!\n", creature->nom);
    }
    else if (strcmp(creature->effet_special, "carapace") == 0) {
        // Crabe: Réduit dégâts subis de 20% (géré quand il prend des dégâts)
    }
}

// Utiliser un objet
static bool handle_item_use(Plongeur* player) {
    clear_screen();
    printf("--- INVENTAIRE ---\n");
    printf("Quel objet utiliser ? (0 pour annuler)\n\n");

    for (int i = 0; i < 8; i++) {
        printf("  [%d] - %s\n", i + 1, player->inventaire[i].nom);
    }
    printf("\n> Votre choix : ");

    int slot_choice = -1;
    char term;
    while (scanf("%d%c", &slot_choice, &term) != 2 || term != '\n' || (slot_choice < 0 || slot_choice > 8)) {
        printf("Erreur : Entrez un numero de slot (1-8) ou 0 pour annuler.\n");
        while (getchar() != '\n');
        printf("> Votre choix : ");
    }

    if (slot_choice == 0) {
        printf("Action annulee.\n");
        return false;
    }

    int slot_index = slot_choice - 1;

    if (player->inventaire[slot_index].id == 0) {
        printf("Ce slot est vide ! Action annulee.\n");
        return false;
    }

    Consommable* item = &player->inventaire[slot_index];
    printf("\nVous utilisez %s !\n", item->nom);

    if (item->soin > 0) {
        player->points_de_vie += item->soin;
        if (player->points_de_vie > player->points_de_vie_max) {
            player->points_de_vie = player->points_de_vie_max;
        }
        printf("Vous recuperez %d PV. (Vie: %d/%d)\n", item->soin, player->points_de_vie, player->points_de_vie_max);
    }

    if (item->oxygene > 0) {
        player->niveau_oxygene += item->oxygene;
        if (player->niveau_oxygene > player->niveau_oxygene_max) {
            player->niveau_oxygene = player->niveau_oxygene_max;
        }
        printf("Vous recuperez %d O2. (Oxygene: %d/%d)\n", item->oxygene, player->niveau_oxygene, player->niveau_oxygene_max);
    }

    if (item->fatigue < 0) {
        player->niveau_fatigue += item->fatigue; // fatigue est négatif
        if (player->niveau_fatigue < 0) player->niveau_fatigue = 0;
        printf("Votre fatigue diminue de %d. (Fatigue: %d/5)\n", -item->fatigue, player->niveau_fatigue);
    }

    // Effet spécial Antidote: annule la paralysie
    if (strcmp(item->effet_special, "antidote") == 0) {
        if (paralysie_actif > 0) {
            paralysie_actif = 0;
            printf("La paralysie a ete annulee!\n");
        } else {
            printf("Vous n'etes pas affecte par un effet negatif.\n");
        }
    }

    player->inventaire[slot_index] = creer_slot_vide();
    return true;
}

// Trier les créatures par vitesse (ordre décroissant)
void trier_creatures_par_vitesse(CreatureMarine creatures[], int indices[], int nb_creatures) {
    // Initialiser les indices
    for (int i = 0; i < nb_creatures; i++) {
        indices[i] = i;
    }

    // Tri à bulles par vitesse
    for (int i = 0; i < nb_creatures - 1; i++) {
        for (int j = 0; j < nb_creatures - i - 1; j++) {
            if (creatures[indices[j]].vitesse < creatures[indices[j+1]].vitesse) {
                int temp = indices[j];
                indices[j] = indices[j+1];
                indices[j+1] = temp;
            }
        }
    }
}

// Combat principal
void start_combat(Plongeur* player) {
    start_combat_profondeur(player, -150);
}

void start_combat_profondeur(Plongeur* player, int profondeur) {
    CreatureMarine creatures[MAX_CREATURES];
    int nb_creatures = generer_groupe_creatures(creatures, MAX_CREATURES, profondeur);

    printf("\n");
    printf("╔═══════════════════════════════════════════════════════════╗\n");
    printf("║          ⚔️  COMBAT SOUS-MARIN! ⚔️                       ║\n");
    printf("╚═══════════════════════════════════════════════════════════╝\n");
    printf("\n>>> %d creature(s) apparaisse(nt) des tenebres!\n\n", nb_creatures);
    for (int i = 0; i < nb_creatures; i++) {
        printf("    🐟 %s (PV: %d/%d, ATK: %d-%d)\n",
               creatures[i].nom,
               creatures[i].points_de_vie_actuels,
               creatures[i].points_de_vie_max,
               creatures[i].attaque_minimale,
               creatures[i].attaque_maximale);
    }
    printf("\n");
    wait_for_enter();

    // Initialiser les compétences
    Competence competences[NB_COMPETENCES];
    init_competences(competences);

    paralysie_actif = 0;

    int tour = 0;
    while (player->points_de_vie > 0) {
        tour++;

        // Vérifier s'il reste des créatures vivantes
        int creatures_vivantes = 0;
        for (int i = 0; i < nb_creatures; i++) {
            if (creatures[i].est_vivant) creatures_vivantes++;
        }

        if (creatures_vivantes == 0) {
            break; // Victoire
        }

        display_combat_status(player, creatures, nb_creatures, profondeur);

        printf("=== TOUR %d ===\n\n", tour);

        // Calculer le nombre d'attaques disponibles selon la fatigue
        int max_attaques = get_max_attaques_from_fatigue(player->niveau_fatigue);
        if (paralysie_actif > 0) {
            max_attaques--;
            if (max_attaques < 1) max_attaques = 1;
            printf("*** Vous etes paralyse! Attaques reduites de 1 ***\n");
        }

        printf("Attaques disponibles ce tour: %d (Fatigue: %d/5)\n\n", max_attaques, player->niveau_fatigue);

        int attaques_effectuees = 0;
        bool tour_termine = false;

        // Phase du joueur - plusieurs attaques possibles
        while (attaques_effectuees < max_attaques && !tour_termine) {
            printf("Actions disponibles:\n");
            printf("1 - Attaquer avec %s (Cout: %d O2)\n",
                   player->arme_equipee.nom, player->arme_equipee.consommation_oxygene);
            printf("2 - Utiliser un objet\n");
            printf("3 - Utiliser une competence\n");
            printf("4 - Terminer le tour\n");
            printf("> Votre choix : ");

            int choice = -1;
            char term;
            while (scanf("%d%c", &choice, &term) != 2 || term != '\n' || (choice < 1 || choice > 4)) {
                printf("Erreur : Veuillez entrer 1, 2, 3 ou 4.\n");
                while (getchar() != '\n');
                printf("> Votre choix : ");
            }

            if (choice == 4) {
                printf("Vous terminez votre tour.\n");
                tour_termine = true;
                break;
            }

            if (choice == 1) {
                // Attaque
                int oxygen_cost = player->arme_equipee.consommation_oxygene;

                if (player->niveau_oxygene < oxygen_cost) {
                    printf("\nPas assez d'oxygene pour attaquer! (Requis: %d O2, Vous avez: %d O2)\n",
                           oxygen_cost, player->niveau_oxygene);
                    wait_for_enter();
                    continue;
                }

                // Choisir la cible
                printf("\nChoisir la cible:\n");
                for (int i = 0; i < nb_creatures; i++) {
                    if (creatures[i].est_vivant) {
                        printf("  [%d] - %s (%d/%d PV)\n", i, creatures[i].nom,
                               creatures[i].points_de_vie_actuels, creatures[i].points_de_vie_max);
                    }
                }
                printf("> Cible : ");

                int target = -1;
                while (scanf("%d%c", &target, &term) != 2 || term != '\n' ||
                       target < 0 || target >= nb_creatures || !creatures[target].est_vivant) {
                    printf("Erreur : Cible invalide.\n");
                    while (getchar() != '\n');
                    printf("> Cible : ");
                }

                player->niveau_oxygene -= oxygen_cost;
                printf("\nVous consommez %d oxygene. (Restant: %d O2)\n",
                       oxygen_cost, player->niveau_oxygene);

                // Calcul des dégâts avec défense
                int player_damage = randomNumber(
                    player->arme_equipee.attaque_minimale,
                    player->arme_equipee.attaque_maximale
                );

                int defense = creatures[target].defense;

                // Crabe Géant: Carapace réduit dégâts de 20%
                if (strcmp(creatures[target].effet_special, "carapace") == 0) {
                    player_damage = (int)(player_damage * 0.8f);
                    printf(">>> La carapace du %s reduit les degats de 20%%!\n", creatures[target].nom);
                }

                // Armes spéciales: ignorent une partie de la défense
                if (strcmp(player->arme_equipee.effet_special, "electrique") == 0 ||
                    strcmp(player->arme_equipee.effet_special, "laser") == 0) {
                    defense -= player->arme_equipee.special;
                    if (defense < 0) defense = 0;
                    printf(">>> Votre %s ignore %d points de defense!\n",
                           player->arme_equipee.nom, player->arme_equipee.special);
                }

                player_damage -= defense;
                if (player_damage < 1) player_damage = 1; // Minimum garanti

                creatures[target].points_de_vie_actuels -= player_damage;
                printf("Vous attaquez le %s et lui infligez %d degats! (Defense: -%d)\n",
                       creatures[target].nom, player_damage, defense);

                if (creatures[target].points_de_vie_actuels <= 0) {
                    creatures[target].points_de_vie_actuels = 0;
                    creatures[target].est_vivant = false;
                    printf(">>> %s est elimine!\n", creatures[target].nom);
                    player->creatures_tuees++;

                    // Gain d'XP basé sur la créature
                    int xp_gagne = 20 + (abs(profondeur) / 10);
                    gagner_experience(player, xp_gagne);
                }

                attaques_effectuees++;
                player->niveau_fatigue++; // Augmente de +1 par attaque
                if (player->niveau_fatigue > 5) player->niveau_fatigue = 5;

                wait_for_enter();

            } else if (choice == 2) {
                // Utiliser Objet
                if (handle_item_use(player)) {
                    // Utiliser un objet compte comme une action mais n'augmente pas la fatigue
                    wait_for_enter();
                }
            } else if (choice == 3) {
                // Utiliser Compétence
                afficher_menu_competences(competences, player);
                printf("\n> Choisir une competence (1-4) ou 5 pour annuler: ");
                int comp_choice = -1;
                while (scanf("%d%c", &comp_choice, &term) != 2 || term != '\n' || (comp_choice < 1 || comp_choice > 5)) {
                    printf("Erreur : Veuillez entrer un numero entre 1 et 5.\n");
                    while (getchar() != '\n');
                    printf("> Choisir une competence : ");
                }

                if (comp_choice != 5) {
                    if (utiliser_competence(comp_choice - 1, competences, player, creatures, nb_creatures)) {
                        // Compétence utilisée avec succès
                    }
                }
                wait_for_enter();
            }
        }

        // Récupération naturelle d'oxygène en combat (+8)
        player->niveau_oxygene += 8;
        if (player->niveau_oxygene > player->niveau_oxygene_max) {
            player->niveau_oxygene = player->niveau_oxygene_max;
        }
        printf("\n>>> Recuperation naturelle d'oxygene: +8 O2\n");

        // Consommation oxygène automatique par tour selon profondeur
        int conso_auto = 2 + (abs(profondeur) / 50); // -2 à -5 selon profondeur
        player->niveau_oxygene -= conso_auto;
        printf(">>> Consommation oxygene automatique: -%d O2\n", conso_auto);

        // Vérification critique
        if (player->niveau_oxygene <= 10) {
            printf("\n*** ALERTE CRITIQUE: NIVEAU D'OXYGENE FAIBLE! ***\n");
        }

        if (player->niveau_oxygene <= 0) {
            player->niveau_oxygene = 0;
            printf("\n*** OXYGENE EPUISE! VOUS PERDEZ 5 PV! ***\n");
            player->points_de_vie -= 5;
            if (player->points_de_vie < 0) player->points_de_vie = 0;
        }

        // Tour des créatures (par ordre de vitesse)
        int indices_ordre[MAX_CREATURES];
        trier_creatures_par_vitesse(creatures, indices_ordre, nb_creatures);

        printf("\n=== TOUR DES CREATURES ===\n");
        for (int i = 0; i < nb_creatures; i++) {
            int idx = indices_ordre[i];
            if (!creatures[idx].est_vivant) continue;

            int enemy_damage = randomNumber(
                creatures[idx].attaque_minimale,
                creatures[idx].attaque_maximale
            );

            // Appliquer effets spéciaux
            appliquer_effet_special_creature(&creatures[idx], player, &enemy_damage);

            // Appliquer la défense de la combinaison
            int defense_joueur = get_defense_equipement(&player->combinaison_equipee);

            // Poisson-Épée: Ignore 2 points de défense
            if (strcmp(creatures[idx].effet_special, "perforation") == 0) {
                defense_joueur -= 2;
                if (defense_joueur < 0) defense_joueur = 0;
            }

            int degats_finaux = enemy_damage - defense_joueur;
            if (degats_finaux < 1) degats_finaux = 1; // Minimum 1 dégât

            player->points_de_vie -= degats_finaux;
            printf(">>> %s attaque et vous inflige %d degats! (Defense: -%d)\n",
                   creatures[idx].nom, degats_finaux, defense_joueur);

            // Consommation oxygène supplémentaire (stress)
            int stress_oxy = randomNumber(1, 2);
            player->niveau_oxygene -= stress_oxy;
            printf(">>> Stress: -%d O2\n", stress_oxy);

            // Kraken: 2 attaques consécutives
            if (strcmp(creatures[idx].effet_special, "etreinte") == 0) {
                int enemy_damage2 = randomNumber(
                    creatures[idx].attaque_minimale,
                    creatures[idx].attaque_maximale
                );
                int degats_finaux2 = enemy_damage2 - defense_joueur;
                if (degats_finaux2 < 1) degats_finaux2 = 1; // Minimum 1 dégât

                player->points_de_vie -= degats_finaux2;
                printf(">>> %s attaque une 2eme fois et inflige %d degats! (Defense: -%d)\n",
                       creatures[idx].nom, degats_finaux2, defense_joueur);
            }

            if (player->points_de_vie <= 0) {
                player->points_de_vie = 0;
                printf("\nVous avez ete vaincu...\n");
                wait_for_enter();
                return;
            }
        }

        // Réduire les cooldowns
        reduire_cooldowns(competences);

        // Réduire paralysie
        if (paralysie_actif > 0) {
            paralysie_actif--;
        }

        // Récupération fatigue (+1 niveau, donc diminution)
        if (player->niveau_fatigue > 0) {
            player->niveau_fatigue--;
            printf("\n>>> Recuperation fatigue: -1 (Fatigue: %d/5)\n", player->niveau_fatigue);
        }

        wait_for_enter();
    }

    // Victoire
    if (player->points_de_vie > 0) {
        printf("\n");
        printf("╔═══════════════════════════════════════════════════════════╗\n");
        printf("║                  🏆 VICTOIRE! 🏆                         ║\n");
        printf("╚═══════════════════════════════════════════════════════════╝\n");
        printf("    Vous avez vaincu toutes les creatures!\n\n");

        // Récompenses
        printf("=== RECOMPENSES ===\n");
        int total_perles = 0;

        for (int i = 0; i < nb_creatures; i++) {
            int perles = randomNumber(creatures[i].perles_min, creatures[i].perles_max);
            total_perles += perles;
            printf("  %s: %d perles\n", creatures[i].nom, perles);
        }
        player->perles += total_perles;
        printf("\nTotal perles: +%d (Total: %d perles)\n", total_perles, player->perles);

        // Drops d'équipements (chance selon profondeur)
        int prof_abs = abs(profondeur);
        int drop_chance = 10 + (prof_abs / 100); // 10-25% de chance

        if (rand() % 100 < drop_chance) {
            printf("\n>>> Un objet a ete trouve!\n");

            // Déterminer le type de drop
            int drop_type = rand() % 100;

            if (drop_type < 60) { // 60% consommables
                Consommable item;
                int item_roll = rand() % 100;
                if (item_roll < 40) {
                    item = creer_trousse_de_soin();
                } else if (item_roll < 70) {
                    item = creer_capsule_oxygene();
                } else if (item_roll < 85) {
                    item = creer_stimulant_marin();
                } else {
                    item = creer_antidote();
                }

                // Trouver un slot vide
                int slot = -1;
                for (int i = 0; i < 8; i++) {
                    if (player->inventaire[i].id == 0) {
                        slot = i;
                        break;
                    }
                }

                if (slot != -1) {
                    player->inventaire[slot] = item;
                    printf(">>> %s ajoute a l'inventaire (Slot %d)!\n", item.nom, slot + 1);
                } else {
                    printf(">>> Inventaire plein! %s perdu.\n", item.nom);
                }
            } else { // 40% armes/combinaisons (selon profondeur)
                if (prof_abs > 500 && rand() % 100 < 30) {
                    // Chance d'arme rare en profondeur
                    if (prof_abs > 1000 && rand() % 100 < 20) {
                        printf(">>> RARE: Harpon Electrique trouve!\n");
                        printf(">>> (Disponible chez le marchand pour %d perles)\n", 150);
                        player->perles += 75; // Bonus perles à la place
                    } else {
                        printf(">>> RARE: Harpon d'Acier trouve!\n");
                        printf(">>> (Disponible chez le marchand pour %d perles)\n", 80);
                        player->perles += 40; // Bonus perles à la place
                    }
                }
            }
        }

        // Gain d'oxygène
        int oxygen_gain = 20;
        player->niveau_oxygene += oxygen_gain;
        if (player->niveau_oxygene > player->niveau_oxygene_max) {
            player->niveau_oxygene = player->niveau_oxygene_max;
        }
        printf("Vous reprenez votre souffle: +%d O2! (Actuel: %d/%d)\n",
               oxygen_gain, player->niveau_oxygene, player->niveau_oxygene_max);

        wait_for_enter();
    }
}

// Combat avec créatures déjà générées - version qui retourne bool
// Retourne true si victoire, false si défaite
bool combat(Plongeur* player, CreatureMarine* creatures, int nb_creatures, int profondeur) {
    printf("\n");
    printf("╔═══════════════════════════════════════════════════════════╗\n");
    printf("║          ⚔️  COMBAT SOUS-MARIN! ⚔️                       ║\n");
    printf("╚═══════════════════════════════════════════════════════════╝\n");
    printf("\n>>> %d creature(s) apparaisse(nt) des tenebres!\n\n", nb_creatures);
    for (int i = 0; i < nb_creatures; i++) {
        printf("    🐟 %s (PV: %d/%d, ATK: %d-%d)\n",
               creatures[i].nom,
               creatures[i].points_de_vie_actuels,
               creatures[i].points_de_vie_max,
               creatures[i].attaque_minimale,
               creatures[i].attaque_maximale);
    }
    printf("\n");
    wait_for_enter();

    // Initialiser les compétences
    Competence competences[NB_COMPETENCES];
    init_competences(competences);

    paralysie_actif = 0;

    int tour = 0;
    while (player->points_de_vie > 0) {
        tour++;

        // Vérifier s'il reste des créatures vivantes
        int creatures_vivantes = 0;
        for (int i = 0; i < nb_creatures; i++) {
            if (creatures[i].est_vivant) creatures_vivantes++;
        }

        if (creatures_vivantes == 0) {
            break; // Victoire
        }

        display_combat_status(player, creatures, nb_creatures, profondeur);

        printf("=== TOUR %d ===\n\n", tour);

        // Calculer le nombre d'attaques disponibles selon la fatigue
        int max_attaques = get_max_attaques_from_fatigue(player->niveau_fatigue);
        if (paralysie_actif > 0) {
            max_attaques--;
            if (max_attaques < 1) max_attaques = 1;
            printf("*** Vous etes paralyse! Attaques reduites de 1 ***\n");
        }

        printf("Attaques disponibles ce tour: %d (Fatigue: %d/5)\n\n", max_attaques, player->niveau_fatigue);

        int attaques_effectuees = 0;
        bool tour_termine = false;

        // Phase du joueur - plusieurs attaques possibles
        while (attaques_effectuees < max_attaques && !tour_termine) {
            printf("Actions disponibles:\n");
            printf("1 - Attaquer avec %s (Cout: %d O2)\n",
                   player->arme_equipee.nom, player->arme_equipee.consommation_oxygene);
            printf("2 - Utiliser un objet\n");
            printf("3 - Utiliser une competence\n");
            printf("4 - Terminer le tour\n");
            printf("> Votre choix : ");

            int choice = -1;
            char term;
            while (scanf("%d%c", &choice, &term) != 2 || term != '\n' || (choice < 1 || choice > 4)) {
                printf("Erreur : Veuillez entrer 1, 2, 3 ou 4.\n");
                while (getchar() != '\n');
                printf("> Votre choix : ");
            }

            if (choice == 4) {
                printf("Vous terminez votre tour.\n");
                tour_termine = true;
                break;
            }

            if (choice == 1) {
                // Attaque
                int oxygen_cost = player->arme_equipee.consommation_oxygene;

                if (player->niveau_oxygene < oxygen_cost) {
                    printf("\nPas assez d'oxygene pour attaquer! (Requis: %d O2, Vous avez: %d O2)\n",
                           oxygen_cost, player->niveau_oxygene);
                    wait_for_enter();
                    continue;
                }

                // Choisir la cible
                printf("\nChoisir la cible:\n");
                for (int i = 0; i < nb_creatures; i++) {
                    if (creatures[i].est_vivant) {
                        printf("  [%d] - %s (%d/%d PV)\n", i, creatures[i].nom,
                               creatures[i].points_de_vie_actuels, creatures[i].points_de_vie_max);
                    }
                }
                printf("> Cible : ");

                int target = -1;
                while (scanf("%d%c", &target, &term) != 2 || term != '\n' ||
                       target < 0 || target >= nb_creatures || !creatures[target].est_vivant) {
                    printf("Erreur : Cible invalide.\n");
                    while (getchar() != '\n');
                    printf("> Cible : ");
                }

                player->niveau_oxygene -= oxygen_cost;
                printf("\nVous consommez %d oxygene. (Restant: %d O2)\n",
                       oxygen_cost, player->niveau_oxygene);

                // Calcul des dégâts avec défense
                int player_damage = randomNumber(
                    player->arme_equipee.attaque_minimale,
                    player->arme_equipee.attaque_maximale
                );

                int defense = creatures[target].defense;

                // Crabe Géant: Carapace réduit dégâts de 20%
                if (strcmp(creatures[target].effet_special, "carapace") == 0) {
                    player_damage = (int)(player_damage * 0.8f);
                    printf(">>> La carapace du %s reduit les degats de 20%%!\n", creatures[target].nom);
                }

                // Armes spéciales: ignorent une partie de la défense
                if (strcmp(player->arme_equipee.effet_special, "electrique") == 0 ||
                    strcmp(player->arme_equipee.effet_special, "laser") == 0) {
                    defense -= player->arme_equipee.special;
                    if (defense < 0) defense = 0;
                    printf(">>> Votre %s ignore %d points de defense!\n",
                           player->arme_equipee.nom, player->arme_equipee.special);
                }

                player_damage -= defense;
                if (player_damage < 1) player_damage = 1; // Minimum garanti

                creatures[target].points_de_vie_actuels -= player_damage;
                printf("Vous attaquez le %s et lui infligez %d degats! (Defense: -%d)\n",
                       creatures[target].nom, player_damage, defense);

                if (creatures[target].points_de_vie_actuels <= 0) {
                    creatures[target].points_de_vie_actuels = 0;
                    creatures[target].est_vivant = false;
                    printf(">>> %s est elimine!\n", creatures[target].nom);
                    player->creatures_tuees++;

                    // Gain d'XP basé sur la créature
                    int xp_gagne = 20 + (abs(profondeur) / 10);
                    gagner_experience(player, xp_gagne);
                }

                attaques_effectuees++;
                player->niveau_fatigue++; // Augmente de +1 par attaque
                if (player->niveau_fatigue > 5) player->niveau_fatigue = 5;

                wait_for_enter();

            } else if (choice == 2) {
                // Utiliser Objet
                if (handle_item_use(player)) {
                    // Utiliser un objet compte comme une action mais n'augmente pas la fatigue
                    wait_for_enter();
                }
            } else if (choice == 3) {
                // Utiliser Compétence
                afficher_menu_competences(competences, player);
                printf("\n> Choisir une competence (1-4) ou 5 pour annuler: ");
                int comp_choice = -1;
                while (scanf("%d%c", &comp_choice, &term) != 2 || term != '\n' || (comp_choice < 1 || comp_choice > 5)) {
                    printf("Erreur : Veuillez entrer un numero entre 1 et 5.\n");
                    while (getchar() != '\n');
                    printf("> Choisir une competence : ");
                }

                if (comp_choice != 5) {
                    if (utiliser_competence(comp_choice - 1, competences, player, creatures, nb_creatures)) {
                        // Compétence utilisée avec succès
                    }
                }
                wait_for_enter();
            }
        }

        // Récupération naturelle d'oxygène en combat (+8)
        player->niveau_oxygene += 8;
        if (player->niveau_oxygene > player->niveau_oxygene_max) {
            player->niveau_oxygene = player->niveau_oxygene_max;
        }
        printf("\n>>> Recuperation naturelle d'oxygene: +8 O2\n");

        // Consommation oxygène automatique par tour selon profondeur
        int conso_auto = 2 + (abs(profondeur) / 50); // -2 à -5 selon profondeur
        player->niveau_oxygene -= conso_auto;
        printf(">>> Consommation oxygene automatique: -%d O2\n", conso_auto);

        // Vérification critique manque d'oxygène
        if (player->niveau_oxygene <= 0) {
            player->niveau_oxygene = 0;
            printf("\n💀 VOUS MANQUEZ D'OXYGENE!\n");
            wait_for_enter();
            return false;  // Défaite par manque d'oxygène
        }

        // Phase des créatures - attaquent le joueur
        for (int i = 0; i < nb_creatures; i++) {
            if (!creatures[i].est_vivant) continue;

            int idx = i;

            // Poisson-Épée: ignore 2 points de défense
            int defense_joueur = get_defense_equipement(&player->combinaison_equipee);
            if (strcmp(creatures[idx].effet_special, "perforation") == 0) {
                defense_joueur -= 2;
                if (defense_joueur < 0) defense_joueur = 0;
            }

            int enemy_damage = randomNumber(
                creatures[idx].attaque_minimale,
                creatures[idx].attaque_maximale
            );

            int degats_finaux = enemy_damage - defense_joueur;
            if (degats_finaux < 1) degats_finaux = 1; // Minimum 1 dégât

            player->points_de_vie -= degats_finaux;
            printf(">>> %s attaque et vous inflige %d degats! (Defense: -%d)\n",
                   creatures[idx].nom, degats_finaux, defense_joueur);

            // Consommation oxygène supplémentaire (stress)
            int stress_oxy = randomNumber(1, 2);
            player->niveau_oxygene -= stress_oxy;
            printf(">>> Stress: -%d O2\n", stress_oxy);

            // Kraken: 2 attaques consécutives
            if (strcmp(creatures[idx].effet_special, "etreinte") == 0) {
                int enemy_damage2 = randomNumber(
                    creatures[idx].attaque_minimale,
                    creatures[idx].attaque_maximale
                );
                int degats_finaux2 = enemy_damage2 - defense_joueur;
                if (degats_finaux2 < 1) degats_finaux2 = 1; // Minimum 1 dégât

                player->points_de_vie -= degats_finaux2;
                printf(">>> %s attaque une 2eme fois et inflige %d degats! (Defense: -%d)\n",
                       creatures[idx].nom, degats_finaux2, defense_joueur);
            }

            if (player->points_de_vie <= 0) {
                player->points_de_vie = 0;
                printf("\nVous avez ete vaincu...\n");
                wait_for_enter();
                return false;  // Défaite
            }
        }

        // Réduire les cooldowns
        reduire_cooldowns(competences);

        // Réduire paralysie
        if (paralysie_actif > 0) {
            paralysie_actif--;
        }

        // Récupération fatigue (+1 niveau, donc diminution)
        if (player->niveau_fatigue > 0) {
            player->niveau_fatigue--;
            printf("\n>>> Recuperation fatigue: -1 (Fatigue: %d/5)\n", player->niveau_fatigue);
        }

        wait_for_enter();
    }

    // Victoire
    if (player->points_de_vie > 0) {
        printf("\n");
        printf("╔═══════════════════════════════════════════════════════════╗\n");
        printf("║                  🏆 VICTOIRE! 🏆                         ║\n");
        printf("╚═══════════════════════════════════════════════════════════╝\n");
        printf("    Vous avez vaincu toutes les creatures!\n\n");

        // Récompenses
        printf("=== RECOMPENSES ===\n");
        int total_perles = 0;

        for (int i = 0; i < nb_creatures; i++) {
            int perles = randomNumber(creatures[i].perles_min, creatures[i].perles_max);
            total_perles += perles;
            printf("  %s: %d perles\n", creatures[i].nom, perles);
        }
        player->perles += total_perles;
        printf("\nTotal perles: +%d (Total: %d perles)\n", total_perles, player->perles);

        wait_for_enter();
        return true;  // Victoire!
    }

    return false;  // Défaite (ne devrait pas arriver ici)
}
