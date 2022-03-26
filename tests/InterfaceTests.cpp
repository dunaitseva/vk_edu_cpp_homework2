#include <cstdio>

extern "C" {
#include "include/utils.h"
}

#include "gtest/gtest.h"

TEST(CheckExtension, ValidArguments) {
  int res1 = check_extension("some_file.matrix", MATRIX_EXTENSION);
  ASSERT_NE(res1, 0);
  int res2 = check_extension("path/to/some_file.matrix", MATRIX_EXTENSION);
  ASSERT_NE(res2, 0);
}
