#include "include/matrix.h"
#include "include/utils.h"

matrix_t *transpose(matrix_t *matrix, int *status) {
  if (matrix == NULL) {
	set_status(status, EPTR);
	return NULL;
  }

  matrix_t *transposed = create_matrix(matrix->cols, matrix->rows, NULL);
  if (transposed == NULL) {
	set_status(status, EALLOC);
	return NULL;
  }

  for (size_t i = 0; i < matrix->rows; ++i) {
	for (size_t j = 0; j < matrix->cols; ++j) {
	  matrix_set_val(transposed, matrix_get_val(matrix, i, j, NULL), j, i);
	}
  }

  set_status(status, OK);
  return transposed;
}
