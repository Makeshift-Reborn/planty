#include <Arduino.h>
#include <U8g2lib.h>
#include "RTC.h"
#include <NTPClient.h>
#include <WiFiS3.h>
#include <WiFiUdp.h>
#include "arduino_secrets.h"
#include <WiFi.h>

char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS; 
int wifiStatus = WL_IDLE_STATUS;
WiFiUDP Udp;
NTPClient timeClient(Udp);
int sensorPinZero = A0;
int sensorPinOne = A1;
int lightSensor = A2;
int moistValues[2];
bool happyPlanties = true;

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void connectToWiFi(){
  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

  // attempt to connect to WiFi network:
  while (wifiStatus != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    wifiStatus = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }

  Serial.println("Connected to WiFi");
  printWifiStatus();
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while (!Serial);
  connectToWiFi();
  RTC.begin();
  Serial.println("\nStarting connection to server...");
  timeClient.begin();
  timeClient.update();

  // You may change the time zone offset to your local one.
  auto timeZoneOffsetHours = 6;
  auto unixTime = timeClient.getEpochTime() + (timeZoneOffsetHours * 3600);
  Serial.print("Unix time = ");
  Serial.println(unixTime);
  RTCTime timeToSet = RTCTime(unixTime);
  RTC.setTime(timeToSet);
  RTCTime currentTime;
  RTC.getTime(currentTime); 
  Serial.println("The RTC was just set to: " + String(currentTime));

}

int returnValueMoist(int sensor){
  switch(sensor){
    case 0:
      return analogRead(sensorPinZero);
      break;
    case 1:
      return analogRead(sensorPinOne);
      break;
  }
}

int returnLightValue(void){
  return analogRead(lightSensor);
}

bool lowMoistCheck(void){
  for(int j=0; j < 2; j++){
    if(moistValues[j] < 30){
      Serial.print("plantie ");
      Serial.print(j);
      Serial.println(" is a sad plantie :(");
      return false;
    } else {
      Serial.print(j);
      Serial.println(" is a happy plantie!");
    }
  }
}





void loop() {
  // put your main code here, to run repeatedly:
  for(int i = 0; i < 2; i++){
    moistValues[i] = returnValueMoist(i);
  }

  //Serial.print("sensor 0: ");
  //Serial.println(moistValues[0]);
  //Serial.print("sensor 1: ");
  //Serial.println(moistValues[1]);
  //Serial.print("Light sensor: ");
  //Serial.println(returnLightValue());
  //Serial.println("===============================================================================");

  delay(5000);




}
