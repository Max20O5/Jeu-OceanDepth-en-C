#include "include.h"
#include "game.h"
#include "menu.h"

int main() {
    
    srand(time(NULL));

    bool running = true;

    while (running) {
        int choice = print_menu();

        switch (choice) {
            case 1:
                start_game();
                break;
            case 0:
                running = false;
                break;
        }
    }

    printf("\nMerci d'avoir joué à OceanDepth\n");
    return EXIT_SUCCESS;
}