#include "LedControl.h"
#include "DS3231.h"
#include "Wire.h"

const int numDevices = 4;
DS3231 myRTC;
DateTime myDT(2022, 9, 20, 18, 7, 00);

LedControl lc = LedControl(11, 13, 10, numDevices);

unsigned long delaytime = 1000;


void setup() {
  Serial.begin(9600);
  Wire.begin();
  Serial.print(myDT.day());
  int devices = lc.getDeviceCount();
  for (int i = 0; i < devices; i++) {
    lc.shutdown(i, false);
    lc.setIntensity(i, 1);
    lc.clearDisplay(i);
  }
}

void writeArduinoOnMatrix() {
  byte A[5] = {B01111110, B10001000, B10001000, B10001000, B01111110};
  byte E[5] = {B11111110, B10010010, B10010010, B10010010, B10010010};
  byte r[5] = {B00111110, B00010000, B00100000, B00100000, B00010000};
  byte d[5] = {B00011100, B00100010, B00100010, B00010010, B11111110};
  byte u[5] = {B00111100, B00000010, B00000010, B00000100, B00111110};
  byte i[5] = {B00000000, B00100010, B10111110, B00000010, B00000000};
  byte n[5] = {B00111110, B00010000, B00100000, B00100000, B00011110};
  byte o[5] = {B00011100, B00100010, B00100010, B00100010, B00011100};
  byte H[5] = {B11111110, B00010000, B00010000, B00010000, B11111110};
  byte L[1] = {B11111110};
  byte w[5] = {B00011100, B00000010, B00011100, B00000010, B00011100};

  // E
  lc.setColumn(3, 0, E[0]);
  lc.setColumn(3, 1, E[1]);
  lc.setColumn(3, 2, E[2]);
  lc.setColumn(3, 3, E[3]);
  lc.setColumn(3, 4, E[4]);

  // w
  lc.setColumn(3, 6, w[0]);
  lc.setColumn(3, 7, w[1]);
  lc.setColumn(2, 0, w[2]);
  lc.setColumn(2, 1, w[3]);
  lc.setColumn(2, 2, w[4]);
}

void writeTimeOnMatrix() {
  int hour = myDT.hour();
  int mins = myDT.minute();

  Serial.print(hour % 10);
  Serial.println(hour);

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
    if ((hour % 10) == i) {
      lc.setColumn(1,6, nums[i][0]);
      lc.setColumn(1,7, nums[i][1]);
      lc.setColumn(0,0, nums[i][2]);
      }
    }
}

void loop() {
  int devices = lc.getDeviceCount();
  writeTimeOnMatrix();
}
