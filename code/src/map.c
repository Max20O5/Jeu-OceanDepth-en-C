#include "map.h"
#include "config.h"
#include "utils.h"
#include "combat.h"
#include "marchand.h"
#include "joueur.h"
#include "creature.h"
#include "save.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Fonction utilitaire pour lire une ligne (définie dans joueur.c)
extern void read_line(char* buffer, int size);

// ===== FONCTIONS UTILITAIRES =====

// Obtenir l'icône d'une tuile
const char* get_tile_icon(TileType type) {
    switch (type) {
        case TILE_EMPTY:    return "  ";     // Vide
        case TILE_COMBAT:   return "⚔️ ";     // Combat
        case TILE_TREASURE: return "💰";     // Trésor
        case TILE_MERCHANT: return "🎒";     // Marchand
        case TILE_BOSS:     return "💀";     // Boss
        case TILE_SAVE:     return "🕳️ ";     // Grotte (sauvegarde)
        case TILE_PLAYER:   return "🌊";     // Joueur (plongeur)
        default:            return "??";
    }
}

// Libérer une tuile
void free_tile(Tile* tile) {
    if (tile->enemy_ids) {
        free(tile->enemy_ids);
        tile->enemy_ids = NULL;
    }
}

// Libérer une zone
void free_zone(Zone* zone) {
    if (!zone) return;

    // Libérer toutes les tuiles
    for (int y = 0; y < zone->height; y++) {
        for (int x = 0; x < zone->width; x++) {
            free_tile(&zone->tiles[y][x]);
        }
        free(zone->tiles[y]);
    }
    free(zone->tiles);

    // Libérer récursivement les zones suivantes
    if (zone->next_zone) {
        free_zone(zone->next_zone);
    }

    free(zone);
}

// Libérer la carte
void free_carte(Carte* carte) {
    if (!carte) return;
    if (carte->current_zone) {
        // Trouver la première zone pour libérer toute la chaîne
        Zone* first = carte->current_zone;
        while (first->zone_number > 1) {
            // On ne peut pas remonter, donc on libère juste depuis la zone actuelle
            break;
        }
        free_zone(first);
    }
    free(carte);
}

// ===== GÉNÉRATION DE MAP =====

// Générer une tuile selon les probabilités de la config
void generer_tuile(Tile* tile, int x __attribute__((unused)), int y, int zone_number, MapConfig* config) {
    tile->visited = false;
    tile->cleared = false;
    tile->nb_ennemis = 0;
    tile->enemy_ids = NULL;
    tile->depth = config->zone_base_depth +
                  (zone_number - 1) * config->zone_depth_increment +
                  y * randomNumber(config->depth_increment_min, config->depth_increment_max);

    // Générer le type de tuile selon les probabilités
    int rand = randomNumber(1, 100);
    int cumulative = 0;

    cumulative += config->combat_tile_probability;
    if (rand <= cumulative) {
        tile->type = TILE_COMBAT;
        // Nombre d'ennemis augmente avec la zone
        int bonus_enemies = (int)((zone_number - 1) * config->zone_enemy_bonus);
        tile->nb_ennemis = randomNumber(
            config->enemy_count_min + bonus_enemies,
            config->enemy_count_max + bonus_enemies
        );

        // Allouer et générer les IDs des ennemis
        tile->enemy_ids = malloc(sizeof(int) * tile->nb_ennemis);
        for (int i = 0; i < tile->nb_ennemis; i++) {
            // IDs d'ennemis (1-5 pour les créatures de base, à adapter selon votre système)
            tile->enemy_ids[i] = randomNumber(1, 5);
        }
        return;
    }

    cumulative += config->treasure_tile_probability;
    if (rand <= cumulative) {
        tile->type = TILE_TREASURE;
        return;
    }

    cumulative += config->merchant_tile_probability;
    if (rand <= cumulative) {
        tile->type = TILE_MERCHANT;
        return;
    }

    cumulative += config->boss_tile_probability;
    if (rand <= cumulative) {
        tile->type = TILE_BOSS;
        tile->nb_ennemis = 1;
        tile->enemy_ids = malloc(sizeof(int));
        // Boss ID (6 pour boss normal, à adapter)
        tile->enemy_ids[0] = 6;
        return;
    }

    cumulative += config->save_tile_probability;
    if (rand <= cumulative) {
        tile->type = TILE_SAVE;
        return;
    }

    // Sinon, tuile vide
    tile->type = TILE_EMPTY;
}

// Générer une zone complète
Zone* generer_zone(int zone_number, MapConfig* config) {
    Zone* zone = malloc(sizeof(Zone));
    if (!zone) {
        printf("ERREUR: Impossible d'allouer la mémoire pour la zone!\n");
        return NULL;
    }

    zone->zone_number = zone_number;
    zone->width = config->map_width;
    zone->height = config->map_height;
    zone->player_x = config->map_width / 2;  // Commencer au milieu
    zone->player_y = 0;                       // Commencer en haut
    zone->base_depth = config->zone_base_depth + (zone_number - 1) * config->zone_depth_increment;
    zone->boss_defeated = false;
    zone->next_zone = NULL;

    // Nom de la zone
    if (zone_number <= config->zone_count) {
        strcpy(zone->zone_name, config->zone_names[zone_number - 1]);
    } else {
        sprintf(zone->zone_name, "Zone %d: Profondeurs inconnues", zone_number);
    }

    // Allouer la matrice de tuiles
    zone->tiles = malloc(sizeof(Tile*) * zone->height);
    for (int y = 0; y < zone->height; y++) {
        zone->tiles[y] = malloc(sizeof(Tile) * zone->width);
        for (int x = 0; x < zone->width; x++) {
            generer_tuile(&zone->tiles[y][x], x, y, zone_number, config);
        }
    }

    printf("\n╔═══════════════════════════════════════════════════════════╗\n");
    printf("║        🌊 NOUVELLE ZONE GÉNÉRÉE 🌊                      ║\n");
    printf("╚═══════════════════════════════════════════════════════════╝\n");
    printf("  %s\n", zone->zone_name);
    printf("  Profondeur de base: %dm\n", zone->base_depth);
    printf("  Dimensions: %dx%d tuiles\n\n", zone->width, zone->height);

    return zone;
}

// ===== INITIALISATION =====

Carte* init_carte() {
    Carte* carte = malloc(sizeof(Carte));
    if (!carte) {
        printf("ERREUR: Impossible d'allouer la mémoire pour la carte!\n");
        return NULL;
    }

    // Charger la configuration
    load_map_config(&carte->config);

    // Générer la première zone
    carte->current_zone = generer_zone(1, &carte->config);
    carte->total_zones_generated = 1;

    if (!carte->current_zone) {
        free(carte);
        return NULL;
    }

    return carte;
}

// ===== AFFICHAGE =====

void afficher_carte(Carte* carte) {
    if (!carte || !carte->current_zone) return;

    Zone* zone = carte->current_zone;

    printf("\n");
    printf("╔════════════════════════════════════════════════════════════════╗\n");
    printf("║  %s", zone->zone_name);
    for (int i = strlen(zone->zone_name); i < 60; i++) printf(" ");
    printf("║\n");
    printf("╠════════════════════════════════════════════════════════════════╣\n");

    // Afficher la grille
    for (int y = 0; y < zone->height; y++) {
        printf("║ ");
        for (int x = 0; x < zone->width; x++) {
            if (x == zone->player_x && y == zone->player_y) {
                printf("🌊");  // Position du joueur
            } else {
                Tile* tile = &zone->tiles[y][x];
                if (tile->visited) {
                    if (tile->cleared) {
                        printf("✓ ");  // Tuile complétée
                    } else {
                        printf("%s", get_tile_icon(tile->type));
                    }
                } else {
                    printf("❓");  // Non exploré
                }
            }
            printf(" ");
        }
        printf("║\n");
    }

    printf("╠════════════════════════════════════════════════════════════════╣\n");
    printf("║ Profondeur actuelle: %dm                                    ",
           zone->tiles[zone->player_y][zone->player_x].depth);
    printf("║\n");
    printf("║ Position: [%d, %d]                                             ",
           zone->player_x, zone->player_y);
    printf("║\n");
    printf("╠════════════════════════════════════════════════════════════════╣\n");
    printf("║ Légende: 🌊=Vous | ⚔️=Combat | 💰=Trésor | 🎒=Marchand          ║\n");
    printf("║          💀=Boss | 🕳️=Grotte (Sauvegarde)                     ║\n");
    printf("║          ❓=Inexploré | ✓=Complété                            ║\n");
    printf("╚════════════════════════════════════════════════════════════════╝\n\n");
}

// ===== NAVIGATION =====

int get_profondeur_actuelle(Carte* carte) {
    if (!carte || !carte->current_zone) return 0;
    return carte->current_zone->tiles[carte->current_zone->player_y][carte->current_zone->player_x].depth;
}

bool deplacer_joueur(Carte* carte, Plongeur* joueur, int dx, int dy) {
    if (!carte || !carte->current_zone) return false;

    Zone* zone = carte->current_zone;
    int new_x = zone->player_x + dx;
    int new_y = zone->player_y + dy;

    // Vérifier les limites
    if (new_x < 0 || new_x >= zone->width || new_y < 0 || new_y >= zone->height) {
        printf("❌ Vous ne pouvez pas aller plus loin dans cette direction!\n");
        return false;
    }

    // Événements aléatoires pendant le déplacement
    int event_chance = randomNumber(1, 100);

    if (event_chance <= 15) {
        printf("\n🌀 Un courant violent vous secoue!\n");
        joueur->niveau_oxygene -= 5;
        if (joueur->niveau_oxygene < 0) joueur->niveau_oxygene = 0;
        printf("   Vous perdez 5 points d'oxygène. (O2: %d/%d)\n",
               joueur->niveau_oxygene, joueur->niveau_oxygene_max);
    } else if (event_chance <= 25) {
        printf("\n💨 Un courant favorable vous porte!\n");
        printf("   Le déplacement ne vous coûte pas d'oxygène.\n");
    } else {
        // Déplacement normal, consomme de l'oxygène
        int o2_cost = 2 - joueur->combinaison_equipee.consommation_oxygene_reduction;
        if (o2_cost < 0) o2_cost = 0;
        joueur->niveau_oxygene -= o2_cost;
        if (joueur->niveau_oxygene < 0) joueur->niveau_oxygene = 0;
    }

    // Déplacer le joueur
    zone->player_x = new_x;
    zone->player_y = new_y;

    // Marquer la tuile comme visitée
    zone->tiles[new_y][new_x].visited = true;

    printf("\n✅ Vous vous déplacez vers la nouvelle position [%d, %d]\n", new_x, new_y);
    printf("   Profondeur: %dm\n", zone->tiles[new_y][new_x].depth);

    // Vérifier si le joueur est mort par manque d'oxygène
    if (joueur->niveau_oxygene <= 0) {
        printf("\n💀 Vous manquez d'oxygène et perdez connaissance...\n");
        return false;
    }

    return true;
}

void passer_zone_suivante(Carte* carte) {
    if (!carte || !carte->current_zone) return;

    Zone* current = carte->current_zone;

    if (current->next_zone) {
        // La zone suivante existe déjà
        carte->current_zone = current->next_zone;
    } else {
        // Générer une nouvelle zone
        Zone* next = generer_zone(current->zone_number + 1, &carte->config);
        current->next_zone = next;
        carte->current_zone = next;
        carte->total_zones_generated++;
    }

    printf("\n");
    printf("╔═══════════════════════════════════════════════════════════╗\n");
    printf("║     🎉 VOUS PROGRESSEZ VERS UNE NOUVELLE ZONE! 🎉       ║\n");
    printf("╚═══════════════════════════════════════════════════════════╝\n\n");
}

// ===== EXPLORATION =====

void explorer_tuile(Carte* carte, Plongeur* joueur) {
    if (!carte || !carte->current_zone || !joueur) return;

    Zone* zone = carte->current_zone;
    Tile* tile = &zone->tiles[zone->player_y][zone->player_x];

    // Si déjà complétée
    if (tile->cleared) {
        printf("\n✓ Cette zone a déjà été explorée et nettoyée.\n");
        // Petite récupération d'oxygène
        int o2_gain = 5;
        joueur->niveau_oxygene += o2_gain;
        if (joueur->niveau_oxygene > joueur->niveau_oxygene_max) {
            joueur->niveau_oxygene = joueur->niveau_oxygene_max;
        }
        printf("  Vous récupérez %d points d'oxygène en vous reposant. (O2: %d/%d)\n",
               o2_gain, joueur->niveau_oxygene, joueur->niveau_oxygene_max);
        return;
    }

    tile->visited = true;

    switch (tile->type) {
        case TILE_EMPTY:
            printf("\n🌊 Zone vide - Vous explorez les environs...\n");

            // Événements aléatoires en exploration
            int event = randomNumber(1, 100);
            if (event <= 20) {
                printf("   💎 Vous trouvez des cristaux d'oxygène!\n");
                int o2_gain = 20;
                joueur->niveau_oxygene += o2_gain;
                if (joueur->niveau_oxygene > joueur->niveau_oxygene_max) {
                    joueur->niveau_oxygene = joueur->niveau_oxygene_max;
                }
                printf("   +%d O2 (Total: %d/%d)\n", o2_gain, joueur->niveau_oxygene, joueur->niveau_oxygene_max);
            } else if (event <= 30) {
                printf("   🐟 Des poissons lumineux vous entourent!\n");
                int perles = randomNumber(5, 15);
                joueur->perles += perles;
                printf("   +%d perles (Total: %d)\n", perles, joueur->perles);
            } else {
                printf("   Rien d'intéressant ici...\n");
                int o2_gain = 15;
                joueur->niveau_oxygene += o2_gain;
                if (joueur->niveau_oxygene > joueur->niveau_oxygene_max) {
                    joueur->niveau_oxygene = joueur->niveau_oxygene_max;
                }
                printf("   Vous récupérez %d points d'oxygène. (O2: %d/%d)\n",
                       o2_gain, joueur->niveau_oxygene, joueur->niveau_oxygene_max);
            }

            // Proposer de sauvegarder (zone sûre)
            printf("\n   💾 Voulez-vous sauvegarder votre partie? (o/n): ");
            char choix_save_empty[10];
            read_line(choix_save_empty, sizeof(choix_save_empty));
            if (choix_save_empty[0] == 'o' || choix_save_empty[0] == 'O') {
                sauvegarder_partie(joueur, carte);
            }

            tile->cleared = true;
            break;

        case TILE_COMBAT:
            printf("\n⚔️  COMBAT - %d ennemis détectés!\n", tile->nb_ennemis);

            // Créer les créatures pour le combat
            CreatureMarine* ennemis = malloc(sizeof(CreatureMarine) * tile->nb_ennemis);
            for (int i = 0; i < tile->nb_ennemis; i++) {
                ennemis[i] = creerCreature(tile->enemy_ids[i], get_profondeur_actuelle(carte));
                ennemis[i].zone = zone->zone_number;  // Assigner la zone
            }

            // Lancer le combat
            bool victoire = combat(joueur, ennemis, tile->nb_ennemis, get_profondeur_actuelle(carte));

            free(ennemis);

            if (victoire) {
                tile->cleared = true;
                printf("\n✅ Zone de combat nettoyée!\n");
            } else {
                printf("\n💀 Vous avez été vaincu...\n");
            }
            break;

        case TILE_TREASURE:
            printf("\n💰 TRÉSOR - Vous trouvez un coffre!\n");

            int tresor_type = randomNumber(1, 100);
            if (tresor_type <= 40) {
                // Oxygène
                int o2 = randomNumber(20, 50);
                joueur->niveau_oxygene += o2;
                if (joueur->niveau_oxygene > joueur->niveau_oxygene_max) {
                    joueur->niveau_oxygene = joueur->niveau_oxygene_max;
                }
                printf("   💎 Capsule d'oxygène! +%d O2\n", o2);
            } else if (tresor_type <= 70) {
                // Perles
                int perles = randomNumber(20, 60);
                joueur->perles += perles;
                printf("   🪙 Perles! +%d perles\n", perles);
            } else if (tresor_type <= 90) {
                // Item
                printf("   🎁 Objet trouvé! (Système d'items à implémenter)\n");
            } else {
                printf("   📦 Le coffre est vide...\n");
            }

            // Proposer de sauvegarder (zone sûre)
            printf("\n   💾 Voulez-vous sauvegarder votre partie? (o/n): ");
            char choix_save_treasure[10];
            read_line(choix_save_treasure, sizeof(choix_save_treasure));
            if (choix_save_treasure[0] == 'o' || choix_save_treasure[0] == 'O') {
                sauvegarder_partie(joueur, carte);
            }

            tile->cleared = true;
            break;

        case TILE_MERCHANT:
            printf("\n🎒 MARCHAND - Un marchand ambulant vous propose ses services!\n");
            open_shop(joueur);

            // Proposer de sauvegarder (zone sûre)
            printf("\n   💾 Voulez-vous sauvegarder votre partie? (o/n): ");
            char choix_save_merchant[10];
            read_line(choix_save_merchant, sizeof(choix_save_merchant));
            if (choix_save_merchant[0] == 'o' || choix_save_merchant[0] == 'O') {
                sauvegarder_partie(joueur, carte);
            }

            tile->cleared = true;
            break;

        case TILE_BOSS:
            printf("\n💀 BOSS - Un gardien de zone vous barre la route!\n");
            printf("   Vous devez le vaincre pour progresser!\n\n");

            // Créer le boss
            CreatureMarine boss = creerCreature(6, get_profondeur_actuelle(carte));  // ID 6 = boss
            boss.zone = zone->zone_number;

            // Appliquer le multiplicateur de difficulté de zone
            float multiplier = 1.0f;
            for (int i = 1; i < zone->zone_number; i++) {
                multiplier *= carte->config.zone_difficulty_multiplier;
            }
            boss.points_de_vie_max = (int)(boss.points_de_vie_max * multiplier);
            boss.points_de_vie_actuels = boss.points_de_vie_max;
            boss.attaque_minimale = (int)(boss.attaque_minimale * multiplier);
            boss.attaque_maximale = (int)(boss.attaque_maximale * multiplier);

            bool victoire_boss = combat(joueur, &boss, 1, get_profondeur_actuelle(carte));

            if (victoire_boss) {
                printf("\n");
                printf("╔═══════════════════════════════════════════════════════════╗\n");
                printf("║         🏆 BOSS VAINCU! 🏆                              ║\n");
                printf("╚═══════════════════════════════════════════════════════════╝\n");

                // Récompenses de boss
                int bonus_xp = 200 + (zone->zone_number * 50);
                int bonus_perles = 100 + (zone->zone_number * 25);

                gagner_experience(joueur, bonus_xp);
                joueur->perles += bonus_perles;
                joueur->boss_tues++;

                printf("  Récompenses:\n");
                printf("    • +%d XP\n", bonus_xp);
                printf("    • +%d perles\n", bonus_perles);
                printf("\n  La voie vers la zone suivante est ouverte!\n\n");

                tile->cleared = true;
                zone->boss_defeated = true;

                // Proposer de passer à la zone suivante
                printf("Voulez-vous progresser vers la zone suivante? (o/n): ");
                char choix[10];
                read_line(choix, sizeof(choix));

                if (choix[0] == 'o' || choix[0] == 'O') {
                    passer_zone_suivante(carte);
                }
            }
            break;

        case TILE_SAVE:
            printf("\n🕳️  GROTTE SÉCURISÉE - Un endroit sûr pour se reposer et sauvegarder!\n");
            printf("   Cette grotte est protégée des créatures marines.\n\n");

            // Récupération d'oxygène importante
            int o2_recovery = 50;
            joueur->niveau_oxygene += o2_recovery;
            if (joueur->niveau_oxygene > joueur->niveau_oxygene_max) {
                joueur->niveau_oxygene = joueur->niveau_oxygene_max;
            }
            printf("   💎 Vous récupérez %d points d'oxygène! (O2: %d/%d)\n",
                   o2_recovery, joueur->niveau_oxygene, joueur->niveau_oxygene_max);

            // Récupération de fatigue
            if (joueur->niveau_fatigue > 0) {
                joueur->niveau_fatigue = 0;
                printf("   ✨ Votre fatigue disparaît complètement!\n");
            }

            // Proposer de sauvegarder
            printf("\n   Voulez-vous sauvegarder votre partie? (o/n): ");
            char choix_save[10];
            read_line(choix_save, sizeof(choix_save));

            if (choix_save[0] == 'o' || choix_save[0] == 'O') {
                sauvegarder_partie(joueur, carte);
            } else {
                printf("   Partie non sauvegardée.\n");
            }

            tile->cleared = true;
            break;

        default:
            printf("\n❓ Zone inconnue...\n");
            break;
    }
}

// Menu de navigation
void naviguer_carte(Carte* carte, Plongeur* joueur) {
    if (!carte || !joueur) return;

    while (true) {
        afficher_carte(carte);

        printf("\n=== NAVIGATION ===\n");
        printf("  [z] Haut    [s] Bas\n");
        printf("  [q] Gauche  [d] Droite\n");
        printf("  [e] Explorer la zone actuelle\n");
        printf("  [r] Retour au menu principal\n");
        printf("\nVotre choix: ");

        char choix[10];
        read_line(choix, sizeof(choix));

        if (choix[0] == 'z' || choix[0] == 'Z') {
            if (!deplacer_joueur(carte, joueur, 0, -1)) {
                if (joueur->niveau_oxygene <= 0 || joueur->points_de_vie <= 0) {
                    break;  // Sortir si le joueur est mort
                }
            }
        } else if (choix[0] == 's' || choix[0] == 'S') {
            if (!deplacer_joueur(carte, joueur, 0, 1)) {
                if (joueur->niveau_oxygene <= 0 || joueur->points_de_vie <= 0) {
                    break;
                }
            }
        } else if (choix[0] == 'q' || choix[0] == 'Q') {
            if (!deplacer_joueur(carte, joueur, -1, 0)) {
                if (joueur->niveau_oxygene <= 0 || joueur->points_de_vie <= 0) {
                    break;
                }
            }
        } else if (choix[0] == 'd' || choix[0] == 'D') {
            if (!deplacer_joueur(carte, joueur, 1, 0)) {
                if (joueur->niveau_oxygene <= 0 || joueur->points_de_vie <= 0) {
                    break;
                }
            }
        } else if (choix[0] == 'e' || choix[0] == 'E') {
            explorer_tuile(carte, joueur);

            // Vérifier si le joueur est mort
            if (joueur->points_de_vie <= 0 || joueur->niveau_oxygene <= 0) {
                break;
            }
        } else if (choix[0] == 'r' || choix[0] == 'R') {
            break;
        } else {
            printf("❌ Choix invalide!\n");
        }

        // Pause pour que le joueur puisse lire les messages
        printf("\n[Appuyez sur Entrée pour continuer]");
        getchar();
    }
}
