# cheali-charger
Alternative firmware for various LiPo chargers. [Supported Hardware](README.md#hardware)

**Note:** Not fully implemented yet. Only use if needed.

Feedback is welcome: [cheali-charger Google Group](http://groups.google.com/group/cheali-charger)

## Features
- **Li-ion, LiPo, LiHV, LiFe:**
  - Charging (terminates at 10% of Ic, adjustable)
  - Fast charging (terminates at 20% of Ic)
  - Charging + balancing
  - Discharging
  - Balancing
  - Storage
  - Storage + balancing
- **NiCd and NiMH:**
  - Charging (-dV/dT method)
  - Discharging
  - Cycling
- **NiZn:**
  - Charging
  - Fast charging
  - Charging + balancing
  - Discharging
  - Balancing
- **Pb (not well tested):**
  - Charging
  - Discharging
- **Other:**
  - Internal resistance display
     - Single cell resistance
     - Whole battery resistance
     - Battery leads resistance
  - Internal/external temperature monitoring
  - Overcharge/discharge monitoring
  - Input voltage monitoring
  - Memory for 30 batteries
  - LogView support

## Warnings
- [Calibrate](README.md#calibration) charger before use.
- Use an external temperature probe (if you like your house ;) )
- **ALWAYS** Connect power supply **first**, then battery.
- **ALWAYS** Disconnect battery **before** disconnecting power supply.

Hardware
--------

**WARNING:** You can get two chargers with the same name but different CPUs even if you buy from the same seller.  
 Only chargers with a ATmega32 or M0517LBN CPU are supported.

**ATmega32 CPU:**
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


## Usage
1. **Flash** your charger and reset to default settings ("options" -> "reset default").
2. Then [calibrate](README.md#calibration) it, now your charger is ready to use.

### Programming Charger:
- Select a battery slot (1., 2., ...)
- Go to "Edit Battery":
 - Change battery type "Bat:"
 - Set battery voltage (number of cells) "V:"
 - Set battery capacity "Ch:"
 - Set charge current "Ic:"
 - Set discharge current "Id:"
 - Set time limit "Tlim:" (can be unlimited)
 - Press "create name"


### Charging/Discharging:
- Select battery.
- Choose program: "Charge", "Discharge", etc.
- You should see an "Info" screen.
  (If you hear beeps, check your battery connections)
- Hold "Start" for 2s to begin.
- Use "inc"/"dec" to navigate screens.
- Press "Stop" to exit.

1. Information about [settings](docs/settings/settings.md)
2. [Charging NiMH and NiCd batteries](docs/nimh_nicd_charging.md)


### Calibration 

**Important:** Do NOT connect a fully charged LiPo battery to both the main leads and the balance port. 
If you don't have a battery with a balance connector, use a regular one (~4V). Connect it to the main leads and the first two pins of the balance port [pins](docs/connectors/balancePortPins.jpeg) 
(pin "0" to Bat-, pin "1" to Bat+).

**Steps:**

1. **Voltage Calibration:**
   - Go to: "options" -> "calibrate" -> "voltage".
   - Measure the voltage on all cells and the power supply voltage (Vin) using a voltmeter.
   - Set the voltage for Vin, Vb1, Vb2, ..., Vb6.
     - Only Vb1 is mandatory. The battery main leads and balance port must be connected.
     - Change at least one value (this will copy V1-6 voltage to Vbat).

2. **Charge Current Calibration:**
   - Connect your ammeter in series with the battery, using the 10A(20A) input.
   - Disconnect the balance port.
   - Go to: "I charge".
     - For "50mA" (or "100mA" on some versions):
       - Press the "start" button (current flow should be visible on the ammeter).
       - Use the "Inc" and "Dec" buttons until the ammeter shows 50mA (or 100mA on some versions).
       - Press the "start" button to save the setting.
     - For "1000mA":
       - Press the "start" button.
       - Use the "Inc" and "Dec" buttons until the ammeter shows 1000mA.
       - Press the "start" button to save the setting.
       - **WARNING:** The battery will be charged with high current!

3. **Discharge Current Calibration:**
   - Go to: "I discharge".
   - Repeat the same steps as in the charge current calibration.
   - **WARNING:** The battery will be discharged with high current!

4. **Temperature Probe Calibration (if needed):**
   - Go to: "temp extern" (or "temp intern").
   - Set two calibration points.

**Done.**

If you encounter any problems with calibration, go to "options" -> "reset default" and try again.


## Troubleshooting

1. After flashing, I see "options" and some strange characters (e.g., squares) on the second line. What should I do?
    - Reset the charger to default settings: Go to "options" -> "reset default" and press the "start" button.
  
2. I get **"calib. error"**. See [this guide](docs/calibration_error_codes.md).

3. Occasionally, when the power is turned on, an "eeprom error: 2 yes" (or any other number) pops up. See [issue #291](https://github.com/stawel/cheali-charger/issues/291).

4. After flashing, the charger doesn't work (display shows squares):
    - Download the *.hex file again using the "RAW" button on GitHub.
    - Check the SHA-1 sum of the file and compare it with the *.sha1:
        - Linux: `$ sha1sum cheali-charger*.hex`
        - Windows: Install [Microsoft File Checksum Integrity Verifier](http://www.microsoft.com/en-us/download/details.aspx?id=11533)
            - In cmd.exe: `fciv.exe -sha1 -add cheali-charger-*.hex`

5. [Atmega32 CPU] If the SHA-1 sum is correct and the charger still doesn't work (display shows squares):
    - Reset Atmega32 fuses using avrdude:
        - Windows: `avrdude.exe -patmega32 -cusbasp -Uhfuse:w:0xc5:m -Ulfuse:w:0x3f:m`
        - Linux: `avrdude -patmega32 -cusbasp -Uhfuse:w:0xc5:m -Ulfuse:w:0x3f:m`


## Additional Resources
- [Flashing](docs/flashing.md)
- [Calibration - Expert (IMAX B6)](docs/calibration_expert.md)
- [Building from Source](docs/building.md)
- [Troubleshooting](docs/troubleshooting.md)
- [User Guides and Documentation](docs/users_docs)

### Useful materials
- [Iggnus fork](https://github.com/Iggnus/cheali-charger-i1), branch: [v0.99](https://github.com/Iggnus/cheali-charger-i1/tree/v0.99), [v0.33+](https://github.com/Iggnus/cheali-charger-i1/tree/v0.33+)
- [sasam M0517 flash tools](https://github.com/sasam/M0517_flash_tools)
- [Cheali Charger v1.00 Manual / User Guide](<docs/users_docs/Cheali Charger 1.00 User Manual English_draft_002.pdf>) (draft 002) by PascalRZ
- Cheali Charger V0.33m - User Guide: [English](https://docs.google.com/document/d/1Nv2vBXbWo6qE2U9rXZfzVDTfWu3j778flImbFJp74tk), [Hungarian](https://docs.google.com/file/d/0B1RXXTatsA1cWVJYbERUSWo5Q28)
- balancer modification, Hungarian: [pdf](http://file.emiter.hu/file/Modellezes/Cheali/Tuning/HK_es_TURNIGY_TOLTO_BALANSZ_tuning_javitott.pdf), [website](http://rc-miskolc.emiter.hu/rc-miskolc/index.php?option=com_content&view=article&id=278&Itemid=205)
- Gyuiri's schematics: [turnigy 2X400](https://drive.google.com/file/d/0B1RXXTatsA1cczlMR184LUVZSkE), [turnigy 2x200](https://drive.google.com/file/d/0B1RXXTatsA1cb1R5NHM3MEtsakE), [turnigy 8150](https://drive.google.com/file/d/0B1RXXTatsA1cbkM2dXFxTldjTUU)
- [Imax B6 Schematic](docs/imaxB6/imax_b6_hobbyking_eco06_digital_charger.pdf) from: [rcgroups](https://www.rcgroups.com/forums/showthread.php?827821-iMax-B6-service-menu/page21) 
- Panasonic [ni-mh-handbook-2014](http://eu.industrial.panasonic.com/sites/default/pidseu/files/downloads/files/ni-mh-handbook-2014_interactive.pdf), Duracell [Ni-MH_Rechargeable_Batteries_2007](http://www6.zetatalk.com/docs/Batteries/Chemistry/Duracell_Ni-MH_Rechargeable_Batteries_2007.pdf)
- [batteryuniversity.com](http://batteryuniversity.com/)
- Atmel [AVR463: Charging Nickel-Metal Hydride Batteries with ATAVRBC100](http://www.atmel.com/Images/doc8098.pdf)
- Atmel [AVR121: Enhancing ADC resolution by oversampling](http://www.atmel.com/images/doc8003.pdf)
- [Programming of different charge methods with the BaSyTec Battery Test System](http://www.basytec.de/applications/charge.pdf)
- Pulse Charging NiMh Batteries [Candle Power Forums](http://www.candlepowerforums.com/vb/showthread.php?312094-Pulse-Charging-NiMh-Batteries&p=3609673&viewfull=1#post3609673)
- [9 stages Charge algorithm](http://www.hkwp.com/2006/ASIA/web/specification.html)

### Mailing list

If you have any questions or suggestions please write to us at: cheali-chargerATgooglegroups.com  
or visit: http://groups.google.com/group/cheali-charger  
The mailing list is open for all.

Have fun!
