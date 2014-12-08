
Building from Source
====================

atmega32 - linux
----------------
dependencies: git, cmake, avrdude, avr-libc, gcc-avr
<pre>
user@~$ sudo apt-get install cmake avrdude avr-libc gcc-avr git
user@~$ git clone https://github.com/stawel/cheali-charger.git
user@~$ cd cheali-charger
user@~/cheali-charger$ ./bootstrap
user@~/cheali-charger$ make
      
flashing [your charger] with USBasp:
    
user@~/cheali-charger$ cd src/hardware/atmega32/targets/[your charger]
user@~/cheali-charger/src/hardware/atmega32/targets/[your charger]$ ./progUSBasp.sh
</pre>
      
Now You should see a "cheali-charger welcome" screen.


nuvoton M0517 - linux
---------------------
dependencies: git, cmake, gcc-arm-none-eabi, openocd (patched)  

install [gcc-arm-none-eabi](https://launchpad.net/~terry.guo/+archive/ubuntu/gcc-arm-embedded)  
compile and install [openocd (patched)](https://github.com/stawel/openocd)  
copy [openocd/contrib/99-openocd.rules](https://github.com/stawel/openocd/blob/master/contrib/99-openocd.rules) to /etc/udev/rules.d

<pre>
user@~$ sudo apt-get install git cmake
user@~$ git clone https://github.com/stawel/cheali-charger.git
user@~$ cd cheali-charger
user@~/cheali-charger$ ./bootstrap-arm
user@~/cheali-charger$ make

flashing [your charger] with st-link-V2 or stm32f4discovery:
(patched openocd needed)
  
user@~/cheali-charger$ cd src/hardware/nuvoton-M0517/targets/[your charger]
user@~/cheali-charger/src/hardware/nuvoton-M0517/targets/[your charger]$ ./progStLink.sh
</pre>
      
Now You should see a "cheali-charger welcome" screen.


atmega32 - windows
------------------
**Atmel Studio**
- install [Atmel Studio](http://www.atmel.com/tools/atmelstudio.aspx) and [cmake](http://www.cmake.org/)
- start a "Atmel Studio Command Prompt" and go to your cheali-charger directory.
- run:
<pre>
  s:\cheali-charger> cmake . -G "Unix Makefiles"
  s:\cheali-charger> make
</pre>
- hex file should be in s:\cheali-charger\src\atmega32\targets\\[your charger]\cheali-charger*.hex


**cygwin**
- install [cygwin](https://www.cygwin.com/)
- in cygwin install: cmake avrdude avr-libc gcc-avr git make
- in cygwin run:
<pre>
  ~/cheali-charger$ ./bootstrap
  ~/cheali-charger$ make
</pre>
- hex file should be in ~/cheali-charger/src/atmega32/targets/[your charger]/cheali-charger*.hex


nuvoton M0517 - windows
-----------------------
- install [CooCox CoIDE](http://www.coocox.org/)
- import project: cheali-charger\CoIDE\cheali-charger.coproj
- build


atmega32 - OSX
--------------
dependencies: git, cmake, avrdude, avr-libc, gcc-avr

install [macports](https://www.macports.org/)

<pre>
user@~$ sudo port install cmake avrdude avr-libc gcc-avr git
user@~$ git clone https://github.com/stawel/cheali-charger.git
user@~$ cd cheali-charger
user@~/cheali-charger$ ./bootstrap
user@~/cheali-charger$ make
      
flashing [your charger] with USBasp:
      
user@~/cheali-charger$ cd src/hardware/atmega32/targets/[your charger]
user@~/cheali-charger/src/hardware/atmega32/targets/[your charger]$ ./progUSBasp.sh
</pre>
      
Now You should see a "cheali-charger welcome" screen.

nuvoton M0517 - OSX
-------------------
(TODO)

