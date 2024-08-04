#include "game.h"
#include "../system/utilities.h"
#include "../system/iohandler.h"
#include "../data_structures/matrix.h"
#include "../data_structures/shot_linkedlist.h"

#include <stdio.h>
#include <unistd.h>

#define PLAYER_START_X (MAX_COLS/2)
#define PLAYER_START_Y (MAX_ROWS-1)

int isGameOver;
int gameScore;
int playerX, playerY;
Matrix gameBoard;
ShotManager* shotManager;

void shoot();
void checkShotsCollision();
void updateShots();
int isShot(int x, int y);

void getKeyboardActions(){
    enableRawMode();
    switch (readKey()) {
        case ARROW_LEFT:
            if(playerX>1)
                playerX--;
            break;
        case ARROW_RIGHT:
            if(playerX<MAX_COLS-2)
                playerX++;
            break;
        case ARROW_UP:
            if(playerY>MAX_ROWS-3)
                playerY--;
            break;
        case ARROW_DOWN:
            if(playerY<MAX_ROWS-1)
                playerY++;
            break;
        case 's':
            shoot();
            break;
        default:
            break;
    }
    disableRawMode();
}

void updateScreen(){
    checkShotsCollision();
    updateShots();

    for(int r=0;r<MAX_ROWS;r++) {
        for (int c = 0; c < MAX_COLS; c++) {
            if (c == 0 || c == MAX_COLS - 1)draw_element(&gameBoard,r,c,'B');
            else if (c == playerX && r == playerY)draw_element(&gameBoard,r,c,'A');
            else if (isShot(c,r))draw_element(&gameBoard, r, c, '*');
            else draw_element(&gameBoard,r,c,' ');
        }
    }
}

void drawScreen(){
    clearScreen();
    printf("Score: %d\n",gameScore);
    printf("\n");
    print_matrix(&gameBoard);

    //For Debug Purposes
    /*Shot *current = shotManager->shot_list;
    while (current != NULL) {
        printf("{%d, %d} ",current->x, current->y);
        current=current->next;
    }*/
}

void initGame(){
    disableRawMode();
    isGameOver=0;
    gameScore=0;
    playerX=PLAYER_START_X, playerY=PLAYER_START_Y;
    init_matrix(&gameBoard, MAX_ROWS, MAX_COLS);
    shotManager = init_shot_manager();
}

void runGame(){
    initGame();
    shoot();
    while(!isGameOver){
        updateScreen();
        //usleep(700000);
        drawScreen();
        getKeyboardActions();
    }
}

void shoot(){
    shotManager->add_shot(shotManager,playerX,playerY-1);
}

void checkShotsCollision(){
    while(1){
        int collisionDetected=0;

        Shot *current = shotManager->shot_list;
        while (current != NULL) {
            if (current->y <= 1) {
                remove_shot(shotManager,current->x,current->y);
                collisionDetected=1;
                break;
            }
            current = current->next;
        }

        if(!collisionDetected)break;
    }
}

void updateShots(){
    Shot *current = shotManager->shot_list;
    while (current != NULL) {
        current->y = current->y-1;
        current = current->next;
    }
}

int isShot(int x, int y){
    Shot *current = shotManager->shot_list;
    while (current != NULL) {
        if(current->x == x && current->y == y)return 1;
        current = current->next;
    }
    return 0;
}