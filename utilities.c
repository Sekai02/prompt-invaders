#include "globals.h"

#include <stdio.h>
#include <stdlib.h>

void clearScreen() {
#if defined(__linux__) || defined(__unix__) || defined(__APPLE__)
    system("clear");
#endif

#if defined(_WIN32) || defined(_WIN64)
    system("cls");
#endif
}

void setColor(){
    printf("%s", TEXT_COLOR);
    //printf("%s", BACK_COLOR);
}

void resetColor(){
    printf("%s", RESET_COLOR);
}