#include "menu.h"   
#include "include.h" 

static void clear_screen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

int afficher_menu_principal(void) {
    clear_screen();
    printf("==================================================\n");
    printf("     _   _   _   _   _   _     _   _   _   _   _    \n");
    printf("    / \\ / \\ / \\ / \\ / \\ / \\   / \\ / \\ / \\ / \\ / \\   \n");
    printf("   ( O | c | e | a | n | D ) ( e | p | t | h | s )  \n");
    printf("    \\_/ \\_/ \\_/ \\_/ \\_/ \\_/   \\_/ \\_/ \\_/ \\_/ \\_/   \n");
    printf("\n");
    printf("==================================================\n\n");
    printf("        Entrez Dans les PROFONDEURS !\n\n");
    printf("    [ 1 ] - Commencer une nouvelle plongée\n");
    printf("    [ 0 ] - Quitter le jeu\n\n");

    int choix = -1;
    char term;
    printf("> Votre choix : ");
    while (scanf("%d%c", &choix, &term) != 2 || term != '\n' || (choix != 0 && choix != 1)) {
        printf("Erreur : Veuillez entrer 1 ou 0.\n");
        while (getchar() != '\n');
        printf("> Votre choix : ");
    }
    return choix;
}