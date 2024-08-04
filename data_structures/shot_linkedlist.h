#ifndef PROMPT_INVADERS_SHOT_LINKEDLIST_H
#define PROMPT_INVADERS_SHOT_LINKEDLIST_H

typedef struct Shot {
    int x;
    int y;
    struct Shot* next;
} Shot;

typedef struct ShotManager {
    Shot* shot_list;
    void (*add_shot)(struct ShotManager*, int, int);
    void (*remove_shot)(struct ShotManager*, int, int);
    void (*print_shots)(struct ShotManager*);
    void (*free_shots)(struct ShotManager*);
} ShotManager;

void add_shot(ShotManager* manager, int x, int y);
void remove_shot(ShotManager* manager, int x, int y);
void free_shots(ShotManager* manager);
ShotManager* init_shot_manager();

#endif
