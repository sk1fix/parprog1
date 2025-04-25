import os

import numpy as np


def save_file(matrix, path):
    np.savetxt(path, matrix, fmt='%d')

def create_matrix(sizes):
    current_dir = os.path.dirname(os.path.abspath(__file__))
    for size in sizes:
        a_matrix = np.random.randint(0, 999, (size, size))
        b_matrix = np.random.randint(0, 999, (size, size))
        dir_path = os.path.join(current_dir, str(size))
        os.makedirs(dir_path, exist_ok=True)
        save_file(a_matrix, os.path.join(dir_path, 'a_matrix.txt'))
        save_file(b_matrix, os.path.join(dir_path, 'b_matrix.txt'))


def main():
    sizes = [10, 100, 200, 300, 400, 500, 600, 700, 800, 900, 1000]
    create_matrix(sizes)

if __name__ == '__main__':
    main()