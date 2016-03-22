#!/usr/bin/python


import sys
from subprocess import call

xpm_path = './xpm/'
montage = 'montage'
extra_params = ['-background', '#0877ed', '-geometry', '+2+4', '-tile', 'x1' ]

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
