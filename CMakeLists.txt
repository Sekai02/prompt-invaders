cmake_minimum_required(VERSION 3.22)
project(prompt_invaders C)

set(CMAKE_C_STANDARD 11)

add_executable(prompt_invaders main.c
        scenes/game.c
        scenes/game.h
        scenes/start.c
        scenes/start.h
        scenes/menu.c
        scenes/menu.h
        system/gamestate.c
        system/gamestate.h
        scenes/gameover.c
        scenes/gameover.h
)

target_link_libraries(${PROJECT_NAME} ncurses)
