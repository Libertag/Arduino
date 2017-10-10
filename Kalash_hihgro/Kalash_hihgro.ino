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

#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SimpleDHT.h>
#include <SimpleTimer.h>
#include <TimeLib.h>

int pinDHT11 = 2;
SimpleDHT11 dht11;

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "2f2a7dc05c154e8595cfff62e474ab17";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "dlink300";
char pass[] = "Awldqolz145236";
SimpleTimer timer;

void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
 
  timer.setInterval(10000, sendDHT);
}

void sendDHT()
{
  byte temperature = 0;
  byte humidity = 0;
  dht11.read(pinDHT11, &temperature, &humidity, NULL);
  
  Blynk.virtualWrite(0, (int)temperature);
  Blynk.virtualWrite(1, (int)humidity);
}

void loop()
{
  Blynk.run();
  timer.run();
}

