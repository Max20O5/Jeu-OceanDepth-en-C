#include "combat.h"
#include "creature.h"
#include "include.h"
#include "utils.h"

static void wait_for_enter() {
    printf("\nAppuyez sur Entrée pour continuer...");

    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

static void display_combat_status(Plongeur* player, CreatureMarine* creature) {
    clear_screen();
    printf("==================== COMBAT  ====================\n\n");
    
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


void start_combat(Plongeur* player) {    
    CreatureMarine enemy = create_kraken();
    printf("Un %s sauvage apparaît des profondeurs !\n", enemy.nom);
    wait_for_enter();

    while (player->points_de_vie > 0 && enemy.est_vivant) {
        
        display_combat_status(player, &enemy);
        
        printf("\nActions disponibles:\n");
        printf("1 - Attaquer avec %s\n", player->arme_equipee.nom);
        printf("> Votre choix : ");
        
        int choice = -1;
        char term;
        while (scanf("%d%c", &choice, &term) != 2 || term != '\n' || choice != 1) {
            printf("Erreur : Veuillez entrer 1 pour attaquer.\n");
            while (getchar() != '\n');
            printf("> Votre choix : ");
        }

        int player_damage = randomNumber(
            player->arme_equipee.attaque_minimale,
            player->arme_equipee.attaque_maximale
        );
        enemy.points_de_vie_actuels -= player_damage;
        printf("\nVous attaquez le %s et lui infligez %d dégâts !\n", enemy.nom, player_damage);

        if (enemy.points_de_vie_actuels <= 0) {
            enemy.points_de_vie_actuels = 0;
            enemy.est_vivant = false;
            printf("Vous avez vaincu le %s !\n", enemy.nom);
        } else {
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
        }
        
        if (player->points_de_vie > 0 && enemy.est_vivant) {
            wait_for_enter();
        }
    }

    printf("\nFin du combat.\n");
    wait_for_enter();
}