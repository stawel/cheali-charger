eepromExtractor
===============

extract eeprom
--------------

Install python and python-crcmod:
<pre>
sudo apt-get install python python-crcmod
</pre>

extract calibration data from eeprom.bin:
<pre>
cd cheali-charger/utils/eepromExtractor
cheali-charger/utils/eepromExtractor$ ./eeprom.py eprom.bin
</pre>
this should generate defaultCalibration.cpp.



Edit existing eeprom
--------------------

see example: cheali-charger/utils/eepromExtractor line 42


Add new eeprom
--------------

1. install g++-4.8, see: http://askubuntu.com/questions/466651/how-do-i-use-the-latest-gcc-on-ubuntu-14-04
<pre>
cheali-charger$ g++ --version
g++ (Ubuntu 4.8.5-1ubuntu1) 4.8.5
Copyright (C) 2015 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
</pre>
2. install gccxml from: https://github.com/gccxml/gccxml
<pre>
cheali-charger$ mkdir ../gccxml
cheali-charger$ cd ../gccxml
gccxml$ git clone https://github.com/gccxml/gccxml
gccxml$ cmake .
gccxml$ make
gccxml$ sudo make install
</pre>
3. install ctypeslib:
<pre>
gccxml$ mkdir ../ctypeslib
gccxml$ cd ../ctypeslib
ctypeslib$ svn co http://svn.python.org/projects/ctypes/trunk/ctypeslib
ctypeslib$ ./setup.py build
</pre>
4. edit cheali-charger/utils/eepromExtractor/getctypes.sh:11 according to your ctypeslib path
5. create new eeproms:
<pre>
cheali-charger$ ./bootstrap-avr
cheali-charger$ cd utils/eepromExtractor
cheali-charger/utils/eepromExtractor$ ./getctypes.sh
</pre>
