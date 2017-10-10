const int analogInPin = A0;  // Куда подключили MQ7
const int ledPin = 5;                 // Куда подключили диод

int sensorValue = 0;       




void setup() {
  Serial.begin(9600); 
  pinMode(ledPin, OUTPUT);      
}

void loop() {
  sensorValue = analogRead(analogInPin);            
  if (sensorValue >= 150)
  {
    digitalWrite(ledPin, HIGH);   
  }
  else
  {
  digitalWrite(ledPin, LOW);    
  }

  Serial.print("sensor = " );                       
  Serial.println(sensorValue);     

  delay(10);                     
}
