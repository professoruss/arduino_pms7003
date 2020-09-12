# arduino_pms7003
Horrible code to read data from a Plantower PMS7003 sensor via Arduino and serve via http/json and on OLED of Heltec HTIT-W8266.  This also supports being added to HomeKit and will report a "ghetto Air Quality", PM 2.5/10 Densities.

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

## Wiring
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
