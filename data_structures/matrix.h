#ifndef PROMPT_INVADERS_MATRIX_H
#define PROMPT_INVADERS_MATRIX_H

#define MAX_ROWS 20
#define MAX_COLS 78

typedef struct {
    char data[MAX_ROWS][MAX_COLS];
    int rows;
    int cols;
} Matrix;

void init_matrix(Matrix *matrix, int rows, int cols);
void draw_element(Matrix *matrix, int row, int col, char element);
void print_matrix(Matrix *matrix);

#endif
