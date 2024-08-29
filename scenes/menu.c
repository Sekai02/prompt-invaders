#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define TIME_WAIT 1 // seconds

void printMenu(int selectedOption)
{
    clear(); // Clear the screen

    // Print the title and copyright information
    attron(A_BOLD); // Enable bold text
    mvprintw(LINES / 2 - 4, (COLS - strlen("PROMPT-INVADERS")) / 2, "PROMPT-INVADERS");
    attroff(A_BOLD); // Disable bold text
    mvprintw(LINES / 2 - 3, (COLS - strlen("Copyright (c) 2024 Luis Alejandro Arteaga & Jossue Arteche")) / 2, "Copyright (c) 2024 Luis Alejandro Arteaga & Jossue Arteche");
    mvprintw(LINES / 2 - 2, (COLS - strlen("Operative Systems Project")) / 2, "Operative Systems Project");
    mvprintw(LINES / 2 - 1, (COLS - strlen("University of Havana")) / 2, "University of Havana");

    // Print the menu options
    if (selectedOption == 0)
    {
        attron(A_REVERSE); // Enable reverse video
        mvprintw(LINES / 2 + 2, (COLS - strlen("Play Game")) / 2, "Play Game");
        attroff(A_REVERSE); // Disable reverse video
        mvprintw(LINES / 2 + 4, (COLS - strlen("Exit")) / 2, "Exit");
    }
    else
    {
        mvprintw(LINES / 2 + 2, (COLS - strlen("Play Game")) / 2, "Play Game");
        attron(A_REVERSE); // Enable reverse video
        mvprintw(LINES / 2 + 4, (COLS - strlen("Exit")) / 2, "Exit");
        attroff(A_REVERSE); // Disable reverse video
    }

    refresh(); // Refresh the screen
}

void menuSelection()
{
    int selectedOption = 0;
    int key;

    initscr();            // Initialize ncurses
    noecho();             // Don't echo keyboard input
    curs_set(FALSE);      // Hide the cursor
    keypad(stdscr, TRUE); // Enable keypad

    do
    {
        printMenu(selectedOption);
        key = getch(); // Get a character from the keyboard

        switch (key)
        {
        case KEY_UP:
            beep();
            selectedOption = (selectedOption - 1 + 2) % 2;
            break;

        case KEY_DOWN:
            beep();
            selectedOption = (selectedOption + 1) % 2;
            break;

        case '\n': // Enter key
            beep();
            printMenu(selectedOption);
            refresh();
            sleep(TIME_WAIT); // Wait for TIME_WAIT seconds
            break;

        default:
            break;
        }
    } while (key != '\n'); // Repeat until Enter key is pressed

    if (selectedOption == 1)
    {
        exit(0);
    }

    endwin(); // Clean up ncurses
}