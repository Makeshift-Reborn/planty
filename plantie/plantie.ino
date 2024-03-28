#include <Arduino.h>
#include <U8g2lib.h>

int sensorPinZero = A0;
int sensorPinOne = A1;
int lightSensor = A2;
int moistValues[2];
bool happyPlanties = true;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

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

  Serial.print("sensor 0: ");
  Serial.println(moistValues[0]);
  Serial.print("sensor 1: ");
  Serial.println(moistValues[1]);
  Serial.print("Light sensor: ");
  Serial.println(returnLightValue());
  Serial.println("===============================================================================");

  delay(5000);




}
