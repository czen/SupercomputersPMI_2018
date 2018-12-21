import numpy as np
import os


def generateBlockSymmetricMatrix(block_size, matrix_size, is_integer=True):
    block_count = matrix_size // block_size
    matrix = [[0 for i in range(0, block_count)]
              for j in range(0, block_count)]
    for i in range(0, block_count):
        for j in range(0, i + 1):
            if is_integer:
                data = np.random.randint(-1000, 1000 + 1,
                                         size=(block_size, block_size))
            else:
                data = 1000 * np.random.random_sample((block_size, block_size))
            matrix[i][j] = data
            matrix[j][i] = data
    return np.block(matrix)


N = 2880

block_sizes = [1,  6,  10,  15,  20,  24,  30,  36,  40,
               60, 72, 80, 96, 120, 144, 160, 180, 240, 360, 480, 720, 2880]

for block_size in block_sizes:

    print('Handle ' + str(block_size) + ' block size...')

    path = 'size' + str(block_size)
    if not os.path.exists(path):
        os.makedirs(path)

    a_tril_int = np.tril(np.random.randint(-1000, 1000 + 1, size=(N, N)))
    np.savetxt(path + '/A_int.txt', a_tril_int, fmt='%d')

    a_tril_float = np.tril(1000 * np.random.random_sample((N, N)))
    a_tril_float = np.around(a_tril_float, decimals=2)
    np.savetxt(path + '/A_float.txt', a_tril_float, fmt='%.2f')

    b_symmetric_int = generateBlockSymmetricMatrix(block_size, N)
    np.savetxt(path + '/B_int.txt', b_symmetric_int, fmt='%d')

    b_symmetric_float = generateBlockSymmetricMatrix(
        block_size, N, is_integer=False)
    b_symmetric_float = np.around(b_symmetric_float, decimals=2)
    np.savetxt(path + '/B_float.txt', b_symmetric_float, fmt='%.2f')

    c = np.matmul(a_tril_int, b_symmetric_int)
    np.savetxt(path + '/C_int.txt', c, fmt='%d')

    c = np.matmul(a_tril_float, b_symmetric_float)
    np.savetxt(path + '/C_float.txt', c, fmt='%.2f')
