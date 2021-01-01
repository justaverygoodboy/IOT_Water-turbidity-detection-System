#include <DFRobot_PAJ7620U2.h>
#include <SoftwareSerial.h>

DFRobot_PAJ7620U2 paj;
int switch_state = 1;
int water_state = 0;

void setup()
{
  Serial.begin(9600);
  pinMode(4, OUTPUT);
  while(paj.begin() != 0){
    Serial.println("initial PAJ7620U2 failure! Please check if all the connections are fine, or if the wire sequence is correct?");
    delay(100);
  }
  paj.setGestureHighRate(true);
}

void loop()
{
  String JSONData = "{\"water\":";
  int dirtyValue = analogRead(A0);
  float dirvol = dirtyValue*(5.0/1024.0);
  if(switch_state){
    digitalWrite(4,HIGH);
    int waterValue = analogRead(A3);
      if(waterValue<=100){
        water_state=1;
      }else{
        water_state=0;
      }
  }else{
    digitalWrite(4,LOW);
  }
  JSONData.concat(dirvol);
  JSONData.concat(",\"switch\":");
  JSONData.concat(switch_state);
  JSONData.concat(",\"state\":");
  JSONData.concat(water_state);
  JSONData.concat("}");
  Serial.println(JSONData);
  delay(500);
  DFRobot_PAJ7620U2::eGesture_t gesture = paj.getGesture();
  if(gesture != paj.eGestureNone ){
    String description  = paj.gestureDescription(gesture);
    Serial.println(gesture);
    if(gesture==2){
      //左，打开
      switch_state=1;
    }
    if(gesture==1){
      //右，关闭
      switch_state=0;
    }
    delay(500);
  }
}
