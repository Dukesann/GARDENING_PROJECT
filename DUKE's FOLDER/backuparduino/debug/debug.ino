#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <Adafruit_AM2315.h>

#include <SparkFun_TB6612.h>
#include "RTClib.h"
Adafruit_AM2315 am2315;



#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial(22, 24);
Adafruit_GPS GPS(&mySerial);

#define GPSECHO  true

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

#define Sun A8 //sunlight sensor analog pin
#define Color A9 //color sensor analog pin

int e = 12;
//int threshold = 20; //for outdoor testing
int threshold = 100; //for indoor testing
int d = 0;
int distance = 97.8605; //Every tick on the color sensor is 97.8605mm in distance 
int loopcount = 1;
int Dloop = 0; //loop for how many times it couldnt find the tags 
int DDloop = 1;
int key = 0; //key is given when time hits beyond 6:00pm might add more keys 
int master = 1;
int secondtimeloop = 0;

boolean condition = true; //condition for April Tags 
boolean cond = true; //condition for color sensor 

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
    Serial.print("working?");
  }
  while (!Serial) {
    delay(10);
  }
  Serial.println("AM2315 Test!");

  if (! am2315.begin()) {
     Serial.println("Sensor not found, check wiring & pullups!");
     while (1);
  }
  
  delay(1000);
    
  bno.setExtCrystalUse(true);

  while (!Serial); // for Leonardo/Micro/Zero

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2019, 7, 30, 1, 1, 9));
  }

  
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

  pinMode(Sun, INPUT);
  pinMode(Color, INPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  sensors_event_t event; 
  bno.getEvent(&event);

  /* Display the floating point data */
  Serial.print("X: ");
  Serial.print(event.orientation.x, 4);
  Serial.print("\tY: ");
  Serial.print(event.orientation.y, 4);
  Serial.print("\tZ: ");
  Serial.print(event.orientation.z, 4);
  Serial.println("");
  delay(100);
    

}
