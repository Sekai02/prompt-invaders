#include <unistd.h>
#include <ncurses.h>
#include <string.h>

#include "start.h"

#define CHARACTER_DELAY 50000

void drawTextLetterByLetter(const char *text)
{
    // Get screen dimensions
    int cols = COLS;
    int lines = LINES;

    // Center coordinates for bottom of screen
    int x = COLS / 2 - strlen(text) / 2;
    int y = LINES / 2;

    // Clear the screen
    clear();

    // Set cursor position
    move(y, x);

    // Draw characters one by one with delay and beep
    for (int i = 0; text[i]; i++)
    {
        addch(text[i]);
        refresh();
        usleep(CHARACTER_DELAY); // 500ms delay between each character
        beep();
    }
}

void drawStartMessage()
{
    // Initialize ncurses
    initscr();
    cbreak();
    noecho();

    atexit((void (*)())endwin);

    drawTextLetterByLetter("Welcome soldier!\n");
    sleep(1);
    drawTextLetterByLetter("\n");

    drawTextLetterByLetter("Defeat the PROMPT INVADERS and come back a hero.\n");
    sleep(1);
    drawTextLetterByLetter("\n");

    drawTextLetterByLetter("Your operating system is depending upon you.\n");
    sleep(1);
    drawTextLetterByLetter("\n");

    drawTextLetterByLetter("Good luck.\n");
    sleep(1);
    drawTextLetterByLetter("\n");

    /*drawTextLetterByLetter("Press enter to start.");
    sleep(1);
    drawTextLetterByLetter("\n");

    while(getch()!='\n'){}*/

    // Clean up
    endwin();
}