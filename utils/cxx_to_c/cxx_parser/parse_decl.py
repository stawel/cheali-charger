#!/usr/bin/env python
""" Usage: call with <input filename> <output filename> [include dir]
"""

import sys
import clang.cindex
from clang.cindex import *
import pprint
import fileinput
from operator import itemgetter
import debug


pdebug = False
#pdebug = True

decl = {}
decl_names = {}

def print_decl_names():
    for key, info in decl_names.iteritems():
        print key ,':', info

def print_decl():
    for key, info in decl.iteritems():
        print key ,':', info


def get_default_args(usr):
    if usr in decl:
        (spelling, type, namespace, fullname, def_args) = decl[usr]
        return def_args
    else:
        return []


def get_new_name(usr):
    if usr in decl:
        (spelling, type, namespace, fullname, _) = decl[usr]
        if len(decl_names[fullname]) > 1:
            suffix = 1
            for (_, type2, _, _, _) in decl_names[fullname]:
                if type == type2: break;
                suffix += 1
        else:
            suffix = ''
        return fullname + str(suffix)
    else:
        return ''


def name_with_namespace(info):
    (spelling, type, namespace, _, _) = info
    name = spelling
    for i in namespace:
        name = i + '_' + name
    return name

def add_decl(usr, spelling, type, namespace, default_arg):
    info = (spelling, type, namespace, '', default_arg)
    name = name_with_namespace(info)
    info = (spelling, type, namespace, name, default_arg)
    if pdebug: print info

    if usr not in decl:
        decl[usr] = info
        if name in decl_names:
            decl_names[name].append(info)
        else:
            decl_names[name] = [info]


def get_def_arg(cursor):
    for t in cursor.get_tokens():
        return t.spelling
    return []

def add_func_decl(node, namespace):
        default_arg = []
        for arg_cursor in node.get_arguments():
            for i in arg_cursor.get_children():
                default_arg.append(get_def_arg(i))
        add_decl(str(node.get_usr()), node.spelling, node.type.spelling, namespace, default_arg)


def find_decl(in_file, node, n, namespace):
    """ Find all references to the type named 'typename'
    """
    if pdebug:
        debug.print_cursor(node,n)
        print debug.s(n), 'namespace: ', namespace

    if node.kind == CursorKind.NAMESPACE:
        namespace = list(namespace)
        namespace.append(node.spelling)


    if (node.kind == CursorKind.FUNCTION_DECL):
        add_func_decl(node, namespace)

    if (node.kind == CursorKind.VAR_DECL):
        add_decl(str(node.get_usr()), node.spelling, node.type.spelling, namespace, [])

    for c in node.get_children():
#        if in_file == str(c.location.file):
            find_decl(in_file, c, n + 1, namespace)




def find_all_decl(in_file, params):

    index = clang.cindex.Index.create()
    tu = index.parse(in_file, args=params, options = 1)
    #, options=TranslationUnit.PARSE_DETAILED_PROCESSING_RECORD)

    find_decl(in_file, tu.cursor, 1, [])
    if pdebug:
        print_decl_names()
        print '-------------------'
        print_decl()


