#!/usr/bin/python


import sys
import inspect, os
from subprocess import call

current_path=os.path.dirname(os.path.abspath(inspect.getfile(inspect.currentframe())))

xpm_path = current_path+'/xpm/'
montage = 'montage'
convert = 'convert'
extra_params = ['-background', '#0877ed', '-geometry', '+2+4', '-tile', 'x1' ]

output = 'output.png'
string1 = '>test 123'
string2 = '>test 456'

if len(sys.argv) > 1:
    string1 = sys.argv[1]

if len(sys.argv) > 2:
    string2 = sys.argv[2]

if len(sys.argv) > 3:
    output = sys.argv[3]


fonts1 = [xpm_path + '0x' + i.encode('hex').upper()+'.xpm' for i in string1]
fonts2 = [xpm_path + '0x' + i.encode('hex').upper()+'.xpm' for i in string2]

#print fonts
exec_cmd1 = [montage] + fonts1 + extra_params + ['1' + output]
exec_cmd2 = [montage] + fonts2 + extra_params + ['2' + output]

print 'command:', exec_cmd1
call(exec_cmd1)

print 'command:', exec_cmd2
call(exec_cmd2)



exec_cmd = [convert, '1' + output, '2' +  output, '-append', output]
print exec_cmd
call(exec_cmd)

os.remove('1' + output)
os.remove('2' + output)

if len(sys.argv) > 4:
    call([convert, '-resize', sys.argv[4], output, output])

