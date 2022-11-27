import csv
from matplotlib import pyplot

i = []
plot = []
motion = []

with open ('./sensor.csv', 'r', newline='\n') as f:
    reader = csv.reader (f, delimiter=',')
    header = next (reader, None)
    for row in reader:
        i.append (int(row[0]))
        plot.append (float(row[1]))
        motion.append (1 if row[2] == 'True' else 0)

pyplot.subplot (2,1,1)
pyplot.plot (i, plot, '-')
pyplot.title ('Line - ' + header[1])
pyplot.xlim ([1,50])
pyplot.xlabel ('Indice')
pyplot.ylabel ('Valeur')

pyplot.subplot (2,1,2)
pyplot.plot (i, motion)
pyplot.title ('Bar - ' + header[2])
pyplot.xlim ([1,50])
pyplot.xlabel ('Indice')
pyplot.ylabel ('Etat')

pyplot.show ()
