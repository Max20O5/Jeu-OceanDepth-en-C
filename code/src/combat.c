#include "combat.h"
#include "creature.h"
#include "consommable.h"
#include "competence.h"
#include "include.h"
#include "utils.h"
#include "effects.h"

void print_progress_bar(int current, int max, int length) {
    if (max == 0) max = 1;
    if (current < 0) current = 0;
    float percent = (float)current / max;
    int filled = (int)(percent * length);
    int empty = length - filled;
    printf("[");
    for (int i = 0; i < filled; i++) printf("█");
    for (int i = 0; i < empty; i++) printf("▒");
    printf("]");
}

void display_combat_status_profondeur(Plongeur* player, CreatureMarine* enemies, int enemy_count, int profondeur) {
    clear_screen();

    // --- Section Joueur ---
    printf("OceanDepths - Profondeur: %dm | Niv: %d | XP: %d/%d | Perles: %d\n",
           profondeur, player->niveau, player->experience, player->experience_prochain_niveau, player->perles);
    printf("Vie     ");
    print_progress_bar(player->points_de_vie, player->points_de_vie_max, 50);
    printf(" %d/%d\n", player->points_de_vie, player->points_de_vie_max);
    printf("Oxygène ");
    print_progress_bar(player->niveau_oxygene, player->niveau_oxygene_max, 40);
    printf(" %d/%d", player->niveau_oxygene, player->niveau_oxygene_max);
    if (player->niveau_oxygene <= 10) {
        printf(" *** ALERTE CRITIQUE ***");
    }
    printf("\n");
    printf("Fatigue ");
    print_progress_bar(player->niveau_fatigue, 5, 10);
    printf(" %d/5\n", player->niveau_fatigue);

    printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");

    // --- Section Ennemis ---
    for (int i = 0; i < enemy_count; i++) {
        if (enemies[i].est_vivant) {
            printf("  [%d] %s \t\t (%d/%d PV)\n",
                   i + 1, enemies[i].nom,
                   enemies[i].points_de_vie_actuels, enemies[i].points_de_vie_max);
        }
    }
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
}

void display_combat_status(Plongeur* player, CreatureMarine* enemies, int enemy_count) {
    display_combat_status_profondeur(player, enemies, enemy_count, -150);
}

// === Gestion de l'inventaire ===

void apply_item_effects(Plongeur* player, Consommable* item) {
    printf("\nVous utilisez %s !\n", item->nom);
    if (item->soin > 0) {
        player->points_de_vie += item->soin;
        if (player->points_de_vie > player->points_de_vie_max) player->points_de_vie = player->points_de_vie_max;
        printf("Vous récupérez %d PV. (Vie: %d/%d)\n", item->soin, player->points_de_vie, player->points_de_vie_max);
    }
    if (item->oxygene > 0) {
        player->niveau_oxygene += item->oxygene;
        if (player->niveau_oxygene > player->niveau_oxygene_max) player->niveau_oxygene = player->niveau_oxygene_max;
        printf("Vous récupérez %d O². (Oxygène: %d/%d)\n", item->oxygene, player->niveau_oxygene, player->niveau_oxygene_max);
    }
    if (item->fatigue != 0) {
        player->niveau_fatigue += item->fatigue;
        
        if (player->niveau_fatigue < 0) player->niveau_fatigue = 0;
        if (player->niveau_fatigue > 5) player->niveau_fatigue = 5;
        
        if (item->fatigue < 0) {
            printf("Vous sentez l'adrénaline monter ! (Fatigue: %d/5)\n", player->niveau_fatigue);
        } else {
            printf("Vous vous sentez plus lourd... (Fatigue: %d/5)\n", player->niveau_fatigue);
        }
    }
}    


bool handle_item_use(Plongeur* player) {
    clear_screen();
    
    printf("---  INVENTAIRE ---\n");
    printf("Quel objet utiliser ? (0 pour annuler)\n\n");
    for (int i = 0; i < 8; i++) printf("  [%d] - %s\n", i + 1, player->inventaire[i].nom);
    printf("\n> Votre choix : ");
    int slot_choice = -1; char term;

    while (scanf("%d%c", &slot_choice, &term) != 2 || term != '\n' || (slot_choice < 0 || slot_choice > 8)) {
        printf("Erreur : Entrez un numéro de slot (1-8) ou 0 pour annuler.\n");
        clear_input_buffer();
        printf("> Votre choix : ");
    }

    if (slot_choice == 0) { printf("Action annulée.\n"); return false; }
    int slot_index = slot_choice - 1;
    if (player->inventaire[slot_index].id == 0) {
        printf("Ce slot est vide ! Action annulée.\n");
        return false;
    }
    apply_item_effects(player, &player->inventaire[slot_index]);
    player->inventaire[slot_index] = get_consommable_by_id(0);
    return true;
}

// === Logique d'attaque du joueur ===

bool handle_player_attack(Plongeur* player, CreatureMarine* enemies, int enemy_count) {
    printf("\nQui attaquer ? (1-%d, 0 pour annuler)\n", enemy_count);
    printf("> Votre choix : ");

    int target_choice = -1;
    char term;
    while (scanf("%d%c", &target_choice, &term) != 2 || term != '\n' || (target_choice < 0 || target_choice > enemy_count)) {
        printf("Erreur : Entrez 1-%d ou 0.\n", enemy_count);
        clear_input_buffer();
        printf("> Votre choix : ");
    }

    if (target_choice == 0) {
        printf("Action annulée.\n");
        return false;
    }

    int target_index = target_choice - 1;

    // Vérifier si la cible est déjà morte
    if (!enemies[target_index].est_vivant) {
        printf("Cette créature est déjà morte ! Action annulée.\n");
        return false;
    }

    // Vérifier l'oxygène
    int oxygen_cost = player->arme_equipee.consommation_oxygene;
    if (player->niveau_oxygene < oxygen_cost) {
        printf("\nPas assez d'oxygène pour attaquer ! (Requis: %d O², Vous avez: %d O²)\n", 
               oxygen_cost, player->niveau_oxygene);
        return false;
    }

    // --- L'ATTAQUE EST VALIDE ---
    player->niveau_oxygene -= oxygen_cost;
    printf("\nVous consommez %d oxygène. (Restant: %d O²)\n", 
           oxygen_cost, player->niveau_oxygene);
    
int damage = randomNumber(player->arme_equipee.attaque_minimale, player->arme_equipee.attaque_maximale);
    enemies[target_index].points_de_vie_actuels -= damage;
    printf("Vous attaquez le %s et lui infligez %d dégâts !\n", enemies[target_index].nom, damage);
    apply_weapon_effects(player, &enemies[target_index], player->arme_equipee, damage);

    // Vérifier si l'ennemi est mort
    if (enemies[target_index].points_de_vie_actuels <= 0) {
        enemies[target_index].points_de_vie_actuels = 0;
        enemies[target_index].est_vivant = false;
        printf("Vous avez vaincu le %s !\n", enemies[target_index].nom);
    }
    
    return true;
}

// === Tour des ennemis ===

void handle_enemies_turn(Plongeur* player, CreatureMarine* enemies, int enemy_count) {
    printf("\n--- Tour des Ennemis ---\n");

    for (int i = 0; i < enemy_count; i++) {
        if (enemies[i].est_vivant) {
            
            // 1. Gérer les effets de statut SUR l'ennemi
            bool can_act = process_enemy_effects(&enemies[i]);

            // Si l'ennemi n'est pas mort ou paralysé, il attaque
            if (can_act) {
                // Attaque de base
                int damage = randomNumber(enemies[i].attaque_minimale, enemies[i].attaque_maximale);
                player->points_de_vie -= damage;
                printf("%s riposte et vous inflige %d dégâts !\n", enemies[i].nom, damage);

                // Gérer l'application de l'effet spécial de la créature
                
                if (strcmp(enemies[i].effet_special, "Paralysie") == 0) {
                    if (randomNumber(1, 100) <= 30) {
                        printf("Touché ! L'attaque de %s vous paralyse !\n", enemies[i].nom);
                        apply_status_to_player(player, EFFET_PARALYSIE, 1, 0);
                    }
                }
                else if (strcmp(enemies[i].effet_special, "Charme") == 0) {
                     if (randomNumber(1, 100) <= 15) {
                        printf("Le chant de %s vous charme !\n", enemies[i].nom);
                        apply_status_to_player(player, EFFET_CHARME, 2, 0);
                    }
                }
                else if (strcmp(enemies[i].effet_special, "Brulure") == 0) {
                     if (randomNumber(1, 100) <= 25) {
                        printf("%s vous inflige des brûlures !\n", enemies[i].nom);
                        apply_status_to_player(player, EFFET_BRULURE, 3, 5);
                    }
                }
            }
        }
    }

    if (player->points_de_vie <= 0) {
        player->points_de_vie = 0;
        printf("Vous avez été vaincu...\n");
    }
}

// === Vérifier la victoire ===

bool are_all_enemies_dead(CreatureMarine* enemies, int enemy_count) {
    for (int i = 0; i < enemy_count; i++) {
        if (enemies[i].est_vivant) {
            return false;
        }
    }
    return true; 
}

// === Gérer les récompenses ===

void handle_victory(Plongeur* player, CreatureMarine* enemies, int enemy_count) {
    printf("\n--- VICTOIRE ! ---\n");
    int total_perles = 0;
    
    // On additionne les perles de tous les ennemis
    for (int i = 0; i < enemy_count; i++) {
        total_perles += randomNumber(enemies[i].perles_min, enemies[i].perles_max);
    }
    
    player->perles += total_perles;
    printf("Vous récupérez un total de %d perles!\n", total_perles);
    printf("Vous avez maintenant %d perles au total.\n", player->perles);

    int oxygen_gain = 20 * enemy_count; 
    player->niveau_oxygene += oxygen_gain;
    if (player->niveau_oxygene > player->niveau_oxygene_max) {
        player->niveau_oxygene = player->niveau_oxygene_max;
    }
    printf("Vous reprenez votre souffle et gagnez %d O² ! (Actuel: %d/%d)\n", 
           oxygen_gain, player->niveau_oxygene, player->niveau_oxygene_max);
}

// Calculer le nombre d'attaques max selon la fatigue
int get_max_attaques_from_fatigue(int fatigue) {
    if (fatigue <= 1) return 3;
    if (fatigue <= 3) return 2;
    return 1; // fatigue 4-5
}

void start_combat(Plongeur* player, CreatureMarine* enemies, int enemy_count) {

    printf("Des créatures sauvages apparaissent des profondeurs !\n");
    wait_for_enter();

    // Boucle de combat principale
    while (player->points_de_vie > 0) {

        display_combat_status(player, enemies, enemy_count);

        // Calculer le nombre max d'attaques selon la fatigue
        int max_attaques = get_max_attaques_from_fatigue(player->niveau_fatigue);
        int attaques_effectuees = 0;
        bool tour_termine = false;

        printf("\n--- VOTRE TOUR ---\n");
        printf("Attaques disponibles ce tour: %d (Fatigue: %d/5)\n", max_attaques, player->niveau_fatigue);

        // Phase du joueur - plusieurs attaques possibles
        while (attaques_effectuees < max_attaques && !tour_termine) {
            printf("\nActions disponibles:\n");
            printf("1 - Attaquer (Coût: %d O²)\n", player->arme_equipee.consommation_oxygene);
            printf("2 - Utiliser un objet\n");
            printf("3 - Utiliser une compétence\n");
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

            // --- GESTION DES ACTIONS ---
            bool action_reussie = false;
            switch (choice) {
                case 1:
                    action_reussie = handle_player_attack(player, enemies, enemy_count);
                    if (action_reussie) {
                        attaques_effectuees++;
                        player->niveau_fatigue++;
                        if (player->niveau_fatigue > 5) player->niveau_fatigue = 5;
                        printf("(Attaques effectuées: %d/%d) (Fatigue: %d/5)\n",
                               attaques_effectuees, max_attaques, player->niveau_fatigue);
                    }
                    break;
                case 2:
                    action_reussie = handle_item_use(player);
                    if (action_reussie) {
                        tour_termine = true; // Utiliser un objet termine le tour
                    }
                    break;
                case 3:
                    action_reussie = handle_skill_use(player, enemies, enemy_count);
                    if (action_reussie) {
                        tour_termine = true; // Utiliser une compétence termine le tour
                    }
                    break;
            }

            // --- VERIFICATION DE VICTOIRE ---
            if (are_all_enemies_dead(enemies, enemy_count)) {
                handle_victory(player, enemies, enemy_count);
                return;
            }
        }

        // --- TOUR DES ENNEMIS ---
        handle_enemies_turn(player, enemies, enemy_count);
        if (player->points_de_vie <= 0) {
            printf("\nVous sombrez dans les limbes pour l'éternité...\n");
            break;
        }

        // Réduction de la fatigue en fin de tour
        if (player->niveau_fatigue > 0) {
            player->niveau_fatigue--;
            printf("\n~ Vous reprenez votre souffle. (Fatigue réduite à %d/5) ~\n", player->niveau_fatigue);
        }

        reduce_all_skill_cooldowns(player);

        if (player->points_de_vie > 0) {
            wait_for_enter();
        }
    }

    printf("\nFin du combat.\n");
    wait_for_enter();
}