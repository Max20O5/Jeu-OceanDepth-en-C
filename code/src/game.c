#include "game.h"
#include "joueur.h"
#include "combat.h"
#include "creature.h"
#include "marchand.h"
#include <stdio.h>

#define MAX_ENEMIES_IN_ENCOUNTER 4

void start_game() {
    Plongeur player = create_player();
 
    display_player_stats(player);
    printf("La plongée commence...\n");
    printf("Appuyez sur Entrée pour descendre...");
    while(getchar() != '\n');

    CreatureMarine encounter_enemies[MAX_ENEMIES_IN_ENCOUNTER];
    int enemy_count = 2;
    
    encounter_enemies[0] = get_creature_by_id(10);
    encounter_enemies[1] = get_creature_by_id(20);

    start_combat(&player, encounter_enemies, enemy_count); 

    printf("\nDe retour de votre premier combat :\n");
    display_player_stats(player);
    
    printf("Vous trouvez une grotte abritant un mystérieux marchand...\n");
    printf("Appuyez sur Entrée pour entrer...");
    while(getchar() != '\n');
    
    open_shop(&player);

    printf("\nVous quittez la grotte et remontez à la surface.\n");
    printf("Appuyez sur Entrée pour revenir au menu principal...");
    while(getchar() != '\n');
}

void load_game() {
    printf("Chargement de la partie\n");
    printf("Appuyez sur Entrée pour revenir au menu principal");
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}