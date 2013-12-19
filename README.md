Welcome to cheali-charger!

This project is an alternative firmware for the
G.T. POWER A6-10 200W
or IMAX B6
Lipo charger.

Don't use it if You don't need to  
(not everything is implemented yet).  

Any feedback is very welcome!  
https://github.com/stawel/cheali-charger

Features
--------
- Li-ion, LiPo and LiFe:
  - charging
  - fast charging
  - charging + balancing at the same time
  - discharging
  - balancing
  - storage
  - storage + balancing
- NiCd and NiMH:
  - charging, method: -dV/dT
  - discharging
  - cycling (not implemented yet)
- NiZn: - not well tested!
  - charging
  - fast charging
  - charging + balancing at the same time
  - discharging
  - balancing
- Pb: - not well tested!
  - charging
  - discharging
- Internal resistance display
  - single cell resistance
  - whole battery resistance
  - battery leads resistance
- Internal and external temperature monitoring
- Overcharge and overdischarge monitoring
- Input voltage monitoring
- Memory for 32 batteries
- LogView support
- CALIBATION!


WARNING
-------
Please CALIBRATE the charger before use!  
Use an external temperature probe.  
(if You like your house ;) )

Hardware
--------
- G.T. POWER A6-10 200W (http://www.hobbyking.com/hobbyking/store/__7386__GT_A_6_10_200W_Balance_charger_discharger.html)
- IMAX B6 Charger/Discharger 1-6 Cells:
    - original:http://www.hobbyking.com/hobbyking/store/__5548__IMAX_B6_Charger_Discharger_1_6_Cells_GENUINE_.html
    - clone:http://www.hobbyking.com/hobbyking/store/__15379__IMAX_B6_Charger_Discharger_1_6_Cells_COPY_.html
- .... - any suggestions are very welcome


Building from Source
--------------------
dependencies: cmake, avrdude, avr-libc, gcc-avr

<pre>
user@~$ sudo apt-get install cmake avrdude avr-libc gcc-avr git

user@~$ git clone https://github.com/stawel/cheali-charger.git
user@~$ cd cheali-charger

user@~/cheali-charger$ ./bootstrap
user@~/cheali-charger$ make

flashing GTPowerA6-10 with USBasp:

user@~/cheali-charger$ cd src/hardware/GTPowerA6-10-original/
user@~/cheali-charger/src/hardware/GTPowerA6-10-original$ ./progUSBasp.sh

flashing imaxB6-original with USBasp:

user@~/cheali-charger$ cd src/hardware/imaxB6-original/
user@~/cheali-charger/src/hardware/imaxB6-original$ ./progUSBasp.sh

flashing imaxB6-clone with USBasp:

user@~/cheali-charger$ cd src/hardware/imaxB6-clone/
user@~/cheali-charger/src/hardware/imaxB6-clone$ ./progUSBasp.sh


</pre>

Now You should see a "ChealiCharger welcome" screen.

Done.

Flashing: G.T. POWER A6-10 200W
-------------------------------

- Connect a standard AVR ISP 10-pin cable to the board: https://github.com/stawel/cheali-charger/blob/master/docs/GTPowerA6-10/front.jpg
- set "Device" to: atmega32
- use the cheali-charger/hex/cheali-charger-GTPowerA6-10-0.32.hex file as "Flash"


Flashing: IMAX B6
-------------------------------

- Connect a AVR ISP to the board, make sure that you have the appropriate connector!
    - IMAX B6 - original: https://github.com/stawel/cheali-charger/blob/master/docs/imaxB6/imaxB6-original-front.jpg
    - IMAX B6 - clone: https://github.com/stawel/cheali-charger/blob/master/docs/imaxB6/imaxB6-clone-front.jpg
- set "Device" to: atmega32
- use the cheali-charger/hex/cheali-charger-imaxB6-original-0.32.hex  (or cheali-charger-imaxB6-clone-0.32.hex) file as "Flash"


Calibration (IMAX B6 and G.T. POWER A6-10 200W)
-----------
Connect a NOT fully charged LiPo battery to the main leads
and the balance port.

go to: "options"->"calibrate":
- voltage calibration: go to "voltage"
   - use a voltmeter to measure the voltage of all cells and the power supply voltage (Vin)  
     and set the values in the appropriate fields.
- charge current calibration: go to "I charge"  
  connect your ampere meter in series with the battery on the red wire! (+)  
  - go to: "50mA"  
    press "Inc", "Dec" buttons until the amperemeter shows 50mA  
    press "start" button  
  - go to: "1000mA"  
    press "Inc", "Dec" buttons until the amperemeter shows 1000mA  
    press "start" button  
    WARNING: the battery will be charged with high current!
- discharge current calibration: go to "I discharge"  
    Repeat the same steps as before  
    WARNING: the battery will be discharged with high current!
- when needed: external (internal) temperature probe calibration: go to "temp extern" ("temp intern")
    You have to set two calibration points

Done.

Calibration - Expert (IMAX B6)
-----------
If you want to improve the accuracy of the balancing on the first two cells you can try  
to calibrate the voltage on the first three pins of the balance port.
BE AWARE THAT ON SOME CHARGERS THIS CAN BE DANGEROUS!!!

First check if you can safely proceed by using the following test:

- disconnect everything from the charger
- measure the resistance between the BLACK battery terminal and Pin "-" on the balance port  
  (this is the furthest pin from the battery terminals and is  
  common to all the balance port connectors - it is also the most negative terminal of the battery)
- if the resistance is very low (less than 1k Ohm) STOP THIS PROCEDURE!

If the measured resistance is much more than 1k Ohm you can proceed as follows:

- connect the power supply to the ImaxB6 and go to "options" -> "calibrate" -> "expert DANGER!"
- connect a 1.5V battery and a resistor (1k Ohm to 10kOhm) in the following manner:  
  BLACK battery terminal of ImaxB6 to one end of the resistor, the other end of the resistor  
  to the negative pole of the battery, and the positive pole of the battery to Pin "-" (as above) on the balance port
- with a multimeter measure the voltage between the BLACK battery terminal of ImaxB6 and Pin "-" on the balance port.
  Adjust "Vb0pin" to this value using the inc/dec buttons
- disconnect the 1.5V battery
- connect a 2-cell (or more) LiPo or LiIon battery to the charger  
  (connecting both the main battery terminals and the balance port connections)
- measure the voltage between the BLACK battery terminal and Pin "1" (next to pin "-") on the balance port,  
  and set the appropriate values on "Vb1pin"
- measure the voltage between the BLACK battery terminal and Pin "2" on the balance port, and set the appropriate values on "Vb2pin"
- the Expert Calibration is now completed - now proceed with the normal calibration


Troubleshooting
---------------


Mailing list
------------
If you have any questions or suggestions please write to us at: cheali-chargerATgooglegroups.com  
or visit: http://groups.google.com/group/cheali-charger  
The mailing list is open for all.


Have fun!



