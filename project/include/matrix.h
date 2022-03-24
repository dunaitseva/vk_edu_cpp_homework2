#ifndef PROJECT_INCLUDE_MATRIX_H_
#define PROJECT_INCLUDE_MATRIX_H_

#include <stddef.h>
#include <stdio.h>

typedef struct {
  size_t rows;
  size_t cols;

  double *data;
} matrix_t;

enum supported_matrix_errors {
  ESIZE,    // Error in matrix size
  ESTREAM,  // Error with matrix reading/writing stream
  EALLOC,   // Error in allocating memory for matrix
};

int matrix_set_val(double val, size_t col, size_t row);
double matrix_get_val(size_t col, size_t row, int *err);

matrix_t *create_matrix(size_t cols, size_t rows);
matrix_t *read_matrix(FILE *input);
matrix_t *write_matrix(FILE *output, matrix_t *matrix);

void delete_matrix(matrix_t *matrix);
void transpose(matrix_t *matrix);

#endif  // PROJECT_INCLUDE_MATRIX_H_
