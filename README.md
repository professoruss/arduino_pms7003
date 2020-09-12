__This is still work in progress, code and documentation changing frequently - 9/12/2020__
# arduino_pms7003
Horrible code to read data from a Plantower PMS7003 sensor via Arduino and serve via http/json and on OLED of Heltec HTIT-W8266.  This also supports being added to HomeKit and will report a "ghetto Air Quality", PM 2.5/10 Densities.

## Wifi setup
After flashing, on first boot, the device should start in Access Point mode to allow you to setup your wifi.

Connect to the access point named `ESPP-<uniqueid>` and it will redirect you to a webpage that will allow you to select your SSID and enter your password.

Once this is configured, it will restart into Wifi client mode and connect to your local network.

## MDNS
Device will come up and broadcast multicast dns for _http._tcp.80 using as `ESPP-<uniqueid>.local`.  You can discover the uniqueid by running `dns-sd -B _http._tcp` on MacOS.
```
$ dns-sd -B _http._tcp
Browsing for _http._tcp
DATE: ---Sat 12 Sep 2020---
14:55:28.605  ...STARTING...
Timestamp     A/R    Flags  if Domain               Service Type         Instance Name
14:55:28.607  Add        3   4 local.               _http._tcp.          ESP8266-4a9f
...
```

## JSON output
Device will also output data via http/json. You can run `curl http://ESP8266-<uniqueid>.local` to view.  Use to pull data, consume, graph, etc
```
$ curl http://ESP8266-4a9f.local
{ "Hostname": "ESP8266-4a9f", "Data": { "PM1.0": "14", "PM2.5": "24", "PM10": "30"}}
```

## HomeKit
Device will be accessible to pair with HomeKit.
By default, pairing code is `867-53-069` and can be modified in `my_accessory.c`

## Wiring
```
    __usb__                               _____
  -|       |- GND ----------------- GND -|     |
  -|       |- 5V ------------------ VCC -|     |
  -|       |-                    |-  RX -|     |
  -|       |-                |---|-- TX -|_____|
  -|       |-                |   |       PMS7003
  -|       |-                |   |
  -|       |-                |   |
  -|       |- TX (GPIO1/TXD0)-   |
  -|       |- RX (GPIO3/RXD0)----|
  -|       |-
  -|       |-
  -|_______|-
   HTIT-W8266
```
![](/images/arduino.jpg)

## Parts
You can find parts pretty easily on Amazon, but they are cheaper from overseas/AliExpress (Non sponsored links)

__HTIT-W8266__
- [AliExpress](https://www.aliexpress.com/item/33015504632.html) ~$4.50
- [Amazon](https://www.amazon.com/ESP8266-development-Arduino-NodeMcu-0-91inch/dp/B074373KTY) ~$10

__PMS7003__
- [AliExpress](https://www.aliexpress.com/item/32784279004.html) ~$17
- [Amazon](https://www.amazon.com/KOOBOOK-PMS7003-Sensor-Precision-Particle/dp/B07SJ4NBT8/) ~$22 without cables
- [eBay](https://www.ebay.com/itm/PMS7003-G7-High-Precision-Laser-Dust-Sensor-PM1-0-PM2-5-PM10-with-Adapter-Cable/183413484479) ~$20
