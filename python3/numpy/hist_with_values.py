# ukazka pouzitia funkcie hist() s hodnotami stlpcou

import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import numpy as np
from pandas import *

#passangers = np.genfromtxt('TitanicSurvival.csv', delimiter=',', 
#	names=True)

passangers = read_csv('TitanicSurvival.csv', header=0)
passangers['age'] = passangers['age'].fillna(0)

survived = passangers[passangers['survived'] == 'yes']['age']
deads = passangers[passangers['survived'] == 'no']['age']
passanger_count = len(passangers['survived'])

plt.figure(figsize=(8, 16))
plt.hist([survived, deads], 
	label=['Zachranenych - %s/%s' % (len(survived), passanger_count),
		'Umrtia - %s/%s' % (len(deads), passanger_count)],
	orientation='horizontal', bins=30)

for i in plt.gca().patches:
	if i.get_width() > 0:
		plt.gca().text(i.get_width(), i.get_y(), int(i.get_width()), fontsize=8)

plt.grid(True, alpha=0.25)
plt.title('Statistika umrti katastrofy plavidla RMS Titanic podla veku')
plt.xlabel('Pocet cestujucich')
plt.ylabel('Vek')
plt.legend()
plt.text(0, -20, 'Poznamka: Data su iba ilustracne a nie uplne.'
		'U cestujucich s vekom nizsim ako 0 nebol uvedeny vek.',
	fontsize=8)
plt.savefig('titanic_histogram2')
