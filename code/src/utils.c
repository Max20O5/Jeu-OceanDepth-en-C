#include "utils.h"
#include "include.h"

void clear_screen(void) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

int randomNumber(int min, int max) {
    return rand() % (max - min + 1) + min;
}

void clear_input_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void wait_for_enter(void) {
    printf("\nAppuyez sur Entrée pour continuer...");
    clear_input_buffer();
}