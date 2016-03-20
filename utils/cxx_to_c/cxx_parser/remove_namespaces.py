#!/usr/bin/env python
""" Usage: call with <input filename> <output filename> [include dir]
"""

import sys
import clang.cindex
from clang.cindex import *
import pprint
import fileinput
from operator import itemgetter
from replace import *
from parse_decl import *
import debug

rdebug = False
#rdebug = True

def get_def_usr(cursor):
    d = cursor.get_definition()
    if d is None:
        r = cursor.referenced
        if r is None:
            return ''
        else:
            return r.get_usr()
    else:
        return d.get_usr()


def add2(l,p1,p2, s1, s2):
    add(l, p1, p2, s1, s2)
    if rdebug: print 'added:',(l, p1, p2, s1, s2)


def find_typerefs(in_file, node, n):
    """ Find all references to the type named 'typename'
    """

    new_name = get_new_name(get_def_usr(node))
    default_args = get_default_args(get_def_usr(node))

    if rdebug:
        print debug.s(n), 'new name: ', new_name
        print debug.s(n), 'def args: ', default_args
        debug.print_cursor(node,n)

    if node.kind == CursorKind.TYPEDEF_DECL:
        return

    if node.kind == CursorKind.NAMESPACE:
        for t in node.get_tokens():
            if t.cursor.kind == CursorKind.NAMESPACE and (t.location.line == node.extent.start.line or t.location.line == node.extent.end.line):
                add(t.extent.end.line, t.extent.start.column, t.extent.end.column-1, "", t.spelling)



    if (node.kind == CursorKind.FUNCTION_DECL)  and len(new_name) > 0:
        add(node.location.line, node.location.column, node.location.column + len(node.spelling) -2, new_name, node.spelling)
        for arg in node.get_arguments():
            #remove default arguments
            tokens = list(arg.get_tokens())
            for t in tokens:
                if t.spelling == '=':
                    c = list(arg.get_children())[-1]
                    t2 = list(c.get_tokens())[-2]
#                    print 'tokens:', c.spelling, t2.spelling
                    add2(arg.location.line, t.extent.start.column, t2.extent.end.column -2, '', str([t.spelling for t in tokens]))
                    break



    if (node.kind == CursorKind.DECL_REF_EXPR 
       or node.kind == CursorKind.VAR_DECL
       )  and len(new_name) > 0:
            tokens = reversed(list(node.get_tokens()))
            for t in tokens:
                if t.spelling == node.spelling:
                    if t.location.line == node.location.line:
                        add2(t.location.line, t.location.column, t.location.column + len(t.spelling) -2, new_name, t.spelling)
                    else:
                        if(rdebug):  print debug.s(n), "macro found"
                    break

    if node.kind == CursorKind.NAMESPACE_REF:
            tokens = list(node.get_tokens())
            if len(tokens)>=2:
                t1 = tokens[0]
                t2 = tokens[-1]
                if t1.spelling == node.spelling:
                    if t1.location.line == node.location.line:
                        add(t1.extent.start.line, t1.extent.start.column, t2.extent.end.column-2, "", node.spelling)
                    else:
                        if(rdebug):  print debug.s(n), "macro found"



    if node.kind == CursorKind.CALL_EXPR:
        #add default arguments
#        print '!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1'
#        print node.spelling
#        print_decl()
#        print default_args
        default_args = list(default_args)
        for token in reversed(list(node.get_tokens())):
            if token.spelling == ')':
                break;
        comma = ''
        for arg in node.get_arguments():
#            print arg.location.line 
            if arg.location.line != 0:
                comma = ', '
            else:
                new_arg = default_args.pop()
#                print new_arg
                add2(token.extent.start.line, token.extent.end.column-1, token.extent.end.column-3, comma + str(new_arg), "")



    for c in node.get_children():
#        print c.location.file, in_file, in_file == str(c.location.file)
        if in_file == str(c.location.file):
            find_typerefs(in_file, c, n + 1)






def cxx_to_c(in_file, params, out_file):

    find_all_decl(in_file, params)

    index = clang.cindex.Index.create()
    tu = index.parse(in_file, args=params, options = 1)
    #, options=TranslationUnit.PARSE_DETAILED_PROCESSING_RECORD)

    find_typerefs(in_file, tu.cursor, 1)

#    print_decl_names()

    replace(in_file, out_file)

