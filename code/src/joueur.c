#include "joueur.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool is_name_valid(const char* name) {
    int len = strlen(name);
    if (len < 3 || len > 50) {
        printf("Erreur: Le nom doit être en 3 et 50 caractères.\n");
        return false;
    }

    for (int i = 0; i < len; i++) {
        char letter = name[i];
        if (!((letter >= 'a' && letter <= 'z') || (letter >= 'A' && letter <= 'Z'))) {
            printf("Erreur: Le nom doit contenir seulement des lettres.\n");
            return false;
        }
    }
    return true;
}

void read_line(char* buffer, int size) {
    if (fgets(buffer, size, stdin)) {
        buffer[strcspn(buffer, "\n")] = 0;
    }
}

Plongeur create_player(void) {
    Plongeur new_player;
    char name_buffer[50];

    printf("\n--- Créer votre plongeur ---\n");

    do {
        printf("Entrez le nom de votre plongeur: ");
        read_line(name_buffer, sizeof(name_buffer));
    } while (!is_name_valid(name_buffer));

    strcpy(new_player.name, name_buffer);

    new_player.points_de_vie_max = 100;
    new_player.points_de_vie = 100;
    new_player.niveau_oxygene_max = 100;
    new_player.niveau_oxygene = 100;
    new_player.niveau_fatigue = 0;
    new_player.perles = 0;

    printf("\nLe plongeur '%s' a été crée!\n", new_player.name);
    return new_player;

}

void display_player_stats(Plongeur player) {
    printf("\n--- STATS OF %s ---\n", player.name);
    printf("Vie    : %d / %d\n", player.points_de_vie, player.points_de_vie_max);
    printf("Oxygène    : %d / %d\n", player.niveau_oxygene, player.niveau_oxygene_max);
    printf("Fatigue   : %d / 5\n", player.niveau_fatigue);
    printf("Perles     : %d\n", player.perles);
    printf("--------------------------\n");
}
