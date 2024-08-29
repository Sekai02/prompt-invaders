#ifndef PROMPT_INVADERS_GAMESTATE_H
#define PROMPT_INVADERS_GAMESTATE_H

int loadMaxScore();
void saveMaxScore(int maxScore);

int loadLastScore();
void saveLastScore(int lastScore);
void resetLastScore();

#endif
