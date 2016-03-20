#!/usr/bin/env python
""" Usage: call with <input filename> <output filename> [include dir]
"""

import sys
import clang.cindex
from clang.cindex import *
import pprint
import fileinput
from operator import itemgetter


rep = {}

def add(line, c_from, c_to, s, s2):
    if not (line in rep):
        rep[line] = []
    rep[line].append( (c_from, c_to, s, s2) );

def replace2(line, r):
    (c_from, c_to, s, s2) = r
    return line[:c_from-1] + s + line[c_to+1:]

def replace3(line, nr):
    if nr in rep:
        list_t = rep[nr]
        list_t.sort(key=itemgetter(1) ,reverse=True)
#        print list_t
        last = list_t[0][1]
        no_overlap_l = []
        for (c_form, c_to, s1, s2) in list_t:
            if c_to <= last:
                no_overlap_l.append( (c_form, c_to, s1, s2) )
                last = c_form

        for r in no_overlap_l:
            line = replace2(line ,r)

    if len(line) == 0:
        return "\n";
    return line


def print_rep():
    print rep



def replace(in_file, out_file):
    output = open(out_file, 'w')

    nr = 1;
    for line in fileinput.FileInput(in_file):
        new_line = replace3(line, nr)
#       new_line = line
        nr = nr + 1
        output.write(new_line)

    output.close()
