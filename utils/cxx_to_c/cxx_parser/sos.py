#!/usr/bin/env python
""" Usage: call with <input filename> <output filename> [include dir]
"""

import sys
import clang.cindex
from clang.cindex import *
import pprint
import fileinput
from operator import itemgetter


debug = False
debug2 = False
#debug = True
#debug2 = True

in_file = sys.argv[1]
out_file = sys.argv[2]
include_dirs = sys.argv[3:]


rep = {}

def add(line, c_from, c_to, s, s2):
    if not (line in rep):
        rep[line] = []
    rep[line].append( (c_from, c_to, s, s2) );

def replace2(line, r):
    (c_from, c_to, s, s2) = r
    return line[:c_from-1] + s + line[c_to+1:]

def replace(line, nr):
    if nr in rep:
        list_t = rep[nr]
        list_t.sort(key=itemgetter(0) ,reverse=True)
        for r in list_t:
            line = replace2(line ,r)

    if len(line) == 0:
        return "\n";
    return line

def s(n):
    return " "*(n*4)

def get(x):
    if x is None:
        return None
    else:
        return x.displayname

def get_defin_mangled_name(c):
    d = c.get_definition()
    if d is None:
        r = c.referenced
        if r is None:
            return ""
        else:
            return r.mangled_name
    else:
        return d.mangled_name


def get_filename(location):
    if location.file is None:
        return ""
    else:
        return location.file.name

def find_typerefs(node, n):
    """ Find all references to the type named 'typename'
    """

    if debug:
        print s(n), 'translan_unit: ', node.translation_unit.spelling
        print s(n), 'location.file: ', get_filename(node.location)
        print s(n), "tab          : ", n
        print s(n), "extent       : ", node.extent
        print s(n), "location     : ", node.location
        print s(n), "spelling     : ", node.spelling
        print s(n), "kind         : ", node.kind
        print s(n), "displayname  : ", node.displayname
        print s(n), "mangled_name : ", node.mangled_name
        print s(n), "get_usr()    : ", node.get_usr()
        print s(n), "storage_class: ", node.storage_class
        print s(n), "access_specif: ", node.access_specifier
        print s(n), "type.spelling: ", node.type.spelling
        print s(n), "type.kind    : ", node.type.kind
        print s(n), "type.declara : ", node.type.get_declaration().location
        print s(n), "semantic_pare: ", get(node.semantic_parent)
        print s(n), "lexical_paren: ", get(node.lexical_parent)
        print s(n), "canonical    : ", node.canonical.location
        defin = node.get_definition()
        print s(n), "definition   : ", defin
        print s(n), "referenced   : ", node.referenced

        if node.referenced is not None:
            print s(n), "referenced   : ", node.referenced.location

        if not (defin is None):
            print s(n), "defin    : " , defin.spelling, " ", defin.mangled_name
        if node.kind.is_reference():
            print s(n), "ref      : ", get(node.referenced)

        print s(n), [ t.kind for t in node.get_tokens()]
        print s(n), [ t.spelling for t in node.get_tokens()]

        print s(n), "------------------"


    if node.kind == CursorKind.TYPEDEF_DECL:
        return

    if node.kind == CursorKind.NAMESPACE:
        for t in node.get_tokens():
            if t.cursor.kind == CursorKind.NAMESPACE and (t.location.line == node.extent.start.line or t.location.line == node.extent.end.line):
                add(t.extent.end.line, t.extent.start.column, t.extent.end.column-1, "", t.spelling)



    if (node.kind == CursorKind.FUNCTION_DECL)  and len(node.mangled_name) > 0:
        add(node.location.line, node.location.column, node.location.column + len(node.spelling) -2, node.mangled_name, node.spelling)


    if (node.kind == CursorKind.DECL_REF_EXPR 
       or node.kind == CursorKind.VAR_DECL
       )  and len(get_defin_mangled_name(node)) > 0 and node.referenced.kind != CursorKind.PARM_DECL:
            for t in node.get_tokens():
                if t.spelling == node.spelling:
                    if t.location.line == node.location.line:
                        add(t.location.line, t.location.column, t.location.column + len(t.spelling) -2, get_defin_mangled_name(node), t.spelling)
                    else:
                        if(debug):  print s(n), "macro found"
                    break

    if node.kind == CursorKind.NAMESPACE_REF:
            for t in node.get_tokens():
                if t.spelling == node.spelling:
                    if t.location.line == node.location.line:
                        add(node.extent.start.line, node.extent.start.column, node.extent.end.column, "", node.spelling)
                    else:
                        if(debug):  print s(n), "macro found"
                    break



    for c in node.get_children():
#        print c.location.file, in_file, in_file == str(c.location.file)
        if in_file == str(c.location.file):
            find_typerefs(c, n + 1)





params = ['-x','c++', '-DCHEALI_EEPROM_PACKED' ]
for i in include_dirs:
    params.append("-I")
    params.append(i)



index = clang.cindex.Index.create()
tu = index.parse(in_file, args=params, options = 1)
#, options=TranslationUnit.PARSE_DETAILED_PROCESSING_RECORD)


if debug2:
    print "in_file: ", in_file
    print "out_file: ", out_file
    print "params: ", params
    print 'Translation unit:', tu.spelling
    #print tu.diagnostics


find_typerefs(tu.cursor, 1)


output = open(out_file, 'w')

#print rep

nr = 1;
for line in fileinput.FileInput(in_file):
    new_line = replace(line, nr)
#    new_line = line
    nr = nr + 1
    output.write(new_line)

output.close()

if debug2:
    print rep


