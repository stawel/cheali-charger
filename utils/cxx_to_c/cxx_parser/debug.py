#!/usr/bin/env python
""" Usage: call with <input filename> <output filename> [include dir]
"""

import sys
import clang.cindex
from clang.cindex import *
import pprint
import fileinput
from operator import itemgetter

debug_mangled = False

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


def print_cursor(node, n):
    print s(n), "------------------"
    print s(n), 'translan_unit: ', node.translation_unit.spelling
    print s(n), 'location.file: ', get_filename(node.location)
    print s(n), "tab          : ", n
    print s(n), "extent       : ", node.extent
    print s(n), "location     : ", node.location
    print s(n), "spelling     : ", node.spelling
    print s(n), "kind         : ", node.kind
    print s(n), "displayname  : ", node.displayname
    if debug_mangled: print s(n), "mangled_name : ", node.mangled_name
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
    if not (defin is None) and debug_mangled:
        print s(n), "defin    : " , defin.spelling, " ", defin.mangled_name
    if node.kind.is_reference():
        print s(n), "ref      : ", get(node.referenced)

    print s(n), 'tokens:'
    print s(n), [ t.kind for t in node.get_tokens()]
    print s(n), [ t.spelling for t in node.get_tokens()]

    print s(n), 'arguments:'
    print s(n), [ a.kind for a in node.get_arguments()]
    print s(n), [ a.spelling for a in node.get_arguments()]

    for a in node.get_arguments():
        print_cursor_tree(a, 0)



def print_cursor_tree(node, n):
    """ Find all references to the type named 'typename'
    """
    print_cursor(node, n)

    for c in node.get_children():
#        print c.location.file, in_file, in_file == str(c.location.file)
#        if in_file == str(c.location.file):
            print_cursor_tree(c, n + 1)





