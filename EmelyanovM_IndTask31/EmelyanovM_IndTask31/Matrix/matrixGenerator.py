import numpy as np
import os


def generateBlockSymmetricMatrix(blockLen, matrixSize, isInteger=True):
    blockCount = matrixSize // blockLen
    matrix = [[0 for i in range(0, blockCount)]
              for j in range(0, blockCount)]
    for i in range(0, blockCount):
        for j in range(0, i + 1):
            if isInteger:
                data = np.random.randint(-1000, 1000 + 1,
                                         size=(blockLen, blockLen))
            else:
                data = 1000 * np.random.random_sample((blockLen, blockLen))
            matrix[i][j] = data
            matrix[j][i] = data
    return np.block(matrix)


N = 1440

blocks = { 1, 6, 8, 9, 10, 12, 15, 16, 18, 20, 24, 30, 32, 36, 40, 45, 48, 60, 72, 80, 90, 96,  120,  144,  160,  180,  240,  288,  360,  480,  720, 1440 };
for blockLen in blocks:

    print('Constuct ' + str(blockLen) + ' block size...')

    path = 'size' + str(blockLen)
    if not os.path.exists(path):
        os.makedirs(path)

    SymmetricMatrix = generateBlockSymmetricMatrix(blockLen, N, isInteger=False)
    SymmetricMatrix = np.around(SymmetricMatrix, decimals=2)
    np.savetxt(path + '/Matrix_A.txt', SymmetricMatrix, fmt='%.2f')
	
    UpperTriangleMatrix = np.triu(1000 * np.random.random_sample((N, N)))
    UpperTriangleMatrix = np.around(UpperTriangleMatrix, decimals=2)
    np.savetxt(path + '/Matrix_B.txt', UpperTriangleMatrix, fmt='%.2f')
	
    c = np.matmul(SymmetricMatrix, UpperTriangleMatrix)
    np.savetxt(path + '/Matrix_C.txt', c, fmt='%.2f')

input("Complete!")