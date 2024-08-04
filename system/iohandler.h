#ifndef UNTITLED_IOHANDLER_H
#define UNTITLED_IOHANDLER_H

void disableRawMode();
void enableRawMode();
void die(const char *s);
int readKey();

enum keyboardKey {
    ARROW_LEFT = 1000,
    ARROW_RIGHT,
    ARROW_UP,
    ARROW_DOWN,
    ENTER = '\r'
};

#endif