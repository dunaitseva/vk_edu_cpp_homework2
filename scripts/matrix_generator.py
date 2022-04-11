import numpy as np
import sys

EXPECTED_ARGC_AMOUNT = 4
ROWS_ARG = 1
COLS_ARG = 2
DEST_ARG = 3


def check_args():
    error_msg = '''
    Wrong arguments amount.
    Usage: pass 3 arguments
    First - rows amount
    Second - columns amount
    Third - Destination file
    '''

    if len(sys.argv) < EXPECTED_ARGC_AMOUNT:
        raise RuntimeError(error_msg)


def write_in_file(file_name, matrix, r, c):
    with open(file_name, 'w') as file:
        file.write(f"{r} {c}\n")
        for i in range(len(matrix)):
            for j in range(len(matrix[i])):
                file.write(f"{matrix[i][j]}")
                if j != len(matrix[i]) - 1:
                    file.write(" ")
            file.write("\n")


if __name__ == '__main__':
    check_args()
    rows = int(sys.argv[ROWS_ARG])
    cols = int(sys.argv[COLS_ARG])
    destination_file = sys.argv[DEST_ARG]
    print(f"Matrix will be created with size {rows} X {cols} in file '{destination_file}'")
    result_matrix = np.random.random((rows, cols))
    write_in_file(destination_file, result_matrix, rows, cols)
    print(f"Matrix created")
