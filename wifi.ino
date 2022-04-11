#include "FirebaseESP8266.h"
#include <ESP8266WiFi.h>

#include <SoftwareSerial.h>

#define WIFI_SSID "UR_WIFI_ID"
#define WIFI_PASSWORD "UR_WIFI_PASSWORD"
#define FIREBASE_HOST "Link to Firebase database"
#define FIREBASE_AUTH "Firebase authentification key "
#define distance_obs 50
#define obstacle "NO_obstacle"
#define error "No error"
 int vitesse =0;
 String message ="___";
 
FirebaseData firebaseData;

void wifiConnect()
{
    Serial.begin(115200);
 // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }

  int teller = 0;
  while (WiFi.status() != WL_CONNECTED)
  { // Wait for the Wi-Fi to connect
    delay(1000);
    Serial.print(++teller);
    Serial.print(' ');
  }
  Serial.println("Connection established!");
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());         // Send the IP address of the ESP8266 to the computer
}

void fire_fail(){

//Failed?, get the error reason from firebaseData

    Serial.print("Error in setting ");
    Serial.println(firebaseData.errorReason());
    }
    
void data_transfer(){
if(Firebase.setString(firebaseData,"/robot/error", error))
  {
    //Success
  //Serial.println("Set string data success");

  }else{
fire_fail();
  }
delay(10);
if(Firebase.setString(firebaseData,"/robot/obstacle",obstacle))
  {
    //Success
  //Serial.println("Set string data success");

  }else{
fire_fail();
  }
delay(10);
if(Firebase.setString(firebaseData,"/robot/App_message", message))
  {
    //Success
 //  Serial.println("Set string data success");

  }else{
fire_fail();
  }
delay(10);
if(Firebase.getInt(firebaseData, "/robot/vitesse"))
  {
    //Success
   // Serial.print("Get int data success, int = ");
    vitesse=firebaseData.intData();
   // Serial.println(vitesse);

  }else{
 fire_fail();
  }

return;
}

void data_bridge(){ 
 String message = "";
  while (Serial.available())
    message.concat((char) Serial.read());
  if (message != "")
    Serial.println(message);
  
  delay(1000);
}

void setup()
{
  Serial.begin(115200);
  delay(1000);
  Serial.println('\n');
  wifiConnect();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
  delay(30);
  Serial.begin(115200);
while (!Serial) {
; // wait for serial port to connect. Needed for native USB port only
}
}

void loop()
{
  data_transfer();
  Serial.println( message+"#"+vitesse);
  data_bridge();
  
  if (WiFi.status() != WL_CONNECTED)
  {
    wifiConnect();
  }
  delay(1000);
}
