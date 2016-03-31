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


params = ['-x','c++',
# '-DCHEALI_EEPROM_PACKED'
'-D__WCHAR_T_DEFINED',
#'-D__SDC51_STDARG_H',
'-DENABLE_SETTINGS_MENU_RESET', '-DENABLE_SCREEN_ANIMATION' ,'-nostdinc', '-DNULL=0']

for i in include_dirs:
    params.append(i)




if cxx_to_c(in_file, params, out_file):
    sys.exit(1)

