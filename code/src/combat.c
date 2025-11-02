#include "combat.h"
#include "creature.h"
#include "consommable.h"
#include "include.h"
#include "utils.h"

static void wait_for_enter() {
    printf("\nAppuyez sur Entrée pour continuer...");
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

static void display_combat_status(Plongeur* player, CreatureMarine* creature) {
    clear_screen();
    printf("====================  COMBAT  ====================\n\n");
    
    // Stats du Joueur
    printf("  PLAYER: %s\n", player->name);
    printf("  Vie      [ %d / %d ]\n", player->points_de_vie, player->points_de_vie_max);
    printf("  Oxygène  [ %d / %d ]\n", player->niveau_oxygene, player->niveau_oxygene_max);
    printf("  Fatigue  [ %d / 5 ]\n", player->niveau_fatigue);
    
    printf("\n                  VS\n\n");
    
    // Stats de la Créature
    printf("  ENNEMI: %s\n", creature->nom);
    printf("  Vie      [ %d / %d ]\n", creature->points_de_vie_actuels, creature->points_de_vie_max);
    
    printf("\n=======================================================\n");
}

static void apply_item_effects(Plongeur* player, Consommable* item) {
    printf("\nVous utilisez %s !\n", item->nom);

    if (item->soin > 0) {
        player->points_de_vie += item->soin;
        if (player->points_de_vie > player->points_de_vie_max) {
            player->points_de_vie = player->points_de_vie_max;
        }
        printf("Vous récupérez %d PV. (Vie: %d/%d)\n", item->soin, player->points_de_vie, player->points_de_vie_max);
    }

    if (item->oxygene > 0) {
        player->niveau_oxygene += item->oxygene;
        if (player->niveau_oxygene > player->niveau_oxygene_max) {
            player->niveau_oxygene = player->niveau_oxygene_max;
        }
        printf("Vous récupérez %d O². (Oxygène: %d/%d)\n", item->oxygene, player->niveau_oxygene, player->niveau_oxygene_max);
    }
}

static bool handle_item_use(Plongeur* player) {
    clear_screen();
    printf("---  INVENTAIRE  ---\n");
    printf("Quel objet utiliser ? (0 pour annuler)\n\n");

    for (int i = 0; i < 8; i++) {
        printf("  [%d] - %s\n", i + 1, player->inventaire[i].nom);
    }
    printf("\n> Votre choix : ");

    int slot_choice = -1;
    char term;
    while (scanf("%d%c", &slot_choice, &term) != 2 || term != '\n' || (slot_choice < 0 || slot_choice > 8)) {
        printf("Erreur : Entrez un numéro de slot (1-8) ou 0 pour annuler.\n");
        while (getchar() != '\n');
        printf("> Votre choix : ");
    }

    if (slot_choice == 0) {
        printf("Action annulée.\n");
        return false;
    }

    int slot_index = slot_choice - 1;

    if (player->inventaire[slot_index].id == 0) {
        printf("Ce slot est vide ! Action annulée.\n");
        return false;
    }

    apply_item_effects(player, &player->inventaire[slot_index]);
    player->inventaire[slot_index] = creer_slot_vide();
    
    return true;
}

void start_combat(Plongeur* player) {    
    CreatureMarine enemy = create_kraken();
    printf("Un %s sauvage apparaît des profondeurs !\n", enemy.nom);
    wait_for_enter();

    while (player->points_de_vie > 0 && enemy.est_vivant) {
        
        display_combat_status(player, &enemy);
        
        bool player_turn_over = false;

        printf("\nActions disponibles:\n");
        printf("1 - Attaquer avec %s (Coût: %d O²)\n", 
               player->arme_equipee.nom, player->arme_equipee.consommation_oxygene);
        printf("2 - Utiliser un objet\n");
        printf("> Votre choix : ");
        
        int choice = -1;
        char term;
        while (scanf("%d%c", &choice, &term) != 2 || term != '\n' || (choice != 1 && choice != 2)) {
            printf("Erreur : Veuillez entrer 1 ou 2.\n");
            while (getchar() != '\n');
            printf("> Votre choix : ");
        }

        // --- GESTION DU CHOIX ---
        if (choice == 1) {
            // --- Attaque ---
            int oxygen_cost = player->arme_equipee.consommation_oxygene;
            
            if (player->niveau_oxygene < oxygen_cost) {
                printf("\nPas assez d'oxygène pour attaquer ! (Requis: %d O², Vous avez: %d O²)\n", 
                       oxygen_cost, player->niveau_oxygene);
                player_turn_over = false;
            } else {
                player->niveau_oxygene -= oxygen_cost;
                printf("\nVous consommez %d oxygène. (Restant: %d O²)\n", 
                       oxygen_cost, player->niveau_oxygene);
                
                int player_damage = randomNumber(
                    player->arme_equipee.attaque_minimale,
                    player->arme_equipee.attaque_maximale
                );
                enemy.points_de_vie_actuels -= player_damage;
                printf("Vous attaquez le %s et lui infligez %d dégâts !\n", enemy.nom, player_damage);
                player_turn_over = true; 
            }

        } else if (choice == 2) {
            // --- Utiliser Objet ---
            player_turn_over = handle_item_use(player);
        }

        // --- TOUR DE L'ENNEMI ---
        if (player_turn_over && enemy.points_de_vie_actuels > 0) {
            int enemy_damage = randomNumber(
                enemy.attaque_minimale,
                enemy.attaque_maximale
            );
            player->points_de_vie -= enemy_damage;
            printf("Le %s riposte et vous inflige %d dégâts !\n", enemy.nom, enemy_damage);
            
            if (player->points_de_vie <= 0) {
                player->points_de_vie = 0;
                printf("Vous avez été vaincu...\n");
            }
        
        } else if (enemy.points_de_vie_actuels <= 0) {
            // --- L'ENNEMI EST MORT ---
            enemy.est_vivant = false;
            printf("Vous avez vaincu le %s !\n", enemy.nom);
            
            // Gain de perles
            int perles_gagnees = randomNumber(enemy.perles_min, enemy.perles_max);
            player->perles += perles_gagnees;
            printf("\n--- RÉCOMPENSES ---\n");
            printf("Vous récupérez %d perles sur la créature !\n", perles_gagnees);
            printf("Vous avez maintenant %d perles au total.\n", player->perles);

            //Gain d'oxygène
            int oxygen_gain = 20;
            player->niveau_oxygene += oxygen_gain;
            if (player->niveau_oxygene > player->niveau_oxygene_max) {
                player->niveau_oxygene = player->niveau_oxygene_max;
            }
            printf("\n--- BONUS D'OXYGENE ---\n");
            printf("Vous reprenez votre souffle et gagnez %d O² ! (Actuel: %d/%d)\n", 
                   oxygen_gain, player->niveau_oxygene, player->niveau_oxygene_max);
        }
        
        if (player->points_de_vie > 0 && enemy.est_vivant) {
            wait_for_enter();
        }
    }

    printf("\nFin du combat.\n");
    wait_for_enter();
}