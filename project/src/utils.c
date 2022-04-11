#include "utils.h"

#include <string.h>

const char *get_filename_extension(const char *file_name) {
  const char *dot = strrchr(file_name, '.');
  if (!dot || dot == file_name) {
    return NULL;
  }
  return dot + 1;
}

int check_extension(const char *file_name, const char *expected_extension) {
  if (file_name == NULL) {
    return 0;
  }

  if (expected_extension == NULL) {
    return 0;
  }

  const char *extension = get_filename_extension(file_name);
  if (extension == NULL) {
    return 0;
  }

  return strcmp(extension, expected_extension) == 0;
}

void set_status(int *status, int status_code) {
  if (status != NULL) {
    *status = status_code;
  }
}
