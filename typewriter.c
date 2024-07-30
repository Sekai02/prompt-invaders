#include "typewriter.h"

#include <stdio.h>
#include <unistd.h>
#include <string.h>

void typeText(const char *text) {
    for (int i = 0; text[i] != '\0'; i++) {
        printf("%c", text[i]);
        fflush(stdout);
        usleep(100000);
    }
}

void typeTextWithSound(const char *text) {
    for (int i = 0; text[i] != '\0'; i++) {
        printf("%c", text[i]);
        fflush(stdout);
        usleep(100000); // Delay for visual effect

        // Play a beep sound
        fprintf(stdout, "\a");
    }
}

void printCentered(char *text) {
    int consoleWidth = 80; // Default console width
    int textWidth = strlen(text);

    int padding = (consoleWidth - textWidth) / 2;

    for (int i = 0; i < padding; i++) {
        printf(" ");
    }

    printf("%s\n", text);
}

void customEndl(int lines){
    for(int i=0;i<lines;i++)
        printf("\n");
}