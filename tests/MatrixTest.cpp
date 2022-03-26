#include <cstdio>

#include <stdexcept>
#include <exception>

extern "C" {
#include "include/matrix.h"
}

#include <gtest/gtest.h>

class MatrixSetGet : public ::testing::Test {
 protected:
  static constexpr size_t MATRIX_ROWS = 5;
  static constexpr size_t MATRIX_COLS = 10;

  matrix_t *matrix1;

  void SetUp() {
	matrix1 = create_matrix(MATRIX_ROWS, MATRIX_COLS, nullptr);

	if (matrix1 == nullptr) {
	  throw std::runtime_error("One of pointers is null");
	}
  }

  void TearDown() {
	delete_matrix(matrix1);
  }

};

class MatrixReadWrite : public ::testing::Test {
 protected:
  static constexpr size_t MATRIX_MAX_BUFFER = 4096;
  char valid_input[MATRIX_MAX_BUFFER] = "2 3\n"
										"0.1 -4.3 22.1\n"
										"1.2e-9 -4.3 2.1\n";
  char invalid_input_size[MATRIX_MAX_BUFFER] = "2a 1\n1.2\n3.2\n";
  char invalid_input_value[MATRIX_MAX_BUFFER] = "2 1\n1.2asd\n3.2\n";
  char output_write_buffer[MATRIX_MAX_BUFFER];
  char output_write_buffer_expected[MATRIX_MAX_BUFFER] = "2 3\n"
														 "0.100000 -4.300000 22.100000\n"
														 "0.000000 -4.300000 2.100000\n";

  FILE *valid_input_stream;
  FILE *invalid_input_size_stream;
  FILE *invalid_input_value_stream;
  FILE *output_write_stream;

  void SetUp() {
	valid_input_stream = fmemopen(valid_input, MATRIX_MAX_BUFFER, "r");
	invalid_input_size_stream = fmemopen(invalid_input_size, MATRIX_MAX_BUFFER, "r");
	invalid_input_value_stream = fmemopen(invalid_input_value, MATRIX_MAX_BUFFER, "r");
	output_write_stream = fmemopen(output_write_buffer, MATRIX_MAX_BUFFER, "w");
	if (valid_input_stream == nullptr ||
		invalid_input_size_stream == nullptr ||
		output_write_stream == nullptr ||
		invalid_input_value_stream == nullptr) {
	  throw std::runtime_error("Error opening stream");
	}
  }

  void TearDown() {
	fclose(valid_input_stream);
	fclose(invalid_input_size_stream);
	fclose(invalid_input_value_stream);
	fclose(output_write_stream);
  }
};

class MatrixTranspose : public ::testing::Test {
 protected:
  static constexpr size_t MATRIX_MAX_BUFFER = 512;
  char valid_input[MATRIX_MAX_BUFFER] = "2 3\n"
										"0.1 -4.3 22.1\n"
										"1.2e-9 -4.3 2.1\n";
  char output_write_buffer[MATRIX_MAX_BUFFER];
  char output_write_buffer_expected[MATRIX_MAX_BUFFER] = "3 2\n"
														 "0.100000 0.000000\n"
														 "-4.300000 -4.300000\n"
														 "22.100000 2.100000\n";
  FILE *valid_input_stream;
  FILE *output_write_stream;

  void SetUp() {
	valid_input_stream = fmemopen(valid_input, MATRIX_MAX_BUFFER, "r");
	output_write_stream = fmemopen(output_write_buffer, MATRIX_MAX_BUFFER, "w");
	if (valid_input_stream == nullptr ||
		output_write_stream == nullptr) {
	  throw std::runtime_error("Error opening stream");
	}
  }

  void TearDown() {
	fclose(valid_input_stream);
	fclose(output_write_stream);
  }
};

TEST(CreateMatrix, CreationTest) {
  int status;
  matrix_t *matrix = create_matrix(10, 20, &status);
  ASSERT_NE(matrix, nullptr);
  ASSERT_EQ(status, OK);
  delete_matrix(matrix);
}

TEST(DeleteMatrix, ValidMatrixPointer) {
  int status;
  matrix_t *matrix = create_matrix(10, 20, &status);
  ASSERT_NE(matrix, nullptr);
  EXPECT_EQ(status, OK);
  status = delete_matrix(matrix);
  ASSERT_EQ(status, OK);
}

TEST(DeleteMatrix, InvalidMatrixPointer) {
  int status;
  status = delete_matrix(nullptr);
  ASSERT_EQ(status, EPTR);
}

TEST_F(MatrixSetGet, ValidValuesGet) {
  matrix1->data[41] = 3.0;
  int status;
  double result = matrix_get_val(matrix1, 4, 1, &status);
  EXPECT_EQ(status, OK);
  ASSERT_FLOAT_EQ(result, 3.0);
}

TEST_F(MatrixSetGet, InvalidValuesGetPointer) {
  int status;
  matrix_get_val(nullptr, 1, 0, &status);
  ASSERT_EQ(status, EPTR);
}

TEST_F(MatrixSetGet, InvalidValuesGetSizeLess) {
  int status;
  matrix_get_val(matrix1, -1, 0, &status);
  ASSERT_EQ(status, ESIZE);
  matrix_get_val(matrix1, 0, -1, &status);
  ASSERT_EQ(status, ESIZE);
}

TEST_F(MatrixSetGet, InvalidValuesGetSizeEqual) {
  int status;
  matrix_get_val(matrix1, MATRIX_ROWS, 0, &status);
  ASSERT_EQ(status, ESIZE);
  matrix_get_val(matrix1, 0, MATRIX_COLS, &status);
  ASSERT_EQ(status, ESIZE);
}

TEST_F(MatrixSetGet, ValidValuesSet) {
  int status = matrix_set_val(matrix1, 1.0, 0, 0);
  ASSERT_EQ(status, OK);
}

TEST_F(MatrixSetGet, InvalidValuesSetPointer) {
  int status = matrix_set_val(nullptr, 1.0, 0, 0);
  ASSERT_EQ(status, EPTR);
}

TEST_F(MatrixSetGet, InvalidValuesSetSizeLess) {
  int status = matrix_set_val(matrix1, 1.0, -1, 0);
  ASSERT_EQ(status, ESIZE);
  status = matrix_set_val(matrix1, 1.0, 0, -1);
  ASSERT_EQ(status, ESIZE);
}

TEST_F(MatrixSetGet, InvalidValuesSetSizeEqual) {
  int status = matrix_set_val(matrix1, 1.0, MATRIX_ROWS, 0);
  ASSERT_EQ(status, ESIZE);
  status = matrix_set_val(matrix1, 1.0, 0, MATRIX_COLS);
  ASSERT_EQ(status, ESIZE);
}

TEST_F(MatrixReadWrite, ValidInput) {
  int status;
  matrix_t *matrix = read_matrix(valid_input_stream, &status);
  ASSERT_NE(matrix, nullptr);
  ASSERT_EQ(status, OK);
  EXPECT_FLOAT_EQ(0.1, matrix_get_val(matrix, 0, 0, nullptr));
  EXPECT_FLOAT_EQ(-4.3, matrix_get_val(matrix, 0, 1, nullptr));
  EXPECT_FLOAT_EQ(22.1, matrix_get_val(matrix, 0, 2, nullptr));
  EXPECT_FLOAT_EQ(1.2e-9, matrix_get_val(matrix, 1, 0, nullptr));
  EXPECT_FLOAT_EQ(-4.3, matrix_get_val(matrix, 1, 1, nullptr));
  EXPECT_FLOAT_EQ(2.1, matrix_get_val(matrix, 1, 2, nullptr));
  delete_matrix(matrix);
}

TEST_F(MatrixReadWrite, InvalidInputSize) {
  int status;
  matrix_t *matrix = read_matrix(invalid_input_size_stream, &status);
  ASSERT_EQ(matrix, nullptr);
  ASSERT_EQ(status, EREAD);
  delete_matrix(matrix);
}

TEST_F(MatrixReadWrite, InvalidInputValue) {
  int status;
  matrix_t *matrix = read_matrix(invalid_input_value_stream, &status);
  ASSERT_EQ(matrix, nullptr);
  ASSERT_EQ(status, EREAD);
  delete_matrix(matrix);
}

TEST_F(MatrixReadWrite, WriteValidArguments) {
  int status;
  matrix_t *matrix = read_matrix(valid_input_stream, &status);
  ASSERT_NE(matrix, nullptr);
  ASSERT_EQ(status, OK);
  write_matrix(output_write_stream, matrix);
  fflush(output_write_stream);
  delete_matrix(matrix);
  ASSERT_STREQ(output_write_buffer_expected, output_write_buffer);
}

TEST_F(MatrixReadWrite, WriteInalidArguments) {
  matrix_t tmp;
  int status = write_matrix(nullptr, &tmp);
  ASSERT_EQ(status, ESTREAM);
  status = write_matrix(stdin, nullptr);
  ASSERT_EQ(status, EPTR);
}

TEST_F(MatrixTranspose, TransposeOperation) {
  matrix_t *matrix = read_matrix(valid_input_stream, nullptr);
  ASSERT_NE(matrix, nullptr);
  transpose(matrix);
  int status = write_matrix(output_write_stream, matrix);
  EXPECT_EQ(status, OK);
  fflush(output_write_stream);
  ASSERT_STREQ(output_write_buffer, output_write_buffer_expected);
  delete_matrix(matrix);
}