#include <ncurses.h>
#include <string.h>

void drawGameOverScreen(int score, int maxScore) {
    initscr();              // Initialize ncurses
    noecho();               // Don't echo keyboard input
    curs_set(FALSE);        // Hide the cursor

    int maxY, maxX;         // Get the maximum y and x coordinates
    getmaxyx(stdscr, maxY, maxX);

    // Set up the game over screen
    clear();

    // Print the title in the center
    attron(A_BOLD);         // Enable bold text
    mvprintw(maxY / 2 - 3, (maxX - strlen("PROMPT-INVADERS")) / 2, "PROMPT-INVADERS");
    attroff(A_BOLD);        // Disable bold text

    // Print GAME OVER in bigger font in the center
    attron(A_BOLD | A_UNDERLINE);
    mvprintw(maxY / 2 - 1, (maxX - strlen("GAME OVER")) / 2, "GAME OVER");
    attroff(A_BOLD | A_UNDERLINE);

    // Print score and max score in the center
    mvprintw(maxY / 2 + 1, (maxX - strlen("Score: XXX")) / 2, "Score: %d", score);
    mvprintw(maxY / 2 + 2, (maxX - strlen("Max Score: XXX")) / 2, "Max Score: %d", maxScore);

    refresh();              // Refresh the screen
    getchar();               // Wait for a key press
    endwin();              // Clean up ncurses
}