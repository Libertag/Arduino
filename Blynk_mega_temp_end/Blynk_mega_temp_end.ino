//  Выходы виртуальные
//  V0  ------  
//  V1  ------  слайдер установки температуры
//  V2  ------  виджет ввода времени
//  V3  ------  
//  V4  ------  индикатор температуры 1 датчика ( комната)
//  V5  ------  индикатор температуры 2 датчика ( улица)
//  V6  ------  слайдер установки температуры по таймеру
//  V7  ------  светодиод вкл (Led1)
//  V8  ------  светодиод вкл реле (Led2)
//  V9  ------  
//  V10 ------
//  V11 ------  кнопка включения
//  V12 ------
//  V13 ------
//  V14 ------
//  V15 ------
//  V16 ------
//  V17 ------
//  V18 ------
//  V19 ------
//  V20 ------
//  V21 ------
//  V22 ------
//  V23 ------
//  V24 ------
//  V25 ------
//  Выходы цифровые
//  D1  ------
//  D2  ------
//  D3  ------
//  D4  ------
//  D5  ------
//  D6  ------
//  D7  ------
//  D8  ------
//  D9  ------
//  D10 ------
//  D11 ------
//  D12 ------
//  D13 ------
//  D14 ------
//  D15 ------
//  D16 ------
//  D17 ------
//  D18 ------
//  D19 ------
//  D20 ------  SDA RTC
//  D21 ------  SCL RTC
//  D22 ------
//  D23 ------  выход на реле
//  D24 ------
//  D25 ------  выход на реле
//  D26 ------
//  D27 ------  выход на реле
//  D28 ------  
//  D29 ------  выход на реле
//  D30 ------  
//  D31 ------  выход на реле
//  D32 ------
//  D33 ------  выход на реле
//  D34 ------
//  D35 ------  выход на реле
//  D36 ------
//  D37 ------  выход на реле
//  D38 ------
//  D39 ------
//  D40 ------  ONE_WIRE_BUS температура
//  D41 ------
//  D42 ------
//  D43 ------
//  D44 ------
//  D45 ------
//  D46 ------
//  D47 ------
//  D48 ------
//  D49 ------
//  D50 ------
//добавим коментарий для проверки


#include <Time.h>
#include <TimeLib.h>
#include <SPI.h>
#include <Ethernet.h>
#include <BlynkSimpleEthernet.h>
#include <TimerOne.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <SimpleTimer.h>
#include <DS3231.h>
#define ONE_WIRE_BUS 40
#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
//#define BLYNK_DEBUG

SimpleTimer timer;
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DS3231  rtc(SDA, SCL);
long timeStart;
long timeStop;
long timeNowSec;
word tempIn=0;
word tempOut=0;
char auth[] = "37b6b2042b98417d99ad8fb1f85355d3";
WidgetLED led1(V7);
WidgetLED led2(V8);
#define W5100_CS  10
#define SDCARD_CS 4
int relay8 =  23; // выход на реле
int relay7 =  25; // выход на реле
int relay6 =  27; // выход на реле
int relay5 =  29; // выход на реле
int relay4 =  31; // выход на реле
int relay3 =  33; // выход на реле
int relay2 =  35; // выход на реле
int relay1 =  37; // выход на реле
int t=16;
int i=0;
int wt=12;
int tnight=1;


BLYNK_CONNECTED()  //синхронизация
{
      Blynk.syncAll();
      Blynk.syncVirtual(V11);
      Blynk.syncVirtual(V6);
      Blynk.syncVirtual(V2);
}


BLYNK_WRITE(V11)  //обработки виджета кнопки ВКЛЮЧЕНИЕ
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





BLYNK_WRITE(V1) //обработка виджета слайдера установки температуры
{
  t=param.asInt();
  
}

BLYNK_WRITE(V2) //обработка виджета ввода времени
{
  timeStart=param[0].asLong();
  timeStop=param[1].asLong();
  
}

BLYNK_WRITE(V6) //обработка виджета слайдера установки ночного времени
{
  wt=param.asInt();
  
}


void setup()
{
  pinMode(SDCARD_CS, OUTPUT);
  digitalWrite(SDCARD_CS, HIGH); // Deselect the SD card
  timer.setInterval(2000, sendTemps); //вызов подпрограммы получения температуры 2 сек
  timer.setInterval(1000, relayTemp); //вызов подпрограммы управления реле температуры 1 сек
  timer.setInterval(10000, sendTime); //вызов подпрограммы вывода времени 10 сек
  Serial.begin(9600);
  Blynk.begin(auth);
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);
  pinMode(relay5, OUTPUT);
  pinMode(relay6, OUTPUT);
  pinMode(relay7, OUTPUT);
  pinMode(relay8, OUTPUT);
  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);
  digitalWrite(relay3, HIGH);
  digitalWrite(relay4, HIGH);
  digitalWrite(relay5, HIGH);
  digitalWrite(relay6, HIGH);
  digitalWrite(relay7, HIGH);
  digitalWrite(relay8, HIGH);
  //rtc.begin();
}

void sendTemps() //получение и отправка температуры
{
  sensors.requestTemperatures(); // Polls the sensors
  tempIn = sensors.getTempCByIndex(0); // Gets first probe on wire in lieu of by address
  tempOut = sensors.getTempCByIndex(1); // Gets first probe on wire in lieu of by address
  Blynk.virtualWrite(4, tempIn);
  Blynk.virtualWrite(5, tempOut);
  
  
  
}

void sendTime() //получение и вывод времени rtc
{
   
  long timeNow=rtc.getUnixTime(rtc.getTime()); // получение Юникстайм
  long timeNowHour=(timeNow/3600)%24; // вычисление количество часов с начала суток
  long timeNowMin=(timeNow/60)%60; // вычисление количества минут с начала часа
  timeNowSec=(timeNowHour*60+timeNowMin)*60; // вычисление количество секунд с начала суток
  
  if (timeNowSec >= timeStart && timeNowSec <= timeStop) 
  { //если количество секунд с начала суток попадает в промежуток установки то работаем по установке таймера

  tnight=1;
  }
 else {
  tnight=0;
   
      }
  }
  
void relayTemp() // управление реле температуры
{
 if (tnight ==0) // если работаем не по установке таймера то сравниваем со значением с виджета V1 иначе с виджета V6
 {
  if (tempIn < t-1 && i==1)  
    {
    digitalWrite(relay1, LOW);
    led2.on();
    }
  else   if (tempIn >t || i==0 ) 
    {
    digitalWrite(relay1, HIGH);
    led2.off();
    }
  }
  else 
  {
   if (tempIn < wt-1 && i==1)  
    {
    digitalWrite(relay1, LOW);
    led2.on();
    }
  else   if (tempIn >wt || i==0 ) 
    {
    digitalWrite(relay1, HIGH);
    led2.off();
    } 
  }
}

void loop()
{
  Blynk.run();
  interrupts();
  timer.run();
  
 
}

