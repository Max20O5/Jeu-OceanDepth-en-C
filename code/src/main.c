#include "include.h"
#include "game.h"
#include "menu.h"

int main() {
    
    srand(time(NULL));

    bool running = true;

    while (running) {
        int choice = print_menu();

        switch (choice) {
            case 0:
                start_game();
                break;
            case 1:
                load_game();
                break;
            case 2:
                running = false;
                break;
        }
    }

    printf("\nMerci d'avoir joué à OceanDepth\n");
    return EXIT_SUCCESS;
}