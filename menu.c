#include "menu.h"
#include "typewriter.h"
#include "utilities.h"
#include "iohandler.h"

#include <stdlib.h>
#include <stdio.h>

void printMenu(int selectedOption) {
    clearScreen();

    disableRawMode();

    printCentered("PROMPT-INVADERS");
    printf("\n");
    printCentered("Copyright (c) 2024 Luis Alejandro Arteaga & Jossue Arteche");
    printCentered("Operative Systems Project");
    printCentered("University of Havana");
    customEndl(5);

    if (selectedOption == 0) {
        printCentered("■ Play Game");
        customEndl(5);
        printCentered("Exit");
    } else {
        printCentered("Play Game");
        customEndl(5);
        printCentered("■ Exit");
    }

    enableRawMode();
}

void menuSelection(){
    int selectedOption = 0;
    int key;

    enableRawMode();

    do {
        printMenu(selectedOption);
        key = readKey();

        switch (key) {
            case ARROW_UP:
                selectedOption=(selectedOption - 1 + 2) % 2;
                break;

            case ARROW_DOWN:
                selectedOption=(selectedOption + 1) % 2;
                break;

            default:
                break;
        }
    } while (key != ENTER); // Repeat until Enter key is pressed

    disableRawMode();

    customEndl(5);

    if (selectedOption == 0) {
        typeTextWithSound("Starting the game...");
    } else {
        typeTextWithSound("Exiting the game...");
        exit(0);
    }
}