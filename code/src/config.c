#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Fonction utilitaire pour enlever les espaces et retours à la ligne
void trim(char* str) {
    char* end;
    // Trim leading spaces
    while (*str == ' ' || *str == '\t') str++;

    if (*str == 0) return;

    // Trim trailing spaces
    end = str + strlen(str) - 1;
    while (end > str && (*end == ' ' || *end == '\t' || *end == '\n' || *end == '\r')) {
        end--;
    }
    end[1] = '\0';
}

// Parse une ligne du fichier de configuration
void parse_config_line(char* line, MapConfig* config) {
    // Ignorer les commentaires et lignes vides
    if (line[0] == '#' || line[0] == '\n' || line[0] == '\0') {
        return;
    }

    char key[100];
    char value[256];

    // Séparer clé et valeur
    char* equals = strchr(line, '=');
    if (!equals) return;

    strncpy(key, line, equals - line);
    key[equals - line] = '\0';
    trim(key);

    strcpy(value, equals + 1);
    trim(value);

    // Parser les valeurs selon la clé
    if (strcmp(key, "MAP_WIDTH") == 0) {
        config->map_width = atoi(value);
    } else if (strcmp(key, "MAP_HEIGHT") == 0) {
        config->map_height = atoi(value);
    } else if (strcmp(key, "COMBAT_TILE_PROBABILITY") == 0) {
        config->combat_tile_probability = atoi(value);
    } else if (strcmp(key, "TREASURE_TILE_PROBABILITY") == 0) {
        config->treasure_tile_probability = atoi(value);
    } else if (strcmp(key, "MERCHANT_TILE_PROBABILITY") == 0) {
        config->merchant_tile_probability = atoi(value);
    } else if (strcmp(key, "BOSS_TILE_PROBABILITY") == 0) {
        config->boss_tile_probability = atoi(value);
    } else if (strcmp(key, "SAVE_TILE_PROBABILITY") == 0) {
        config->save_tile_probability = atoi(value);
    } else if (strcmp(key, "ENEMY_COUNT_MIN") == 0) {
        config->enemy_count_min = atoi(value);
    } else if (strcmp(key, "ENEMY_COUNT_MAX") == 0) {
        config->enemy_count_max = atoi(value);
    } else if (strcmp(key, "DEPTH_INCREMENT_MIN") == 0) {
        config->depth_increment_min = atoi(value);
    } else if (strcmp(key, "DEPTH_INCREMENT_MAX") == 0) {
        config->depth_increment_max = atoi(value);
    } else if (strcmp(key, "ZONE_DIFFICULTY_MULTIPLIER") == 0) {
        config->zone_difficulty_multiplier = atof(value);
    } else if (strcmp(key, "ZONE_ENEMY_BONUS") == 0) {
        config->zone_enemy_bonus = atof(value);
    } else if (strcmp(key, "ZONE_RARE_LOOT_BASE") == 0) {
        config->zone_rare_loot_base = atoi(value);
    } else if (strcmp(key, "ZONE_RARE_LOOT_INCREASE") == 0) {
        config->zone_rare_loot_increase = atoi(value);
    } else if (strcmp(key, "ZONE_BASE_DEPTH") == 0) {
        config->zone_base_depth = atoi(value);
    } else if (strcmp(key, "ZONE_DEPTH_INCREMENT") == 0) {
        config->zone_depth_increment = atoi(value);
    } else if (strcmp(key, "ZONE_NAMES") == 0) {
        // Parser les noms de zones séparés par des points-virgules
        config->zone_count = 0;
        char* token = strtok(value, ";");
        while (token != NULL && config->zone_count < 10) {
            trim(token);
            strcpy(config->zone_names[config->zone_count], token);
            config->zone_count++;
            token = strtok(NULL, ";");
        }
    }
}

// Charge la configuration depuis le fichier
bool load_map_config(MapConfig* config) {
    // Toujours initialiser avec des valeurs par défaut
    config->map_width = 10;
    config->map_height = 8;
    config->combat_tile_probability = 35;
    config->treasure_tile_probability = 25;
    config->merchant_tile_probability = 5;
    config->boss_tile_probability = 8;
    config->save_tile_probability = 10; // 10% de chance de point de sauvegarde
    config->enemy_count_min = 1;
    config->enemy_count_max = 3;
    config->depth_increment_min = 0;
    config->depth_increment_max = 15;
    config->zone_difficulty_multiplier = 1.2f;
    config->zone_enemy_bonus = 0.5f;
    config->zone_rare_loot_base = 5;
    config->zone_rare_loot_increase = 3;
    config->zone_base_depth = 0;
    config->zone_depth_increment = 100;
    
    // Noms de zones par défaut
    config->zone_count = 6;
    strcpy(config->zone_names[0], "Zone 1: Plage de sable");
    strcpy(config->zone_names[1], "Zone 2: Récif corallien");
    strcpy(config->zone_names[2], "Zone 3: Grotte sous-marine");
    strcpy(config->zone_names[3], "Zone 4: Canyon abyssal");
    strcpy(config->zone_names[4], "Zone 5: Fosse océanique");
    strcpy(config->zone_names[5], "Zone 6: Abîme sans fin");

    FILE* file = fopen("config/config.cfg", "r");
    if (!file) {
        printf("ERREUR: Impossible d'ouvrir le fichier config/config.cfg!\n");
        printf("Utilisation des valeurs par défaut...\n");
        return false;
    }

    // Lire le fichier et écraser les valeurs par défaut
    char line[512];
    while (fgets(line, sizeof(line), file)) {
        parse_config_line(line, config);
    }

    fclose(file);
    printf("Configuration chargée depuis config/config.cfg\n");
    return true;
}

// Affiche la configuration (pour debug)
void print_map_config(MapConfig* config) {
    printf("\n========== CONFIGURATION DE CARTE ==========\n");
    printf("Dimensions: %d x %d\n", config->map_width, config->map_height);
    printf("Probabilités:\n");
    printf("  - Combat: %d%%\n", config->combat_tile_probability);
    printf("  - Trésor: %d%%\n", config->treasure_tile_probability);
    printf("  - Marchand: %d%%\n", config->merchant_tile_probability);
    printf("  - Boss: %d%%\n", config->boss_tile_probability);
    printf("  - Sauvegarde: %d%%\n", config->save_tile_probability);
    printf("Ennemis par combat: %d - %d\n", config->enemy_count_min, config->enemy_count_max);
    printf("Incrément de profondeur: %d - %d m\n", config->depth_increment_min, config->depth_increment_max);
    printf("Zones définies: %d\n", config->zone_count);
    for (int i = 0; i < config->zone_count; i++) {
        printf("  - %s\n", config->zone_names[i]);
    }
    printf("===========================================\n\n");
}
