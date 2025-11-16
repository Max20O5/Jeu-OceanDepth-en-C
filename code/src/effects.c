#include "effects.h"
#include "include.h"
#include "utils.h"

/*
=====================================================================
 APPLICATION DES STATUTS SUR L'ENNEMI OU LE JOUEUR
=====================================================================
*/

void apply_status_to_enemy(CreatureMarine* enemy, EffectType type, int duration, int potency) {
    for (int i = 0; i < enemy->active_effect_count; i++) {
        if (enemy->active_effects[i].type == type) {
            enemy->active_effects[i].duration = duration;
            enemy->active_effects[i].potency = potency;
            printf("(L'effet sur %s a été rafraîchi.)\n", enemy->nom);
            return;
        }
    }
    
    // Ajouter un nouvel effet s'il y a de la place
    if (enemy->active_effect_count < 5) {
        enemy->active_effects[enemy->active_effect_count].type = type;
        enemy->active_effects[enemy->active_effect_count].duration = duration;
        enemy->active_effects[enemy->active_effect_count].potency = potency;
        enemy->active_effect_count++;
        printf("%s souffre d'un nouvel effet !\n", enemy->nom);
    } else {
        printf("%s a déjà trop d'effets actifs !\n", enemy->nom);
    }
}

void apply_status_to_player(Plongeur* player, EffectType type, int duration, int potency) {
    // Vérifier si l'effet est déjà appliqué pour le rafraîchir
    for (int i = 0; i < player->active_effect_count; i++) {
        if (player->active_effects[i].type == type) {
            player->active_effects[i].duration = duration;
            player->active_effects[i].potency = potency;
            printf("(Votre effet a été rafraîchi.)\n");
            return;
        }
    }
    
    // Ajouter un nouvel effet
    if (player->active_effect_count < 5) {
        player->active_effects[player->active_effect_count].type = type;
        player->active_effects[player->active_effect_count].duration = duration;
        player->active_effects[player->active_effect_count].potency = potency;
        player->active_effect_count++;
        printf("Vous souffrez d'un nouvel effet !\n");
    } else {
         printf("Vous avez déjà trop d'effets actifs !\n");
    }
}


/*
=====================================================================
 ACTIVATION DES EFFETS D'ARME TOUR DU JOUEUR
=====================================================================
*/


void apply_weapon_effects(Plongeur* player, CreatureMarine* target, Arme weapon, int damage_dealt) {
    
    // ---  GESTION DES EFFETS INSTANTANES ---

    if (strcmp(weapon.effet_special, "VolDeVie") == 0) {
        int heal_amount = (int)(damage_dealt * 0.25); 
        if (heal_amount < 1) heal_amount = 1;
        
        player->points_de_vie += heal_amount;
        if (player->points_de_vie > player->points_de_vie_max) {
            player->points_de_vie = player->points_de_vie_max;
        }
        printf("Votre %s draine l'ennemi ! Vous récupérez %d PV.\n", weapon.nom, heal_amount);
    }
    else if (strcmp(weapon.effet_special, "Force") == 0) {
        int bonus_damage = 8; 
        target->points_de_vie_actuels -= bonus_damage;
        printf("Votre %s frappe avec FORCE ! +%d dégâts supplémentaires !\n", weapon.nom, bonus_damage);
    }

    // --- GESTION DES APPLICATIONS DE STATUT ---
  
    else if (strcmp(weapon.effet_special, "Brulure_Faible") == 0) {
        printf("\n%s prend feu !\n", target->nom);
        apply_status_to_enemy(target, EFFET_BRULURE, 3, 2); 
    }
    else if (strcmp(weapon.effet_special, "Brulure_Forte") == 0) {
        printf("\n%s est en feu ! (Intense)\n", target->nom);
        apply_status_to_enemy(target, EFFET_BRULURE, 2, 5); 
    }
    else if (strcmp(weapon.effet_special, "Charme") == 0) {
        printf("\n%s est soudainement... charmé ?\n", target->nom);
        apply_status_to_enemy(target, EFFET_CHARME, 2, 0); 
    }
    else if (strcmp(weapon.effet_special, "Paralysie") == 0) {
        printf("\n%s est paralysé par votre attaque !\n", target->nom);
        apply_status_to_enemy(target, EFFET_PARALYSIE, 1, 0);
    }
    
}


/*
=====================================================================
 TRAITEMENT DES EFFETS ACTIFS DEBUT DU TOURS
=====================================================================
*/

bool process_enemy_effects(CreatureMarine* enemy) {
    bool can_act = true;
    int burn_damage = 0;

    // On parcourt les effets à l'envers pour suppression facile
    for (int j = enemy->active_effect_count - 1; j >= 0; j--) {
        StatusEffect* effect = &enemy->active_effects[j];
        
        // Gérer l'effet
        switch (effect->type) {
            case EFFET_BRULURE:
                burn_damage += effect->potency; 
                break;
                
            case EFFET_PARALYSIE:
                can_act = false;
                printf("%s est paralysé et ne peut pas bouger !\n", enemy->nom);
                break;
                
            case EFFET_CHARME:
                can_act = false;
                printf("%s est charmé et passe son tour, confus.\n", enemy->nom);
                break;
                
            default: break;
        }

        // Réduire la durée
        effect->duration--;
        
        // Supprimer l'effet s'il est terminé
        if (effect->duration <= 0) {
            printf("L'effet (%d) s'est dissipé sur %s.\n", effect->type, enemy->nom);
            // Décaler les éléments pour écraser l'effet terminé
            for (int k = j; k < enemy->active_effect_count - 1; k++) {
                enemy->active_effects[k] = enemy->active_effects[k + 1];
            }
            enemy->active_effect_count--; 
        }
    }

    // Appliquer les dégâts de brûlure
    if (burn_damage > 0) {
        enemy->points_de_vie_actuels -= burn_damage;
        printf("%s subit %d dégâts de brûlure !\n", enemy->nom, burn_damage);
        if (enemy->points_de_vie_actuels <= 0) {
            enemy->points_de_vie_actuels = 0;
            enemy->est_vivant = false;
            printf("%s succombe à ses brûlures !\n", enemy->nom);
            can_act = false;
        }
    }
    
    return can_act;
}

bool process_player_effects(Plongeur* player) {
    bool can_act = true;
    int burn_damage = 0;

    for (int j = player->active_effect_count - 1; j >= 0; j--) {
        StatusEffect* effect = &player->active_effects[j];
        
        switch (effect->type) {
            case EFFET_BRULURE:
                burn_damage += effect->potency;
                break;
            case EFFET_PARALYSIE:
                can_act = false;
                printf("Vous êtes paralysé et ne pouvez pas bouger !\n");
                break;
            case EFFET_CHARME:
                can_act = false;
                printf("Vous êtes charmé et passez votre tour, confus.\n");
                break;
            default: break;
        }

        effect->duration--;
        if (effect->duration <= 0) {
            printf("Un effet sur vous s'est dissipé.\n");
            for (int k = j; k < player->active_effect_count - 1; k++) {
                player->active_effects[k] = player->active_effects[k + 1];
            }
            player->active_effect_count--;
        }
    }

    if (burn_damage > 0) {
        player->points_de_vie -= burn_damage;
        printf("Vous subissez %d dégâts de brûlure !\n", burn_damage);
        if (player->points_de_vie <= 0) {
            player->points_de_vie = 0;
            printf("Vous succombez à vos brûlures !\n");
            can_act = false;
        }
    }
    return can_act;
}