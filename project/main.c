#include <stdio.h>

#include "include/matrix.h"
#include "include/utils.h"

#define REQ_ARGS 3
#define ERR_USAGE_MSG \
  "Usage: two command line argument file with input matrix and output file."
#define EXT_ERR_MSG "Wrong file extension. Expected '.matrix'."
#define IN_OPEN_ERR_MSG "Error of opening input matrix file."
#define OUT_OPEN_ERR_MSG "Error of opening output matrix file."
#define MATRIX_READ_ERR_MSG "Matrix reading error. Check matrix format."
#define TRANSPOSE_ERR_MSG "Matrix transpose error."
#define WRITE_ERR_MSG "Matrix write error."
#define INPUT_FILE 1
#define OUTPUT_FILE 2

void free_res(FILE *input, FILE *output, matrix_t *matrix,
              matrix_t *transposed) {
  if (input != NULL) {
    fclose(input);
  }

  if (output != NULL) {
    fclose(output);
  }

  if (matrix != NULL) {
    delete_matrix(matrix);
  }

  if (transposed != NULL) {
    delete_matrix(transposed);
  }
}

int main(int argc, char *argv[]) {
  if (argc != REQ_ARGS) {
    fprintf(stderr, "%s\n", ERR_USAGE_MSG);
    return -1;
  }

  if (!check_extension(argv[INPUT_FILE], MATRIX_EXTENSION) ||
      !check_extension(argv[OUTPUT_FILE], MATRIX_EXTENSION)) {
    fprintf(stderr, "%s\n", EXT_ERR_MSG);
    return -1;
  }

  FILE *input = NULL;
  FILE *output = NULL;
  matrix_t *matrix = NULL;
  matrix_t *transposed_matrix = NULL;

  input = fopen(argv[INPUT_FILE], "r");
  if (input == NULL) {
    free_res(input, output, matrix, transposed_matrix);
    fprintf(stderr, "%s\n", IN_OPEN_ERR_MSG);
    return -1;
  }

  // Here I haven't to check status, because only fatal
  // errors possible here
  matrix = read_matrix(input, NULL);
  if (matrix == NULL) {
    free_res(input, output, matrix, transposed_matrix);
    fprintf(stderr, "%s\n", MATRIX_READ_ERR_MSG);
    return -1;
  }

  transposed_matrix = transpose(matrix, NULL);
  if (transposed_matrix == NULL) {
    free_res(input, output, matrix, transposed_matrix);
    fprintf(stderr, "%s\n", TRANSPOSE_ERR_MSG);
    return -1;
  }

  output = fopen(argv[INPUT_FILE], "w");
  if (output == NULL) {
    free_res(input, output, matrix, transposed_matrix);
    fprintf(stderr, "%s\n", OUT_OPEN_ERR_MSG);
    return -1;
  }

  int write_status = write_matrix(output, transposed_matrix);
  if (write_status != OK) {
    free_res(input, output, matrix, transposed_matrix);
    fprintf(stderr, "%s\n", WRITE_ERR_MSG);
    return -1;
  }

  free_res(input, output, matrix, transposed_matrix);
  return 0;
}
