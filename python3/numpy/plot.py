# ukazka funkcie plot

import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import numpy as np

def create_plot_data():
	years = np.array([1848, 1869, 1880, 1890, 1900, 1910, 1921, 1930, 1946, 1950, 1961, 1970, 1980, 1991, 2001, 2011])
	population = np.array([2442000, 2481811, 2477521, 2595180, 2782925, 2916657, 2993859, 3324111, 3327803, 3442317, 4174046, 4537290, 4991168, 5274335, 5379455, 5397036])

	data = np.array([years, population])
	np.savetxt("population.txt", data)


create_plot_data()

years, population = np.genfromtxt('population.txt')

plt.plot(years, population/1000000)
plt.grid(True)
plt.title('Vyvoj obyvatelstva Slovenska od roku 1840 po 2011')
plt.xlabel('Rok scitania')
plt.ylabel('Pocet obyvatelov v milionoch')
plt.savefig('line_plot_1')
