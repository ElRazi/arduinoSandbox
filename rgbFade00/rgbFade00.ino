#include <Ultrasonic.h>

unsigned long x;
int LED0 = 3;
int LED1 = 5;
int LED2 = 6;
int LED3 = 9;
int LED4 = 10;
int LED5 = 11;
int a,b,c = 0;

Ultrasonic ultrasonic(7,8);

void setup() {

  Serial.begin(9600);
  Serial.setTimeout(4);
  pinMode(LED0, OUTPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(LED5, OUTPUT);

}

int getStrength(int cur, int mini, int maxi, bool shutOffWhenOutOfLimit = false, bool inverted = false)
{
  int str = 0;
  if(cur <= mini)
  {
    if(inverted)
      str = 0;
    else
      str = shutOffWhenOutOfLimit ? 255 : 0;
  }
  else if(cur >= maxi)
    if(inverted)
      str = shutOffWhenOutOfLimit ? 255 : 0;
    else
      str = 0;
   else
   {
      int range = maxi - mini;
      int pos_in_range = cur - mini;
      str = 255/range * (range - pos_in_range);
      str = inverted ? 255 - str : str;
   }
    
  return str;
    
}

int getStrengthRez(int cur, int mini, int maxi)
{
  int str = 0;
  int diap = maxi-mini;
  int current = cur - mini;

  str = (255/diap) * current;
  return str; 
  
  
}

const int speedTime = 300; //initial - 100
//const int speedBrightness = 1; // initial - 30

int gr = 0;
int gg = 0;
int gb = 0;
int prevTime = 0;

int modify = 0;

void updateStrength( int r, int g, int b)
{
  int now = millis();
  int timePass = now - prevTime;
  if(timePass > speedTime)
  {
    Serial.print("modify: ");
    Serial.println(modify++);
    gr = modifyStrength(gr, r);
    gg = modifyStrength(gr, g);
    gb = modifyStrength(gr, b);
    prevTime = millis();
  }
}

int modifyStrength(int curr, int dest)
{
  if(curr == dest)
  {
    return curr;
  }

  int delta = curr - dest;

  int speedBrightness = (curr - dest)/(speedTime / 50 );
  return curr + speedBrightness;
  //return delta > 0 ? curr - speedBrightness : curr + speedBrightness;
}

void loop()
{
  int dist = ultrasonic.Ranging(CM);
  delay(50);
  //Serial.println(dist);
//  if(dist>30 && dist<45){
//    analogWrite(LED,strength ); 
//  }
//  if(dist>15 && dist < 30){
//    
//  }
//
//  if (dist > 0 && dist < 15) {
//    
//  
//  }

    int sgl = getStrength(dist, 100, 200, true, true);
    int srl = getStrength (dist, 0, 100, true);

    Serial.print("sg: ");
    Serial.println(sgl);
    Serial.print("sr: ");
    Serial.println(srl);
    
    int sbl = getStrength(dist, 15, 30);
    updateStrength( srl, sgl, sbl);

    analogWrite(LED0, gg );
    analogWrite(LED1, gg);
    analogWrite(LED2, gr);
    analogWrite(LED3, gr );
    analogWrite(LED4, gg);
    analogWrite(LED5, gr);
}

