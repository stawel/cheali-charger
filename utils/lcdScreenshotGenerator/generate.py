#!/usr/bin/python


import sys
import inspect, os
from subprocess import call

current_path=os.path.dirname(os.path.abspath(inspect.getfile(inspect.currentframe())))

xpm_path = current_path+'/xpm/'
montage = 'montage'
convert = 'convert'
extra_params = ['-background', '#0877ed', '-geometry', '+2+4', '-tile', 'x1']

output = 'output.png'
string = '>test 123'

if len(sys.argv) > 1:
    string = sys.argv[1]

if len(sys.argv) > 2:
    output = sys.argv[2]


fonts = [xpm_path + '0x' + i.encode('hex').upper()+'.xpm' for i in string]

#print fonts
exec_cmd = [montage] + fonts + extra_params + [output]


print 'command:', exec_cmd
call(exec_cmd)


if len(sys.argv) > 4:
    border = ['-bordercolor', sys.argv[4] ,'-border', '4']
else:
    border = []


if len(sys.argv) > 3:
    call([convert] + border + ['-resize', sys.argv[3], output, output])


