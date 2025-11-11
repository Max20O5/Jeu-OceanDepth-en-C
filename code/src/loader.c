#include "loader.h"
#include "tab.h"
#include "include.h"

Arme* g_weapon_database = NULL;
int g_weapon_count = 0;

CreatureMarine* g_creature_database = NULL;
int g_creature_count = 0;

Consommable* g_consommable_database = NULL;
int g_consommable_count = 0;

bool parse_line(char* line) {
    if (line[0] == '\n' || line[0] == '#') {
        return false;
    }
    line[strcspn(line, "\n")] = 0;
    return true;
}

bool load_game_data(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Erreur : Impossible d'ouvrir le config.txt");
        return false;
    }

    char buffer[256];
    int weapon_idx = 0;
    int creature_idx = 0;
    int consommable_idx = 0;

    // Lire les comptes d'armes et de créatures et allouer la mémoire
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        if (!parse_line(buffer)) continue;

        if (sscanf(buffer, "WEAPON_COUNT: %d", &g_weapon_count) == 1) {
            g_weapon_database = malloc(sizeof(Arme) * g_weapon_count);
        } else if (sscanf(buffer, "CREATURE_COUNT: %d", &g_creature_count) == 1) {
            g_creature_database = malloc(sizeof(CreatureMarine) * g_creature_count);
        } else if (sscanf(buffer, "CONSUMABLE_COUNT: %d", &g_consommable_count) == 1) {
            g_consommable_database = malloc(sizeof(Consommable) * g_consommable_count);
        }
    }

    if (g_weapon_database == NULL || g_creature_database == NULL || g_consommable_database == NULL) {
        fprintf(stderr, "Erreur : Problème de mémoire ou comptes non trouvés dans config.txt\n");
        fclose(file);
        return false;
    }

    // Lire les données
    rewind(file);

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        if (!parse_line(buffer)) continue;

        if (strncmp(buffer, "WEAPON;", 7) == 0) {
            Arme* w = &g_weapon_database[weapon_idx];
            sscanf(buffer, "WEAPON;%d;%[^;];%d;%d;%d;%s",
                   &w->id, w->nom, &w->attaque_minimale, &w->attaque_maximale,
                   &w->consommation_oxygene, w->effet_special);
            weapon_idx++;
        
        } else if (strncmp(buffer, "CREATURE;", 9) == 0) {
            CreatureMarine* c = &g_creature_database[creature_idx];
            
            sscanf(buffer, "CREATURE;%d;%[^;];%d;%d;%d;%d;%d;%d;%d;%s",
                   &c->id, c->nom, &c->points_de_vie_max, &c->points_de_vie_max,
                   &c->attaque_minimale, &c->attaque_maximale, &c->defense,
                   &c->perles_min, &c->perles_max, c->effet_special);
            
            c->points_de_vie_actuels = c->points_de_vie_max;
            c->est_vivant = true;
            c->active_effect_count = 0;
            creature_idx++;
        } else if (strncmp(buffer, "CONSUMABLE;", 11) == 0) {
            Consommable* c = &g_consommable_database[consommable_idx];
            // Format: CONSUMABLE;id;nom;effet;soin;oxygene;fatigue
            sscanf(buffer, "CONSUMABLE;%d;%[^;];%[^;];%d;%d;%d",
                   &c->id, c->nom, c->effet_special,
                   &c->soin, &c->oxygene, &c->fatigue);
            consommable_idx++;
        }
    }

    fclose(file);
    printf("Données du jeu chargées : %d armes, %d créatures, %d consommables.\n", g_weapon_count, g_creature_count, g_consommable_count);
    return true;
}

void free_game_data(void) {
    free(g_weapon_database);
    free(g_creature_database);
    free(g_consommable_database);
}