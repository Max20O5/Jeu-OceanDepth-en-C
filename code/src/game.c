#include "game.h"
#include "joueur.h"
#include "combat.h"
#include "marchand.h"
#include "map.h"
#include "save.h"
#include <stdio.h>
#include <stdlib.h>

void start_game() {
    Plongeur player = create_player();

    display_player_stats(player);
    printf("\n==================================================\n");
    printf("      Bienvenue dans OceanDepth, %s!\n", player.name);
    printf("==================================================\n\n");
    printf("Votre mission: Explorer les profondeurs oceaniques,\n");
    printf("combattre des creatures marines et decouvrir des tresors!\n\n");
    printf("Appuyez sur Entree pour commencer votre aventure...");
    while(getchar() != '\n');

    // Initialisation de la carte
    Carte* carte = init_carte();
    if (!carte) {
        printf("Erreur lors de l'initialisation de la carte!\n");
        return;
    }

    // Lancer la navigation sur la carte
    naviguer_carte(carte, &player);

    // Libérer la mémoire de la carte
    free_carte(carte);

    printf("\n==================================================\n");
    printf("      Fin de votre expedition, %s!\n", player.name);
    printf("==================================================\n");
    printf("Stats finales:\n");
    display_player_stats(player);
    printf("\nAppuyez sur Entree pour revenir au menu principal...");
    while(getchar() != '\n');
}

void load_game() {
    if (!sauvegarde_existe()) {
        printf("\n==================================================\n");
        printf("      Aucune sauvegarde trouvee!                  \n");
        printf("==================================================\n");
        printf("\nAppuyez sur Entree pour revenir au menu principal...");
        while(getchar() != '\n');
        return;
    }

    Plongeur player;
    Carte* carte = NULL;

    if (!charger_partie(&player, &carte)) {
        printf("\nErreur lors du chargement de la partie!\n");
        printf("Appuyez sur Entree pour revenir au menu principal...");
        while(getchar() != '\n');
        return;
    }

    printf("\nAppuyez sur Entree pour continuer votre aventure...");
    while(getchar() != '\n');

    // Lancer la navigation sur la carte
    naviguer_carte(carte, &player);

    // Libérer la mémoire de la carte
    free_carte(carte);

    printf("\n==================================================\n");
    printf("      Fin de votre expedition, %s!\n", player.name);
    printf("==================================================\n");
    printf("Stats finales:\n");
    display_player_stats(player);
    printf("\nAppuyez sur Entree pour revenir au menu principal...");
    while(getchar() != '\n');
}