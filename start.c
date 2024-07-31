#include "start.h"
#include "typewriter.h"
#include "iohandler.h"

#include <unistd.h>
#include <stdio.h>

void drawStartMessage(){
    enableRawMode();
    typeTextWithSound("Welcome soldier!\n");
    sleep(1);
    disableRawMode();
    printf("\n");

    enableRawMode();
    typeTextWithSound("Defeat the PROMPT INVADERS and come back a hero.\n");
    sleep(1);
    disableRawMode();
    printf("\n");

    enableRawMode();
    typeTextWithSound("Your operating system is depending upon you.\n");
    sleep(1);
    disableRawMode();
    printf("\n");

    enableRawMode();
    typeTextWithSound("Good luck.\n");
    sleep(1);
    disableRawMode();
    printf("\n");

    enableRawMode();
    typeTextWithSound("Press enter to start.");
    disableRawMode();

    getchar();
}