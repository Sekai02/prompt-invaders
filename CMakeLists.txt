cmake_minimum_required(VERSION 3.28)
project(prompt_invaders C)

set(CMAKE_C_STANDARD 11)

add_executable(prompt_invaders main.c
        scenes/start.c
        scenes/start.h
        system/typewriter.c
        system/typewriter.h
        globals.c
        globals.h
        scenes/menu.c
        scenes/menu.h
        system/utilities.c
        system/utilities.h
        system/iohandler.c
        system/iohandler.h
        scenes/game.c
        scenes/game.h
        data_structures/matrix.c
        data_structures/matrix.h
        data_structures/shot_linkedlist.c
        data_structures/shot_linkedlist.h
)
