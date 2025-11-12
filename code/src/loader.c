#include "loader.h"
#include "tab.h"
#include "include.h"
#include "struct.h"

Arme* g_weapon_database = NULL;
int g_weapon_count = 0;
CreatureMarine* g_creature_database = NULL;
int g_creature_count = 0;
Consommable* g_consommable_database = NULL;
int g_consommable_count = 0;
CompetenceAquatique* g_skill_database = NULL;
int g_skill_count = 0;

bool parse_line(char* line) {
    if (line[0] == '\n' || line[0] == '#' || line[0] == '\0') {
        return false;
    }
    line[strcspn(line, "\n")] = 0; 
    return true;
}

SkillEffectType skill_effect_from_string(const char* effect_str) {
    if (strcmp(effect_str, "HEAL_OXYGEN") == 0) return SKILL_EFFECT_HEAL_OXYGEN;
    if (strcmp(effect_str, "DAMAGE_AOE") == 0) return SKILL_EFFECT_DAMAGE_AOE;
    if (strcmp(effect_str, "PACIFY") == 0) return SKILL_EFFECT_PACIFY;
    if (strcmp(effect_str, "DEBUFF_SPEED") == 0) return SKILL_EFFECT_DEBUFF_SPEED;
    return SKILL_EFFECT_NONE;
}

bool load_weapons(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Erreur : Impossible d'ouvrir %s\n", filename);
        return false;
    }

    char buffer[256];
    int weapon_idx = 0;

    // Lire le compteur et allouer la mémoire
    fgets(buffer, sizeof(buffer), file);
    if (sscanf(buffer, "WEAPON_COUNT: %d", &g_weapon_count) != 1) {
        fprintf(stderr, "Erreur : %s n'a pas de WEAPON_COUNT valide.\n", filename);
        fclose(file);
        return false;
    }
    g_weapon_database = malloc(sizeof(Arme) * g_weapon_count);

    // Lire les données
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        if (!parse_line(buffer)) continue;

        Arme* w = &g_weapon_database[weapon_idx];
        sscanf(buffer, "WEAPON;%d;%[^;];%d;%d;%d;%[^\n]",
               &w->id, w->nom, &w->attaque_minimale, &w->attaque_maximale,
               &w->consommation_oxygene, w->effet_special);
        weapon_idx++;
    }

    fclose(file);
    printf("... %d Armes chargées.\n", g_weapon_count);
    return true;
}

bool load_creatures(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Erreur : Impossible d'ouvrir %s\n", filename);
        return false;
    }

    char buffer[256];
    int creature_idx = 0;

    // Lire le compteur et allouer
    fgets(buffer, sizeof(buffer), file);
    if (sscanf(buffer, "CREATURE_COUNT: %d", &g_creature_count) != 1) {
        fprintf(stderr, "Erreur : %s n'a pas de CREATURE_COUNT valide.\n", filename);
        fclose(file);
        return false;
    }
    g_creature_database = malloc(sizeof(CreatureMarine) * g_creature_count);

    // Lire les données
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        if (!parse_line(buffer)) continue;

        CreatureMarine* c = &g_creature_database[creature_idx];
        sscanf(buffer, "CREATURE;%d;%[^;];%d;%d;%d;%d;%d;%d;%d;%[^\n]",
               &c->id, c->nom, &c->points_de_vie_max, &c->points_de_vie_max,
               &c->attaque_minimale, &c->attaque_maximale, &c->defense,
               &c->perles_min, &c->perles_max, c->effet_special);
        
        // Initialiser les valeurs par défaut
        c->points_de_vie_actuels = c->points_de_vie_max;
        c->est_vivant = true;
        c->active_effect_count = 0;
        creature_idx++;
    }

    fclose(file);
    printf("... %d Créatures chargées.\n", g_creature_count);
    return true;
}

bool load_consumables(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Erreur : Impossible d'ouvrir %s\n", filename);
        return false;
    }

    char buffer[256];
    int consumable_idx = 0;

    // Lire le compteur et allouer
    fgets(buffer, sizeof(buffer), file);
    if (sscanf(buffer, "CONSUMABLE_COUNT: %d", &g_consommable_count) != 1) {
        fprintf(stderr, "Erreur : %s n'a pas de CONSUMABLE_COUNT valide.\n", filename);
        fclose(file);
        return false;
    }
    g_consommable_database = malloc(sizeof(Consommable) * g_consommable_count);

    // Lire les données
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        if (!parse_line(buffer)) continue;

        Consommable* c = &g_consommable_database[consumable_idx];
        sscanf(buffer, "CONSUMABLE;%d;%[^;];%[^;];%d;%d;%d",
               &c->id, c->nom, c->effet_special,
               &c->soin, &c->oxygene, &c->fatigue);
        consumable_idx++;
    }

    fclose(file);
    printf("... %d Consommables chargés.\n", g_consommable_count);
    return true;
}

bool load_skills(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Erreur : Impossible d'ouvrir %s\n", filename);
        return false;
    }

    char buffer[256];
    int skill_idx = 0;

    //  Lire le compteur et allouer
    fgets(buffer, sizeof(buffer), file);
    if (sscanf(buffer, "SKILL_COUNT: %d", &g_skill_count) != 1) {
        fprintf(stderr, "Erreur : %s n'a pas de SKILL_COUNT valide.\n", filename);
        fclose(file);
        return false;
    }
    g_skill_database = malloc(sizeof(CompetenceAquatique) * g_skill_count);

    // Lire les données
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        if (!parse_line(buffer)) continue;

        CompetenceAquatique* s = &g_skill_database[skill_idx];
        char temp_effect_type[30];

        int result = sscanf(buffer, "SKILL;%d;%[^;];%d;%d;%[^;];%d;%d",
               &s->id, 
               s->nom,            
               &s->cost_oxygene, 
               &s->cost_fatigue,
               temp_effect_type,
               &s->potency, 
               &s->cooldown);
        
        if (result == 7) { 

            s->effect_type = skill_effect_from_string(temp_effect_type);
            skill_idx++;
        } else {
            fprintf(stderr, "Erreur : Ligne de compétence mal formatée : %s\n", buffer);
        }
    }
    fclose(file);
    printf("... %d Compétences chargées.\n", g_skill_count);
    return true;

}


bool load_all_game_data(const char* config_path) {
    char weapon_file[100];
    char creature_file[100];
    char consumable_file[100];
    char skill_file[100];

    sprintf(weapon_file, "%s/weapons.cfg", config_path);
    sprintf(creature_file, "%s/creatures.cfg", config_path);
    sprintf(consumable_file, "%s/consumables.cfg", config_path);
    sprintf(skill_file, "%s/skills.cfg", config_path);
    
    printf("Chargement des données du jeu depuis '%s'...\n", config_path);


    if (!load_weapons(weapon_file)) return false;
    if (!load_creatures(creature_file)) return false;
    if (!load_consumables(consumable_file)) return false;
    if (!load_skills(skill_file)) return false;

    printf("Toutes les données du jeu ont été chargées avec succès.\n");
    return true;
}

void free_game_data(void) {
    free(g_weapon_database);
    free(g_creature_database);
    free(g_consommable_database);
    free(g_skill_database);
    
    g_weapon_database = NULL;
    g_creature_database = NULL;
    g_consommable_database = NULL;
    g_skill_database = NULL;
}