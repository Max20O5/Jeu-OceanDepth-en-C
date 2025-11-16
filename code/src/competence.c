#include "competence.h"
#include "tab.h"        
#include "utils.h"    
#include "effects.h"
#include "include.h"

CompetenceAquatique get_skill_by_id(int id) {
    for (int i = 0; i < g_skill_count; i++) {
        if (g_skill_database[i].id == id) {
            return g_skill_database[i];
        }
    }
    return g_skill_database[0]; 
}

void reduce_all_skill_cooldowns(Plongeur* player) {
    for (int i = 0; i < 4; i++) {
        if (player->competences_cooldowns[i] > 0) {
            player->competences_cooldowns[i]--;
        }
    }
}


bool handle_skill_use(Plongeur* player, CreatureMarine* enemies, int enemy_count) {    
    clear_screen();

    printf("--- COMPÉTENCES MARINES ---\n\n");
    printf("Oxygène: %d/%d | Fatigue: %d/5\n", 
           player->niveau_oxygene, player->niveau_oxygene_max, player->niveau_fatigue);
    printf("--------------------------------\n");

    // Afficher les compétences apprises
    for (int i = 0; i < 4; i++) {
        CompetenceAquatique* skill = &player->competences_apprises[i];
        printf(" [%d] %s (O²: %d, Fat: +%d) \t| ", 
               i + 1, skill->nom, skill->cost_oxygene, skill->cost_fatigue);
        
        if (player->competences_cooldowns[i] > 0) {
            printf("RECHARGE: %d tour(s)\n", player->competences_cooldowns[i]);
        } else {
            printf("PRÊT\n");
        }
    }
    printf("\n [0] - Annuler\n");
    printf("\n> Votre choix : ");

    // Lire le choix
    int choice = -1;
    char term;
    while (scanf("%d%c", &choice, &term) != 2 || term != '\n' || (choice < 0 || choice > 4)) {
        printf("Erreur : Entrez 0-4.\n");
        clear_input_buffer();
        printf("> Votre choix : ");
    }

    if (choice == 0) {
        printf("Action annulée.\n");
        return false;
    }
    
    int skill_index = choice - 1;
    CompetenceAquatique* skill = &player->competences_apprises[skill_index];

    if (player->competences_cooldowns[skill_index] > 0) {
        printf("Compétence en cours de recharge !\n");
        return false;
    }
    if (player->niveau_oxygene < skill->cost_oxygene) {
        printf("Pas assez d'oxygène ! (Requis: %d)\n", skill->cost_oxygene);
        return false;
    }
    if (player->niveau_fatigue + skill->cost_fatigue > 5) {
        printf("Vous êtes trop fatigué pour utiliser cette compétence !\n");
        return false;
    }

    printf("\nVous utilisez %s !\n", skill->nom);

    player->niveau_oxygene -= skill->cost_oxygene;
    player->niveau_fatigue += skill->cost_fatigue;

    switch (skill->effect_type) {
        
        case SKILL_EFFECT_HEAL_OXYGEN:
            player->niveau_oxygene += skill->potency; 
            if (player->niveau_oxygene > player->niveau_oxygene_max) {
                player->niveau_oxygene = player->niveau_oxygene_max;
            }
            printf("Vous récupérez %d Oxygène !\n", skill->potency);
            break;

        case SKILL_EFFECT_DAMAGE_AOE:
            printf("Une décharge électrique frappe tous les ennemis !\n");
            for (int i = 0; i < enemy_count; i++) {
                if (enemies[i].est_vivant) {
                    enemies[i].points_de_vie_actuels -= skill->potency;
                    printf("%s subit %d dégâts !\n", enemies[i].nom, skill->potency);
                }
            }
            break;
            
        case SKILL_EFFECT_PACIFY:
            printf("Vous pacifiez une créature... (Logique à implémenter)\n");
            break;
            
        case SKILL_EFFECT_DEBUFF_SPEED:
            printf("Un tourbillon ralentit les ennemis... (Logique à implémenter)\n");
            break;

        case SKILL_EFFECT_NONE:
        default:
            printf("Cette compétence n'a aucun effet.\n");
            break;
    }

    player->competences_cooldowns[skill_index] = skill->cooldown;
    
    return true;
}