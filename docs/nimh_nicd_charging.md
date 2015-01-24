

charging NiMH, NiCd
===================

Cheali-charger uses the ΔV method and the dT/dt method (if external temperature probe is enabled) 
to determine when a battery is fully charged,
in order for these methods to be effective, the charge current must be no less than 0.5C, see 
[NiMH charging](http://en.wikipedia.org/wiki/Nickel%E2%80%93metal_hydride_battery#Charging).

WARNING: make sure your battery is suitable for high current charging.


charging with small current
---------------------------

If the charging current is too small (for example 0.1C) it is very likely that cheali-charger will not
be able to determine the end of charge, in this case you should set a appropriate charging time limit (`edit battery`->`Tlim:`)
and capacity cut-off limit (see [settings](settings/settings.md#general-settings)).


ΔV method and atmega32
----------------------

In the ΔV method we are trying to measure a voltage drop of about -5mV (per cell) on output.

![dV charge](LogView/dV_charge_2xNiMH.png)

The atmega32 ADC resolution is only 10bit and we are measuring a voltage span of 30V,
so the voltage resolution is about: 30V/1024 = 29mV, this might be not enough for the ΔV method to work.
Cheali-charger is trying to enhance the ADC resolution by using a method called [‘Oversampling and
Decimation’](http://www.atmel.com/images/doc8003.pdf). This method requires a noisy signal at input,
although the charger itself generates some noise, the noise level might be not enough,
for this reason on atmega32 based chargers it is recommended to 
enable artificial noise in [settings](settings/settings.md#nimh-nicd-specific-settings) (`adc noise: yes`), 
see also [artificial adc noise](artificial_adc_noise.md).


charging wires resistance: `Rwire`
----------------------------------
To minimize the chance of overcharging cheali-charger uses a charging voltage limit of 1.6V (per cell) for NiMH batteries 
and 1.82V (per cell) for NiCd, if this limit gets exceeded the charging program stops with a message: `V limit`.
Since the limit is a safety feature in normal use you shouldn't see this message. If you do, it might be due to
a voltage drop on your charging wires, in this case you can compensate the wires resistance by setting `Rwire:` in 
[settings](settings/settings.md#nimh-nicd-specific-settings).

If you set `Rwire` the battery voltage will be equal to:  
`Vbat = Vout - Rwire * Ic`  
and the voltage limit will refer to `Vbat`.  

note: `Rwire` is only used during NiMH, NiCd charging.


measuring charging wires resistance
-----------------------------------

1. go to `edit battery` and set:
  - battery type to unknown (`Bat: Unkn`)
  - voltage to 1V (`V: 1.000V`)
  - charging current to 1A (`Ic: 1000mA`)
2. connect your charging wires to the charger and connect the end of them to each other (make a loop).
3. go to `charge` and hold `start` for 2 seconds (ignore beeping)
4. wait until current reaches 1A
5. go to `batt. R=`, this is the wires resistance








