#include "game.h"
#include "joueur.h"
#include <stdio.h>

void start_game() {
    Plongeur player = create_player();
    display_player_stats(player);

    printf("Le jeu a commencé !\n");
    printf("Appuyez sur Entrée pour revenir au menu principal");

    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    while(getchar() != '\n');
}


void load_game() {
    printf("Chargement de la partie\n");
    printf("Appuyez sur Entrée pour revenir au menu principal");
    while(getchar() != '\n');
    getchar();
}