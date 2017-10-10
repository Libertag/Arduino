#define BLYNK_DEBUG
#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <SPI.h>
#include <Ethernet.h>
#include <BlynkSimpleEthernet.h>
#include <TimerOne.h>
#include <Ultrasonic.h>
Ultrasonic ultrasonic(21,20);// (echoPin,triqPin)
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 30
#include <SimpleTimer.h>
SimpleTimer timer;


OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
float tempIn=0;
float tempOut=0;
// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "37b6b2042b98417d99ad8fb1f85355d3";
WidgetLED led1(V7);
WidgetLED led2(V8);
WidgetLED led3(V9);
#define W5100_CS  10
#define SDCARD_CS 4
int relayPin =  53; // выход на реле
//int echoPin = 20; 
//int trigPin = 21; 
int t=16;
int i=0;
void setup()
{
  pinMode(SDCARD_CS, OUTPUT);
  digitalWrite(SDCARD_CS, HIGH); // Deselect the SD card
  timer.setInterval(2000, sendTemps); //вызов подпрограммы получения температуры 2 сек
  timer.setInterval(1500, sendDuration); //вызов подпрограммы получения расстояния 1.5 сек
  timer.setInterval(1000, relayTemp); //вызов подпрограммы управления реле температуры 1 сек
  Serial.begin(9600);
  Blynk.begin(auth);
  // You can also specify server.
  // For more options, see BoardsAndShields/Arduino_Ethernet_Manual example
  //Blynk.begin(auth, "your_server.com", 8442);
  //Blynk.begin(auth, IPAddress(192,168,1,100), 8888);
//  pinMode(trigPin, OUTPUT); 
//  pinMode(echoPin, INPUT); 
  pinMode(relayPin, OUTPUT);


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


BLYNK_CONNECTED() { //синхронизация
      Blynk.syncAll();
      Blynk.syncVirtual(V11);
}

BLYNK_READ(V3)
{
  Blynk.virtualWrite(V3, 256);
}

BLYNK_WRITE(V1) //обработка виджета слайдера
{
  t=param.asInt();
  Blynk.virtualWrite(V0,t);
}

void sendTemps() //получение и отправка температуры
{
  sensors.requestTemperatures(); // Polls the sensors
  tempIn = sensors.getTempCByIndex(0); // Gets first probe on wire in lieu of by address
  tempOut = sensors.getTempCByIndex(1); // Gets first probe on wire in lieu of by address
  Blynk.virtualWrite(4, tempIn);
  Blynk.virtualWrite(5, tempOut);
  
  
}

void sendDuration() //получение расстояния
{
 
 int dur = ultrasonic.Ranging(CM);
 Blynk.virtualWrite(2, dur); 
  if (dur <= 50) {
    led3.on();
  }
  else {
    led3.off();
  }
  
}

void relayTemp() // управление реле температуры
{
  if (tempIn < t-1 && i==1)  {
    digitalWrite(relayPin, LOW);
    led2.on();
    }
  else   if (tempIn >t || i==0 ) {
    digitalWrite(relayPin, HIGH);
    led2.off();
    }
  
  
}


void loop()
{
  Blynk.run();
  //interrupts();
 timer.run();
 
}

