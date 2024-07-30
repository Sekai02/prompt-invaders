#include "globals.h"
#include "typewriter.h"
#include "utilities.h"
#include "iohandler.h"
#include "start.h"
#include "menu.h"

#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

// Define different scene identifiers
typedef enum{
    SCENE_START_MESSAGE,
    SCENE_MAIN_MENU,
    SCENE_GAME,
    SCENE_GAME_OVER
} Scene;

// Function prototypes for scene functions
void startMessage();
void mainMenu();
void game();
void gameOver();

void switchScene(Scene newScene){
    switch (newScene) {
        case SCENE_START_MESSAGE:
            startMessage();
            break;
        case SCENE_MAIN_MENU:
            mainMenu();
            break;
        case SCENE_GAME:
            game();
            break;
        case SCENE_GAME_OVER:
            gameOver();
            break;
        default:
            printf("Invalid scene\n");
            break;
    }
}

void startMessage() {
    drawStartMessage();
    switchScene(SCENE_MAIN_MENU);
}

void mainMenu(){
    menuSelection();
    getchar();
    switchScene(SCENE_GAME);
}

void game(){

}

void gameOver(){

}

int main(void) {
    switchScene(SCENE_START_MESSAGE);
    return 0;
}
