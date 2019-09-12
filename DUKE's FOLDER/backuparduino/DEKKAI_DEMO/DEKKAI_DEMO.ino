#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <Adafruit_AM2315.h>

#include <SparkFun_TB6612.h>
#include "RTClib.h"

Adafruit_BNO055 bno = Adafruit_BNO055(55);

RTC_DS1307 rtc;

#define BRAKE 0
#define CW    1
#define CCW   2
#define CS_THRESHOLD 15   // Definition of safety current (Check: "1.3 Monster Shield Example").

// Motor direction pins
#define MOTOR_A1 29
#define MOTOR_B1 33
#define MOTOR_A2 27
#define MOTOR_B2 35
#define MOTOR_A3 41
#define MOTOR_B3 45
#define MOTOR_A4 39
#define MOTOR_B4 47

// Motor PWM pins
#define PWM_MOTOR_1 2
#define PWM_MOTOR_2 3
#define PWM_MOTOR_3 4
#define PWM_MOTOR_4 5

// Current sense pins (analog)
#define CURRENT_SEN_1 A2
#define CURRENT_SEN_2 A3
#define CURRENT_SEN_3 A6
#define CURRENT_SEN_4 A7

// Enable pins (analog)
#define EN_1 A0
#define EN_2 A1
#define EN_3 A4
#define EN_4 A5

#define MOTOR_1 0
#define MOTOR_2 1
#define MOTOR_3 2
#define MOTOR_4 3

#define Pzero 6
#define Pone 7
#define Ptwo 8
#define Pthree 9
#define Pfour 10 //id 1 
#define Pfive 11 //z value
#define Psix 12 //id 3

int e = 7;

boolean condition = true;

short usSpeed = 150;  //default motor speed
unsigned short usMotor_Status = BRAKE;
 
void setup()                         
{

    Serial.begin(9600);
  Serial.println("Orientation Sensor Test"); Serial.println("");
  
  /* Initialise the sensor */
  if(!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
  
  delay(1000);
    
  bno.setExtCrystalUse(true);
  
  pinMode(MOTOR_A1, OUTPUT);
  pinMode(MOTOR_B1, OUTPUT);
  pinMode(MOTOR_A2, OUTPUT);
  pinMode(MOTOR_B2, OUTPUT);
  pinMode(MOTOR_A3, OUTPUT);
  pinMode(MOTOR_B3, OUTPUT);
  pinMode(MOTOR_A4, OUTPUT);
  pinMode(MOTOR_B4, OUTPUT);

  pinMode(PWM_MOTOR_1, OUTPUT);
  pinMode(PWM_MOTOR_2, OUTPUT);
  pinMode(PWM_MOTOR_3, OUTPUT);
  pinMode(PWM_MOTOR_4, OUTPUT);

  pinMode(CURRENT_SEN_1, OUTPUT);
  pinMode(CURRENT_SEN_2, OUTPUT);  
  pinMode(CURRENT_SEN_3, OUTPUT);
  pinMode(CURRENT_SEN_4, OUTPUT);  

  pinMode(EN_1, OUTPUT);
  pinMode(EN_2, OUTPUT);
  pinMode(EN_1, OUTPUT);
  pinMode(EN_2, OUTPUT);

  pinMode(Pzero,INPUT);
  pinMode(Pone,INPUT);
  pinMode(Ptwo,INPUT);
  pinMode(Pthree,INPUT);
  pinMode(Pfour,INPUT);
  pinMode(Pfive,INPUT);
  pinMode(Psix, INPUT);
  
  Serial.begin(9600);              // Initiates the serial to do the monitoring 

}

void loop(){

    sensors_event_t event; 
    bno.getEvent(&event);
    
    digitalWrite(EN_1, HIGH);
    digitalWrite(EN_2, HIGH); 
    digitalWrite(EN_3, HIGH); 
    digitalWrite(EN_4, HIGH);

if( digitalRead(Pzero) > 0 ){ //found tag ID = 0 Goes after ID zero

    if( digitalRead(Pone) > 0 && digitalRead(Ptwo)==0 && digitalRead(Pthree)==0){
      Serial.println("Turn Left");
      Left();
      delay(700);
    
    }
    else if( digitalRead(Ptwo) > 0 && digitalRead(Pone)==0 && digitalRead(Pthree)==0){
      Serial.println("Turn Right");
      Right();
      delay(700);
    }
    
    else if( digitalRead(Pthree) > 0 && digitalRead(Pone)==0 && digitalRead(Ptwo)==0){
      Serial.println("Straight");
      Forward();
      delay(700);
    
    }
    else{
      Stop();
      delay(10);
    }

    if( digitalRead(Psix) > 0 ){ //detects ID 3  When it detects ID 3 it will go ape shit 

      RRight();
      delay(10000);
      Stop();
      delay(10);
  
}
}

if( digitalRead(Pfour) > 0 ){ //found tag ID = 1 Goes after ID 1 and if it gets too close it will go around you at least it will try to 
  

    if( digitalRead(Pone) > 0 && digitalRead(Ptwo)==0 && digitalRead(Pthree)==0){
      Serial.println("Turn Left");
      Left();
      delay(700);
      Stop();
      delay(10);
    
    }
    else if( digitalRead(Ptwo) > 0 && digitalRead(Pone)==0 && digitalRead(Pthree)==0){
      Serial.println("Turn Right");
      Right();
      delay(700);
      Stop();
      delay(10);
    }
    
    else if( digitalRead(Pthree) > 0 && digitalRead(Pone)==0 && digitalRead(Ptwo)==0){
      Serial.println("Forward");
      Forward();
      delay(700);
      Stop();
      delay(10);
    
    }
    else{
      Stop();
      delay(10);
    }

    if( digitalRead(Pfive) > 0 ){ 

      turn();
      Stop();
      delay(10);
  
      }
      

}

if( digitalRead(Psix) > 0 ){ //detects ID 3  When it detects ID 3 it will go ape shit 

RRight();
delay(10000);
Stop();
delay(10);
  
}
}

void turn()
{
  int condition = true;
  sensors_event_t event;
  bno.getEvent(&event);
  while(condition){
  sensors_event_t event;
  bno.getEvent(&event);
  RRight();
  Serial.print("Right: ");
  Serial.println(event.orientation.x);
  if(event.orientation.x>=88 && event.orientation.x<=92){
  condition = false;
  }
  }
  Stop();
  delay(500);
  Forward();
  delay(1000);
  condition = true;
  while(condition){
  sensors_event_t event;
  bno.getEvent(&event);
  Serial.print("Left: ");
  Serial.println(event.orientation.x);
  LLeft();
  //if(event.orientation.x>=268 && event.orientation.x<=272){
  if(event.orientation.x>=0 && event.orientation.x<=2){
  condition = false;
  }
  }
  Stop();
  delay(500);
  Forward();
  delay(2000);
  condition = true;
  while(condition){
  sensors_event_t event;
  bno.getEvent(&event);
  Serial.print("Left2: ");
  Serial.println(event.orientation.x);
  LLeft();
  if(event.orientation.x>=268 && event.orientation.x<=272){
  condition = false;
  }
  }
  Stop();
  delay(500);
  Forward();
  delay(1000);
  condition = true;
  while(condition){
  sensors_event_t event;
  bno.getEvent(&event);
  Serial.print("Right2: ");
  Serial.println(event.orientation.x);
  RRight();
  //if(event.orientation.x>=88 && event.orientation.x<=92){
  if(event.orientation.x>=356 && event.orientation.x<=359){
  condition = false;
  }
  }
  Stop();
  delay(10);
  condition = true;
}

void Stop()
{
  usMotor_Status = BRAKE;
  motorGo(MOTOR_1, usMotor_Status, 0);
  motorGo(MOTOR_2, usMotor_Status, 0);
  motorGo(MOTOR_3, usMotor_Status, 0);
  motorGo(MOTOR_4, usMotor_Status, 0);
}

void Bright()
{
  usMotor_Status = CW;
  motorGo(MOTOR_1, usMotor_Status, 100);
  motorGo(MOTOR_2, usMotor_Status, 100);
  motorGo(MOTOR_3, usMotor_Status, 50);
  motorGo(MOTOR_4, usMotor_Status, 50); 
}

void Back()
{
  usMotor_Status = CW;
  motorGo(MOTOR_1, usMotor_Status, 80);
  motorGo(MOTOR_2, usMotor_Status, 80);
  motorGo(MOTOR_3, usMotor_Status, 80);
  motorGo(MOTOR_4, usMotor_Status, 80); 
}

void Bleft()
{
  usMotor_Status = CW;
  motorGo(MOTOR_1, usMotor_Status, 50);
  motorGo(MOTOR_2, usMotor_Status, 50);
  motorGo(MOTOR_3, usMotor_Status, 100);
  motorGo(MOTOR_4, usMotor_Status, 100); 
}

void Forward()
{
  usMotor_Status = CCW;
  motorGo(MOTOR_1, usMotor_Status, 80);
  motorGo(MOTOR_2, usMotor_Status, 80);
  motorGo(MOTOR_3, usMotor_Status, 80);
  motorGo(MOTOR_4, usMotor_Status, 80);
}

void RRight()
{
  
  motorGo(MOTOR_1, CCW, 100);
  motorGo(MOTOR_2, CCW, 100);
  motorGo(MOTOR_3, CW, 100);
  motorGo(MOTOR_4, CW, 100);
}

void LLeft()
{
  
  motorGo(MOTOR_1, CW, 100);
  motorGo(MOTOR_2, CW, 100);
  motorGo(MOTOR_3, CCW, 100);
  motorGo(MOTOR_4, CCW, 100);
}
void Reverse()
{
  usMotor_Status = CW;
  motorGo(MOTOR_1, usMotor_Status, usSpeed);
  motorGo(MOTOR_2, usMotor_Status, usSpeed);
  motorGo(MOTOR_3, usMotor_Status, usSpeed);
  motorGo(MOTOR_4, usMotor_Status, usSpeed);
}

void Right()
{
  usMotor_Status = CCW;
  motorGo(MOTOR_1, usMotor_Status, 50);
  motorGo(MOTOR_2, usMotor_Status, 50);
  motorGo(MOTOR_3, usMotor_Status, 100);
  motorGo(MOTOR_4, usMotor_Status, 100); 
}

void Left()
{
  usMotor_Status = CCW;
  motorGo(MOTOR_1, usMotor_Status, 100);
  motorGo(MOTOR_2, usMotor_Status, 100);
  motorGo(MOTOR_3, usMotor_Status, 50);
  motorGo(MOTOR_4, usMotor_Status, 50); 
  
}

void IncreaseSpeed()
{
  usSpeed = usSpeed + 10;
  if(usSpeed > 255)
  {
    usSpeed = 255;  
  }
  
  Serial.print("Speed +: ");
  Serial.println(usSpeed);

  motorGo(MOTOR_1, usMotor_Status, usSpeed);
  motorGo(MOTOR_2, usMotor_Status, usSpeed);  
  motorGo(MOTOR_3, usMotor_Status, usSpeed);
  motorGo(MOTOR_4, usMotor_Status, usSpeed);
}

void DecreaseSpeed()
{
  usSpeed = usSpeed - 10;
  if(usSpeed < 0)
  {
    usSpeed = 0;  
  }
  
  Serial.print("Speed -: ");
  Serial.println(usSpeed);

  motorGo(MOTOR_1, usMotor_Status, usSpeed);
  motorGo(MOTOR_2, usMotor_Status, usSpeed);  
  motorGo(MOTOR_3, usMotor_Status, usSpeed);
  motorGo(MOTOR_4, usMotor_Status, usSpeed);
}

void motorGo(uint8_t motor, uint8_t direct, uint8_t pwm)         //Function that controls the variables: motor(0 ou 1), direction (cw ou ccw) e pwm (entra 0 e 255);
{
  if(motor == MOTOR_1)
  {
    if(direct == CCW)
    {
      digitalWrite(MOTOR_A1, LOW); 
      digitalWrite(MOTOR_B1, HIGH);
    }
    else if(direct == CW)
    {
      digitalWrite(MOTOR_A1, HIGH);
      digitalWrite(MOTOR_B1, LOW);      
    }
    else
    {
      digitalWrite(MOTOR_A1, LOW);
      digitalWrite(MOTOR_B1, LOW);            
    }
    
    analogWrite(PWM_MOTOR_1, pwm); 
  }
  else if(motor == MOTOR_2)
  {
    if(direct == CCW)
    {
      digitalWrite(MOTOR_A2, LOW);
      digitalWrite(MOTOR_B2, HIGH);
    }
    else if(direct == CW)
    {
      digitalWrite(MOTOR_A2, HIGH);
      digitalWrite(MOTOR_B2, LOW);      
    }
    else
    {
      digitalWrite(MOTOR_A2, LOW);
      digitalWrite(MOTOR_B2, LOW);            
    }
    
    analogWrite(PWM_MOTOR_2, pwm);
  }

  else if(motor == MOTOR_3)
  {
    if(direct == CW)
    {
      digitalWrite(MOTOR_A3, LOW);
      digitalWrite(MOTOR_B3, HIGH);
    }
    else if(direct == CCW)
    {
      digitalWrite(MOTOR_A3, HIGH);
      digitalWrite(MOTOR_B3, LOW);      
    }
    else
    {
      digitalWrite(MOTOR_A3, LOW);
      digitalWrite(MOTOR_B3, LOW);            
    }
    
    analogWrite(PWM_MOTOR_3, pwm);
  }
  else if(motor == MOTOR_4)
  {
    if(direct == CW)
    {
      digitalWrite(MOTOR_A4, LOW);
      digitalWrite(MOTOR_B4, HIGH);
    }
    else if(direct == CCW)
    {
      digitalWrite(MOTOR_A4, HIGH);
      digitalWrite(MOTOR_B4, LOW);      
    }
    else
    {
      digitalWrite(MOTOR_A4, LOW);
      digitalWrite(MOTOR_B4, LOW);            
    }
    
    analogWrite(PWM_MOTOR_4, pwm);
  }
}
