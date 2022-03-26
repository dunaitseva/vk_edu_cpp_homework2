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

TEST(CheckExtension, InvalidArgumentsEmptyPtr) {
  int res1 = check_extension("some_file.matrix", nullptr);
  ASSERT_EQ(res1, 0);
  int res2 = check_extension(nullptr, MATRIX_EXTENSION);
  ASSERT_EQ(res2, 0);
}

TEST(CheckExtension, InvalidArgumentsWrongExtension) {
  int res1 = check_extension("some_file.matrix", ".txt");
  ASSERT_EQ(res1, 0);
  int res2 = check_extension("some_file", MATRIX_EXTENSION);
  ASSERT_EQ(res2, 0);
  int res3 = check_extension("some_file.txt", MATRIX_EXTENSION);
  ASSERT_EQ(res3, 0);
}
