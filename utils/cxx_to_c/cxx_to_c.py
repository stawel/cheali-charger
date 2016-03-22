#!/usr/bin/env python
""" Usage: call with <input filename> <output filename> [include dir]
"""

import sys
import clang.cindex
from cxx_parser.debug import *
from cxx_parser.parse_decl import *
from cxx_parser.remove_namespaces import *


in_file = sys.argv[1]
out_file = sys.argv[2]
include_dirs = sys.argv[3:]


params = ['-x','c++', '-DCHEALI_EEPROM_PACKED', '-DENABLE_SETTINGS_MENU_RESET', '-DENABLE_SCREEN_ANIMATION' ]
for i in include_dirs:
    params.append("-I")
    params.append(i)



#index = clang.cindex.Index.create()
#tu = index.parse(in_file, args=params, options = 1)

#print_cursor_tree(tu.cursor, 1)


#find_all_decl(in_file, params)
#print decl
#print_decl_names()

cxx_to_c(in_file, params, out_file)
