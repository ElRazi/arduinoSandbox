#include "Ultrasonic.h"
#define REDPIN 11
Ultrasonic ultrasonic(6,7);

void setup() {
Serial.begin(9600);
Serial.println("HC-SR4 testing..");
delay(1000);
pinMode(REDPIN, OUTPUT);
}

void loop()
{
  //lcd.clear();

  analogWrite(REDPIN, ultrasonic.Ranging(CM)*10);
  Serial.println(ultrasonic.Ranging(CM));
  Serial.print("cm  ");
    
  delay(100);
}




