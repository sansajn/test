import numpy as np
import pandas as pd
from sklearn.datasets.samples_generator import make_blobs

X, labels = make_blobs(n_samples=100, n_features=5, centers=6,
	cluster_std=0.6, random_state=0)
    
df = pd.DataFrame({
	'X': X[:,0], 'Y': X[:,1], 'Z': X[:,2],
	'C': X[:,3],
	'T': X[:,4],
	'Label': labels},
	columns=['X', 'Y', 'Z', 'C', 'T', 'Label'])
	
df.to_csv('dataset.csv', index=None, header=None)
