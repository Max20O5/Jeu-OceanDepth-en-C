#include "utils.h"
#include <stdlib.h>

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