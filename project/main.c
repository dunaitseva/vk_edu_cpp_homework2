#include "include/matrix.h"

#include <stdio.h>

int main() {
  matrix_t *matrix = read_matrix(stdin, NULL);
  write_matrix(stdout, matrix);
  transpose(matrix);
  write_matrix(stdout, matrix);
  return 0;
}
