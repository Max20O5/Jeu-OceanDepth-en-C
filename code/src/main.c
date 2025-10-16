#include "include.h"
#include "game.h"
#include "menu.h"

int main() {
    
    srand(time(NULL));

    bool running = true;

    while (running) {
        int choix_menu = afficher_menu_principal();

        switch (choix_menu) {
            case 1:
                start_game();
                break;
            case 0:
                running = false;
                break;
        }
    }

    printf("\nMerci d'avoir joué à OceanDepth\n");
    return 0;
}