#include "game.h"
#include "joueur.h"
#include "combat.h"
#include "marchand.h"
#include <stdio.h>

void start_game() {
    Plongeur player = create_player();

    display_player_stats(player);
    printf("La plongée commence...\n");
    printf("Appuyez sur Entrée pour descendre...");
    while(getchar() != '\n');

    start_combat(&player); 

    printf("\nDe retour de votre premier combat :\n");
    display_player_stats(player);
    
    printf("Vous trouvez une grotte abritant un mystérieux marchand...\n");
    printf("Appuyez sur Entrée pour entrer...");
    while(getchar() != '\n');
    
    open_shop(&player);

    printf("\nVous quittez la grotte.\n");
    printf("Appuyez sur Entrée pour revenir au menu principal...");
    while(getchar() != '\n');
}

void load_game() {
    printf("Chargement de la partie\n");
    printf("Appuyez sur Entrée pour revenir au menu principal");

    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}