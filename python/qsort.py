# -*- coding: utf-8 -*-

# rekurzivna verzia q-sortu, prvý nástrel
def qsort(X):
	if len(X) < 2:
		return X
	V = []; M = []
	p = X[len(X)/2]
	for x in X:
		if x < p:
			M.append(x)
		else:
			V.append(x)
	qsort(M)
	qsort(V)
	return M.extend(p).extend(V)
		
		
def main():
	X=[9, 2, 7, 5, 1]
	X=qsort(X)
	print X


if __name__ == '__main__':
	main()
		
