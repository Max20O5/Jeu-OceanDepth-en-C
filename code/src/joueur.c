#include "joueur.h"
#include "weapon.h"
#include "consommable.h"
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
        bool is_letter = (letter >= 'a' && letter <= 'z') || (letter >= 'A' && letter <= 'Z');
        bool is_space = (letter == ' ');

        if (!is_letter && !is_space) {
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
    new_player.active_effect_count = 0;
    new_player.arme_equipee = get_weapon_by_id(3);

    for (int i = 0; i < 8; i++) {
        new_player.inventaire[i] = get_consommable_by_id(0); 
    }
    new_player.inventaire[0] = get_consommable_by_id(1);
    new_player.inventaire[1] = get_consommable_by_id(1);
    new_player.inventaire[2] = get_consommable_by_id(2);

    printf("\nLe plongeur '%s' a été crée!\n", new_player.name);
    printf("Arme équipée: %s\n", new_player.arme_equipee.nom);
    return new_player;

}

void display_player_stats(Plongeur player) {
    printf("\n--- STATS DE %s ---\n", player.name);
    printf("Vie    : %d / %d\n", player.points_de_vie, player.points_de_vie_max);
    printf("Oxygène    : %d / %d\n", player.niveau_oxygene, player.niveau_oxygene_max);
    printf("Fatigue   : %d / 5\n", player.niveau_fatigue);
    printf("Perles     : %d\n", player.perles);
    printf("--------------------------\n");
    printf("Arme équipée: %s (Dégats: %d-%d)\n",
         player.arme_equipee.nom
         , player.arme_equipee.attaque_minimale
         , player.arme_equipee.attaque_maximale);
    printf("--------------------------\n");        
    
    printf("INVENTAIRE (8 slots max):\n");
    for (int i = 0; i < 8; i++) {
        // Affiche l'objet seulement s'il n'est pas "Vide" (ID 0)
        // if (player.inventaire[i].id != 0) { 
        // }
        
        // Pour l'instant, on affiche tout pour voir les slots vides
        printf("  [%d] - %s\n", i + 1, player.inventaire[i].nom);
    }
    printf("--------------------------\n");
}
