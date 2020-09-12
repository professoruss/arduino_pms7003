# arduino_pms7003
Horrible code to read data from a Plantower PMS7003 sensor via Arduino and serve via http/json and on OLED of Heltec HTIT-W8266.  This also supports being added to HomeKit and will report a "ghetto Air Quality", PM 2.5/10 Densities.

## wiring
```
HTIT-W8266 | PMS7003
1. GND     | 1. GND
2. 5V      | 2. VCC
9. RX      | 3. RX
8. TX      | 4. TX
```
