#!/usr/bin/python
# -*- coding: utf-8 -*-
from __future__ import unicode_literals
from numpy import *


dolar_channel1_info = [
#    name,      factor, offset, symbol, color
    ("state",   1.,     0.,     "",     ''),
    ("time",    1.,     0.,     "s",    ''),
    ("Vout",    0.001,  0.,     "V",    'b-'),
    ("Iout",    0.001,  0.,     "A",    'r-'),
    ("Charge",  0.001,  0.,     "Ah",   'b-'),
    ("Power",   0.01,   0.,     "W",    'y-'),
    ("Energy",  0.01,   0.,     "Wh",   'c-'),
    ("Text",    0.01,   0.,     "C",    'm-'),
    ("Tint",    0.01,   0.,     "C",    'g-'),
    ("Vin",     0.001,  0.,     "V",    'g-'),
    ("Vb1",     0.001,  0.,     "V",    'k-'),
    ("Vb2",     0.001,  0.,     "V",    'b-'),
    ("Vb3",     0.001,  0.,     "V",    'y-'),
    ("Vb4",     0.001,  0.,     "V",    'c-'),
    ("Vb5",     0.001,  0.,     "V",    'm-'),
    ("Vb6",     0.001,  0.,     "V",    'g-'),
    ("R1",      0.001,  0.,     "Ohm",  'r-'),
    ("R2",      0.001,  0.,     "Ohm",  'k-'),
    ("R3",      0.001,  0.,     "Ohm",  'b-'),
    ("R4",      0.001,  0.,     "Ohm",  'y-'),
    ("R5",      0.001,  0.,     "Ohm",  'c-'),
    ("R6",      0.001,  0.,     "Ohm",  'm-'),
    ("Rbat",    0.001,  0.,     "Ohm",  'g-'),
    ("Rwire",   0.001,  0.,     "Ohm",  'r-'),
    ("Percent", 0.001,  0.,     "%",    'k-'),
    ("ETA",     0.016666667,0., "min.", 'b-'),
    ("checksum",1.,     0.,     "",     '')
]


dolar_channel_info = { 
    "$1": dolar_channel1_info,
    "P1": [("P1", 1., 0., "V", 'r-')]
};

output = {}


def get_color(name):
    for ch in dolar_channel_info.values():
        for i in range(0, len(ch)):
            if ch[i][0] == name:
                return ch[i][4]
    return ''


def init_output():
    for ch in dolar_channel_info.values():
        for i in range(0, len(ch)):
            output[ch[i][0]] = ([],[])

def parse_P(line):
    (x, y) = output["P1"]
    y.append(float(line.split(' ')[1]))

def finalize_P():
    (x, y) = output["P1"]
    x = arange(0, len(y)) / 2.
    output["P1"] = (x, y)

def add_dolar(name, vx, vy):
    (x, y) = output[name]
    x.append(vx)
    y.append(vy)

def parse_dolar(line):
    data = line.split(';')
    channel_info = dolar_channel_info[data[0]];
    time = float(data[2])
    #TODO: check checksum
    for i in range(1, len(data) - 2):
        add_dolar(channel_info[i - 1][0], time, float(data[i]) * channel_info[i - 1][1] - channel_info[i - 1][2])

def parse_line(line):
    if line[0] == 'P':
        parse_P(line)
    if line[0] == '$':
        parse_dolar(line)

def read_cheali(f):
    init_output()
    for line in f:
        parse_line(line)
    finalize_P()
    return output
