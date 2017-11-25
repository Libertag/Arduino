/*
 * abashin@ukr.net
 * abashin
 * virtual:
 * V1=Led1
 * V2=Led2
 * V3=Кнопка включение
 * V4=Слайдер установки температуры
 * V5=Температура
 * D37=Реле
*/

#define BLYNK_PRINT Serial
#define ONE_WIRE_BUS 2 //Вывод подключение датчика температуры OneWare
#define BLYNK_DEBUG

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SimpleTimer.h>
#include <DallasTemperature.h>
#include <OneWire.h>


int i=0;
int t=16;
float tempIn=16;
int relay1 =  5; // выход на реле

OneWire oneWire_in(ONE_WIRE_BUS);
DallasTemperature sensor_inhouse(&oneWire_in);
SimpleTimer timer;
WidgetLED led1(V1);
WidgetLED led2(V2);

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "efb1c46461fd49298a0b7c94447c1e81";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "wasp";
char pass[] = "volvo940";


BLYNK_CONNECTED()  //синхронизация
{
      Blynk.syncAll();
      Blynk.syncVirtual(V3);
      Blynk.syncVirtual(V4);
      Blynk.syncVirtual(V5);
     
}


BLYNK_WRITE(V3)  //обработки виджета кнопки ВКЛЮЧЕНИЕ
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

BLYNK_WRITE(V4) //обработка виджета слайдера установки температуры
{
  t=param.asInt();
  
  
}

void relayTemp() // управление реле температуры
{
  
 
  if (tempIn < t-1 && i==1)  
    {
    digitalWrite(relay1, LOW);
    pinMode(relay1, OUTPUT);
    led2.on();
    }
  else   if (tempIn >t || i==0 ) 
    {
    digitalWrite(relay1, HIGH);
    pinMode(relay1, INPUT);
    led2.off();
    }

  }
  
void sendTemps() //получение и отправка температуры
{
  sensor_inhouse.requestTemperatures();
  tempIn = sensor_inhouse.getTempCByIndex(0); // Gets first probe on wire in lieu of by address
  
  Blynk.virtualWrite(5, tempIn);
 
  
  
  
}     
  


void setup()
{

  timer.setInterval(5000, sendTemps); //вызов подпрограммы получения температуры 5 сек
  timer.setInterval(5000, relayTemp); //вызов подпрограммы управления реле температуры 5 сек
  sensor_inhouse.begin();
  digitalWrite(relay1, HIGH);
  Serial.begin(9600);

  Blynk.begin(auth, ssid, pass);
  
}

void loop()
{
  Blynk.run();
  timer.run();
}

