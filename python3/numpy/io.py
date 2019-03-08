# zapis a citanie do/zo suboru
import numpy as np

M = np.array([1, 2, 3, 4, 5])
N = np.array([6, 7, 8, 9, 10])

data = np.array([M, N])

fname = 'data.txt'
np.savetxt(fname, data)

M2, N2 = np.genfromtxt(fname)

print('M2 = ', M2)
print('N2 = ', N2)
