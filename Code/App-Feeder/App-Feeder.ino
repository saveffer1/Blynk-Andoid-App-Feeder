#include "FirebaseESP8266.h"
#include <ESP8266WiFi.h>
#include <Servo.h>

#define FIREBASE_HOST "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx/" //Without http:// or https:// schemes
#define FIREBASE_AUTH "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
// แก้
#define WIFI_SSID "ชื่อ wifi"
#define WIFI_PASSWORD "รหัสผ่าน"

FirebaseData firebaseData;
int sw = 0;
int set = 0;

Servo petGate;
int gateStart = 45;
int gateEnd = 105;
int gateOpenTime = 150;

void setup() {
    connectWifi();
    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
    petGate.attach(D6);
    petGate.write(gateStart);
}

void loop() {
    //Firebase.getString
    sw = Firebase.getInt(firebaseData,"/feeder/sw");
    set = Firebase.getInt(firebaseData,"/feeder/set");
    if(sw == 1){
      Serial.println("Opened");
      if(set == 1){
        petGate.write(gateEnd);
        delay(gateOpenTime);
        petGate.write(gateStart);
      }
      else if(set == 0){
        for(int i=0;i<5;i++){
          petGate.write(gateEnd);
          delay(gateOpenTime);
          //petGate.write(gateStart);
          delay(gateOpenTime);
        }
      }
      else{
        Serial.println("error val is invalid!! 0xSETNOT0OR1");
      }
    }
    else if(sw == 0){
      Serial.println("Closed");
    }
    else{
      Serial.println("error val is invalid!! 0xSWNOt0OR1");
    }
}

void connectWifi() {
    Serial.begin(115200);
    Serial.println(WiFi.localIP());
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("connecting");
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }
    Serial.println();
    Serial.print("connected: ");
    Serial.println(WiFi.localIP());
}
