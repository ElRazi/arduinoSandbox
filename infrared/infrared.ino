//
// SHARP IR sensor test distance calculation (cm)
//
// http://robocraft.ru
//

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

void setup() {
  
  Serial.begin(9600);                             // start the serial port
}

void loop() {
  
  // 5v/1024 = 0.0048828125
  
  float volts = analogRead(IRpin)*0.0048828125;   // value from sensor * (5/1024) - if running 3.3.volts then change 5 to 3.3
  float dist_cm = get_distance_from_volts(volts);
  int filtered = distanceFilter(dist_cm);
  Serial.println(filtered);                       // print the distance
  delay(10);                                     // arbitary wait time.
}
