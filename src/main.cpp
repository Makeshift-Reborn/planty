#include <Arduino.h>
#include <U8g2lib.h>
#include <NTPClient.h>
#include <WiFiS3.h>
#include <WiFiUdp.h>
#include "RTC.h"
#include "arduino_secrets.h"

// changing this to a one plant thing for now, I am just commenting out the parts that will have to do with multiple plants
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;
int wifiStatus = WL_IDLE_STATUS;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "us.pool.ntp.org", -18000);
int sensorPinZero = A0;
// int sensorPinOne = A1;
int lightSensor = A2;
// int moistValues[2];
bool happyPlants = true;
int directionPin = 12;
int pwmPin = 3;
int brakePin = 9;
bool directionState;
int relayPin = 2;

void printWifiStatus(void)
{
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
}

void connectToWiFi(void)
{
  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE)
  {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true)
      ;
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION)
  {
    Serial.println("Please upgrade the firmware");
  }

  // attempt to connect to WiFi network:
  while (wifiStatus != WL_CONNECTED)
  {
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

int returnValueMoist(void)
{
  /* switch (sensor)
  {
  case 0:
    return analogRead(sensorPinZero);
    break;
  case 1:
    return analogRead(sensorPinOne);
    break;
  } */

  return analogRead(sensorPinZero);
}

int returnLightValue(void)
{
  return analogRead(lightSensor);
}

bool lowMoistCheck(void)
{
  /* for(int j=0; j < 2; j++){
    if(moistValues[j] < 30){
      Serial.print("plantie ");
      Serial.print(j);
      Serial.println(" is a sad plantie :(");
      return false;
    } else {
      Serial.print(j);
      Serial.println(" is a happy plantie!");
      return true;
    }
  } */
  int moistValue = returnValueMoist();
  bool result; // Declare the variable before using it
  if (moistValue < 30)
  {
    Serial.println("Plantie is sad :(");
    result = true; // Assign a value to the declared variable
    return result; // Use the declared variable in the return statement
  }
  else
  {
    Serial.println("Plantie is happy!");
    result = false; // Assign a value to the declared variable
    return result; // Use the declared variable in the return statement
  }
}

void runPump(void)
{
  digitalWrite(directionPin, HIGH);
  digitalWrite(brakePin, LOW);
  analogWrite(pwmPin, 255);
  delay(3000);
  analogWrite(pwmPin, 0);
}

void waterPlantie(void)
{
  if (lowMoistCheck())
  {
    Serial.println("Watering planties");
    runPump();
  }
  else
  {
    Serial.println("Planties are happy, no need to water");
  }
}

void lightOn(void){
  digitalWrite(relayPin, HIGH);
}
void lightOff(void){
  digitalWrite(relayPin, LOW);
}

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  while (!Serial)
    ;
  pinMode(directionPin, OUTPUT);
  pinMode(brakePin, OUTPUT);
  pinMode(relayPin, OUTPUT);
  connectToWiFi();
  RTC.begin();
  timeClient.begin();
  timeClient.update();
  auto epochTime = timeClient.getEpochTime();
  RTCTime timeSet = RTCTime(epochTime);
  RTC.setTime(timeSet);
  RTCTime currentTime;
  RTC.getTime(currentTime);
  Serial.println("The RTC was just set to: " + String(currentTime));

}

void loop()
{
  // put your main code here, to run repeatedly:

  /* for(int i = 0; i < 2; i++){
    moistValues[i] = returnValueMoist(i);
  } */
  /*   Serial.print("NTP time: ");
    Serial.println(timeClient.getFormattedTime());
    Serial.println("===============================================================================");
    Serial.print("RTC time: ");
    RTCTime currentTime;
    RTC.getTime(currentTime);
    Serial.println(currentTime);
    Serial.println("==============================================================================="); */

  /* Serial.print("sensor 0: ");
  Serial.println(moistValues[0]);
  Serial.print("sensor 1: ");
  Serial.println(moistValues[1]);
  Serial.print("Light sensor: ");
  Serial.println(returnLightValue());
  Serial.println("==============================================================================="); */

  waterPlantie();
  // waterPlantie() will call lowMoistCheck() which will call returnValueMoist() to get the moisture value of the plant and check if it is below a value of 30
  // if it is below 30, it will return true and waterPlantie() will call runPump() to water the plant
  // if it is above 30, it will return false and waterPlantie() will print that the plant is happy and not water it
  // this should run every 5 seconds with a delay of 5000
  RTCTime currentTime;
  RTC.getTime(currentTime);
  int currentHour = currentTime.getHour();
  if(currentHour >= 6 && currentHour <= 18){
    lightOn();
  } else {
    lightOff();
  }
  //this is a test!!!!
  delay(5000);
}
