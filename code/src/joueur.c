#include "joueur.h"
#include "weapon.h"
#include "consommable.h"
#include "equipement.h"
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

    // Système de progression
    new_player.niveau = 1;
    new_player.experience = 0;
    new_player.experience_prochain_niveau = 100;
    new_player.creatures_tuees = 0;
    new_player.boss_tues = 0;

    new_player.arme_equipee = creer_harpon_rouille();
    new_player.combinaison_equipee = creer_neoprene_basic();

    for (int i = 0; i < 8; i++) {
        new_player.inventaire[i] = creer_slot_vide();
    }

    new_player.inventaire[0] = creer_trousse_de_soin();
    new_player.inventaire[1] = creer_trousse_de_soin();
    new_player.inventaire[2] = creer_capsule_oxygene();

    printf("\nLe plongeur '%s' a été crée!\n", new_player.name);
    printf("Arme équipée: %s\n", new_player.arme_equipee.nom);
    printf("Combinaison équipée: %s (Defense: +%d)\n",
           new_player.combinaison_equipee.nom,
           get_defense_equipement(&new_player.combinaison_equipee));
    return new_player;

}

void display_player_stats(Plongeur player) {
    printf("\n=== STATS DE %s ===\n", player.name);
    printf("Niveau     : %d (XP: %d/%d)\n", player.niveau, player.experience, player.experience_prochain_niveau);
    printf("Vie        : %d / %d\n", player.points_de_vie, player.points_de_vie_max);
    printf("Oxygene    : %d / %d\n", player.niveau_oxygene, player.niveau_oxygene_max);
    printf("Fatigue    : %d / 5\n", player.niveau_fatigue);
    printf("Perles     : %d\n", player.perles);
    printf("--------------------------\n");
    printf("Creatures tuees: %d\n", player.creatures_tuees);
    printf("Boss tues      : %d\n", player.boss_tues);
    printf("--------------------------\n");
    printf("Arme équipée: %s (Dégats: %d-%d)\n",
         player.arme_equipee.nom
         , player.arme_equipee.attaque_minimale
         , player.arme_equipee.attaque_maximale);
    printf("Combinaison équipée: %s (Defense: +%d)\n",
         player.combinaison_equipee.nom,
         get_defense_equipement(&player.combinaison_equipee));
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

// Système d'XP et de progression
void gagner_experience(Plongeur* player, int xp) {
    printf("\n>>> Vous gagnez %d XP!\n", xp);
    player->experience += xp;

    // Vérifier si le joueur monte de niveau
    while (player->experience >= player->experience_prochain_niveau) {
        monter_niveau(player);
    }
}

void monter_niveau(Plongeur* player) {
    player->niveau++;
    player->experience -= player->experience_prochain_niveau;
    player->experience_prochain_niveau = (int)(player->experience_prochain_niveau * 1.5f);

    printf("\n");
    printf("╔═══════════════════════════════════════════════════════════╗\n");
    printf("║            ✨ NIVEAU SUPERIEUR ATTEINT! ✨              ║\n");
    printf("╚═══════════════════════════════════════════════════════════╝\n");
    printf("    Vous etes maintenant niveau %d!\n\n", player->niveau);

    // Bonus de niveau
    int bonus_vie = 10;
    int bonus_oxygene = 10;

    player->points_de_vie_max += bonus_vie;
    player->points_de_vie += bonus_vie; // Restaure aussi les PV
    player->niveau_oxygene_max += bonus_oxygene;
    player->niveau_oxygene += bonus_oxygene; // Restaure aussi l'O2

    printf("    [+] Vie max     : +%d (Total: %d)\n", bonus_vie, player->points_de_vie_max);
    printf("    [+] Oxygene max : +%d (Total: %d)\n", bonus_oxygene, player->niveau_oxygene_max);
    printf("    [+] Vie et Oxygene restaures!\n");
    printf("\n    Prochain niveau : %d XP\n", player->experience_prochain_niveau);
    printf("\n");
}
