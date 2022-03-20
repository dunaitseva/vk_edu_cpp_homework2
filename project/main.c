#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <inttypes.h>
#include <string.h>

typedef int T;

typedef struct {
  size_t rows;
  size_t cols;
  T *data;
} matrix_t;

void matrix_set_val(matrix_t *matrix, size_t row, size_t col, T val) {
  if (matrix == NULL) {
	return;
  }

  if (row >= matrix->rows || col >= matrix->cols) {
	return;
  }

  matrix->data[row * matrix->cols + col] = val;
}

T matrix_get_val(matrix_t *matrix, size_t row, size_t col) {
  if (matrix == NULL) {
	return -1;
  }

  if (row >= matrix->rows || col >= matrix->cols) {
	return -1;
  }

  return matrix->data[row * matrix->cols + col];
}

void transpose(matrix_t *matrix) {
  if (matrix == NULL) {
	return;
  }

  T *new_data = (T *)calloc(matrix->rows * matrix->cols, sizeof(T));
  if (new_data == NULL) {
	return;
  }

  for (size_t i = 0; i < matrix->rows; ++i) {
	for (size_t j = 0; j < matrix->cols; ++j) {
	  new_data[j * matrix->rows + i] = matrix->data[i * matrix->cols + j];
	}
  }

  free(matrix->data);
  matrix->data = new_data;

  size_t tmp = matrix->rows;
  matrix->rows = matrix->cols;
  matrix->cols = tmp;
}

int main() {
  size_t rows = 100;
  size_t cols = 50;

  matrix_t m;
  m.rows = rows;
  m.cols = cols;
  m.data = (T *)calloc(m.rows * m.cols, sizeof(T));
  if (m.data == NULL) {
	return -1;
  }

  for (size_t i = 0; i < rows; ++i) {
	for (size_t j = 0; j < cols; ++j) {
	  matrix_set_val(&m, i, j, i + j);
	}
  }

  printf("Source\n");
  for (size_t i = 0; i < rows; ++i) {
	for (size_t j = 0; j < cols; ++j) {
	  printf("%3d ", matrix_get_val(&m, i, j));
	}
	printf("\n");
  }

  printf("\n%lu %lu\n", m.rows, m.cols);
//  transpose(&m);
//  printf("%lu %lu\n", m.rows, m.cols);
//  printf("Transposed\n");
//  for (size_t i = 0; i < m.rows; ++i) {
//	for (size_t j = 0; j < m.cols; ++j) {
//	  printf("%3d ", matrix_get_val(&m, i, j));
//	}
//	printf("\n");
//  }

  free(m.data);
  return 0;
}
