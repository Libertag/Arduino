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
int relay = 13;


void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth, "DMS_ODESSA", "0677336591");
  pinMode(relay, OUTPUT);
  sensors.begin();
  
  timer.setInterval(2000L, sendTemps); // Temperature sensor polling interval (2000L = 2 seconds)

}

BLYNK_WRITE(V2)
  {
    int button = param.asInt();
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
  if button == 1  digitalWrite(relay,LOW);
  
  else digitalWrite(relay.HIGH);


  
 }



void loop()
{
  Blynk.run();
  timer.run();
  
}
