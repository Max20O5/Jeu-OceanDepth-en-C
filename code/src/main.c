#include "include.h"
#include "game.h"
#include "menu.h"
#include "loader.h"

int main() {
    
    srand(time(NULL));

    if (!load_all_game_data("config")) {
        printf("Echec du chargement des données. Vérifiez les fichiers dans /config.\n");
        return EXIT_FAILURE;
    }

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

    free_game_data();
    printf("\nMerci d'avoir joué à OceanDepth\n");
    return EXIT_SUCCESS;
}