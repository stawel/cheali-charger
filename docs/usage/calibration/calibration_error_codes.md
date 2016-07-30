
Calibration error codes:
-----------------------------------------

If you see **calib. error** try to calibrate your charger again,  
if that doesn't help try the following solutions:


for: **I charge "nr"**

- **"nr"** = 1,11 - increase the minimum charge current: "settings"->"min Ic:"
- **"nr"** = 2,3,5,6,7,12,13,15,16,17 - calibrate "I charge" again
- **"nr"** = 4,14 - decrease the maximum charge current: "settings"->"max Ic:"


for: **I discharge "nr"**

- **"nr"** = 1,11 - increase the minimum discharge current: "settings"->"min Id:"
- **"nr"** = 2,3,5,6,7,12,13,15,16,17 - calibrate "I discharge" again
- **"nr"** = 4,14 - decrease the maximum discharge current: "settings"->"max Id:"


### determine minimum (maximum) charge current
The simplest way to find out your minimum (maximum) charge current is  
to note down the **value** and **ADC** readings during "I charge" calibration:

**I charge: 100mA**
```
value:        [value1]
I:    100mA   [ADC1]
```

**I charge: 1000mA**
```
value:        [value2]
I:    1000mA  [ADC2]
```

use wolfram alfa to determine the minimum current:

1.  [script for value1 and value2](http://www.wolframalpha.com/input/?i=%7BFit%5B%7B%7Bvalue1,100%7D,+%7Bvalue2,1000%7D%7D,%7B1,x%7D,x%5D+,+x%3D1%7D),
     replace **value1** and **value2** with your readings
2.  [script for ADC1 and ADC2](http://www.wolframalpha.com/input/?i=%7BFit%5B%7B%7BADC1,+100%7D,+%7BADC2,+1000%7D%7D,%7B1,x%7D,x%5D+,+x%3D1%7D),
     replace **ADC1** and **ADC2** with your readings

now take the **maximum** of these two solutions (Substitution:) and set "settings"->"min Ic" according to it.  


for maximum current:

1.  [script for value1 and value2](http://www.wolframalpha.com/input/?i=%7BFit%5B%7B%7Bvalue1,100%7D,+%7Bvalue2,1000%7D%7D,%7B1,x%7D,x%5D+,+x%3D65400%7D),
     replace **value1** and **value2** with your readings
2.  [script for ADC1 and ADC2](http://www.wolframalpha.com/input/?i=%7BFit%5B%7B%7BADC1,+100%7D,+%7BADC2,+1000%7D%7D,%7B1,x%7D,x%5D+,+x%3D65400%7D),
     replace **ADC1** and **ADC2** with your readings

this time take the **minimum** of these two solutions (Substitution:) and set "settings"->"max Ic" according to it.  

### determine minimum (maximum) discharge current

For discharge current use the same method as above, but this time use **value** and **ADC** readings from the "I discharge" calibration.  

**I discharge: 100mA**
```
value:        [value1]
I:    100mA   [ADC1]
```

**I discharge: 300mA**
```
value:        [value2]
I:    300mA   [ADC2]
```


Minimum current scripts:

1.  [script for value1 and value2](http://www.wolframalpha.com/input/?i=%7BFit%5B%7B%7Bvalue1,100%7D,+%7Bvalue2,300%7D%7D,%7B1,x%7D,x%5D+,+x%3D1%7D),
     replace **value1** and **value2** with your readings
2.  [script for ADC1 and ADC2](http://www.wolframalpha.com/input/?i=%7BFit%5B%7B%7BADC1,+100%7D,+%7BADC2,+300%7D%7D,%7B1,x%7D,x%5D+,+x%3D1%7D),
     replace **ADC1** and **ADC2** with your readings

now take the **maximum** of these two solutions (Substitution:) and set "settings"->"min Id" according to it.  

Maximum current:

1.  [script for value1 and value2](http://www.wolframalpha.com/input/?i=%7BFit%5B%7B%7Bvalue1,100%7D,+%7Bvalue2,300%7D%7D,%7B1,x%7D,x%5D+,+x%3D65400%7D),
     replace **value1** and **value2** with your readings
2.  [script for ADC1 and ADC2](http://www.wolframalpha.com/input/?i=%7BFit%5B%7B%7BADC1,+100%7D,+%7BADC2,+1000%7D%7D,%7B1,x%7D,x%5D+,+x%3D65400%7D),
     replace **ADC1** and **ADC2** with your readings

this time take the **minimum** of these two solutions (Substitution:) and set "settings"->"max Id" according to it.  
**Note:** if your second discharge current is not **300mA** please change the number **300** (in all scripts) accordingly.


