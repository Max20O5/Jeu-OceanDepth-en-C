#include "game.h"
#include <stdio.h>

void start_game() {
    printf("Le jeu a commencé !\n");
    printf("Appuyez sur Entrée pour revenir au menu principal...");
    while(getchar() != '\n');
}