#include <Ultrasonic.h>

#define echoPin 7 // Echo Pin
#define trigPin 8 // Trigger Pin

Ultrasonic ultrasonic(7,8);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
 // Serial.setTimeout(4);
}

void loop() {
  int DISTANCE = ultrasonic.Ranging(CM);
  Serial.println(DISTANCE); // put your main code here, to run repeatedly:
  delay(50);
}
