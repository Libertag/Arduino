

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
SimpleTimer timer;
const int relayPin =  16; // выход на реле
void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth, "DMS_ODESSA", "0677336591"); // нустановки WIFI и подключения
  pinMode(relayPin, OUTPUT); // устанавливаем пин на выход
  sensors.begin();
  
  timer.setInterval(5000L, sendTemps); // Temperature sensor polling interval (5000L = 5 seconds)

  
}

BLYNK_WRITE(V11)  //таймер обработки реле
 {
 if(param.asInt()==1)
  {
     digitalWrite(relayPin, LOW);
     led1.on();
  }
  else
  {
     digitalWrite(relayPin, HIGH);
     led1.off();
  }
 }

BLYNK_CONNECTED() { //синхронизация
      Blynk.syncAll();
      Blynk.syncVirtual(V11);
}

void sendTemps() //отправка температуры
{
  sensors.requestTemperatures(); // Polls the sensors
  float tempBabyRoom = sensors.getTempCByIndex(0); // Gets first probe on wire in lieu of by address
  float tempmomRoom = sensors.getTempCByIndex(1); // Gets first probe on wire in lieu of by address
  Blynk.virtualWrite(4, tempBabyRoom);
  Blynk.virtualWrite(5, tempmomRoom);
  
  
}


void loop()
{
  Blynk.run();
  timer.run();
}
