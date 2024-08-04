#include "shot_linkedlist.h"

#include <stdio.h>
#include <stdlib.h>

void add_shot(ShotManager* manager, int x, int y) {
    // Check if a shot with the same coordinates already exists
    Shot* current = manager->shot_list;
    while (current != NULL) {
        if (current->x == x && current->y == y) {
            return;
        }
        current = current->next;
    }

    // If no shot with the same coordinates exists, add the new shot
    Shot* new_shot = (Shot*)malloc(sizeof(Shot));
    if (new_shot == NULL) {
        fprintf(stderr, "Memory allocation failed");
        exit(1);
    }
    new_shot->x = x;
    new_shot->y = y;
    new_shot->next = manager->shot_list;
    manager->shot_list = new_shot;
}

void remove_shot(ShotManager* manager, int x, int y) {
    Shot* current = manager->shot_list;
    Shot* prev = NULL;

    while (current != NULL) {
        if (current->x == x && current->y == y) {
            if (prev == NULL) {
                manager->shot_list = current->next;
            } else {
                prev->next = current->next;
            }
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
}

void free_shots(ShotManager* manager) {
    Shot* current = manager->shot_list;
    Shot* temp;
    while (current != NULL) {
        temp = current;
        current = current->next;
        free(temp);
    }
}

ShotManager* init_shot_manager() {
    ShotManager* manager = (ShotManager*)malloc(sizeof(ShotManager));
    if (manager == NULL) {
        fprintf(stderr, "Memory allocation failed");
        exit(1);
    }
    manager->shot_list = NULL;
    manager->add_shot = add_shot;
    manager->remove_shot = remove_shot;
    manager->free_shots = free_shots;
    return manager;
}
