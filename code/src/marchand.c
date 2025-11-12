#include "marchand.h"
#include "consommable.h"
#include "weapon.h"
#include "competence.h"
#include "tab.h"
#include "include.h"
#include "utils.h"

// --- PRIX DES OBJETS ---
#define PRIX_TROUSSE_SOIN 15
#define PRIX_CAPSULE_O2   20
#define PRIX_STIMULANT    35

// Armes
#define PRIX_HARPON_VAMPIRE 75
#define PRIX_HACHE_FEU      120
#define PRIX_TRIDENT_LASER  250

// Compétences
#define PRIX_SKILL_PACIFY   150
#define PRIX_SKILL_TOURBILLON 200


int find_empty_inventory_slot(Plongeur* player) {
    for (int i = 0; i < 8; i++) {
        if (player->inventaire[i].id == 0) {
            return i;
        }
    }
    return -1;
}

bool has_skill(Plongeur* player, int skill_id) {
    for (int i = 0; i < 4; i++) {
        if (player->competences_apprises[i].id == skill_id) {
            return true;
        }
    }
    return false;
}

int find_empty_skill_slot(Plongeur* player) {
    for (int i = 0; i < 4; i++) {
        if (player->competences_apprises[i].id == 0) {
             return i;
        }
    }

    if (player->competences_apprises[2].id == 0) return 2;
    if (player->competences_apprises[3].id == 0) return 3;
    
    return -1;
}

void show_buy_consumables(Plongeur* player) {
    clear_screen();
    printf("--- ACHAT CONSOMMABLES ---\n");
    printf("Vous avez %d perles.\n\n", player->perles);
    printf("  [1] Trousse de Soin   - %d Perles\n", PRIX_TROUSSE_SOIN);
    printf("  [2] Capsule O2        - %d Perles\n", PRIX_CAPSULE_O2);
    printf("  [3] Stimulant Marin - %d Perles\n", PRIX_STIMULANT);
    printf("  [0] Retour\n");
    printf("\n> Votre choix : ");

    int choice = -1;
    char term;
    scanf("%d%c", &choice, &term); clear_input_buffer();

    int prix = 0;
    int item_id = 0;

    switch (choice) {
        case 1: item_id = 1; prix = PRIX_TROUSSE_SOIN; break;
        case 2: item_id = 2; prix = PRIX_CAPSULE_O2; break;
        case 3: item_id = 3; prix = PRIX_STIMULANT; break;
        case 0: return;
        default: printf("Choix invalide.\n"); wait_for_enter(); return;
    }

    if (player->perles < prix) {
        printf("\nPas assez de perles !\n");
    } else {
        int slot = find_empty_inventory_slot(player);
        if (slot == -1) {
            printf("\nVotre inventaire de consommables est plein !\n");
        } else {
            player->perles -= prix;
            player->inventaire[slot] = get_consommable_by_id(item_id);
            printf("\nAchat effectué ! %s ajouté.\n", player->inventaire[slot].nom);
        }
    }
    wait_for_enter();
}

void show_buy_weapons(Plongeur* player) {
    clear_screen();
    printf("--- ACHAT ARMES ---\n");
    printf("Vous avez %d perles.\n",  player->perles);
    printf("Arme actuelle : %s\n\n", player->arme_equipee.nom);
    
    printf("  [1] %s - %d Perles\n", g_weapon_database[1].nom, PRIX_HARPON_VAMPIRE); 
    printf("  [2] %s - %d Perles\n", g_weapon_database[2].nom, PRIX_HACHE_FEU);
    printf("  [3] %s - %d Perles\n", g_weapon_database[3].nom, PRIX_TRIDENT_LASER);
    printf("  [0] Retour\n");
    printf("\n> Votre choix : ");

    int choice = -1;
    char term;
    scanf("%d%c", &choice, &term); clear_input_buffer();

    int prix = 0;
    int weapon_id = 0;

    switch (choice) {
        case 1: weapon_id = 2; prix = PRIX_HARPON_VAMPIRE; break;
        case 2: weapon_id = 3; prix = PRIX_HACHE_FEU; break;
        case 3: weapon_id = 4; prix = PRIX_TRIDENT_LASER; break;
        case 0: return;
        default: printf("Choix invalide.\n"); wait_for_enter(); return;
    }

    if (player->perles < prix) {
        printf("\nPas assez de perles !\n");
    } 
    else if (player->arme_equipee.id == weapon_id) {
        printf("\nVous possédez déjà cette arme !\n");
    } 
    else {
        Arme new_weapon = get_weapon_by_id(weapon_id);
        
        printf("\nAcheter %s pour %d perles ?\n", new_weapon.nom, prix);
        printf("(Cela remplacera votre %s actuel)\n", player->arme_equipee.nom);
        printf("Confirmer ? (O/N) : ");

        char confirm_choice = 'N';
        scanf(" %c", &confirm_choice); clear_input_buffer();

        if (confirm_choice == 'O' || confirm_choice == 'o') {
            player->perles -= prix;
            player->arme_equipee = new_weapon;
            printf("\nAchat effectué ! %s équipé.\n", new_weapon.nom);
        } else {
            printf("\nAchat annulé.\n");
        }
    }
    wait_for_enter();
}
void show_buy_skills(Plongeur* player) {
    clear_screen();
    printf("--- APPRENDRE COMPÉTENCES ---\n");
    printf("Vous avez %d perles.\n\n", player->perles);

    printf("  [1] %s - %d Perles\n", g_skill_database[2].nom, PRIX_SKILL_PACIFY);
    printf("  [2] %s - %d Perles\n", g_skill_database[3].nom, PRIX_SKILL_TOURBILLON);
    printf("  [0] Retour\n");
    printf("\n> Votre choix : ");
    
    int choice = -1;
    char term;
    scanf("%d%c", &choice, &term); clear_input_buffer();

    int prix = 0;
    int skill_id = 0;

    switch (choice) {
        case 1: skill_id = 3; prix = PRIX_SKILL_PACIFY; break;
        case 2: skill_id = 4; prix = PRIX_SKILL_TOURBILLON; break;
        case 0: return;
        default: printf("Choix invalide.\n"); wait_for_enter(); return;
    }
    
    if (player->perles < prix) {
        printf("\nPas assez de perles !\n");
    } else if (has_skill(player, skill_id)) {
        printf("\nVous connaissez déjà cette compétence !\n");
    } else {
        int slot = find_empty_skill_slot(player);
        if (slot == -1) {
            printf("\nVous ne pouvez pas apprendre plus de compétences (4/4) !\n");
        } else {
            printf("\nApprendre %s pour %d perles ? (O/N) : ", g_skill_database[skill_id - 1].nom, prix);
            char confirm_choice = 'N';
            scanf(" %c", &confirm_choice); clear_input_buffer();

            if (confirm_choice == 'O' || confirm_choice == 'o') {
                player->perles -= prix;
                player->competences_apprises[slot] = get_skill_by_id(skill_id);
                printf("\nCompétence apprise ! %s ajoutée (Slot %d).\n", 
                       player->competences_apprises[slot].nom, slot + 1);
            } else {
                printf("\nAchat annulé.\n");
            }
        }
    }
    wait_for_enter();
}

// --- MENU PRINCIPAL DU MARCHAND ---

void open_shop(Plongeur* player) {
    bool is_shopping = true;
    while (is_shopping) {
        clear_screen();
        printf("====================  MARCHAND  ====================\n\n");
        printf("    « Salutations, plongeur. Que puis-je pour vous ? »\n");
        printf("    Vous avez %d perles.\n\n", player->perles);
        printf("    [ 1 ] - Acheter des Consommables\n");
        printf("    [ 2 ] - Acheter des Armes\n");
        printf("    [ 3 ] - Apprendre des Compétences\n");
        printf("\n    [ 0 ] - Quitter la boutique\n");
        printf("\n======================================================\n");
        printf("> Votre choix : ");

        int choice = -1;
        char term;
        scanf("%d%c", &choice, &term); clear_input_buffer();

        switch (choice) {
            case 1: show_buy_consumables(player); break;
            case 2: show_buy_weapons(player); break;
            case 3: show_buy_skills(player); break;
            case 0: is_shopping = false; break;
            default:
                printf("\nChoix invalide, veuillez réessayer.\n");
                wait_for_enter();
                break;
        }
    }
    printf("\nMerci de votre visite ! Plongez prudemment !\n");
    wait_for_enter();
}