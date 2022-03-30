#include <stdio.h>

#include "include/interface.h"
#include "include/matrix.h"
#include "include/utils.h"

#define REQ_ARGS 3
#define ERR_USAGE_MSG \
  "Usage: two command line argument file with input matrix and output file."
#define EXT_ERR_MSG "Wrong file extension. Expected '.matrix'."
#define OPEN_ERR_MSG "Error of performing file open."
#define FATAL_ERR_MSG \
  "Fatal error occur while matrix performing. Transposition impossible."
#define INPUT_FILE 1
#define OUTPUT_FILE 2

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
  int err_open =
      open_streams(argv[INPUT_FILE], &input, argv[OUTPUT_FILE], &output);
  if (err_open != OK_INTERFACE) {
    fprintf(stderr, "%s\n", OPEN_ERR_MSG);
    return -1;
  }

  int err_perform = perform_matrix_operations(input, output);
  if (err_perform != OK_INTERFACE) {
    fclose(input);
    fclose(output);
    fprintf(stderr, "%s\n", FATAL_ERR_MSG);
    return -1;
  }

  fclose(input);
  fclose(output);
  return 0;
}
