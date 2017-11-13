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
#define ONE_WIRE_BUS 10 //Вывод подключение датчика температуры OneWare
#define BLYNK_DEBUG

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SimpleTimer.h>
#include <DallasTemperature.h>
#include <OneWire.h>


int i=0;
int t=16;
word tempIn=16;
int relay1 =  37; // выход на реле

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

SimpleTimer timer;
WidgetLED led1(V1);
WidgetLED led2(V2);

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "efb1c46461fd49298a0b7c94447c1e81";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "DMS_ODESSA";
char pass[] = "0677336591";


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
  
 
  if (tempIn < t-1 )  
    {
    digitalWrite(relay1, LOW);
    led2.on();
    }
  else   if (tempIn >t ) 
    {
    digitalWrite(relay1, HIGH);
    led2.off();
    }
  }
  
void sendTemps() //получение и отправка температуры
{
  sensors.requestTemperatures(); // Polls the sensors
  //tempIn = sensors.getTempCByIndex(0); // Gets first probe on wire in lieu of by address
  tempIn = t;
  Blynk.virtualWrite(5, tempIn);
 
  
  
  
}     
  


void setup()
{

  timer.setInterval(2000, sendTemps); //вызов подпрограммы получения температуры 2 сек
  timer.setInterval(1000, relayTemp); //вызов подпрограммы управления реле температуры 1 сек
  
  Serial.begin(9600);

  Blynk.begin(auth, ssid, pass);
  
}

void loop()
{
  Blynk.run();
  timer.run();
}

