#include "menu.h"   
#include "include.h" 
#include "utils.h"

int print_menu(void) {
    clear_screen();
    printf("==================================================\n");
    printf("     _   _   _   _   _       _   _   _   _   _   _ \n");
    printf("    / \\ / \\ / \\ / \\ / \\     / \\ / \\ / \\ / \\ / \\ / \\\n");
    printf("   ( O | c | e | a | n )   ( D | e | p | t | h | s )\n");
    printf("    \\_/ \\_/ \\_/ \\_/ \\_/     \\_/ \\_/ \\_/ \\_/ \\_/ \\_/\n");
    printf("\n");
    printf("==================================================\n\n");
    printf("        Entrez Dans les PROFONDEURS !\n\n");
    printf("    [ 0 ] - Nouvelle partie\n");
    printf("    [ 1 ] - Charger une partie\n");
    printf("    [ 2 ] - Quitter le jeu\n\n");


    int choix = -1;
    char term;
    printf("> Votre choix : ");
    while (scanf("%d%c", &choix, &term) != 2 || term != '\n' || (choix < 0 || choix > 2)) {
        printf("Erreur : Veuillez entrer 1, 2 ou 0.\n");
        while (getchar() != '\n');
        printf("> Votre choix : ");
    }
    return choix;
}