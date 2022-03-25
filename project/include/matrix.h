#ifndef PROJECT_INCLUDE_MATRIX_H_
#define PROJECT_INCLUDE_MATRIX_H_

#include <stddef.h>
#include <stdio.h>

#define DEFAULT_DOUBLE_RET_VAL 0.0

#define INPUT_DOUBLE_SIZE 21
#define INPUT_DOUBLE_PARAMETERS_AMOUNT 1
#define FORMAT_DOUBLE_INPUT "%20[.0-9e-]%*[ \n\t]"

#define INPUT_SIZE_SIZE 11
#define INPUT_SIZE_PARAMETERS_AMOUNT 2
#define SIZE_INPUT_BASE 10
#define FORMAT_SIZE_INPUT "%10[0-9]%*[ ]%10[0-9]\n"

typedef struct {
  size_t rows;
  size_t cols;

  double *data;
} matrix_t;

enum supported_matrix_errors {
  OK = 1,
  EPTR,     // Invalid pointer passed
  ESIZE,    // Error in matrix size
  ESTREAM,  // Error with matrix reading/writing stream
  EREAD,    // WRONG INPUT
  EALLOC,   // Error in allocating memory for matrix
};

int matrix_set_val(matrix_t *matrix, double val, size_t row, size_t col);
double matrix_get_val(matrix_t *matrix, size_t row, size_t col, int *status);

matrix_t *create_matrix(size_t rows, size_t cols, int *status);
int delete_matrix(matrix_t *matrix);

matrix_t *read_matrix(FILE *input, int *status);
int write_matrix(FILE *output, matrix_t *matrix);

void transpose(matrix_t *matrix);

#endif  // PROJECT_INCLUDE_MATRIX_H_
