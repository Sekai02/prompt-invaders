cmake_minimum_required(VERSION 3.28)
project(prompt_invaders C)

set(CMAKE_C_STANDARD 11)

add_executable(prompt_invaders main.c
        start.c
        start.h
        typewriter.c
        typewriter.h
        globals.c
        globals.h
        menu.c
        menu.h
        utilities.c
        utilities.h
        iohandler.c
        iohandler.h)
