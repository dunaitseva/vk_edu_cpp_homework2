#include "interface.h"

#include <stdio.h>

#include "matrix.h"

int open_streams(const char *filename_input, FILE **input,
                 const char *filename_output, FILE **output) {
  if (input == NULL || output == NULL) {
    return PTR_ERROR;
  }

  *input = fopen(filename_input, "r");
  if (*input == NULL) {
    return OPEN_ERROR;
  }

  *output = fopen(filename_output, "w");
  if (*output == NULL) {
    fclose(*input);
    return OPEN_ERROR;
  }

  return OK_INTERFACE;
}

int perform_matrix_operations(FILE *input, FILE *output) {
  if (input == NULL || output == NULL) {
    return STREAM_ERROR;
  }

  // Here I haven't to check return status type, because
  // only fatal errors possible here.
  matrix_t *matrix = read_matrix(input, NULL);
  if (matrix == NULL) {
    return FATAL_ERROR;
  }

  matrix_t *transposed_matrix = transpose(matrix, NULL);
  if (transposed_matrix == NULL) {
    return FATAL_ERROR;
  }

  int write_status = write_matrix(output, transposed_matrix);
  if (write_status != OK) {
    return FATAL_ERROR;
  }

  return OK_INTERFACE;
}
