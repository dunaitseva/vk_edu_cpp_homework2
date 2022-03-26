#include "include/matrix.h"

#include <stdlib.h>
#include <string.h>

#include "include/utils.h"

static int read_size(FILE *input, size_t *rows, size_t *cols);
static int read_float(FILE *input, double *val);

int matrix_set_val(matrix_t *matrix, double val, size_t row, size_t col) {
  if (matrix == NULL) {
    return EPTR;
  }

  if (row >= matrix->rows || col >= matrix->cols) {
    return ESIZE;
  }

  matrix->data[row * matrix->cols + col] = val;
  return OK;
}

double matrix_get_val(matrix_t *matrix, size_t row, size_t col, int *status) {
  if (matrix == NULL) {
    set_status(status, EPTR);
    return DEFAULT_DOUBLE_RET_VAL;
  }

  if (row >= matrix->rows || col >= matrix->cols) {
    set_status(status, ESIZE);
    return DEFAULT_DOUBLE_RET_VAL;
  }

  set_status(status, OK);
  return matrix->data[row * matrix->cols + col];
}

matrix_t *create_matrix(size_t rows, size_t cols, int *status) {
  matrix_t *target = (matrix_t *)malloc(sizeof(matrix_t));
  if (target == NULL) {
    set_status(status, EALLOC);
    return NULL;
  }

  target->data = (double *)calloc(cols * rows, sizeof(double));
  if (target->data == NULL) {
    set_status(status, EALLOC);
    free(target);
    return NULL;
  }

  memset(target->data, 0, sizeof(double) * rows * cols);
  target->cols = cols;
  target->rows = rows;

  set_status(status, OK);
  return target;
}

int delete_matrix(matrix_t *matrix) {
  // I have to deviate the principle yhat free() follow
  // because in case of matrix_t structure I can have
  // situation, when matrix == NULL and code will try
  // to refer on matrix data.
  if (matrix == NULL) {
    return EPTR;
  }

  free(matrix->data);
  free(matrix);
  return OK;
}

matrix_t *read_matrix(FILE *input, int *status) {
  if (input == NULL) {
    set_status(status, ESTREAM);
    return NULL;
  }

  size_t rows;
  size_t cols;
  if (!read_size(input, &rows, &cols)) {
    set_status(status, EREAD);
    return NULL;
  }

  matrix_t *result_matrix = create_matrix(rows, cols, status);
  if (result_matrix == NULL) {
    return NULL;
  }

  for (size_t i = 0; i < result_matrix->rows; ++i) {
    for (size_t j = 0; j < result_matrix->cols; ++j) {
      double tmp;
      if (!read_float(input, &tmp)) {
        delete_matrix(result_matrix);
        set_status(status, EREAD);
        return NULL;
      }
      matrix_set_val(result_matrix, tmp, i, j);
    }
  }

  set_status(status, OK);
  return result_matrix;
}

int write_matrix(FILE *output, matrix_t *matrix) {
  if (output == NULL) {
    return ESTREAM;
  }

  if (matrix == NULL) {
    return EPTR;
  }

  fprintf(output, "%zu %zu\n", matrix->rows, matrix->cols);
  for (size_t i = 0; i < matrix->rows; ++i) {
    for (size_t j = 0; j < matrix->cols; ++j) {
      double tmp = matrix_get_val(matrix, i, j, NULL);
      fprintf(output, "%lf", tmp);
      if (j != matrix->cols - 1) {
        fprintf(output, " ");
      }
    }
    fprintf(output, "\n");
  }
  return OK;
}

static int read_size(FILE *input, size_t *rows, size_t *cols) {
  // Not required to check passing pointers, because function read_size
  // will not use as interface function of library, and it's impossible
  // to pass pointers with NULL value in function;

  char rows_str_repr[INPUT_SIZE_SIZE];
  memset(rows_str_repr, 0, INPUT_SIZE_SIZE);
  char cols_str_repr[INPUT_SIZE_SIZE];
  memset(cols_str_repr, 0, INPUT_SIZE_SIZE);

  int read = fscanf(input, FORMAT_SIZE_INPUT, rows_str_repr, cols_str_repr);
  if (read < INPUT_SIZE_PARAMETERS_AMOUNT) {
    return 0;
  }

  /* This part means that string performing to float point digit was failed
   according the manual for strtod function. So if  operation was success
   tmp_ptr_<coord>_repr will refer after last byte used in conversion. So
   if all string's bytes of digit was performed, tmp_ptr_<coord>_repr should
   have value 0 or '\0'. In other cases it means that error occurred. */
  char *tmp_rows_str_repr = rows_str_repr;
  char *tmp_cols_str_repr = cols_str_repr;
  *rows = strtoul(rows_str_repr, &tmp_rows_str_repr, SIZE_INPUT_BASE);
  *cols = strtoul(cols_str_repr, &tmp_cols_str_repr, SIZE_INPUT_BASE);
  if (*tmp_rows_str_repr || *tmp_cols_str_repr) {
    return 0;
  }

  return 1;
}

static int read_float(FILE *input, double *val) {
  char float_str_repr[INPUT_DOUBLE_SIZE];
  memset(float_str_repr, 0, INPUT_SIZE_SIZE);

  int read = fscanf(input, FORMAT_DOUBLE_INPUT, float_str_repr);
  if (read < INPUT_DOUBLE_PARAMETERS_AMOUNT) {
    return 0;
  }

  char *tmp_float_str_repr = float_str_repr;
  *val = strtod(float_str_repr, &tmp_float_str_repr);
  if (*tmp_float_str_repr) {
    return 0;
  }

  return 1;
}
