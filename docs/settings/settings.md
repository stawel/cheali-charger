
settings
========

general settings:
-----------------

| name                                           | description     | charger type|
|:-----------------------------------------------|:----------------|------------:|
|backlight: *X*                                  | LCD backlight | 200W/400W   |
|fan on: *X*°C                                   | turn on fan when chargers internal temperature is higher than *X*°C |  200W/400W |
|disch off: *X*°C<br>![disch off:](disch_off.png)| pause discharging when chargers internal temperature is higher than *X*°C | 200W/400W, M0517 see 1.|
|extrn T: *yes/no*<br>![extrn T:](extrn_T.png)   | enable/disable external temperature probe, used in: "extrn TCO:", "dT/dt:"| all| 
|extrn TCO: *X*°C<br>![extrn TCO:](extrn_TCO.png)| external temperature cut-off, stop program when ext. temp. is higher than *X*°C, see also: "extrn T:" | all| 
|beep: *yes/no*<br>![beep:](beep.png)            | enable/disable sound | all|
|cap COff: *X*%<br>![cap COff:](cap_COff.png)    | capacity cut-off, stop charging/discharging when charge reaches *X*% of batteries capacity| all |
|input low: *X*V<br>![input low:](input_low.png) | stop program when input voltage (power supply voltage) is lower than *X*| all | 
|reset<br>![reset](reset.png)                    | reset all setting to default | all |


1. chargers with nuvoton-M0517 CPU don't have a dedicated internal temperature probe, we use the CPUs temperature. 


NiMH, NiCd specific settings:
-----------------------------


| name                                            | description | charger type|
|-------------------------------------------------|-------------|------------:|
|dT/dt: *X*°C/m<br>![dT/dt:](dT_dt.png)           | stop charging when external temperature increases more than *X*°C per minute, see also: "extrn T:"| all |
|enab dV: *yes/no*<br>![enab dV:](enab_dV.png)    | enable ΔV method to determine end of charging for NiMH and NiCd, should be always "yes"!| all |
|NiMH dV: *X*mV<br>![NiMH dV:](NiMH_dV.png)       | ΔV value for NiMH (per cell), see also: "enab dV:"| all |
|NiCd dV: *X*mV<br>![NiCd dV:](NiCd_dV.png)       | ΔV value for NiCd (per cell), see also: "enab dV:"| all |
|D/C cycles: *N*<br>![D/C cycles:](D_C_cycles.png)| number of discharges/charges when cycling (formatting) battery| all |
|D/C rest: *t*m<br>![D/C rest:](D_C_rest.png)     | rest time between discharge and charge during cycling in minutes| all |
|adc noise: *yes/no*                              | add *noise* to batteries main-leads input connector, see 1,2. | atmega32 based chargers |

1. see also [NiMH, NiCd charging](../nimh_nicd_charging.md).
2. see also [artificial adc noise](../artificial_adc_noise.md).


LiXX, NiZn, Pb specific settings:
---------------------------------

| name                                              | description          | affects batteries |
|---------------------------------------------------|----------------------|------------------:|
|min Iout: I/*X*<br>![min Iout: I/](min_Iout_I.png) | stop charging (discharging) when current is lower than Ic/*X* (Id/*X*), see 1., 2.| all see 3.|
|min Iout: *Y*mA<br>![min Iout:](min_Iout.png)      | minimum allowed output current, see 4.| all |
|o.charge: *X*mV<br>![o.charge:](o_charge.png)      | over-charge LiXX battery, for lipo endV = 4.20V + *X* mV (per cell) | LiXX, see 5.| 
|o.disch: *X*mV<br>![o.disch:](o_disch.png)         | over-discharge LiXX battery, for lipo endV = 3.00V + *X* mV (per cell) | LiXX, see 5.|
|dis agres: *yes/no*<br>![dis agres:](dis_agres.png)| aggressive discharging:<br>*off*: stop discharging immediately after battery reaches endV<br>*on*: when battery reaches endV, go down with the discharge current, see "min Iout: I/*X*" and 2.| all |
|force bal: *yes/no*<br>![force bal:](force_bal.png)| force user to connect balance port before starting any program | LiXX, see 5.|
|bal. err: *X*mV<br>![bal. err:](bal_err.png)       | acceptable voltage error between cells when balancing | LiXX, see 5.|

1. minimum allowed charge current is equal: =*max*("min Iout: Ic/*X*", "min Iout: *Y*mA")  
   where: Ic - is batteries charge current ("edit battery"->"Ic:")
2. minimum allowed discharge current is equal: =*max*("min Iout: Id/*X*", "min Iout: *Y*mA")  
   where: Id - is batteries discharge current ("edit battery"->"Id:")  
   "dis agres:" should be set to: *yes*
2. affects: charge/discharge of LiXX, Pb, NiZn and discharge of NiMH, NiCd
3. the chargers design doesn't allow to deliver less current than a certain value,  
   (current measuring op-amp is not "rail-to-rail")  
   You can measure this value in "option"->"calibrate"->"I charge"->"100mA":
   - press "inc","dec" until measured current is the lowest possible but bigger than 0mA.
   - press "stop" to exit (don't save this setting!) 
4. LiXX := Lipo, Life, Lilo, L430, L435, see [ProgramData.cpp](../../src/core/ProgramData.cpp#L50-L54)

UART settings:
--------------
| name                                  | description | charger type|
|---------------------------------------|-------------|------------:|
|UART:<br>![UART:](UART.png)            | enable UART, should be "disabled" or "normal" | all|
|speed:<br>![speed:](speed.png)         | UART speed | all |
|UART inp:<br>![UART inp:](UART_inp.png)| UART input:<br>*temp*: temperature connector<br> *pin7*: M0517 pin 7 (charger needs modification)| M0517 |



