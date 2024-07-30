#include "start.h"
#include "typewriter.h"

#include <unistd.h>
#include <stdio.h>

void drawStartMessage(){
    typeTextWithSound("Welcome soldier!\n");
    sleep(1);
    printf("\n");

    typeTextWithSound("Defeat the PROMPT INVADERS and come back a hero.\n");
    sleep(1);
    printf("\n");

    typeTextWithSound("Your operating system is depending upon you.\n");
    sleep(1);
    printf("\n");

    typeTextWithSound("Good luck.\n");
    sleep(1);
    printf("\n");

    typeTextWithSound("Press enter to start.");

    getchar();
}