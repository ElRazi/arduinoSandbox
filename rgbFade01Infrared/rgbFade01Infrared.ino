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

//int distanceFilter_old(int dist) {
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
int distanceFilter_old(int dist) {
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
  return getPopularElement_old(filterArr);
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
int getDistance() {
  float volts = analogRead(IRpin)*0.0048828125;   // value from sensor * (5/1024) - if running 3.3.volts then change 5 to 3.3
  float dist_cm = get_distance_from_volts(volts); 
  return distanceFilter(dist_cm);
}

void normalMode()
{
  int distances[] = {5,20,30,40,50};
  int sb_local, sr_local, sg_local;
  //DISTANCE = distanceFilter_old(ultrasonic.Ranging(CM));
 // DISTANCE = distanceFilter_old(getDistance_old());
 // DISTANCE = ultrasonic.Ranging(CM);
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

