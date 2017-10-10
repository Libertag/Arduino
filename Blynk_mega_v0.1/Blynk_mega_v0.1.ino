

#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <SPI.h>
#include <Ethernet.h>
#include <BlynkSimpleEthernet.h>
#include <TimerOne.h>
#include <Ultrasonic.h>
Ultrasonic ultrasonic(21,20);// (echoPin,triqPin)
// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "d138ea882f7641d0aca13d400db7a194";

#define W5100_CS  10
#define SDCARD_CS 4
//int echoPin = 20; 
//int trigPin = 21; 
void setup()
{
  pinMode(SDCARD_CS, OUTPUT);
  digitalWrite(SDCARD_CS, HIGH); // Deselect the SD card
  Timer1.initialize(1500000);
  Timer1.attachInterrupt(sendDuration);
  Serial.begin(9600);
  Blynk.begin(auth);
  // You can also specify server.
  // For more options, see BoardsAndShields/Arduino_Ethernet_Manual example
  //Blynk.begin(auth, "your_server.com", 8442);
  //Blynk.begin(auth, IPAddress(192,168,1,100), 8888);
//  pinMode(trigPin, OUTPUT); 
//  pinMode(echoPin, INPUT); 
  
}

void sendDuration() //отправка температуры
{
 //  int duration, cm; 
 // digitalWrite(trigPin, LOW); 
 // delayMicroseconds(2); 
 // digitalWrite(trigPin, HIGH); 
 // delayMicroseconds(10); 
 // digitalWrite(trigPin, LOW); 
 // duration = pulseIn(echoPin, HIGH); 
 // cm = duration / 58;
 // Serial.print(cm); 
 // Serial.println(" cm"); 
 // delay(100);
  Serial.print(ultrasonic.Ranging(CM));
  Serial.println("cm");
  
  
}




void loop()
{
  Blynk.run();
  interrupts();
}

