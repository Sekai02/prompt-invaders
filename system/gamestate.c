#include <stdio.h>

#include "gamestate.h"

int loadMaxScore() {
    int maxScore=0;
    FILE* file = fopen("maxScore.txt", "r");
    if (file!= NULL) {
        fscanf(file, "%d", &maxScore);
        fclose(file);
    } else {
        //printf("Error loading maxScore\n");
        maxScore=0; // Return a default value if file not found
    }

    return maxScore;
}

void saveMaxScore(int maxScore) {
    FILE* file = fopen("maxScore.txt", "w");
    if (file!= NULL) {
        fprintf(file, "%d", maxScore);
        fclose(file);
    } else {
        //printf("Error saving maxScore\n");
    }
}

int loadLastScore() {
    int lastScore=0;
    FILE* file = fopen("lastScore.txt", "r");
    if (file!= NULL) {
        fscanf(file, "%d", &lastScore);
        fclose(file);
    } else {
        //printf("Error loading lastScore\n");
        lastScore=0; // Return a default value if file not found
    }

    return lastScore;
}

void saveLastScore(int lastScore) {
    FILE* file = fopen("lastScore.txt", "w");
    if (file!= NULL) {
        fprintf(file, "%d", lastScore);
        fclose(file);
    } else {
        //printf("Error saving lastScore\n");
    }
}

void resetLastScore(){
    FILE* file = fopen("lastScore.txt", "w");
    if (file!= NULL) {
        fprintf(file, "%d", 0);
        fclose(file);
    } else {
        //printf("Error saving lastScore\n");
    }
}