#include <Ultrasonic.h>

#define echoPin 8 // Echo Pin
#define trigPin 7 // Trigger Pin

#define switchPin 2

#define FADESPEED 20 

int IRpin = A0;                                    // analog pin for reading the IR sensor

// linear coefficients to calculate the distance
#define SHARP_GP2Y0A710K0F_K 137.5
#define SHARP_GP2Y0A710K0F_B 1.125

float get_distance_from_volts(float volts)
{
  return SHARP_GP2Y0A710K0F_K/(volts - SHARP_GP2Y0A710K0F_B);
}

unsigned long x;
int LED0 = 9;//blue
int LED1 = 10;//green
int LED2 = 11;//red
Ultrasonic ultrasonic(7,8);

int STEP_TIME = 50;
int DELAY_TIME = 10;


int STEPS_COUNT = 0;
int DISTANCE = ultrasonic.Ranging(CM);
int prevDist = ultrasonic.Ranging(CM);


//timing
int prevTime = millis();
int currTime = millis();


int deltaDist = 0;
int setDist = 0;



//RGB Strength

int sb = 0; //blue
int sr = 0; //red
int sg = 0; //green


//filter
int filterArr[10] = {0,0,0,0,0,0,0,0,0,0};
int incr = 0;

void setup() {

  Serial.begin(9600);
  Serial.setTimeout(4);
  pinMode(LED0, OUTPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(switchPin, INPUT);
  digitalWrite(switchPin, HIGH);
  
}

int getDistance0()
{
 digitalWrite(trigPin, LOW); 
 delayMicroseconds(2); 

 digitalWrite(trigPin, HIGH);
 delayMicroseconds(10); 
 
 digitalWrite(trigPin, LOW);
 int duration = pulseIn(echoPin, HIGH);
 
 //Calculate the distance (in cm) based on the speed of sound.
 return duration/58.2;
}

float getDistance()
{
  float volts = analogRead(IRpin)*0.0048828125;   // value from sensor * (5/1024) - if running 3.3.volts then change 5 to 3.3
  float dist_cm = get_distance_from_volts(volts);
  return dist_cm;
}

int getPopularElement(int* a)
{
  int count = 1, tempCount;
  int popular = a[0];
  int temp = 0;
  for (int i = 0; i < 10; i++)
  {
    temp = a[i];
    tempCount = 0;
    for (int j = 1; j < 10; j++)
    {
      if (temp == a[j])
        tempCount++;
    }
    if (tempCount > count)
    {
      popular = temp;
      count = tempCount;
    }
  }
  return popular;
}

//int distanceFilter(int dist) {
//  if(incr == 9) {
//    incr = 0;
//    trueAnswer = 0;
//    for(int i = 0; i < +1; i++){
//      
//    }
//    trueAnswer = trueAnswer/10;
//  } else {
//    filterArr[incr] = dist;
//    incr++;
//  }
//  return trueAnswer;
//};
const int max_distance = 400;
int distanceFilter(int dist) {
  if(dist > max_distance) {
    dist = max_distance;
    //int currentIndex = incr > 0 ? incr-1 : 9;
    //return filterArr[incr-1];
  }
  
  filterArr[incr] = dist;
  if(incr == 9) {
    incr = 0;
  } else {
    incr++;
  }  
  return getPopularElement(filterArr);
};

//int smoothColor(int current, int target){
//  return (current +target )/2;
//}

int smoothColor(int current, int target){
  return current + (target - current)/8;
}

int getStrength(int cur, int mini, int maxi, bool shutOffWhenOutOfLimit = false, bool inverted = false);
void normalMode();

void loop()
{
  int mode = digitalRead(switchPin);

 // if(mode == 0) {
    normalMode();
 // } else {
  //  Serial.println("not normal");
  //  simpleMode();
//  }
}

void normalMode()
{
  int distances[] = {70,120,180,270,350};
  int sb_local, sr_local, sg_local;
  //DISTANCE = distanceFilter(ultrasonic.Ranging(CM));
  DISTANCE = distanceFilter(getDistance());
 // DISTANCE = ultrasonic.Ranging(CM);
  Serial.print(DISTANCE); 
  Serial.println(" CM"); 
  currTime = millis();
  delay(DELAY_TIME);
  if(currTime - prevTime > STEP_TIME){
      deltaDist = DISTANCE - prevDist;
      setDist = prevDist;
      STEPS_COUNT = deltaDist/(STEP_TIME/DELAY_TIME);
      prevDist = DISTANCE; 
      prevTime = currTime;
  } else {
      setDist += STEPS_COUNT;  

      if (setDist <= distances[0]){
        sr_local = 255;
        sg_local = 0;
        sb_local = 0;
      }
      else if ( (setDist > distances[0]) && (setDist <= distances[1]) ) {
            sr_local= 255;
            sg_local = getStrength(setDist, distances[0], distances[1], true, true);
            sb_local = 0;
      }
      else if ( (setDist > distances[1]) && (setDist <= distances[2]) ) {
            sr_local = getStrength(setDist, distances[1], distances[2], true, false);
            sg_local = 255;
            sb_local = 0;
      }
      else if ( (setDist > distances[2]) && (setDist <= distances[3]) ) {
            sr_local = 0;
            sg_local = 255;
            sb_local = getStrength(setDist, distances[2], distances[3], true, true);
      }
      else if ( (setDist > distances[3]) && (setDist <= distances[4]) ) {
            sr_local = 0;
            sg_local = getStrength(setDist, distances[3], distances[4], true, false);
            sb_local = 255;
      }
      else
      {
        sr_local = 0;
        sg_local = 0;
        sb_local = 255;
      }

      sr = smoothColor (sr, sr_local);
      sg = smoothColor (sg, sg_local);
      sb = smoothColor (sb, sb_local);
      
      analogWrite(LED0, sb);
      analogWrite(LED1, sg);
      analogWrite(LED2, sr);

  }


}

int getStrength(int cur, int mini, int maxi, bool shutOffWhenOutOfLimit, bool inverted)
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

void simpleMode()
{
 int r, g, b;
  
  // fade от голубого к фиолетовому
  for (r = 0; r < 256; r++) { 
    analogWrite(LED0, r);
    delay(FADESPEED);
  } 
  // fade от фиолетового к красному
  for (b = 255; b > 0; b--) { 
    analogWrite(LED1, b);
    delay(FADESPEED);
  } 
  // fade от красного к желтому
  for (g = 0; g < 256; g++) { 
    analogWrite(LED2, g);
    delay(FADESPEED);
  } 
  // fade от желтого к зеленому
  for (r = 255; r > 0; r--) { 
    analogWrite(LED0, r);
    delay(FADESPEED);
  } 
  // fade от зеленого к зеленовато-голубому
  for (b = 0; b < 256; b++) { 
    analogWrite(LED1, b);
    delay(FADESPEED);
  } 
  // fade от зеленовато-голубого к голубому
  for (g = 255; g > 0; g--) { 
    analogWrite(LED2, g);
    delay(FADESPEED);
  }  
}

