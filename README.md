Welcome to cheali-charger!
==========================

This project is an alternative firmware for a variety of lipo chargers,  
see [hardware.](README.md#hardware)

Don't use it if You don't need to  
(not everything is implemented yet).  

Any feedback is very welcome!  
http://groups.google.com/group/cheali-charger

Features
--------
- Li-ion, LiPo, LiPo-4.30V, LiPo-4.35V, LiFe:
  - charging
    - terminates charging when the current lowers to 10% of Ic (by default, adjustable in battery settings)
  - fast charging
    - terminates charging when the current lowers to 20% of Ic
  - charging + balancing
  - discharging
  - balancing
  - storage
  - storage + balancing
- NiCd and NiMH:
  - charging, method: -dV/dT
  - discharging
  - cycling
- NiZn:
  - charging
  - fast charging
  - charging + balancing
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
- Memory for 30 batteries
- LogView support
- CALIBRATION!


WARNING
-------
Please [CALIBRATE](README.md#calibration) charger before use!  
Use an external temperature probe  
(if You like your house ;) )

**ALWAYS** connect the power supply **first**, then the battery!

**ALWAYS** disconnect the battery **before** disconnecting the power supply!

Hardware
--------

**WARNING:** You can get two chargers with the same name but different CPUs even if you buy from the same seller.  
 Only chargers with a atmega32 or M0517LBN CPU are supported.

**Atmega32 CPU:**
- G.T. POWER A6-10 200W
- IMAX B6 Charger/Discharger 1-6 Cells
- AC/DC Dual Power B6AC 80W RC Balance Charger/Discharger
- Turnigy A-6-10 200W Balance charger & discharger
- Turnigy Accucel-6 50W 5A Balancer/Charger w/ Accessories
- Turnigy Accucel-8 150W 7A Balancer/Charger
- Turnigy MEGA 400Wx2 Battery Charger/Discharger (800W)
- Thunder T610 Balance Charger/Discharger
- Redox Beta 50W 5A charger (at least later model with yellow soldermask)
- ... many more

**Nuvoton NuMicro M0517LBN CPU:**
- IMAX B6 Charger/Discharger 1-6 Cells

**Unsupported due to old or uncommon CPU See [#106](https://github.com/stawel/cheali-charger/issues/106)**
- [Turnigy Accucel-6 80W Balancer/Charger](https://hobbyking.com/en_us/turnigy-accucel-6-80w-10a-1-6s-balancer-charger-suitable-for-lihv.html)
- IMAX B6 Mini 1-6 60W Balancer/Charger
- IMAX B6 V2 Balancer/Charger
- ... others, do your research or ask on the forum

Usage:
------

After [flashing](docs/flashing.md) your charger the first thing you should do is to 
reset the charger to its default settings ("options"->"reset default")
and then [calibrate](README.md#calibration) it, now your charger is ready to use.

programming you charger:
- select a free battery slot (indicated as 1., 2.,...)
- go to "edit battery"
 - change battery type "Bat:"
 - set battery voltage (number of cells) "V:"
 - set battery capacity "Ch:"
 - set charge current "Ic:"
 - set discharge current "Id:"
 - set time limit "Tlim:" (can be unlimited)
 - press "create name"

charing/discharging...:
- select battery
- select program: "charge", "discharge"...
- you should see a "info" screen,  
  (if you hear beeps, check your battery connections)
- hold "start" button for 2s to start the program
- charger is working now, press "inc", "dec" to see more screens
- to exit the program press "stop"

1. informations about [settings](docs/settings/settings.md)
2. [charging NiMH and NiCd batteries](docs/nimh_nicd_charging.md)

[Flashing](docs/flashing.md)
----------------------------

Calibration
-----------
Connect a NOT fully charged LiPo battery to the main leads  
and the balance port, if you don't own a battery with a  
balance connector, just connect a regular one (~4V)  
to the main leads and the balance port first two [pins](docs/connectors/balancePortPins.jpeg)  
(pin "0" <--> Bat-, pin "1" <--> Bat+). 

go to: "options"->"calibrate":
- voltage calibration: go to "voltage"
   - use a voltmeter to measure voltage on all cells and the power supply voltage (Vin)  
     and set voltage on Vin, Vb1, Vb2, .., Vb6  
      - only Vb1 is mandatory, battery main leads and balance port must be connected
      - you need to change at least one value (this will copy V1-6 voltage to Vbat)
- charge current calibration: 
  - connect your amperemeter in series with the battery, use the 10A(20A) input  
  - disconnect balance port
  - go to "I charge"  
    - go to: "50mA" (100mA on some versions)  
      press "start" button (current flow should be visible on amperemeter)  
      press "Inc", "Dec" buttons until the amperemeter shows 50mA (100mA on some versions)  
      press "start" button to save the setting  
    - go to: "1000mA"  
      press "start" button  
      press "Inc", "Dec" buttons until the amperemeter shows 1000mA  
      press "start" button to save the setting  
      WARNING: the battery will be charged with high current!
- discharge current calibration: go to "I discharge"  
    Repeat the same steps as before  
    WARNING: the battery will be discharged with high current!
- when needed: external (or internal) temperature probe calibration: go to "temp extern" ("temp intern")
    You have to set two calibration points

Done.  
If you have any problems with calibration, go to "options"->"reset default" and try again.


[Calibration - Expert (IMAX B6) - optional](docs/calibration_expert.md)
-----------------------------------------------------------------------

[Building from Source](docs/building.md)
----------------------------------------

Troubleshooting
---------------

1. After flashing I see "options" and some strange characters in the second line (for example: squares), what should I do?
  - reset the charger to default settings (go to: "options"->"reset default" and press the "start" button)
2. I get **"calib. error"**: see [this.](docs/calibration_error_codes.md)

**Atmega32 CPU:**

1. After flashing charger doesn't work (display shows squares):
  - download the *.hex again, use the "RAW" button in github
  - check the sha1 sum of the file, compare it with *.sha1:
    - linux: $sha1sum cheali-charger*.hex
    - windows: install [Microsoft File Checksum Integrity Verifier](http://www.microsoft.com/en-us/download/details.aspx?id=11533)
      - in cmd.exe: fciv.exe -sha1 -add cheali-charger-*.hex
2. Sha1 sum is correct and the charger still doesn't work (display shows squares):
  - reset atmega32 fuses using avrdude:
    - windows: avrdude.exe -patmega32 -cusbasp -Uhfuse:w:0xc5:m -Ulfuse:w:0x3f:m
    - linux:   avrdude     -patmega32 -cusbasp -Uhfuse:w:0xc5:m -Ulfuse:w:0x3f:m


Useful materials
----------------
- [Iggnus fork](https://github.com/Iggnus/cheali-charger-i1), branch: [v0.99](https://github.com/Iggnus/cheali-charger-i1/tree/v0.99), [v0.33+](https://github.com/Iggnus/cheali-charger-i1/tree/v0.33+)
- [sasam M0517 flash tools](https://github.com/sasam/M0517_flash_tools)
- [Cheali Charger v1.00 Manual / User Guide](docs/users_docs/Cheali Charger 1.00 User Manual English_draft_002.pdf) (draft 002) by PascalRZ
- Cheali Charger V0.33m - User Guide: [English](https://docs.google.com/document/d/1Nv2vBXbWo6qE2U9rXZfzVDTfWu3j778flImbFJp74tk), [Hungarian](https://docs.google.com/file/d/0B1RXXTatsA1cWVJYbERUSWo5Q28)
- balancer modification, Hungarian: [pdf](http://file.emiter.hu/file/Modellezes/Cheali/Tuning/HK_es_TURNIGY_TOLTO_BALANSZ_tuning_javitott.pdf), [website](http://rc-miskolc.emiter.hu/rc-miskolc/index.php?option=com_content&view=article&id=278&Itemid=205)
- Gyuiri's schematics: [turnigy 2X400](https://drive.google.com/file/d/0B1RXXTatsA1cczlMR184LUVZSkE), [turnigy 2x200](https://drive.google.com/file/d/0B1RXXTatsA1cb1R5NHM3MEtsakE), [turnigy 8150](https://drive.google.com/file/d/0B1RXXTatsA1cbkM2dXFxTldjTUU)
- [Imax B6 Schematic](http://www.rcgroups.com/forums/showatt.php?s=df7049bcbafdb5d7d06765c264e5c4bb&attachmentid=3693125&d=1293732709), [rcgroups](http://www.rcgroups.com/forums/showthread.php?t=1362933) 
- Panasonic [ni-mh-handbook-2014](http://eu.industrial.panasonic.com/sites/default/pidseu/files/downloads/files/ni-mh-handbook-2014_interactive.pdf), Duracell [Ni-MH_Rechargeable_Batteries_2007](http://www6.zetatalk.com/docs/Batteries/Chemistry/Duracell_Ni-MH_Rechargeable_Batteries_2007.pdf)
- [batteryuniversity.com](http://batteryuniversity.com/)
- Atmel [AVR463: Charging Nickel-Metal Hydride Batteries with ATAVRBC100](http://www.atmel.com/Images/doc8098.pdf)
- Atmel [AVR121: Enhancing ADC resolution by oversampling](http://www.atmel.com/images/doc8003.pdf)
- [Programming of different charge methods with the BaSyTec Battery Test System](http://www.basytec.de/applications/charge.pdf)
- Pulse Charging NiMh Batteries [Candle Power Forums](http://www.candlepowerforums.com/vb/showthread.php?312094-Pulse-Charging-NiMh-Batteries&p=3609673&viewfull=1#post3609673)
- [9 stages Charge algorithm](http://www.hkwp.com/2006/ASIA/web/specification.html)

Mailing list
------------

If you have any questions or suggestions please write to us at: cheali-chargerATgooglegroups.com  
or visit: http://groups.google.com/group/cheali-charger  
The mailing list is open for all.

Have fun!
