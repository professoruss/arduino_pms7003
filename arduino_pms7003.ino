#include <Arduino.h>
#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino

//needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager
#include <ESP8266mDNS.h>
#include <arduino_homekit_server.h>

#include <U8g2lib.h>

//U8g2 Contructor
U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ 16, /* clock=*/ 5, /* data=*/ 4);
// Alternative board version. Uncomment if above doesn't work.
// U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ 4, /* clock=*/ 14, /* data=*/ 2);

#include "PMS.h"
#include <SoftwareSerial.h>
SoftwareSerial pmsSerial(01,03);

// PMS_READ_INTERVAL (4:30 min) and PMS_READ_DELAY (30 sec) CAN'T BE EQUAL! Values are also used to detect sensor state.
static const uint32_t PMS_READ_INTERVAL = 90000;
static const uint32_t PMS_READ_DELAY = 30000;

// Default sensor state.
uint32_t timerInterval = PMS_READ_DELAY;

PMS pms(pmsSerial);
PMS::DATA data;


int PM_AE_UG_1_0;
int PM_AE_UG_2_5;
int PM_AE_UG_10_0;

/* Returns a semi-unique id for the device. The id is based
*  on part of a MAC address or chip ID so it won't be
*  globally unique. */
uint16_t GetDeviceId()
{
#if defined(ARDUINO_ARCH_ESP32)
  return ESP.getEfuseMac();
#else
  return ESP.getChipId();
#endif
}

/* Append a semi-unique id to the name template */
String MakeMine(const char *NameTemplate)
{
  uint16_t uChipId = GetDeviceId();
  String Result = String(NameTemplate) + String(uChipId, HEX);
  return Result;
}

String MyName = MakeMine("ESP8266-");


ESP8266WebServer server(80);    // Create a webserver object that listens for HTTP request on port 80

void handleRoot();              // function prototypes for HTTP handlers
void handleNotFound();

void ConnectToWiFi()
{
  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  u8g2.clear();          // clear the internal memory
  u8g2.setCursor(0,20);
  u8g2.setFont(u8g2_font_ncenB08_tr); // choose a suitable font
  u8g2.print("Running WiFiManager");
  u8g2.sendBuffer();
  delay(1000);
  WiFiManager wifiManager;
  
  //exit after config instead of connecting
  wifiManager.setBreakAfterConfig(true);
  
  //reset settings - for testing
  //wifiManager.resetSettings();


  //tries to connect to last known settings
  //if it does not connect it starts an access point with the specified name
  //here  "ESP8266-<uniqueid>" with password "password"
  //and goes into a blocking loop awaiting configuration
  u8g2.clear();          // clear the internal memory
  u8g2.setCursor(0,10);
  u8g2.setFont(u8g2_font_ncenB08_tr); // choose a suitable font
  u8g2.print("Connect to");
  u8g2.setCursor(0,20);
  u8g2.print(MyName);
  u8g2.setCursor(0,30);
  u8g2.print("to configure WIFI");
  u8g2.sendBuffer();
  delay(1000);
  if (!wifiManager.autoConnect(MyName.c_str(), "")) {
    Serial.println("failed to connect, we should reset as see if it connects");
    u8g2.clear();          // clear the internal memory
    u8g2.setCursor(0,20);
    u8g2.setFont(u8g2_font_ncenB08_tr); // choose a suitable font
    u8g2.print("failed to connect, we should reset as see if it connects");
    u8g2.sendBuffer();
    delay(3000);
    ESP.reset();
    delay(5000);
  }

  //if you get here you have connected to the WiFi
  Serial.println("connected...yeey :)");
  u8g2.clear();
  u8g2.setCursor(0,20);
  u8g2.print("Connected");
  u8g2.sendBuffer();
  delay(1000);

  Serial.println("local ip");
  Serial.println(WiFi.localIP());
  u8g2.setFont(u8g2_font_8x13_tr);
  u8g2.clear();
  u8g2.setCursor(0,10);
  u8g2.print("SSID: ");
  u8g2.print(WiFi.SSID());
  u8g2.setCursor(0,20);
  u8g2.print("Status: ");
  u8g2.print(WiFi.status());
  u8g2.setCursor(0,30);
  u8g2.print("IP: ");
  u8g2.print(WiFi.localIP());
  u8g2.sendBuffer();
  delay(1000);

}
void AdvertiseServices() {
  if (MDNS.begin(MyName.c_str())) {             // Start the mDNS responder for esp8266.local
    u8g2.clear();
    u8g2.setCursor(0,10);
    u8g2.print("MDNS started");
    u8g2.sendBuffer();
    delay(1000);

    MDNS.addService("http", "tcp", 80);
/*    MDNS.addService("hap", "tcp", 80);
    MDNS.addServiceTxt("hap", "tcp", "md", "ESP8266AQI"); // model name
    MDNS.addServiceTxt("hap", "tcp", "pv", "1.0"); // protocol version
    MDNS.addServiceTxt("hap", "tcp", "id", "A0:20:A6:15:46:44"); // device id
    MDNS.addServiceTxt("hap", "tcp", "c#", "3"); // configuration number
    MDNS.addServiceTxt("hap", "tcp", "s#", "1"); // state number
    MDNS.addServiceTxt("hap", "tcp", "ff", "0"); // feature flags
    MDNS.addServiceTxt("hap", "tcp", "ci", "19"); // category identifier  https://github.com/maximkulkin/esp-homekit/blob/a4b8d3e8020c7af92d49ed7f2f35c461f1917d01/include/homekit/types.h#L42-L72
    MDNS.addServiceTxt("hap", "tcp", "sf", "1"); // status flags
*/
  } else {
    while (1)
      {
        Serial.println("Error setting up MDNS responder!");
        u8g2.clear();
        u8g2.setCursor(0,10);
        u8g2.print("MDNS error");
        u8g2.sendBuffer();
        delay(1000);
      }
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pmsSerial.begin(9600);
  Serial.println();
  pms.passiveMode();    // Switch to passive mode

  // Default state after sensor power, but undefined after ESP restart e.g. by OTA flash, so we have to manually wake up the sensor for sure.
  // Some logs from bootloader is sent via Serial port to the sensor after power up. This can cause invalid first read or wake up so be patient and wait for next read cycle.
  pms.wakeUp();

#ifdef DEEP_SLEEP
  delay(PMS_READ_DELAY);
  readData();
  pms.sleep();
  ESP.deepSleep(PMS_READ_INTERVAL * 1000);
#endif // DEEP_SLEEP

  u8g2.begin();
  u8g2.setFont(u8g2_font_ncenB14_tr);
  u8g2.setCursor(0,20);
  u8g2.print("AQI Sensor");
  u8g2.sendBuffer();
  delay(1000);

  ConnectToWiFi();

  server.on("/", handleRoot);               // Call the 'handleRoot' function when a client requests URI "/"
  server.onNotFound(handleNotFound);        // When a client requests an unknown URI (i.e. something other than "/"), call function "handleNotFound"

  server.begin();                           // Actually start the server
  Serial.println("HTTP server started");
  u8g2.clear();
  u8g2.setCursor(0,10);
  u8g2.print("HTTP started");
  u8g2.sendBuffer();
  delay(1000);

  AdvertiseServices();
  my_homekit_setup();

}

void readData()
{
  PMS::DATA data;

  // Clear buffer (removes potentially old data) before read. Some data could have been also sent before switching to passive mode.
  while (Serial.available()) { Serial.read(); }

  //DEBUG_OUT.println("Send read request...");
  u8g2.clear();
  u8g2.setCursor(0,10);
  u8g2.print("Send read request.");
  u8g2.sendBuffer();
  pms.requestRead();

  //DEBUG_OUT.println("Reading data...");
  u8g2.clear();
  u8g2.setCursor(0,10);
  u8g2.print("Reading data.");
  u8g2.sendBuffer();
  if (pms.readUntil(data))
  {
    /*DEBUG_OUT.println();
    DEBUG_OUT.print("PM 1.0 (ug/m3): ");
    DEBUG_OUT.println(data.PM_AE_UG_1_0);
    DEBUG_OUT.print("PM 2.5 (ug/m3): ");
    DEBUG_OUT.println(data.PM_AE_UG_2_5);
    DEBUG_OUT.print("PM 10.0 (ug/m3): ");
    DEBUG_OUT.println(data.PM_AE_UG_10_0);
    DEBUG_OUT.println();*/

    u8g2.clear();
    u8g2.setCursor(0,10);
    u8g2.print("PM1.0(ug/m3): ");
    u8g2.print(data.PM_AE_UG_1_0);
    u8g2.setCursor(0,20);
    u8g2.print("PM2.5(ug/m3): ");
    u8g2.print(data.PM_AE_UG_2_5);
    u8g2.setCursor(0,30);
    u8g2.print("PM10(ug/m3): ");
    u8g2.print(data.PM_AE_UG_10_0);
    u8g2.sendBuffer();
    PM_AE_UG_1_0 = data.PM_AE_UG_1_0;
    PM_AE_UG_2_5 = data.PM_AE_UG_2_5;
    PM_AE_UG_10_0 = data.PM_AE_UG_10_0;
    delay(1000);
  }
  else
  {
    //DEBUG_OUT.println("No data.");
    u8g2.clear();
    u8g2.setCursor(0,10);
    u8g2.print("No data.");
    u8g2.sendBuffer();
  }
}
#ifndef DEEP_SLEEP
void timerCallback() {
  if (timerInterval == PMS_READ_DELAY)
  {
    readData();
    //DEBUG_OUT.println("Going to sleep.");
    /*u8g2.clear();
    u8g2.setCursor(0,10);
    u8g2.print("Going to sleep.");
    u8g2.sendBuffer();*/
    pms.sleep();
  }
  else
  {
    //DEBUG_OUT.println("Waking up.");
    u8g2.clear();
    u8g2.setCursor(0,10);
    u8g2.print("Waking up.");
    u8g2.sendBuffer();
    pms.wakeUp();
  }
}
#endif // DEEP_SLEEP



void handleRoot() {
  server.send(200, "text/json", "{ \"Hostname\": \"" + MyName + "\", \"Data\": { \"PM1.0\": \"" + String((int)PM_AE_UG_1_0) + "\", \"PM2.5\": \"" + String((int)PM_AE_UG_2_5) + "\", \"PM10\": \"" + String((int)PM_AE_UG_10_0) + "\"}}");
}

void handleNotFound(){
  server.send(404, "text/plain", "404: Not found"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
}

void loop() {

  MDNS.update();

#ifndef DEEP_SLEEP
  static uint32_t timerLast = 0;
  uint32_t timerNow = millis();
  if (timerNow - timerLast >= timerInterval) {
    timerLast = timerNow;
    timerCallback();
    timerInterval = timerInterval == PMS_READ_DELAY ? PMS_READ_INTERVAL : PMS_READ_DELAY;
  }

#endif // DEEP_SLEEP
  server.handleClient();                    // Listen for HTTP requests from clients
  my_homekit_loop();
}

//==============================
// Homekit setup and loop
//==============================

// access your homekit characteristics defined in my_accessory.c
extern "C" homekit_server_config_t config;
extern "C" homekit_characteristic_t cha_air_quality;
extern "C" homekit_characteristic_t cha_pm25_density;
extern "C" homekit_characteristic_t cha_pm10_density;

static uint32_t next_heap_millis = 0;
static uint32_t next_report_millis = 0;

void my_homekit_setup() {
  arduino_homekit_setup(&config);
}

void my_homekit_loop() {
  arduino_homekit_loop();
  const uint32_t t = millis();
  if (t > next_report_millis) {
    // report sensor values every 10 seconds
    next_report_millis = t + 10 * 1000;
    my_homekit_report();
  }
  if (t > next_heap_millis) {
    // show heap info every 5 seconds
    next_heap_millis = t + 5 * 1000;
/*    LOG_D("Free heap: %d, HomeKit clients: %d",
        ESP.getFreeHeap(), arduino_homekit_connected_clients_count());
*/
  }
}

void my_homekit_report() {
  float PM25_DENSITY = int(PM_AE_UG_2_5);
  float PM10_0_DENSITY = int(PM_AE_UG_10_0);
  float cha_air_quality_value = 1;
  if (PM_AE_UG_2_5 > 200) {
    cha_air_quality_value = 5;
  } else if (PM_AE_UG_2_5 > 150) {
    cha_air_quality_value = 4;
  } else if (PM_AE_UG_2_5 > 100) {
    cha_air_quality_value = 3;
  } else if (PM_AE_UG_2_5 > 35) {
    cha_air_quality_value = 2;
  } else if (PM_AE_UG_2_5 > 0) {
    cha_air_quality_value = 1;
  } else {
    cha_air_quality_value = 0;
  }
  cha_pm25_density.value.float_value = PM_AE_UG_2_5;
  cha_pm10_density.value.float_value = PM_AE_UG_10_0;
  cha_air_quality.value.uint8_value = cha_air_quality_value;
//  LOG_D("PM25_DENSITY: %.1f", PM_AE_UG_2_5);
  homekit_characteristic_notify(&cha_pm25_density, cha_pm25_density.value);
  homekit_characteristic_notify(&cha_pm10_density, cha_pm10_density.value);
  homekit_characteristic_notify(&cha_air_quality, cha_air_quality.value);
}

int random_value(int min, int max) {
  return min + random(max - min);
}
