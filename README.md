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
- Li-ion, LiFe, LiPo, LiPo-4.30V (LiHV), LiPo-4.35V (LiHV):
  - charge, fast charge, charge + balance, discharge, balance, storage, storage + balance, capacity check
  - editable: charge/storage/discharge end voltage, charge/discharge current, capacity
  - capacity limit, time limit, temperature limit
  - displays internal resistance during charge
- NiCd and NiMH:
  - constant current charge (method: -dV, dT/dt), discharge, cycling, capacity check
  - editable: charge/discharge cutoff voltage, charge/discharge current, dV, dT/dt
  - capacity limit, time limit, temperature limit
- NiZn:
  - charge, fast charge, charge + balance, discharge, balance, capacity check
  - editable: charge/discharge end voltage, charge/discharge current, capacity
  - capacity limit, time limit, temperature limit
- Pb:
  - charge, discharge, cycling, capacity check
  - editable: charge/discharge end voltage, charge/discharge current, capacity
  - capacity limit, time limit, temperature limit
- LED (light-emitting diode):
  - constant current source
  - editable: current (on the fly), cutoff voltage
- Supports:
  - internal temperature monitoring (if supported by hardware)
  - power supply voltage monitoring
  - UART logging - [LogView](docs/Logview.md) compatible
  - Memory for 20 batteries
  - calibration


WARNING
-------
Please [calibrate](docs/usage/calibration/calibration.md) charger before use!  
Use an external temperature probe  
(if You like your house ;) )

**ALWAYS** connect power supply **first**, then the battery! disconnect the battery **before** disconnecting power supply!

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
- ... many more

**Nuvoton NuMicro M0517LBN CPU:**
- IMAX B6 Charger/Discharger 1-6 Cells

**Unsupported**
- [Turnigy Accucel-6 80W Balancer/Charger](http://www.hobbyking.com/hobbyking/store/__64345__Turnigy_Accucel_6_80W_10A_Balancer_Charger_LiHV_Capable.html) - Based on an older, uncommon CPU. See [#106](https://github.com/stawel/cheali-charger/issues/106)
- any charger listed above with an **unknown CPU** (**CPU not labeled** - probably Z51F3220)
- any charger with an **mcs51 (8051) family CPU** (MA84G564)

Usage:
------

After [flashing](docs/flashing.md) your charger the first thing you should do is  
reset the charger to default settings (go to: "options"->"reset default" and press the "start" button)  
and then [calibrate](docs/usage/calibration/calibration.md) it, now your charger is ready to [charge](docs/usage/README.md).

[Flashing](docs/flashing.md)
----------------------------

[Calibration](docs/usage/calibration/calibration.md)
----------------------------------------

[Charging](docs/usage/charings.md)
----------------------------------

[Battery settings](docs/usage/battery_settings.md)
--------------------------------------------------

[Settings](docs/usage/settings.md)
----------------------------------

[Building from Source](docs/building.md)
----------------------------------------

[Troubleshooting](docs/troubleshooting.md)
------------------------------------------

Useful materials
----------------
- [Iggnus fork](https://github.com/Iggnus/cheali-charger-i1), branch: [v0.99](https://github.com/Iggnus/cheali-charger-i1/tree/v0.99), [v0.33+](https://github.com/Iggnus/cheali-charger-i1/tree/v0.33+)
- [njozsef fork](https://github.com/njozsef/cheali-charger-test1)
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
