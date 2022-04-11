#ifndef PROJECT_INCLUDE_INTERFACE_H_
#define PROJECT_INCLUDE_INTERFACE_H_

#include "stdio.h"

enum {
  OK_INTERFACE,
  PTR_ERROR,
  OPEN_ERROR,
  STREAM_ERROR,
  FATAL_ERROR,
};

int open_streams(const char *filename_input, FILE **input,
                 const char *filename_output, FILE **output);
int perform_matrix_operations(FILE *input, FILE *output);

#endif  // PROJECT_INCLUDE_INTERFACE_H_
