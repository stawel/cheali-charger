#!/usr/bin/python
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import chealiparser
import sys

display = set([
        'P1',
#        'Vin',
        'Vout',
#        'Iout',
#        'Charge','Energy','Power',
#        'Vb1','Vb2','Vb3','Vb4','Vb5','Vb6',
#        'R1','R2','R3','R4','R5','R6','Rbat','Rwire',
#        'Text','Tint',
#        'Percent', 'ETA',
#        #'checksum','state','time'
])

if len(sys.argv) > 1:
    f = open(sys.argv[1], 'rU')
else:
    print sys.argv[0], '[filename]'
    sys.exit(1)

data = chealiparser.read_cheali(f)


fig1 = plt.figure()
plt.xlabel('time [s]')

for name, (x,y) in data.iteritems():
    if name in display:
        color = chealiparser.get_color(name)
        if len(color) > 0:
            plt.plot(x, y, color)


def update(num):
    #TODO
    return

animation.FuncAnimation(fig1, update, None, interval = 30)
plt.show()

