
Calibration - Expert (IMAX B6) - optional
-----------------------------------------

If you want to improve the accuracy of the balancing on the first two cells you can try..
to calibrate the voltage on the first three pins of the balance port.
BE AWARE THAT ON SOME CHARGERS THIS CAN BE DANGEROUS!!!

First check if you can safely proceed by using the following test:

- disconnect everything from the charger
- measure the resistance between GND and Pin "0" on the balance port..
  (this is the furthest pin from the battery terminals and is..
  common to all balance port connectors)
  you can use GND from ISP connector (programmer connector) or any other GND on the charger
- if the resistance is very low (less than 1k Ohm) STOP THIS PROCEDURE!

If the measured resistance is much higher than 1k Ohm you can proceed as follows:

- connect the power supply to the ImaxB6 and go to "options" -> "calibrate" -> "expert DANGER!"
  - connect a 1.5V battery and a resistor (1k Ohm to 10kOhm) in the following manner:..
    connect ImaxB6 GND to one end of the resistor, the other end to the negative pole of the battery,..
    and the positive pole of the battery to Pin "0" (as above) on the balance port..
    you can use GND from ISP connector (programmer connector)
  - with a multimeter measure the voltage between GND and Pin "0" on the balance port.
    Adjust "Vb0pin" to this value using the inc/dec buttons
  - disconnect the 1.5V battery
- repeat the same steps as before for Pin "1" (balance port first cell)..
  you can use a higer voltage battery ~4.20V
- repeat the same steps as before for Pin "2" (balance port seccond cell)..
  you can use a higer voltage battery ~8.40V
- you can repeat the same steps for the batteries (-) terminal..
  for that you should use a 1.5V battery

