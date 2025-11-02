#include "marchand.h"
#include "consommable.h"
#include "include.h"
#include "utils.h"

#define PRIX_TROUSSE_SOIN 15
#define PRIX_CAPSULE_O2  20

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
        printf("\n    [ 0 ] - Quitter la boutique\n");
        printf("\n=======================================================\n");
        printf("> Votre choix : ");

        int choice = -1;
        char term;
        while (scanf("%d%c", &choice, &term) != 2 || term != '\n' || (choice < 0 || choice > 2)) {
            printf("Erreur : Veuillez entrer 0, 1 ou 2.\n");
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