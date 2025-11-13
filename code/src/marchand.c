#include "marchand.h"
#include "consommable.h"
#include "equipement.h"
#include "weapon.h"
#include "include.h"
#include "utils.h"

#define PRIX_TROUSSE_SOIN 15
#define PRIX_CAPSULE_O2  20
#define PRIX_STIMULANT 25
#define PRIX_ANTIDOTE 30
#define PRIX_HARPON_ACIER 80
#define PRIX_HARPON_ELECTRIQUE 150
#define PRIX_HARPON_LASER 300
#define PRIX_COMBINAISON_RENFORCEE 50
#define PRIX_COMBINAISON_PROFONDEUR 100
#define PRIX_ARMURE_TITANIUM 200

int find_empty_slot(Plongeur* player) {
    for (int i = 0; i < 8; i++) {
        if (player->inventaire[i].id == 0) {
            return i;
        }
    }
    return -1;
}

void buy_item(Plongeur* player, Consommable item, int price) {
    if (player->perles < price) {
        printf("\nPas assez de perles ! (Requis: %d, Vous avez: %d)\n", price, player->perles);
        return;
    }

    int slot = find_empty_slot(player);
    if (slot == -1) {
        printf("\nVotre inventaire est plein ! Achat impossible.\n");
        return;
    }

    player->perles -= price;
    player->inventaire[slot] = item;

    printf("\nMerci pour votre achat ! %s ajouté à l'inventaire (Slot %d).\n", item.nom, slot + 1);
    printf("Il vous reste %d perles.\n", player->perles);
}

void buy_arme(Plongeur* player, Arme arme, int price) {
    if (player->perles < price) {
        printf("\nPas assez de perles ! (Requis: %d, Vous avez: %d)\n", price, player->perles);
        return;
    }

    printf("\nVous remplacez votre %s par %s.\n",
           player->arme_equipee.nom, arme.nom);

    player->perles -= price;
    player->arme_equipee = arme;

    printf("\nMerci pour votre achat ! %s equipee (ATK: %d-%d).\n",
           arme.nom, arme.attaque_minimale, arme.attaque_maximale);
    printf("Il vous reste %d perles.\n", player->perles);
}

void buy_combinaison(Plongeur* player, Equipement combinaison, int price) {
    if (player->perles < price) {
        printf("\nPas assez de perles ! (Requis: %d, Vous avez: %d)\n", price, player->perles);
        return;
    }

    printf("\nVous remplacez votre %s par %s.\n",
           player->combinaison_equipee.nom, combinaison.nom);

    player->perles -= price;
    player->combinaison_equipee = combinaison;

    printf("\nMerci pour votre achat ! %s équipée (Defense: +%d).\n",
           combinaison.nom, get_defense_equipement(&combinaison));
    printf("Il vous reste %d perles.\n", player->perles);
}

void open_shop(Plongeur* player) {
    bool is_shopping = true;

    while (is_shopping) {
        clear_screen();
        printf("====================  MARCHAND  ====================\n\n");
        printf("    Bienvenue dans ma boutique, plongeur !\n");
        printf("    Vous avez %d perles .\n\n", player->perles);
        printf("    Que désirez-vous ?\n");
        printf("    ----------------------------------------\n");
        printf("     CONSOMMABLES :\n");
        printf("    [ 1 ] - Trousse de Soin  (+25 PV)   - Prix: %d\n", PRIX_TROUSSE_SOIN);
        printf("    [ 2 ] - Capsule O2       (+40 O2)   - Prix: %d\n", PRIX_CAPSULE_O2);
        printf("    [ 3 ] - Stimulant Marin  (Fatigue -2) - Prix: %d\n", PRIX_STIMULANT);
        printf("    [ 4 ] - Antidote         (Guérit poison/paralysie) - Prix: %d\n", PRIX_ANTIDOTE);
        printf("\n     ARMES :\n");
        printf("    [ 5 ] - Harpon d'Acier (ATK: 20-28) - Prix: %d\n", PRIX_HARPON_ACIER);
        printf("    [ 6 ] - Harpon Electrique (ATK: 28-42, Ignore 3 DEF) - Prix: %d\n", PRIX_HARPON_ELECTRIQUE);
        printf("    [ 7 ] - Harpon Laser (ATK: 35-50, Ignore 5 DEF) - Prix: %d\n", PRIX_HARPON_LASER);
        printf("\n     COMBINAISONS :\n");
        printf("    [ 8 ] - Combinaison Renforcee (+10 DEF) - Prix: %d\n", PRIX_COMBINAISON_RENFORCEE);
        printf("    [ 9 ] - Combinaison Profondeur (+15 DEF) - Prix: %d\n", PRIX_COMBINAISON_PROFONDEUR);
        printf("    [10 ] - Armure Titanium (+25 DEF) - Prix: %d\n", PRIX_ARMURE_TITANIUM);
        printf("\n    [ 0 ] - Quitter la boutique\n");
        printf("\n=======================================================\n");
        printf("> Votre choix : ");

        int choice = -1;
        char term;
        while (scanf("%d%c", &choice, &term) != 2 || term != '\n' || (choice < 0 || choice > 10)) {
            printf("Erreur : Veuillez entrer un choix entre 0 et 10.\n");
            while (getchar() != '\n');
            printf("> Votre choix : ");
        }

        switch (choice) {
            case 1:
                buy_item(player, creer_trousse_de_soin(), PRIX_TROUSSE_SOIN);
                break;
            case 2:
                buy_item(player, creer_capsule_oxygene(), PRIX_CAPSULE_O2);
                break;
            case 3:
                buy_item(player, creer_stimulant_marin(), PRIX_STIMULANT);
                break;
            case 4:
                buy_item(player, creer_antidote(), PRIX_ANTIDOTE);
                break;
            case 5:
                buy_arme(player, creer_harpon_acier(), PRIX_HARPON_ACIER);
                break;
            case 6:
                buy_arme(player, creer_harpon_electrique(), PRIX_HARPON_ELECTRIQUE);
                break;
            case 7:
                buy_arme(player, creer_harpon_laser(), PRIX_HARPON_LASER);
                break;
            case 8:
                buy_combinaison(player, creer_combinaison_renforcee(), PRIX_COMBINAISON_RENFORCEE);
                break;
            case 9:
                buy_combinaison(player, creer_combinaison_profondeur(), PRIX_COMBINAISON_PROFONDEUR);
                break;
            case 10:
                buy_combinaison(player, creer_armure_titanium(), PRIX_ARMURE_TITANIUM);
                break;
            case 0:
                is_shopping = false;
                break;
        }
        
        if (is_shopping) {
            printf("\nAppuyez sur Entrée pour continuer vos achats...");
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
        }
    }

    printf("\nMerci de votre visite ! Plongez prudemment !\n");
    printf("Appuyez sur Entrée pour continuer votre aventure...");
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}