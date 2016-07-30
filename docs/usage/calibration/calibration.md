
# Calibration

To calibrate your charger you will need a **NOT fully charged** LiPo battery.  
First you should perform the [voltage calibration](calibration.md#voltage-calibration) and then [current calibration](calibration.md#current-calibration).  
External/Internal temperature calibration is usually not required.

If you have any problems with calibration, go to ![options](i_options.png) ![reset default](i_reset_default.png) and try again, this will reset your eeprom to factory settings.

## Voltage calibration

- go to: ![options](i_options.png) ![calibrate](i_calibrate.png) ![voltage](i_voltage.png)
- connect a LiPo battery to the main leads and balance port  
   *note: if you don't have a battery with a balance port just connect a regular one (<4.5V)
to the main leads and the balance port first two [pins](../../hardware/connectors/balancePortPins.jpeg) (pin "0" and pin "1")*
- measure power supply voltage and set it into: ![Vin](i_vin.png)
- measure 1..6 cell voltage and set it into: ![Vb1](i_vb1.png) .. ![Vb6](i_vb6.png)  
   *note: if you don't have a 6 cell battery, just use one with less cells*
- now you should see the same voltage in "V1-6" (balance port voltage) and "Vout" (main leads voltage):  
  ![V1-6:, Vout:](i_vb1-6_vout.png)  
  *note: you need to change at least one value (this will copy the "V1-6" voltage to "Vout")*

voltage calibration done.

*note: you will achieve the best results when battery is fully charged, using a NOT fully charged battery only is mandatory for current calibration.*

## Current calibration

### charge current calibration

- connect a amperemeter in series with a **NOT fully charged** battery to the main leads, use the 10A(20A) input  
 *note: balance port needs to be disconnected*  
 *note: battery will be charged without checking the output voltage, make sure battery is not fully charged*
- go to: ![options](i_options.png) ![calibrate](i_calibrate.png) ![I charge](i_Icharge.png)
  - go to: ![1000mA](i_1000mA.png)  
    press "start" button (current flow should be visible on amperemeter)  
    press "Inc", "Dec" buttons until the amperemeter shows 1000mA  
    press "start" button to save the setting  
    *WARNING: the battery will be* **charged** *with high current!*
  - go to: ![100mA](i_100mA.png) (50mA on some chargers)  
    press "start" button (current flow should be visible on amperemeter)  
    press "Inc", "Dec" buttons until the amperemeter shows 100mA (50mA on some versions)  
    press "start" button to save the setting  
    *note: you may want to repeat this point with amperemeter connected to the 300mA input*

### discharge current calibration

- go to: ![I discharge](i_Idischarge.png)  
    Repeat the same steps as before, this time for 300mA and 100mA (50mA) current  
    *WARNING: the battery will be* **discharged** *with high current!*

current calibration done.

*note: it is possible to change the current calibration points: 1000mA and 100mA,
for that you have to set ![I expected](i_Iexpected.png) and calibrate "value:" according to it.*

## Internal temperature calibration - optional

- go to: ![options](i_options.png) ![calibrate](i_calibrate.png) ![temp intern](i_temp_internal.png)
- set ![calibration point 0](i_cp0.png) to 0
  - set ![temp20](i_temp20.png) to your room temperature
- set ![calibration point 1](i_cp1.png) to 1
  - heat put your charger with a heat-gun to about 60°C
  - set ![temp60](i_temp60.png) accordingly

## External temperature calibration - optional

- connect your temperature sensor to the balance port
- go to: ![options](i_options.png) ![calibrate](i_calibrate.png) ![temp extern](i_temp_external.png)
- set ![calibration point 0](i_cp0.png) to 0
  - set ![temp20](i_temp20.png) to your room temperature
- set ![calibration point 1](i_cp1.png) to 1
  - heat put your temperature proble with a heat-gun to about 60°C (your body temperature will also do the job)
  - set ![temp60](i_temp60.png) accordingly

## [Expert voltage calibration](calibration_expert.md) - optional


# Calibration - all settings
|  | description | 
|:------|:--------------|
|**voltage calibration** |
|![Vin](i_vin.png)| power supply voltage |
|![Vb1](i_vb1.png)| voltage on cell 1|
| ... | |
|![Vb6](i_vb6.png)| voltage on cell 6|
|![V1-6](i_v1-6.png)| balance port voltage|
|![Vout](i_vout.png)| main leads voltage|
|![calibration point](i_cp1_red.png)| current calibration point (see: 1,2)|
|**charge current calibration**| |
|![value:](i_value.png)| internal current value (PID set point or PWM)|
|![I:](i_I.png)| measured current: real value (see 3), ADC value |
|![Iexpec:](i_Iexpected.png)| expected current for the given *internal current value* |
|**discharge current calibration**| |
|![value:](i_value.png)| internal current value (PID set point or PWM)|
|![I:](i_I.png)| measured current: real value (see 3), ADC value |
|![Iexpec:](i_Iexpected.png)| expected current for the given *internal current value* |
|**external temperature calibration**| |
|![temp:](i_temp20.png)| external probe temperature (see 3)|
|![adc:](i_adc.png)| external probe measured ADC value |
|![calibration point](i_cp0.png)| current calibration point |
|**internal temperature calibration**| (see 4) |
|![temp:](i_temp20.png)| internal temperature (see 3)|
|![adc:](i_adc.png)| internal temperature measured ADC value |
|![calibration point](i_cp0.png)| current calibration point |
|**expert voltage calibration**| |
|![Vb0pin](i_vb0pin.png)| voltage between [pin "0"](../../hardware/connectors/balancePortPins.jpeg) and [GND](../../hardware/connectors/isp_imaxB6.jpeg) (see 5) |
|![Vb1pin](i_vb1pin.png)| voltage between [pin "1"](../../hardware/connectors/balancePortPins.jpeg) and [GND](../../hardware/connectors/isp_imaxB6.jpeg) (see 5) |
|![Vb2pin](i_vb1pin.png)| voltage between [pin "2"](../../hardware/connectors/balancePortPins.jpeg) and [GND](../../hardware/connectors/isp_imaxB6.jpeg) (see 5) |
|![Vplus](i_vplus.png)| voltage between main lead red wire and [GND](../../hardware/connectors/isp_imaxB6.jpeg) |
|![Vminus](i_vplus.png)| voltage between main lead black wire and [GND](../../hardware/connectors/isp_imaxB6.jpeg) |
|![calibration point](i_cp1_red.png)| current calibration point (see: 1,2)|

1. all settings with red border are visible in [advanced menus](advanced menus.md)
2. all voltages are set to 0 volts at calibration point 0, calibration should be performed at point 1
3. value taking into account calibration
4. only on chargers with internal temperature probe, on nuvoton M0517 it's the CPUs temperature
5. only on chargers with simplified cell 1,2 measuring circuit
