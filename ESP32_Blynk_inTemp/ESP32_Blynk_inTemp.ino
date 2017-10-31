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
 * This example runs directly on ESP32 chip.
 *
 * Note: This requires ESP32 support package:
 *   https://github.com/espressif/arduino-esp32
 *
 * Please be sure to select the right ESP32 module
 * in the Tools -> Board menu!
 *
 * Change WiFi ssid, pass, and Blynk auth token to run :)
 *
 **************************************************************/
#include <Time.h>
#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <SimpleTimer.h> 

SimpleTimer timer;

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "d959518775f247ac994c79babd8b9933";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "DMS_ODESSA";
char pass[] = "0677336591";
int val = 0;
extern "C" {      
   uint8_t temprature_sens_read(); 
}
void sendTemps() //получение и отправка температуры
{
  
  uint8_t tf = temprature_sens_read();
  float   tc = ( tf - 32 )/1.8;
  Blynk.virtualWrite(1, int(tc));
  
  
}
void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(2000, sendTemps); //вызов подпрограммы получения температуры 2 сек
}
void loop()
{


  Blynk.run();
  
  timer.run();
  
  
}

