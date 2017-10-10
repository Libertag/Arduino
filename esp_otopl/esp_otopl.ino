

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

char auth[] = "fe77fa08b8e64d788fee06cf5d5e5574";
WidgetLED led1(V7);
WidgetLED led2(V8);
SimpleTimer timer;
const int relayPin =  16; // выход на реле
float tempBabyRoom=0;
int t=16;
int i=0;
void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth, "DMS_ODESSA", "0677336591"); // нустановки WIFI и подключения
  pinMode(relayPin, OUTPUT); // устанавливаем пин на выход
  sensors.begin();
  
  timer.setInterval(5000L, sendTemps); // Temperature sensor polling interval (5000L = 5 seconds)
  timer.setInterval(5000L, releyTemp); // Temperature sensor polling interval (5000L = 10 seconds)
  
}

BLYNK_WRITE(V11)  //таймер обработки реле
 {
 if(param.asInt()==1)
  {
     i=1;
     led1.on();
  }
  else
  {
     i=0;
     led1.off();
  }
 }

BLYNK_CONNECTED() { //синхронизация
      Blynk.syncAll();
      Blynk.syncVirtual(V11);
}
BLYNK_WRITE(V1)
{
  t=param.asInt();
  Blynk.virtualWrite(V0,t);
}
void sendTemps() //отправка температуры
{
  sensors.requestTemperatures(); // Polls the sensors
  tempBabyRoom = sensors.getTempCByIndex(0); // Gets first probe on wire in lieu of by address
  int tempmomRoom = sensors.getTempCByIndex(1); // Gets first probe on wire in lieu of by address
  Blynk.virtualWrite(4, tempBabyRoom);
  Blynk.virtualWrite(5, tempmomRoom);
  
  
}

void releyTemp()
{
  if (tempBabyRoom <= t && i==1)  {
    digitalWrite(relayPin, LOW);
    led2.on();
    }
    else {
    digitalWrite(relayPin, HIGH);
    led2.off();
    }
}

void loop()
{
  Blynk.run();
  timer.run();
}
