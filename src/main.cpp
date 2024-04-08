#include <Arduino.h>
#include <U8g2lib.h>
#include <NTPClient.h>
#include <WiFiS3.h>
#include <WiFiUdp.h>
#include "RTC.h"
#include "arduino_secrets.h"

char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;
int wifiStatus = WL_IDLE_STATUS;
WiFiUDP ntpUDP;
NTPClient timeClient(netUDP);
int sensorPinZero = A0;
int sensorPinOne = A1;
int lightSensor = A2;
int moistValues[2];
bool happyPlants = true;




// put function declarations here:
int myFunction(int, int);

void setup() {
  // put your setup code here, to run once:
  int result = myFunction(2, 3);
}

void loop() {
  // put your main code here, to run repeatedly:
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}