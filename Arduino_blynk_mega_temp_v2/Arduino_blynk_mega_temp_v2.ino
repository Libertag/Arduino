
//
#define BLYNK_PRINT Serial   // Comment this out to disable prints and save space
#define BLYNK_DEBUG
#include <Time.h>
//#include <TimeLib.h>
#include <SPI.h>
//#include <Ethernet2.h>
#include <BlynkSimpleEthernet2.h>
//#include <TimerOne.h>
//#include <OneWire.h>
//#include <DallasTemperature.h>
//#include <SimpleTimer.h>
//#include <DS3231.h>
//#define ONE_WIRE_BUS 40

//
// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "37b6b2042b98417d99ad8fb1f85355d3";


void setup()
{
  // Debug console
  Serial.begin(9600);

  Blynk.begin(auth);
  // You can also specify server:
  //Blynk.begin(auth, "blynk-cloud.com", 8442);
  //Blynk.begin(auth, IPAddress(192,168,1,100), 8442);
  // For more options, see Boards_Ethernet/Arduino_Ethernet_Manual example
}

void loop()
{
  Blynk.run();
}

