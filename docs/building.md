
Building from Source
====================

atmega32 - linux
----------------
dependencies: git, cmake, avrdude, avr-libc, gcc-avr
<pre>
user@~$ sudo apt-get install cmake avrdude avr-libc gcc-avr git
user@~$ git clone https://github.com/stawel/cheali-charger.git
user@~$ cd cheali-charger
user@~$ git submodule init
user@~$ git submodule update
user@~/cheali-charger$ ./bootstrap-avr
user@~/cheali-charger$ make
      
flashing [your charger] with USBasp:
    
user@~/cheali-charger$ cd src/hardware/atmega32/targets/[your charger]
user@~/cheali-charger/src/hardware/atmega32/targets/[your charger]$ ./progUSBasp.sh
</pre>
      
Now You should see a "cheali-charger welcome" screen.


nuvoton M0517 - linux
---------------------
dependencies: git, cmake, gcc-arm-none-eabi, openocd (at least version 0.9)  

install [gcc-arm-none-eabi](https://launchpad.net/~terry.guo/+archive/ubuntu/gcc-arm-embedded)  

<pre>
user@~$ sudo apt-get install git cmake openocd
user@~$ git clone https://github.com/stawel/cheali-charger.git
user@~$ cd cheali-charger
user@~$ git submodule init
user@~$ git submodule update
user@~/cheali-charger$ ./bootstrap-arm
user@~/cheali-charger$ make

flashing [your charger] with st-link-V2 or stm32f4discovery:
(openocd at least version 0.9 needed)
  
user@~/cheali-charger$ cd src/hardware/nuvoton-M0517/targets/[your charger]
user@~/cheali-charger/src/hardware/nuvoton-M0517/targets/[your charger]$ ./progStLink.sh
or
user@~/cheali-charger/src/hardware/nuvoton-M0517/targets/[your charger]$ ./progStLink2.sh
</pre>
      
Now You should see a "cheali-charger welcome" screen.


atmega32 - windows
------------------
**Atmel Studio**
- install [Atmel Studio](http://www.atmel.com/tools/atmelstudio.aspx) and [cmake](http://www.cmake.org/)
- start a "Atmel Studio Command Prompt" and go to your cheali-charger directory.
- run:
<pre>
  s:\cheali-charger> cmake . -DCMAKE_TOOLCHAIN_FILE=avr-toolchain.cmake -G "Unix Makefiles"
  s:\cheali-charger> make
</pre>
- hex file should be in s:\cheali-charger\src\atmega32\targets\\[your charger]\cheali-charger*.hex


**cygwin**
- install [cygwin](https://www.cygwin.com/)
- in cygwin install: cmake avrdude avr-libc gcc-avr git make
- in cygwin run:
<pre>
  ~/cheali-charger$ ./bootstrap-avr
  ~/cheali-charger$ make
</pre>
- hex file should be in ~/cheali-charger/src/atmega32/targets/[your charger]/cheali-charger*.hex


nuvoton M0517 - windows
-----------------------
- install [CooCox CoIDE](http://www.coocox.org/)
- install [GNU Tools for ARM Embedded Processors](https://launchpad.net/gcc-arm-embedded)
- open CoIDE, go to Project -> Open Project and select cheali-charger\CoIDE\cheali-charger.coproj
- go to Project -> Select Toolchain Path and select bin folder of [GNU Tools for ARM Embedded Processors](ex. C:\Program Files (x86)\GNU Tools ARM Embedded\4.9 2015q1\bin)
- go to Project -> Build
- hex file should be in cheali-charger\CoIDE\cheali-charger\Debug\bin


atmega32 - OSX
--------------
dependencies: git, cmake, avrdude, avr-libc, gcc-avr

install [macports](https://www.macports.org/)

<pre>
user@~$ sudo port install cmake avrdude avr-libc gcc-avr git
user@~$ git clone https://github.com/stawel/cheali-charger.git
user@~$ cd cheali-charger
user@~$ git submodule init
user@~$ git submodule update
user@~/cheali-charger$ ./bootstrap-avr
user@~/cheali-charger$ make
      
flashing [your charger] with USBasp:
      
user@~/cheali-charger$ cd src/hardware/atmega32/targets/[your charger]
user@~/cheali-charger/src/hardware/atmega32/targets/[your charger]$ ./progUSBasp.sh
</pre>
      
Now You should see a "cheali-charger welcome" screen.

nuvoton M0517 - OSX
-------------------
(TODO)

