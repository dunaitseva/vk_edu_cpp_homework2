#include "include/matrix.h"

#include <inttypes.h>
#include <stdlib.h>

int transpose(matrix_t *matrix) {
  if (matrix == NULL) {
	return EPTR;
  }

  int_fast8_t *processed = (int_fast8_t*)calloc(matrix->rows * matrix->cols, sizeof(int_fast8_t));
  if (processed == NULL) {
	return EALLOC;
  }

  for (size_t i = 0; i < matrix->rows; ++i) {
	for (size_t j = 0; j < matrix->cols; ++j) {
	  if (!(processed[i * matrix->cols + j])) {
		double tmp = matrix->data[i * matrix->cols + j];
		matrix->data[i * matrix->cols + j] = matrix->data[j * matrix->rows + i];
		matrix->data[j * matrix->rows + i] = tmp;
		processed[i * matrix->cols + j] = TRUE;
		processed[j * matrix->rows + i] = TRUE;
	  }
	}
  }

  size_t tmp = matrix->rows;
  matrix->rows = matrix->cols;
  matrix->cols = tmp;

  free(processed);
  return OK;
}

