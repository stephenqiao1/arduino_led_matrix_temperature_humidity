#include "LedControl.h"
#include "DS3231.h"
#include "Wire.h"
#include "DHT.h"

#define Type DHT11
int sensePin = 4;
DHT HT(sensePin, Type);
int humidity;
int tempC;

const int numDevices = 4;
DS3231 myRTC;

LedControl lc = LedControl(11, 13, 10, numDevices);

unsigned long delayTime = 5000;

int hour;
int mins;

bool h12;
bool hPM;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  int devices = lc.getDeviceCount();
  for (int i = 0; i < devices; i++) {
    lc.shutdown(i, false);
    lc.setIntensity(i, 1);
    lc.clearDisplay(i);
  }

  HT.begin();

  myRTC.setClockMode(false);
  myRTC.setSecond(59);
  myRTC.setMinute(3);
  myRTC.setHour(17);
  myRTC.setDoW(3);
  myRTC.setDate(21);
  myRTC.setMonth(9);
  myRTC.setYear(22);

  h12 = false;
  hPM = false;
}

void displayHumidityAndTemp() {
  Serial.print("The Humidity is: ");
  Serial.print(humidity);
  Serial.println("The temperature is: ");
  Serial.print(tempC);
  
  humidity = HT.readHumidity();
  tempC = HT.readTemperature();

  // numbers
  byte zero[3] = {B00111110, B00100010, B00111110};
  byte one[3] = {B00000000, B00000000, B00111110};
  byte two[3] = {B00101110, B00101010, B00111010};
  byte three[3] = {B00101010, B00101010, B00111110};
  byte four[3] = {B00111000, B00001000, B00111110};
  byte five[3] = {B00111010, B00101010, B00101110};
  byte six[3] = {B00111110, B00101010, B00101110};
  byte seven[3] = {B00100000, B00100000, B00111110};
  byte eight[3] = {B00111110, B00101010, B00111110};
  byte nine[3] = {B00111000, B00101000, B00111110};

  byte* nums[10] = {zero, one, two, three, four, five, six, seven, eight, nine};

  // letter C
  byte c[3] = {B00011110,B00010010,B00010010};

  // display humidity
  // 2nd digit
  for (int i=0;i<9;i++) {
    if ((humidity / 10) == i) {
      lc.setColumn(3,1, nums[i][0]);
      lc.setColumn(3,2, nums[i][1]);
      lc.setColumn(3,3, nums[i][2]);
      }
    }

  // 1st digit
  for (int i=0;i<9;i++) {
    if ((humidity % 10) == i) {
      lc.setColumn(3,5, nums[i][0]);
      lc.setColumn(3,6, nums[i][1]);
      lc.setColumn(3,7, nums[i][2]);
      }
    }

  // display temperature
  // 2nd digit
  for (int i=0;i<9;i++) {
    if ((tempC / 10) == i) {
      lc.setColumn(2,5, nums[i][0]);
      lc.setColumn(2,6, nums[i][1]);
      lc.setColumn(2,7, nums[i][2]);
      }
    }

  // 1st digit
  for (int i=0;i<9;i++) {
    if ((tempC % 10) == i) {
      lc.setColumn(1,1, nums[i][0]);
      lc.setColumn(1,2, nums[i][1]);
      lc.setColumn(1,3, nums[i][2]);
      }
    }

   lc.setColumn(1,5, c[0]);
   lc.setColumn(1,6, c[1]);
   lc.setColumn(1,7, c[2]);
  }

void writeTimeOnMatrix() {
  hour = myRTC.getHour(h12, hPM);
  mins = myRTC.getMinute();
  byte zero[3] = {B00111110, B00100010, B00111110};
  byte one[3] = {B00000000, B00000000, B00111110};
  byte two[3] = {B00101110, B00101010, B00111010};
  byte three[3] = {B00101010, B00101010, B00111110};
  byte four[3] = {B00111000, B00001000, B00111110};
  byte five[3] = {B00111010, B00101010, B00101110};
  byte six[3] = {B00111110, B00101010, B00101110};
  byte seven[3] = {B00100000, B00100000, B00111110};
  byte eight[3] = {B00111110, B00101010, B00111110};
  byte nine[3] = {B00111000, B00101000, B00111110};

  byte* nums[10] = {zero, one, two, three, four, five, six, seven, eight, nine};

  byte colon[1] = {B00010100};
  // hour
  // 2nd digit
  if ((hour / 10) == 1) {
    lc.setColumn(2, 0, one[0]);
    lc.setColumn(2, 1, one[1]);
    lc.setColumn(2, 2, one[2]);
  }
  else if ((hour / 10) == 2) {
    lc.setColumn(2, 0, two[0]);
    lc.setColumn(2, 1, two[1]);
    lc.setColumn(2, 2, two[2]);
  }
  else {
    lc.setColumn(2, 0, zero[0]);
    lc.setColumn(2, 1, zero[1]);
    lc.setColumn(2, 2, zero[2]);
  }
  // 1st digit
  for (int i=0;i<9;i++) {
    if ((hour % 10) == i) {
      lc.setColumn(2,4, nums[i][0]);
      lc.setColumn(2,5, nums[i][1]);
      lc.setColumn(2,6, nums[i][2]);
      }
    }

  lc.setColumn(1,0, colon[0]);
  // mins
  // 2nd digit
  for (int i=0;i<6;i++) {
    if ((mins / 10) == i) {
      lc.setColumn(1,2, nums[i][0]);
      lc.setColumn(1,3, nums[i][1]);
      lc.setColumn(1,4, nums[i][2]);
      }
    }

  // 1st digit
  for (int i=0;i<9;i++) {
    if ((mins % 10) == i) {
      lc.setColumn(1,6, nums[i][0]);
      lc.setColumn(1,7, nums[i][1]);
      lc.setColumn(0,0, nums[i][2]);
      }
    }
}

void loop() {
  int devices = lc.getDeviceCount();
  writeTimeOnMatrix();
  delay(delayTime);
  for (int i=0;i<4;i++) {
  lc.clearDisplay(i);
  }
  displayHumidityAndTemp();
  delay(delayTime);
  for (int i=0;i<4;i++) {
  lc.clearDisplay(i);
  }
}
