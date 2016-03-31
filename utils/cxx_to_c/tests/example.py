#!/usr/bin/env python
""" Usage: call with <filename>
"""

import sys
import clang.cindex
import pprint


def s(n):
    return " "*(n*4)

def get(x):
    if x is None:
        return None
    else:
        return x.displayname


def find_typerefs(node, n):
    """ Find all references to the type named 'typename'
    """
    print s(n), "tab          : ", n
    print s(n), "extent       : ",  node.extent
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
    print s(n), "semantic_pare: ", get(node.semantic_parent)
    print s(n), "lexical_paren: ", get(node.lexical_parent)
    defin = node.get_definition()
#    print s(n), defin
    if not (defin is None):
        print s(n), "defin    : " , defin.spelling, " ", defin.mangled_name
    if node.kind.is_reference():
        print s(n), "ref      : ", get(node.referenced)

    print s(n), [ t.kind for t in node.get_tokens()]
    print s(n), [ t.spelling for t in node.get_tokens()]

    print s(n), "------------------"

    for c in node.get_children():
        find_typerefs(c, n + 1)

index = clang.cindex.Index.create()

tu = index.parse(sys.argv[1], args=['-x','c++'])
print 'Translation unit:', tu.spelling
find_typerefs(tu.cursor, 1)
