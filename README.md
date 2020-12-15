# arduino_pms7003
![](/images/hardware.jpg)
HomeKit ready air quality monitor based on a Heltec HTIT-W8266 (WiFi Kit 8) development board with built-in OLED display and a Plantower PMS7003 sensor via Arduino.  Outputs data via http/json and on OLED screen.  HomeKit support includes "Air Quality", as well as PM 2.5/10 Densities.

## WiFi setup
After flashing, on first boot, the device will start in Access Point mode to allow you to setup your WiFi.

Connect to the access point named `ESPP8266-<uniqueid>` and it will redirect you to a webpage that will allow you to select your SSID and enter your password.

Once this is configured, it will restart into WiFi client mode and connect to your local network.

WiFi configuration stored locally and will not reset across power cycles/reboots.

## MDNS
Device will come up and broadcast multicast dns for `ESPP-<uniqueid>.local` and `_http._tcp.80`.

You can discover the uniqueid by running `dns-sd -B _http._tcp` on MacOS.
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

<img src='/images/homekit-fair.png' width='50%'>
<img src='/images/siri-fair.png' width='50%'>

To pair from Home app, click `+`, `Add Accessory`, `I Don't Have a Code or Cannot Scan`.  It will show up under `Nearby Accessories`.

By default, pairing code is `867-53-069` and can be modified in `my_accessory.c`

Due to the nature of HomeKit, reporting is done as levels 0-5, where 0-5 = Unknown, Excellent, Good, Fair, Inferior, Poor

Thresholds are currently set as: (still tweaking to reflect reality/cross referenced from Temtop 1000S)
<table>
<tr><td>PM2.5</td><td>HomeKit</td><td>Description</td></tr>
<tr><td>NA</td><td>0</td><td>Unknown</td></tr>
<tr><td>0-12</td><td>1</td><td>Excellent</td></tr>
<tr><td>13-35</td><td>2</td><td>Good</td></tr>
<tr><td>36-55</td><td>3</td><td>Fair</td></tr>
<tr><td>56-150</td><td>4</td><td>Inferior</td></tr>
<tr><td>151+</td><td>5</td><td>Poor</td></tr>
</table>

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

## Reference
- [ESP8266 Built-in OLED – Heltec WiFi Kit 8](https://robotzero.one/heltec-wifi-kit-8/)
- [ESP8266WiFi library](https://arduino-esp8266.readthedocs.io/en/latest/esp8266wifi/readme.html)
- [DNSServer library](https://arduino-esp8266.readthedocs.io/en/latest/libraries.html?dns-server-dnsserver-library#dns-server-dnsserver-library)
- [WiFiManager library](https://github.com/tzapu/WiFiManager)
- [ESP8266mDNS library](https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266mDNS)
- [U8g2 display library](https://github.com/olikraus/u8g2)
- [arduino_homekit_server library](https://github.com/Mixiaoxiao/Arduino-HomeKit-ESP8266)
