#include <Servo.h>  //servo library
Servo myservo;      // create servo object to control servo

#define Echo A4
#define Trig A5

#define LT_R !digitalRead(10)
#define LT_M !digitalRead(4)
#define LT_L !digitalRead(2)

#define ENA 5
#define ENB 6
#define IN1 7
#define IN2 8
#define IN3 9
#define IN4 11

#define LEDRPIN  14    //LED Front Right   
#define LEDGPIN  15    //LED Front Left   
#define BuzzPIN 18    //Horn Buzzer  
String last_path ;
String next_path ;
char cmd;            
int carSpeed = 100;   
bool R = true ;
int rightDistance = 0, leftDistance = 0, middleDistance = 0;
int forward_distance=0 ,forward_demande=0 ,moved_distance=0;
void back(){ 
  analogWrite(ENA, carSpeed);
  analogWrite(ENB, carSpeed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  Serial.println("Forward");
}

void forward() {
  analogWrite(ENA, carSpeed);
  analogWrite(ENB, carSpeed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  Serial.println("Back");
}

void left() {
  analogWrite(ENA, carSpeed);
  analogWrite(ENB, carSpeed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH); 
  Serial.println("Left");
}

void right() {
  analogWrite(ENA, carSpeed);
  analogWrite(ENB, carSpeed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  Serial.println("Right");
}

void stop() {
  digitalWrite(ENA, LOW);
  digitalWrite(ENB, LOW);
  Serial.println("Stop!");
}

int Distance_test() {
  digitalWrite(Trig, LOW);   
  delayMicroseconds(2);
  digitalWrite(Trig, HIGH);  
  delayMicroseconds(20);
  digitalWrite(Trig, LOW);   
  float Fdistance = pulseIn(Echo, HIGH);  
  Fdistance= Fdistance / 58;       
  return (int)Fdistance;
} 

int count_distance(){
}

void ChangePath () { 
         
if ( R = true )  { 

      delay(1000);
      myservo.write(90);              
      delay(1000);                                                  
      myservo.write(180);              
      delay(1000); 
      leftDistance = Distance_test();
      while ( leftDistance<= 40){
        forward();
        moved_distance = count_distance();
      }
      forward();
      left();
      leftDistance = Distance_test();
      while ( leftDistance<= 40){
        forward();
      }
      left();
      leftDistance = Distance_test();
      while ( leftDistance<= 40 && !(LT_M)&& forward_distance < moved_distance){
        forward();
        forward_distance = count_distance();
      }
      right();
}
else if ( R = false )  { 

      delay(1000);
      myservo.write(90);              
      delay(500);                         
      myservo.write(10);          
      delay(1000);      
      rightDistance = Distance_test();
      
      while ( rightDistance<= 40){
        forward();
        moved_distance = count_distance();
      }
      right();
      while ( rightDistance<= 40){
        forward();
      }
      forward();
      right();
      while ( rightDistance<= 40&& !(LT_M)&& !(LT_M)&& forward_distance < moved_distance){
        forward();
        forward_distance = count_distance();
      }
      left();
}
}

String data_read(){
  if (Serial.available()) {
    next_path =Serial.read();
    Serial.println(next_path);
}
}

void data_write(){
  if (Serial.available()) {
    next_path =Serial.read();
    Serial.println(next_path);
}
}

void forward1() {
while(forward_distance < forward_demande){
    myservo.write(90);  //setservo position according to scaled value
    delay(500); 
    middleDistance = Distance_test();
  if (middleDistance > 40){
    forward();
    forward_distance = count_distance();
    digitalWrite (BuzzPIN, LOW);  
    digitalWrite (LEDRPIN,LOW ); 
    digitalWrite (LEDGPIN,HIGH); 
  }
  else {
  digitalWrite (BuzzPIN, HIGH);  
  digitalWrite (LEDRPIN, HIGH); 
  digitalWrite (LEDGPIN, LOW); 
  last_path=next_path;
  data_write();
  delay(20000);
  data_read();
  if (next_path!=last_path){
     Serial.print("new path assigned");
  }
  else{
    myservo.write(90);  //setservo position according to scaled value
    delay(500); 
    middleDistance = Distance_test();

    if(middleDistance > 40) {
        Serial.print("obstacle removed");
      }
       
     else{
      stop();
      digitalWrite (BuzzPIN, HIGH);  
      digitalWrite (LEDRPIN, HIGH); 
      digitalWrite (LEDGPIN, LOW); 
      delay(500);                         
      myservo.write(10);          
      delay(1000);      
      rightDistance = Distance_test();
      
      delay(500);
      myservo.write(90);              
      delay(1000);                                                  
      myservo.write(180);              
      delay(1000); 
      leftDistance = Distance_test();
      
      delay(500);
      myservo.write(90);              
      delay(1000);
      if((rightDistance >= 40) && rightDistance >= leftDistance) {
        right();
        ChangePath ();
        delay(1000);
      }
      
      else if((leftDistance >= 40) && rightDistance > leftDistance) {
        left();
        R = false ;
        ChangePath ();
        delay(1000);
      }

     else {
        stop();
        digitalWrite (BuzzPIN, HIGH);  
        digitalWrite (LEDRPIN,HIGH ); 
        digitalWrite (LEDGPIN,LOW); 
        delay(180);
        data_write();
     }
    }
   }
  }
 }
}

void path_follower(){
delay(1000);
for (int i=0; i <= next_path.length(); i = i + 1){
cmd=next_path[i];

Serial.print(cmd); 
switch (cmd) {
case 'F':forward1();break;
case 'B':back();break;
case 'L':left();break;
case 'R':right();break;
case 'S':stop();break;
}
}
}

void setup() {  
  myservo.attach(3);  // attach servo on pin 3 to servo object
  Serial.begin(9600); 

  pinMode(Echo, INPUT);    
  pinMode(Trig, OUTPUT); 
  
  pinMode(LT_R,INPUT);
  pinMode(LT_M,INPUT);
  pinMode(LT_L,INPUT); 
     
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  
  pinMode (BuzzPIN, OUTPUT);  
  pinMode (LEDRPIN, OUTPUT); 
  pinMode (LEDGPIN, OUTPUT); 
  stop();
    
    Serial.begin(115200);
while (!Serial) {
; // wait for serial port to connect. Needed for native USB port only
}
  } 
  
void loop(){
data_read();
path_follower();
data_write();

}
