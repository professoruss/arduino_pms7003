# arduino_pms7003
Horrible code to read data from a Plantower PMS7003 sensor via Arduino and serve via http/json and on OLED of Heltec HTIT-W8266.  This also supports being added to HomeKit and will report a "ghetto Air Quality", PM 2.5/10 Densities.

## wiring
```
    __usb__                          _____
  -|       |- GND ------------ GND -|     |
  -|       |- 5V ------------- VCC -|     |
  -|       |-               |-  RX -|     |
  -|       |-           |---|-- TX -|_____|
  -|       |-           |   |       PMS7003
  -|       |-           |   |
  -|       |-           |   |
  -|       |- TX (GPIO3)-   |
  -|       |- RX (GPIO4)----|
  -|       |-
  -|       |-
  -|_______|-
   HTIT-W8266
```
