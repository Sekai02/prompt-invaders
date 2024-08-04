#include "matrix.h"

#include <stdio.h>

void init_matrix(Matrix *matrix, int rows, int cols) {
    matrix->rows = rows;
    matrix->cols = cols;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix->data[i][j] = ' ';
        }
    }
}

void draw_element(Matrix *matrix, int row, int col, char element) {
    if (row >= 0 && row < matrix->rows && col >= 0 && col < matrix->cols) {
        matrix->data[row][col] = element;
    }
}

void print_matrix(Matrix *matrix) {
    for (int i = 0; i < matrix->rows; i++) {
        for (int j = 0; j < matrix->cols; j++) {
            printf("%c", matrix->data[i][j]);
        }
        printf("\n");
    }
}