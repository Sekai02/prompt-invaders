#include <ncurses.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

#include "../system/gamestate.h"
#include "../data_structures/memory.h"

#define PLAYER_START_X 40
#define PLAYER_START_Y 20

int playerX = PLAYER_START_X;
int playerY = PLAYER_START_Y;
int isGameOver = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

#define MONSTER_WAVE_ROWS 5
#define MONSTER_WAVE_COLUMNS 10
#define MONSTER_WAVE_COOLDOWN 500000 // 2 seconds
#define MONSTER_SPEED 1000000
#define MONSTER_BASE_X (COLS / 2 - MONSTER_WAVE_COLUMNS)
#define MONSTER_BASE_Y 0
#define MONSTER_SCORE 100
#define MONSTER_SHOT_SPEED (MONSTER_SPEED / 3)
#define ASTEROID_SPEED 1000000 / 3
#define MAX_MONSTER_SHOTS 100

// this represents the memory of the game, each time an element of the game is created will reserve
// a slot in the memory. This is for demostrate memory management algorithms and the use of a free lists
// 0 -> monsters
// 1 -> shots
// 2 -> monsterShots
// 3 -> asteroids
bool GameMemory[250][4];

typedef struct MonsterNode
{
    int x, y;
    int isAlive;
    struct MonsterNode *next;
} MonsterNode;

MonsterNode *monsters = NULL;

void generateMonsterWave();
void *handleMonsters(void *arg);
void updateMonsterShots();
void *handleMonsterShots(void *arg);

typedef struct ShotNode
{
    int x, y;
    int collided;
    struct ShotNode *next;
} ShotNode;

ShotNode *shots = NULL;

typedef struct MonsterShotNode
{
    int x, y;
    int priority;
    struct MonsterShotNode *next;
} MonsterShotNode;

MonsterShotNode *monsterShots = NULL;

typedef struct AsteroidNode
{
    int pos_x, pos_y, vel_x, vel_y, size, index;
    bool frame;
    struct AsteroidNode *next;
} AsteroidNode;

AsteroidNode *asteroids = NULL;
freeListNode *asteroids_freeList;

void *handleAsteroids(void *arg);

#define SHOT_SPEED 50000

void fire_shot();
void *handlePlayerMovement(void *arg);
void updateShots();
void *handleShots(void *arg);

#define GAME_SPEED 100000

int score = 0;
int maxScore = 0;

#define COLLISION_DETECTION_SPEED 10000
#define DRAWING_SPEED 10000

void *handleCollisions(void *arg);
void *handleDrawing(void *arg);
void freeShots();
void freeMonsters();
void freeMonsterShots();
void freeAsteroids();
void initializeFreeList(freeListNode *freeList, int size);
void destroyFreeList(freeListNode *freeList);

void runGame()
{
    initscr();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);

    isGameOver = 0;
    score = 0;
    maxScore = loadMaxScore();

    pthread_t drawingThread;
    pthread_t movementThread;
    pthread_t shootingThread;
    pthread_t monsterThread;
    pthread_t monsterShootingThread;
    pthread_t collisionThread;
    pthread_t asteroidThread;
    pthread_create(&drawingThread, NULL, handleDrawing, NULL);
    pthread_create(&movementThread, NULL, handlePlayerMovement, NULL);
    pthread_create(&shootingThread, NULL, handleShots, NULL);
    pthread_create(&monsterThread, NULL, handleMonsters, NULL);
    pthread_create(&monsterShootingThread, NULL, handleMonsterShots, NULL);
    pthread_create(&collisionThread, NULL, handleCollisions, NULL);
    pthread_create(&asteroidThread, NULL, handleAsteroids, NULL);

    asteroids_freeList = (freeListNode *)malloc(sizeof(freeListNode));
    initializeFreeList(asteroids_freeList, 250);

    while (!isGameOver)
    {
        pthread_mutex_lock(&mutex);
        pthread_mutex_unlock(&mutex);
        usleep(GAME_SPEED); // Sleep for 0.1 seconds to control the game speed
    }

    endwin();
    pthread_join(movementThread, NULL);
    pthread_join(shootingThread, NULL);
    pthread_join(monsterThread, NULL);
    pthread_join(collisionThread, NULL);
    pthread_join(asteroidThread, NULL);
    pthread_join(drawingThread, NULL);

    // Free linked lists
    freeShots();
    freeMonsters();
    freeMonsterShots();
    freeAsteroids();
    destroyFreeList(asteroids_freeList);

    saveMaxScore(maxScore);
    saveLastScore(score);
}

void generateMonsterWave()
{
    pthread_mutex_lock(&mutex);
    for (int i = 0; i < MONSTER_WAVE_ROWS; i++)
    {
        for (int j = 0; j < MONSTER_WAVE_COLUMNS; j++)
        {
            MonsterNode *newMonster = (MonsterNode *)malloc(sizeof(MonsterNode));
            newMonster->x = MONSTER_BASE_X + j * 2; // spacing between monsters
            newMonster->y = MONSTER_BASE_Y + i;
            newMonster->isAlive = 1;
            newMonster->next = monsters;
            monsters = newMonster;
        }
    }
    pthread_mutex_unlock(&mutex);
}

void *handleMonsters(void *arg)
{
    int waveCount = 0;

    int dirIdx = 0;
    int dirx[11] = {1, 1, 1, 1, 1, 0, -1, -1, -1, -1, -1};
    int diry[11] = {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0};
    int dirlen = sizeof(dirx) / sizeof(dirx[0]);

    while (!isGameOver)
    {
        // Wait for the previous wave to finish
        pthread_mutex_lock(&mutex);
        int allDead = 1;

        MonsterNode *current = monsters;
        while (current != NULL)
        {
            if (current->isAlive)
            {
                allDead = 0;
                break;
            }
            current = current->next;
        }
        pthread_mutex_unlock(&mutex);

        if (allDead)
        {
            // Wait for the cooldown period
            usleep(MONSTER_WAVE_COOLDOWN);

            // Generate a new wave
            generateMonsterWave();
            waveCount++;
        }

        // Move the monsters
        pthread_mutex_lock(&mutex);
        srand((time(NULL)));
        current = monsters;
        while (current != NULL)
        {
            if (current->isAlive)
            {
                int newX = current->x + dirx[dirIdx];
                int newY = current->y + diry[dirIdx];
                current->x = newX;
                current->y = newY;

                // Check if the monster should shoot
                if (rand() % 100 < 1)
                { // 1% chance of shooting
                    MonsterShotNode *newShot = (MonsterShotNode *)malloc(sizeof(MonsterShotNode));
                    newShot->x = current->x;
                    newShot->y = current->y + 1;
                    newShot->priority = 1 / (abs(playerX - newShot->x) + abs(playerY - newShot->y)); // calculate priority
                    newShot->next = monsterShots;
                    monsterShots = newShot;
                }
            }
            current = current->next;
        }

        dirIdx = (dirIdx + 1) % dirlen;

        pthread_mutex_unlock(&mutex);

        usleep(MONSTER_SPEED);
    }
    return NULL;
}

void fire_shot()
{
    ShotNode *newShot = (ShotNode *)malloc(sizeof(ShotNode));
    newShot->x = playerX;
    newShot->y = playerY - 1;
    newShot->next = shots;
    shots = newShot;
}

void *handlePlayerMovement(void *arg)
{
    int ch;
    while (!isGameOver)
    {
        pthread_mutex_lock(&mutex);
        ch = getch();
        pthread_mutex_unlock(&mutex);

        switch (ch)
        {
        case KEY_LEFT:
            if (playerX > 0)
            {
                playerX--;
            }
            break;
        case KEY_RIGHT:
            if (playerX < COLS - 1)
            {
                playerX++;
            }
            break;
        case ' ':
            beep();
            fire_shot();
            break;
        default:
            break;
        }
    }
    return NULL;
}

void updateShots()
{
    pthread_mutex_lock(&mutex);
    ShotNode *current = shots;
    ShotNode *prev = NULL;
    while (current != NULL)
    {
        current->y--;
        if (current->y < 0)
        {
            ShotNode *temp = current;
            if (prev == NULL)
            {
                shots = current->next;
            }
            else
            {
                prev->next = current->next;
            }
            current = current->next;
            if (temp != NULL)
                free(temp);
        }
        else
        {
            prev = current;
            current = current->next;
        }
    }
    pthread_mutex_unlock(&mutex);
}

void *handleShots(void *arg)
{
    while (!isGameOver)
    {
        updateShots();
        usleep(SHOT_SPEED); // Adjust this value to control the shot speed
    }
    return NULL;
}

// Check for collisions between shots and monsters
void *handleCollisions(void *arg)
{
    while (!isGameOver)
    {
        pthread_mutex_lock(&mutex);

        ShotNode *currentShot = shots;
        while (currentShot != NULL)
        {
            MonsterNode *currentMonster = monsters;
            while (currentMonster != NULL)
            {
                if (currentShot->x == currentMonster->x && currentShot->y == currentMonster->y && currentMonster->isAlive)
                {
                    // Collision detected, kill the monster and remove the shot
                    currentMonster->isAlive = 0;
                    score += MONSTER_SCORE;
                    if (score > maxScore)
                    {
                        maxScore = score;
                    }

                    // Remove the shot
                    ShotNode *temp = currentShot;
                    if (currentShot == shots)
                    {
                        shots = currentShot->next;
                    }
                    else
                    {
                        ShotNode *prevShot = shots;
                        while (prevShot->next != currentShot)
                        {
                            prevShot = prevShot->next;
                        }
                        prevShot->next = currentShot->next;
                    }
                    if (temp != NULL)
                        free(temp);

                    // Break out of the inner loop to move on to the next shot
                    break;
                }
                currentMonster = currentMonster->next;
            }
            currentShot = currentShot->next;
        }

        // Check for collisions between monsters and the player
        MonsterNode *currentMonster = monsters;
        while (currentMonster != NULL)
        {
            if (currentMonster->x == playerX && currentMonster->y == playerY && currentMonster->isAlive)
            {
                // Collision detected, game over
                isGameOver = 1;
                break;
            }
            currentMonster = currentMonster->next;
        }

        // Check for collisions between asteroids and the player
        AsteroidNode *currentAsteroid = asteroids;
        while (currentAsteroid != NULL)
        {
            if (currentAsteroid->pos_x <= playerX && playerX < currentAsteroid->pos_x + currentAsteroid->size &&
                currentAsteroid->pos_y <= playerY && playerY < currentAsteroid->pos_y + currentAsteroid->size)
            {
                // Collision detected, game over
                isGameOver = 1;
                break;
            }
            currentAsteroid = currentAsteroid->next;
        }

        // Check for collisions between monsters shots and the player
        MonsterShotNode *currentMonsterShot = monsterShots;
        while (currentMonsterShot != NULL)
        {
            if (currentMonsterShot->x == playerX && currentMonsterShot->y == playerY)
            {
                // Collision detected, game over
                isGameOver = 1;
                break;
            }
            currentMonsterShot = currentMonsterShot->next;
        }

        pthread_mutex_unlock(&mutex);

        usleep(COLLISION_DETECTION_SPEED); // Adjust this value to control the collision detection speed
    }
    return NULL;
}

void drawAsteroid(AsteroidNode *asteroid)
{
    if (asteroid->size == 1)
    {
        if (asteroid->frame)
        {
            mvprintw(asteroid->pos_y, asteroid->pos_x, "=");
        }
        else
        {
            mvprintw(asteroid->pos_y, asteroid->pos_x, "#");
        }
    }
    else if (asteroid->size == 2)
    {
        if (asteroid->frame)
        {
            mvprintw(asteroid->pos_y, asteroid->pos_x, "/-");
            mvprintw(asteroid->pos_y + 1, asteroid->pos_x, "-/");
        }
        else
        {
            mvprintw(asteroid->pos_y, asteroid->pos_x, "-\\");
            mvprintw(asteroid->pos_y + 1, asteroid->pos_x, "\\-");
        }
    }
    else if (asteroid->size == 3)
    {
        if (asteroid->frame == 0)
        {
            mvprintw(asteroid->pos_y, asteroid->pos_x, "·-\\");
            mvprintw(asteroid->pos_y + 1, asteroid->pos_x, "| |");
            mvprintw(asteroid->pos_y + 2, asteroid->pos_x, "\\-·");
        }
        else
        {
            mvprintw(asteroid->pos_y, asteroid->pos_x, "/-·");
            mvprintw(asteroid->pos_y + 1, asteroid->pos_x, "| ·");
            mvprintw(asteroid->pos_y + 2, asteroid->pos_x, "·-/");
        }
    }
}

void *handleDrawing(void *arg)
{
    while (!isGameOver)
    {
        pthread_mutex_lock(&mutex);

        // Clear the screen
        clear();

        // Draw the asteroids
        AsteroidNode *currentAsteroid = asteroids;
        while (currentAsteroid != NULL)
        {
            drawAsteroid(currentAsteroid);
            currentAsteroid = currentAsteroid->next;
        }

        // Draw the monster shots
        MonsterShotNode *currentMonsterShot = monsterShots;
        while (currentMonsterShot != NULL)
        {
            mvprintw(currentMonsterShot->y, currentMonsterShot->x, "o");
            currentMonsterShot = currentMonsterShot->next;
        }

        // Draw the player
        mvprintw(playerY, playerX, "A");

        // Draw the monsters
        MonsterNode *currentMonster = monsters;
        while (currentMonster != NULL)
        {
            if (currentMonster->isAlive)
            {
                mvprintw(currentMonster->y, currentMonster->x, "M");
            }
            currentMonster = currentMonster->next;
        }

        // Draw the shots
        ShotNode *currentShot = shots;
        while (currentShot != NULL)
        {
            if (currentShot->x >= 0 && currentShot->x < COLS && currentShot->y >= 0 && currentShot->y < LINES)
                mvprintw(currentShot->y, currentShot->x, "|");
            currentShot = currentShot->next;
        }

        // Draw the score
        mvprintw(0, 0, "Score: %d", score);
        mvprintw(1, 0, "Max Score: %d", maxScore);

        refresh();

        pthread_mutex_unlock(&mutex);

        usleep(DRAWING_SPEED); // Adjust this value to control the drawing speed
    }
    return NULL;
}

void updateMonsterShots()
{
    pthread_mutex_lock(&mutex);
    MonsterShotNode *current = monsterShots;
    MonsterShotNode *priorityQueue[MAX_MONSTER_SHOTS]; // priority queue
    int queueSize = 0;

    // Add monster shots to the priority queue
    while (current != NULL)
    {
        priorityQueue[queueSize] = current;
        queueSize++;
        current = current->next;
    }

    // Sort the priority queue
    for (int i = 0; i < queueSize - 1; i++)
    {
        for (int j = i + 1; j < queueSize; j++)
        {
            if (priorityQueue[i]->priority < priorityQueue[j]->priority)
            {
                MonsterShotNode *temp = priorityQueue[i];
                priorityQueue[i] = priorityQueue[j];
                priorityQueue[j] = temp;
            }
        }
    }

    // Move the monster shots in the priority queue
    for (int i = 0; i < queueSize; i++)
    {
        MonsterShotNode *shot = priorityQueue[i];
        shot->y++;
        if (shot->y >= LINES)
        {
            // Remove the shot from the queue
            MonsterShotNode *prev = NULL;
            MonsterShotNode *temp = monsterShots;
            while (temp != NULL)
            {
                if (temp == shot)
                {
                    if (prev == NULL)
                    {
                        monsterShots = temp->next;
                    }
                    else
                    {
                        prev->next = temp->next;
                    }
                    if (temp != NULL)
                        free(temp);
                    break;
                }
                prev = temp;
                temp = temp->next;
            }
        }
    }

    pthread_mutex_unlock(&mutex);
}

void *handleMonsterShots(void *arg)
{
    while (!isGameOver)
    {
        updateMonsterShots();
        usleep(MONSTER_SHOT_SPEED); // Adjust this value to control the monster shot speed
    }
    return NULL;
}

void generateNewAsteroid()
{
    int asteroidSize = rand() % 3 + 1; // Random size between 1 and 3
    int memoryIndex = reserveMemoryBlock(&asteroids_freeList, asteroidSize);
    if (memoryIndex < 0)
        return; // No memory available
    AsteroidNode *newAsteroid = (AsteroidNode *)malloc(sizeof(AsteroidNode));
    int border = rand() % 4;
    switch (border)
    {
    case 0:                                              // top border
        newAsteroid->pos_x = rand() % COLS;              // Random x position
        newAsteroid->pos_y = 0;                          // Start at the top of the screen
        newAsteroid->vel_x = (rand() % 2 == 0) ? 1 : -1; // Random x velocity direction
        newAsteroid->vel_y = 1;                          // Move downwards
        break;
    case 1:                                              // right border
        newAsteroid->pos_x = COLS - 1;                   // Start at the rightmost column
        newAsteroid->pos_y = rand() % LINES;             // Random y position
        newAsteroid->vel_x = -1;                         // Move leftwards
        newAsteroid->vel_y = (rand() % 2 == 0) ? 1 : -1; // Random y velocity direction
        break;
    case 2:                                 // bottom border
        newAsteroid->pos_x = rand() % COLS; // Random x position
        if (newAsteroid->pos_x == playerX && newAsteroid->pos_y == playerY)
            newAsteroid->pos_x += 15;                    // avoids generating an asteroid right on the player
        newAsteroid->pos_y = LINES - 1;                  // Start at the bottom of the screen
        newAsteroid->vel_x = (rand() % 2 == 0) ? 1 : -1; // Random x velocity direction
        newAsteroid->vel_y = -1;                         // Move upwards
        break;
    case 3:                                              // left border
        newAsteroid->pos_x = 0;                          // Start at the leftmost column
        newAsteroid->pos_y = rand() % LINES;             // Random y position
        newAsteroid->vel_x = 1;                          // Move rightwards
        newAsteroid->vel_y = (rand() % 2 == 0) ? 1 : -1; // Random y velocity direction
        break;
    default:
        break;
    }
    newAsteroid->size = asteroidSize;
    newAsteroid->frame = 0;
    newAsteroid->next = asteroids;
    newAsteroid->index = memoryIndex;
    GameMemory[memoryIndex][3] = true;
    asteroids = newAsteroid;
}

void updateAsteroids()
{
    pthread_mutex_lock(&mutex);
    AsteroidNode *prev = NULL;
    AsteroidNode *current = asteroids;
    while (current != NULL)
    {
        current->pos_x += current->vel_x;
        current->pos_y += current->vel_y;
        current->frame = !current->frame;
        // Remove the asteroid if it reaches one of the borders
        if (current->pos_x < 0 || current->pos_x >= COLS || current->pos_y < 0 || current->pos_y >= LINES)
        {
            AsteroidNode *temp = current;
            if (prev == NULL)
            {
                asteroids = current->next;
            }
            else
            {
                prev->next = current->next;
            }
            current = current->next;
            if (temp != NULL)
            {
                GameMemory[temp->index][3] = false;
                freeMemoryBlock(&asteroids_freeList, temp->index, temp->size);
                free(temp);
            }
        }
        else
        {
            prev = current;
            current = current->next;
        }
    }
    // Add a new asteroid with a 25% probability
    int random = rand() % 4;
    if (random == 0)
    {
        generateNewAsteroid();
    }
    pthread_mutex_unlock(&mutex);
}

void *handleAsteroids(void *arg)
{
    while (!isGameOver)
    {
        updateAsteroids();
        usleep(ASTEROID_SPEED); // Adjust this value to control the asteroid speed
    }
    return NULL;
}
// Helper functions to free memory
void freeShots()
{
    ShotNode *current = shots;
    while (current != NULL)
    {
        ShotNode *next = current->next;
        free(current);
        current = next;
    }
    shots = NULL;
}

void freeMonsters()
{
    MonsterNode *current = monsters;
    while (current != NULL)
    {
        MonsterNode *next = current->next;
        free(current);
        current = next;
    }
    monsters = NULL;
}

void freeMonsterShots()
{
    MonsterShotNode *current = monsterShots;
    while (current != NULL)
    {
        MonsterShotNode *next = current->next;
        free(current);
        current = next;
    }
    monsterShots = NULL;
}

void freeAsteroids()
{
    AsteroidNode *current = asteroids;
    while (current != NULL)
    {
        AsteroidNode *next = current->next;
        free(current);
        current = next;
    }
    asteroids = NULL;
}

void initializeFreeList(freeListNode *freeList, int size)
{
    freeList->start = 0;
    freeList->size = size;
    freeList->next = NULL;
}

void destroyFreeList(freeListNode *freeList)
{
    freeListNode *current = freeList;
    while (current != NULL)
    {
        freeListNode *next = current->next;
        free(current);
        current = next;
    }
}