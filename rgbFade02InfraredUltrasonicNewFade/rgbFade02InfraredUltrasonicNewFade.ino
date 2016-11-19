// Output
int redPin = 9;   // Red LED,   connected to digital pin 9
int grnPin = 10;  // Green LED, connected to digital pin 10
int bluPin = 11;  // Blue LED,  connected to digital pin 11

// Color arrays
int black[3]  = { 0, 0, 0 };
int white[3]  = { 100, 100, 100 };
int red[3]    = { 100, 0, 0 };
int purpule[3]  = { 100, 0, 100 };
int green[3]  = { 0, 100, 0 };
int blue[3]   = { 0, 0, 100 };
int yellow[3] = { 40, 95, 0 };
int dimWhite[3] = { 30, 30, 30 };
// etc.

// Set initial color
int redVal = black[0];
int grnVal = black[1]; 
int bluVal = black[2];

int wait = 1;      // 10ms internal crossFade delay; increase for slower fades
int hold = 0;       // Optional hold when a color is complete, before the next crossFade
int DEBUG = 1;      // DEBUG counter; if set to 1, will write values back via serial
int loopCount = 60; // How often should DEBUG report?
int repeat = 0;     // How many times should we loop before stopping? (0 for no stop)
int j = 0;          // Loop counter for repeat

// Initialize color variables
int prevR = redVal;
int prevG = grnVal;
int prevB = bluVal;
///////////////////////////////////
/////////////////////////////////End of new fade initializations
///////////////////////////////

int getPopularElement(float* a, int length, int delta)
{
  int count = 1, tempCount;
  float popular = a[0];
  float temp = 0;
  for (int i = 0; i < (length - 1); i++)
  {
    temp = a[i];
    tempCount = 0;
    for (int j = 1; j < length; j++)
    {
      if (abs(temp - a[j]) < delta)
        tempCount++;
    }
    if (tempCount > count)
    {
      popular = temp;
      count = tempCount;
    }
  }
  return (int)popular;
}

int IRpin = A0; 
const int arrOfData_size=20;
const int delta=10;
float arrOfData[arrOfData_size];// analog pin for reading the IR sensor
int arrOfData_current = 0;
int prev = 0;
void push(float elem) {
  if(arrOfData_current >= arrOfData_size - 1) {
    return;
  }
  arrOfData[arrOfData_current++] = elem;
}

// linear coefficients to calculate the distance
#define SHARP_GP2Y0A710K0F_K 137.5
#define SHARP_GP2Y0A710K0F_B 1.125

float get_distance_from_volts(float volts)
{
  return SHARP_GP2Y0A710K0F_K/(volts - SHARP_GP2Y0A710K0F_B);
}

float distanceFilter(float value)
{
  push(value);
  if(arrOfData_current == arrOfData_size - 1 ){
      prev = getPopularElement(arrOfData, arrOfData_size, delta);
      arrOfData_current = 0;
   }

   return prev;
}

///////////// ultrasonic settings
#define echoPin 8 // Echo Pin
#define trigPin 7 // Trigger Pin
#define LEDPin 13 // Onboard LED

int maximumRange = 50; // Maximum range needed
int minimumRange = 5; // Minimum range needed
long duration, distance; // Duration used to calculate distance
/////////


#define switchPin 2

#define FADESPEED 20 

unsigned long x;
int LED0 = 9;//blue
int LED1 = 10;//green
int LED2 = 11;//red

int STEP_TIME = 50;
int DELAY_TIME = 10;


int STEPS_COUNT = 0;
int DISTANCE = 0;
int prevDist = 0;


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
/////////////ultrasonic
 pinMode(trigPin, OUTPUT);
 pinMode(echoPin, INPUT);
 pinMode(LEDPin, OUTPUT); // Use LED indicator (if required)
 /////////////////////////////////
  
}

int getUltrasonicDistance() {
  /* The following trigPin/echoPin cycle is used to determine the
 distance of the nearest object by bouncing soundwaves off of it. */ 
 digitalWrite(trigPin, LOW); 
 delayMicroseconds(2); 

 digitalWrite(trigPin, HIGH);
 delayMicroseconds(10); 
 
 digitalWrite(trigPin, LOW);
 duration = pulseIn(echoPin, HIGH);
 
 //Calculate the distance (in cm) based on the speed of sound.
 distance = duration/58.2;
 
 if (distance >= maximumRange || distance <= minimumRange){
 /* Send a negative number to computer and Turn LED ON 
 to indicate "out of range" */
 //Serial.println("-1");
 digitalWrite(LEDPin, HIGH); 
 }
 else {
 /* Send the distance to the computer using Serial protocol, and
 turn LED OFF to indicate successful reading. */
 Serial.println(distance);
 digitalWrite(LEDPin, LOW); 
 }
 
 return distance;
}

int getPopularElement_old(int* a)
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

const int max_distance = 400;

int smoothColor(int current, int target){
  return current + (target - current)/8;
}

int getStrength(int cur, int mini, int maxi, bool shutOffWhenOutOfLimit = false, bool inverted = false);
void normalMode();

void loop()
{
  //int mode = digitalRead(switchPin);

 // if(mode == 0) {
  //
 // normalMode();
 // } else {
  //  Serial.println("not normal");
  //  simpleMode();
//  }
  newfademode();
}
int getDistance() {
  float volts = analogRead(IRpin)*0.0048828125;   // value from sensor * (5/1024) - if running 3.3.volts then change 5 to 3.3
  float dist_cm = get_distance_from_volts(volts); 
  return distanceFilter(dist_cm);
}

void normalMode()
{
  int distances[] = {5,20,30,40,50};
  int sb_local, sr_local, sg_local;
  //DISTANCE = getDistance();  
  DISTANCE = distanceFilter((int)getUltrasonicDistance());                                                                                                                                                                                                                                                                           
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

///////////////////////////
///////////////////Crossfade 
void newfademode()
{
  int distances[] = {5,20,30,40,50};
  int sb_local, sr_local, sg_local;
  //DISTANCE = getDistance();  
  int setDist = getUltrasonicDistance(); 
  DISTANCE =  setDist;                                                                                                                                                                                                                                                               
  Serial.print(DISTANCE); 
  Serial.println(" CM"); 
  if (DISTANCE <= distances[0]){
        crossFade(red);
       // crossFade(yellow);
      }
      else if ( (DISTANCE > distances[0]) && (DISTANCE <= distances[1]) ) {
        crossFade(red);
        crossFade(yellow);
      }
      else if ( (DISTANCE > distances[1]) && (DISTANCE <= distances[2]) ) {
        crossFade(yellow);
        crossFade(green);
      }
      else if ( (DISTANCE > distances[2]) && (DISTANCE <= distances[3]) ) {
        crossFade(green);
        crossFade(blue);
      }
      else if ( (DISTANCE > distances[3]) && (DISTANCE <= distances[4]) ) {
        crossFade(blue);
        crossFade(purpule);
      }
      else
      {
        crossFade(purpule);
        crossFade(dimWhite);
        
      }

  }


void newfademode1()
{
  int distances[] = {5,20,30,40,50};
  int sb_local, sr_local, sg_local;
  //DISTANCE = getDistance();  
  DISTANCE = distanceFilter((int)getUltrasonicDistance());                                                                                                                                                                                                                                                                           
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
        crossFade(red);
       // crossFade(yellow);
      }
      else if ( (setDist > distances[0]) && (setDist <= distances[1]) ) {
        crossFade(red);
        crossFade(yellow);
      }
      else if ( (setDist > distances[1]) && (setDist <= distances[2]) ) {
        crossFade(yellow);
        crossFade(green);
      }
      else if ( (setDist > distances[2]) && (setDist <= distances[3]) ) {
        crossFade(green);
        crossFade(blue);
      }
      else if ( (setDist > distances[3]) && (setDist <= distances[4]) ) {
        crossFade(blue);
        crossFade(dimWhite);
      }
      else
      {
        crossFade(white);

      }

  }
 /* if (distance < 25){
    crossFade(red);
    crossFade(yellow);
   } else {
    crossFade(dimWhite);
    crossFade(blue);
  
   }

  if (repeat) { // Do we loop a finite number of times?
    j += 1;
    if (j >= repeat) { // Are we there yet?
      exit(j);         // If so, stop.
    }
  }*/
}


int calculateStep(int prevValue, int endValue) {
  int step = endValue - prevValue; // What's the overall gap?
  if (step) {                      // If its non-zero, 
    step = 1020/step;              //   divide by 1020
  } 
  return step;
}

/* The next function is calculateVal. When the loop value, i,
*  reaches the step size appropriate for one of the
*  colors, it increases or decreases the value of that color by 1. 
*  (R, G, and B are each calculated separately.)
*/

int calculateVal(int step, int val, int i) {

  if ((step) && i % step == 0) { // If step is non-zero and its time to change a value,
    if (step > 0) {              //   increment the value if step is positive...
      val += 1;           
    } 
    else if (step < 0) {         //   ...or decrement it if step is negative
      val -= 1;
    } 
  }
  // Defensive driving: make sure val stays in the range 0-255
  if (val > 255) {
    val = 255;
  } 
  else if (val < 0) {
    val = 0;
  }
  return val;
}

/* crossFade() converts the percentage colors to a 
*  0-255 range, then loops 1020 times, checking to see if  
*  the value needs to be updated each time, then writing
*  the color values to the correct pins.
*/

void crossFade(int color[3]) {
  // Convert to 0-255
  int R = (color[0] * 255) / 100;
  int G = (color[1] * 255) / 100;
  int B = (color[2] * 255) / 100;

  int stepR = calculateStep(prevR, R);
  int stepG = calculateStep(prevG, G); 
  int stepB = calculateStep(prevB, B);

  for (int i = 0; i <= 1020; i++) {
    redVal = calculateVal(stepR, redVal, i);
    grnVal = calculateVal(stepG, grnVal, i);
    bluVal = calculateVal(stepB, bluVal, i);

    analogWrite(redPin, redVal);   // Write current values to LED pins
    analogWrite(grnPin, grnVal);      
    analogWrite(bluPin, bluVal); 

    delay(wait); // Pause for 'wait' milliseconds before resuming the loop

   /* if (DEBUG) { // If we want serial output, print it at the 
      if (i == 0 or i % loopCount == 0) { // beginning, and every loopCount times
        Serial.print("Loop/RGB: #");
        Serial.print(i);
        Serial.print(" | ");
        Serial.print(redVal);
        Serial.print(" / ");
        Serial.print(grnVal);
        Serial.print(" / ");  
        Serial.println(bluVal); 
      } 
      DEBUG += 1;
    }*/
  }
  // Update current values for next loop
  prevR = redVal; 
  prevG = grnVal; 
  prevB = bluVal;
  delay(hold); // Pause for optional 'wait' milliseconds before resuming the loop
}

