
/**************************************************************
 * Blynk is a platform with iOS and Android apps to control
 * Arduino, Raspberry Pi and the likes over the Internet.
 * You can easily build graphic interfaces for all your
 * projects by simply dragging and dropping widgets.
 *
 *   Downloads, docs, tutorials: http://www.blynk.cc
 *   Blynk community:            http://community.blynk.cc
 *   Social networks:            http://www.fb.com/blynkapp
 *                               http://twitter.com/blynk_app
 *
 * Blynk library is licensed under MIT license
 * This example code is in public domain.
 *
 **************************************************************
 * This example runs directly on ESP8266 chip.
 *
 * You need to install this for ESP8266 development:
 *   https://github.com/esp8266/Arduino
 *
 * Please be sure to select the right ESP8266 module
 * in the Tools -> Board menu!
 *
 * Change WiFi ssid, pass, and Blynk auth token to run :)
 *
 **************************************************************/
/*
#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "7d63f6f21a1f456eaa3850742ccf46db";

void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth, "DMS_ODESSA", "0677336591");
}

void loop()
{
  Blynk.run();
}
*/
#define BLYNK_DEBUG
#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <TimeLib.h>
#include <SimpleTimer.h>
#define ONE_WIRE_BUS 2

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

char auth[] = "7d63f6f21a1f456eaa3850742ccf46db";
int lightPin = A0;
SimpleTimer timer;
int lightValue = 0;
int releyPin =  16; // выход на реле
void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth, "DMS_ODESSA", "0677336591"); // нустановки WIFI
  pinMode(releyPin, OUTPUT); // устанавливаем пин на выход
  sensors.begin();
  
  timer.setInterval(5000L, sendTemps); // Temperature sensor polling interval (5000L = 5 seconds)
  
}
BLYNK_WRITE(V9) {
   int invertedButtonValue = param.asInt() ;
   Blynk.virtualWrite(10, invertedButtonValue);
}
BLYNK_WRITE(V11) {
if (param.asInt() == 0)
{
digitalWrite(releyPin, HIGH);
}
else
{
digitalWrite(releyPin, LOW);
}
}
void sendTemps()
{
  sensors.requestTemperatures(); // Polls the sensors
  float  lightValue = analogRead(lightPin);
  float tempBabyRoom = sensors.getTempCByIndex(0); // Gets first probe on wire in lieu of by address
  float tempmomRoom = sensors.getTempCByIndex(1); // Gets first probe on wire in lieu of by address
  Blynk.virtualWrite(4, tempBabyRoom);
  Blynk.virtualWrite(5, tempmomRoom);
  Blynk.virtualWrite(1, lightValue);
  
}


void loop()
{
  Blynk.run();
  timer.run();
}
