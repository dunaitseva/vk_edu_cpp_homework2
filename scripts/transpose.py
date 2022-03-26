import numpy as np
import sys


def read_matrix(file_name):
    result = []
    rows, cols = map(int, input().split())
    for i in range(rows):
        result.append(list(map(float, input().split())))
    return rows, cols, np.array(result)


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
    if len(sys.argv) != 3:
        raise RuntimeError('To few arguments')
    rows, cols, matrix = read_matrix(sys.argv[1])
    matrix = matrix.transpose()
    write_in_file(sys.argv[2], matrix, rows, cols)
