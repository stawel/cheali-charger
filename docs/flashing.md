
Flashing
========

Flashing: atmega32 CPU charger
-------------------------------

check ISP pins (at least GND and VCC) on your chargers PCB board:
- G.T. POWER A6-10 200W:  
  ![isp](connectors/isp_GTPowerA6-10.jpeg)
- IMAX B6 - original:  
  ![isp](connectors/isp_imaxB6.jpeg)
- IMAX B6 - clone:  
  ![isp](connectors/isp_imaxB6_clone.jpeg)

select the appropriate [hex file](flashing.md#differences)  
flasing using [avrdude](http://www.nongnu.org/avrdude/),  
you may want to change "-cusbasp" according to your programmer:
- try to make a backup:
<pre>
avrdude.exe -patmega32 -cusbasp -Uflash:r:flash.hex:r -Ulfuse:r:lfuse.hex:r -Uhfuse:r:hfuse.hex:r -Ueeprom:r:eeprom.hex:r
</pre>

- flash cheali-charger into program memory:
<pre>
avrdude.exe -patmega32 -cusbasp -Uflash:w:cheali-charger-[VERSION].hex:a
</pre>

or use any other avr programming tool  
WARNING: Do not touch the AVR fuses, setting them wrong could destroy the charger.


Flashing: nuvoton M0517LBN CPU charger
--------------------------------------

check ISP pins (at least GND and VCC) on your chargers PCB board:  
![isp](connectors/isp_imaxB6_M0517.jpeg)

- [Bu-Link](http://www.aliexpress.com/item/Free-Shipping-1SET-Bu-Link-Compatible-Nuvoton-Nu-Link-for-all-Cortex-M0-MINI51-M052-NUC120/1570055317.html):
  - Bu-Link isp connector:  
  ![isp_programmer](connectors/isp_bu-link.jpg)
  - make a appropriate cable
  - use: [nuvoton icp programming tool](http://download.nuvoton.com/NuvotonMOSS/DownloadService/Member/DocumentsInfo.aspx?tp_GUID=SW0520101208200310)
   (try to make a backup first!)

- ST-LINK/V2 or STM32F4DISCOVERY board:
  - [read](https://groups.google.com/d/msg/cheali-charger/2Rz-dtwZ5Is/zUGr3PzX9bcJ)

- flashing and debugging M0517LBN with CoIDE: TODO
- flashing and debugging M0517LBN with OpenOCD: TODO

Differences between cheali-charger versions
-------------------------------------------
