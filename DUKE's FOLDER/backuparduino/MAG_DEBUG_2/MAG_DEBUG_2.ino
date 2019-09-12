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
#define Prox A10 //proximity sensor analog pin

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
int stopdist = 21;
int indicator = 0;
int min_dist = 3;

float temperature;

boolean condition = true; //condition for April Tags
boolean cond = true; //condition for color sensor

int prev;
int prevread;
int present;
int presentread;

short usSpeed = 150;  //default motor speed
unsigned short usMotor_Status = BRAKE;

void setup()
{

  Serial.begin(9600);

  Serial.println("Orientation Sensor Test"); Serial.println("");


  if (!bno.begin())
  {

    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while (1);
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

  pinMode(Pzero, INPUT);
  pinMode(Pone, INPUT);
  pinMode(Ptwo, INPUT);
  pinMode(Pthree, INPUT);
  pinMode(Pfour, INPUT);
  pinMode(Pfive, INPUT);
  pinMode(Psix, INPUT);

  pinMode(Sun, INPUT);
  pinMode(Color, INPUT);
  pinMode(Prox, INPUT);

}

void loop() {

  ////////////////////////////////////////initializing stuff here ///////////////////////////////////////

  float temperature, humidity;

  /*if (! am2315.readTemperatureAndHumidity(&temperature, &humidity)) {
    Serial.println("Failed to read data from AM2315");
    return;
    }*/

  sensors_event_t event;
  bno.getEvent(&event);

  DateTime now = rtc.now(); //initialize

  digitalWrite(EN_1, HIGH);
  digitalWrite(EN_2, HIGH);
  digitalWrite(EN_3, HIGH);
  digitalWrite(EN_4, HIGH);

  Swivel();
}


//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!












//////////////////////////Crap Ton of Functions////////////////////////////////////////

void Bswivel() { // <============= I think this is fine to use now...

  boolean condition = true; //condition for continuing the loop
  int master = 1; //something like a condition for continuing the big loop
  while (master == 1) { //defintely need this
    sensors_event_t event;
    bno.getEvent(&event); //starts orientation sensor
    while (condition) { //condition for starting the loop for turning right
      sensors_event_t event;
      bno.getEvent(&event);
      RRight();
      if ( digitalRead(Pzero) > 0 || digitalRead(Pfour) > 0 ) { //if in the process of swiveling sees an April Tag, it will stop and break out of the loop
        Stop(); //stops the moving
        master = 0; //this is the key to breaking out of the master loop
        break; //redundant and don't need, but I guess it's there just in case
      }
      Serial.print("Bswivel Right: ");
      Serial.println(event.orientation.x);
      if (event.orientation.x >= 268 && event.orientation.x <= 272) {
        condition = false;
      }
    }
    if ( master == 0 ) { //this line could only be read only when the condition loop is broken out of midway, then breaks out of the master loop
      Dloop = 0; //refreshes Dloop everytime swivel is called
      Stop();
      delay(1000);
      break;
    }
    condition = true;
    while (condition) {
      sensors_event_t event;
      bno.getEvent(&event);
      LLeft();
      if ( digitalRead(Pzero) > 0 || digitalRead(Pfour) > 0 ) {
        Stop();
        master = 0;
        break;
      }
      Serial.print("Bswivel Left: ");
      Serial.println(event.orientation.x);
      if (event.orientation.x >= 88 && event.orientation.x <= 92) {
        condition = false;
      }
    }
    if ( master == 0 ) {
      Dloop = 0; //refreshes Dloop everytime swivel is called
      Stop();
      delay(1000);
      break;
    }
    condition = true;
    while (condition) {
      sensors_event_t event;
      bno.getEvent(&event);
      RRight();
      if ( digitalRead(Pzero) > 0 || digitalRead(Pfour) > 0 ) {
        Stop();
        master = 0;
        break;
      }
      Serial.print("Bswivel Right: ");
      Serial.println(event.orientation.x);
      if (event.orientation.x >= 178 && event.orientation.x <= 182) {
        condition = false;
      }
    }
    if ( master == 0 ) {
      Dloop = 0; //refreshes Dloop everytime swivel is called
      Stop();
      delay(1000);
      break;
    }
    break;
  }
  Dloop = 0; //refreshes Dloop everytime swivel is called
  Stop();
  delay(1000);

}

void Swivel() { // <============= I think this is fine to use now...
  boolean condition = true; //condition for continuing the loop
  int master = 1; //something like a condition for continuing the big loop
  while (master == 1) { //defintely need this
    while (millis() < 10000) {
      sensors_event_t event;
      bno.getEvent(&event); //starts orientation sensor
      Stop();
    }
    while (condition) { //condition for starting the loop for turning right
      sensors_event_t event;
      bno.getEvent(&event);

      RRight();
      if ( digitalRead(Pzero) > 0 || digitalRead(Pfour) > 0 ) { //if in the process of swiveling sees an April Tag, it will stop and break out of the loop
        Stop(); //stops the moving
        master = 0; //this is the key to breaking out of the master loop
        break; //redundant and don't need, but I guess it's there just in case
      }
      Serial.print("Swivel Right1: ");
      Serial.println(event.orientation.x);
      if (event.orientation.x >= 88 && event.orientation.x <= 92) {
        condition = false;
      }
    }
    if ( master == 0 ) { //this line could only be read only when the condition loop is broken out of midway, then breaks out of the master loop
      Dloop = 0; //refreshes Dloop everytime swivel is called
      Stop();
      delay(1000);
      break;
    }
    condition = true;
    while (condition) {
      sensors_event_t event;
      bno.getEvent(&event);
      LLeft();
      if ( digitalRead(Pzero) > 0 || digitalRead(Pfour) > 0 ) {
        Stop();
        master = 0;
        break;
      }
      Serial.print("Swivel Left: ");
      Serial.println(event.orientation.x);
      if (event.orientation.x >= 268 && event.orientation.x <= 272) {
        condition = false;
      }
    }
    if ( master == 0 ) {
      Dloop = 0; //refreshes Dloop everytime swivel is called
      Stop();
      delay(1000);
      break;
    }
    condition = true;
    while (condition) {
      sensors_event_t event;
      bno.getEvent(&event);
      RRight();
      if ( digitalRead(Pzero) > 0 || digitalRead(Pfour) > 0 ) {
        Stop();
        master = 0;
        break;
      }
      Serial.print("Swivel Right2: ");
      Serial.println(event.orientation.x);
      if ((event.orientation.x >= 358 && event.orientation.x <= 359) ||(event.orientation.x>=0 && event.orientation.x<=1 )) {
        condition = false;
      }
    }
    if ( master == 0 ) {
      Dloop = 0; //refreshes Dloop everytime swivel is called
      Stop();
      delay(1000);
      break;
    }
    break;
  }
  Dloop = 0; //refreshes Dloop everytime swivel is called
  Stop();
  delay(1000);
}


void Bavoid() { // <=== It should be good now!!

  int condition = true;
  sensors_event_t event;
  bno.getEvent(&event);
  while (condition) {
    sensors_event_t event;
    bno.getEvent(&event);
    LLeft();
    Serial.print("Bavoid Left: ");
    Serial.println(event.orientation.x);
    if (event.orientation.x >= 88 && event.orientation.x <= 92) {
      condition = false;
    }
  }
  Stop();
  delay(500);
  Forward();
  delay(1000);
  condition = true;
  while (condition) {
    sensors_event_t event;
    bno.getEvent(&event);
    Serial.print("Bavoid Right: ");
    Serial.println(event.orientation.x);
    RRight();
    if (event.orientation.x >= 178 && event.orientation.x <= 182) {
      condition = false;
    }
  }
  Stop();
  delay(500);
  Forward();
  delay(2000);
  condition = true;
  while (condition) {
    sensors_event_t event;
    bno.getEvent(&event);
    Serial.print("Bavoid Left2: ");
    Serial.println(event.orientation.x);
    RRight();
    if (event.orientation.x >= 268 && event.orientation.x <= 272) {
      condition = false;
    }
  }
  Stop();
  delay(500);
  Back();
  delay(1000);
  condition = true;
  while (condition) {
    sensors_event_t event;
    bno.getEvent(&event);
    Serial.print("Bavoid Right2: ");
    Serial.println(event.orientation.x);
    RRight();
    if (event.orientation.x >= 178 && event.orientation.x <= 182) {
      condition = false;
    }
  }
  Stop();
  delay(10);
  condition = true;
  d = d + 100; //pick a value later

}

void Bturn() { //GOOD

  int condition = true;
  sensors_event_t event;
  bno.getEvent(&event);
  while (condition) {
    sensors_event_t event;
    bno.getEvent(&event);
    RRight();
    Serial.print("Bturn Right: ");
    Serial.println(event.orientation.x);
    if (event.orientation.x >= 356 && event.orientation.x <= 359) {
      condition = false;
    }
  }

}

void turn() { //GOOD

  int condition = true;
  sensors_event_t event;
  bno.getEvent(&event);
  while (condition) {
    sensors_event_t event;
    bno.getEvent(&event);
    RRight();
    Serial.print("In this loop: ");
    Serial.print("TURN Right: ");
    Serial.println(event.orientation.x);
    if (event.orientation.x >= 178 && event.orientation.x <= 182) {
      condition = false;
      break;
    }
  }


}
void avoid() //GOOD
{
  int condition = true;
  sensors_event_t event;
  bno.getEvent(&event);
  while (condition) {
    sensors_event_t event;
    bno.getEvent(&event);
    RRight();
    Serial.print("AVOID Right: ");
    Serial.println(event.orientation.x);
    if (event.orientation.x >= 88 && event.orientation.x <= 92) {
      condition = false;
    }
  }
  Stop();
  delay(500);
  Forward();
  delay(1000);
  condition = true;
  while (condition) {
    sensors_event_t event;
    bno.getEvent(&event);
    Serial.print(" AVOID Left: ");
    Serial.println(event.orientation.x);
    LLeft();
    //if(event.orientation.x>=268 && event.orientation.x<=272){
    if (event.orientation.x >= 0 && event.orientation.x <= 2) {
      condition = false;
    }
  }
  Stop();
  delay(500);
  Forward();
  delay(2000);
  condition = true;
  while (condition) {
    sensors_event_t event;
    bno.getEvent(&event);
    Serial.print("AVOID Left2: ");
    Serial.println(event.orientation.x);
    LLeft();
    if (event.orientation.x >= 268 && event.orientation.x <= 272) {
      condition = false;
    }
  }
  Stop();
  delay(500);
  Forward();
  delay(1000);
  condition = true;
  while (condition) {
    sensors_event_t event;
    bno.getEvent(&event);
    Serial.print("AVOID Right2: ");
    Serial.println(event.orientation.x);
    RRight();
    //if(event.orientation.x>=88 && event.orientation.x<=92){
    if (event.orientation.x >= 356 && event.orientation.x <= 359) {
      condition = false;
    }
  }
  Stop();
  delay(10);
  condition = true;
  d = d + 100; //pick a value later
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

  motorGo(MOTOR_1, CCW, 50);
  motorGo(MOTOR_2, CCW, 50);
  motorGo(MOTOR_3, CW, 50);
  motorGo(MOTOR_4, CW, 50);
}

void LLeft()
{

  motorGo(MOTOR_1, CW, 50);
  motorGo(MOTOR_2, CW, 50);
  motorGo(MOTOR_3, CCW, 50);
  motorGo(MOTOR_4, CCW, 50);
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
  motorGo(MOTOR_3, usMotor_Status, 130);
  motorGo(MOTOR_4, usMotor_Status, 130);
}

void Left()
{
  usMotor_Status = CCW;
  motorGo(MOTOR_1, usMotor_Status, 130);
  motorGo(MOTOR_2, usMotor_Status, 130);
  motorGo(MOTOR_3, usMotor_Status, 50);
  motorGo(MOTOR_4, usMotor_Status, 50);

}

void IncreaseSpeed()
{
  usSpeed = usSpeed + 10;
  if (usSpeed > 255)
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
  if (usSpeed < 0)
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
  if (motor == MOTOR_1)
  {
    if (direct == CCW)
    {
      digitalWrite(MOTOR_A1, LOW);
      digitalWrite(MOTOR_B1, HIGH);
    }
    else if (direct == CW)
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
  else if (motor == MOTOR_2)
  {
    if (direct == CCW)
    {
      digitalWrite(MOTOR_A2, LOW);
      digitalWrite(MOTOR_B2, HIGH);
    }
    else if (direct == CW)
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

  else if (motor == MOTOR_3)
  {
    if (direct == CW)
    {
      digitalWrite(MOTOR_A3, LOW);
      digitalWrite(MOTOR_B3, HIGH);
    }
    else if (direct == CCW)
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
  else if (motor == MOTOR_4)
  {
    if (direct == CW)
    {
      digitalWrite(MOTOR_A4, LOW);
      digitalWrite(MOTOR_B4, HIGH);
    }
    else if (direct == CCW)
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

void FirstFollowApril() { //works on ID0 and ID1

  Dloop = 0;
  int prev = analogRead(Color); //color sensor

  if ( digitalRead(Pone) > 0 && digitalRead(Ptwo) == 0 && digitalRead(Pthree) == 0) {
    Serial.println("ID Turn Left");
    Left();
    if (PROX() < min_dist) {
      avoid();
      Stop();
    }
  }
  else if ( digitalRead(Ptwo) > 0 && digitalRead(Pone) == 0 && digitalRead(Pthree) == 0) {
    Serial.println("ID Turn Right");
    Right();
    if (PROX() < min_dist) {
      avoid();
      Stop();
      delay(10);
    }
  }
  else if ( digitalRead(Pthree) > 0 && digitalRead(Pone) == 0 && digitalRead(Ptwo) == 0) {
    Serial.println("ID Straight");
    Forward();
    if (PROX() < min_dist) {
      avoid();
      Stop();
    }
  }
  else {
    Stop();
  }
  if ( digitalRead(Pfive) > 0 ) { //pattern comes too close
    Serial.println("Too close ");
    avoid();
    Stop();
  }
  int present = analogRead(Color); //color sensor
  int diff1 = abs(prev - present); //difference between color sensors

  if ( diff1 >= threshold ) {
    d = d + distance;
    Serial.print(" Previous Sensor: ");
    Serial.print(prev);
    Serial.print(" Present Sensor: ");
    Serial.print(present);
    Serial.print(" Distance Value: ");
    Serial.print(d);
  }
}

void FirstSeesThree() {
  Dloop = 0;
  turn();
  key = 1; // the key to another loop might not even need this
}

void SecondSeesThree() {
  Dloop = 0;
  Bturn();
  key = 2; // Going to change something here
}

void FirstGoStraight() {

  int prev = analogRead(Color); //color sensor

  sensors_event_t event;
  bno.getEvent(&event);

  if (event.orientation.x > 180 ) { //making anything above 180 into a negative number
    event.orientation.x = event.orientation.x * -1; //>>>>>>>>>>>>>>>>>>NEW LINE<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
  }

  if ( (event.orientation.x - e) > 0 ) { //if direction is more than 0, it would turn left
    Left();
    if (PROX() < min_dist) {
      avoid();
      Stop();
    }
    Serial.println(" Make Left ");
  }
  else if ((event.orientation.x + e) < 0) { //if direction is less than 0, it would turn right
    Right();
    if (PROX() < min_dist) {
      avoid();
      Stop();
    }
    Serial.println(" Make Right ");
  }
  else if ( (event.orientation.x + e) >= 0 && (event.orientation.x - e) <= 0) {
    Forward();
    if (PROX() < min_dist) {
      avoid();
      Stop();
    }
    Serial.println(" Go Straight ");
  }

  int present = analogRead(Color); //color sensor
  int diff1 = abs(prev - present); //difference between color sensors

  if ( diff1 >= threshold ) {
    d = d + distance;
    Serial.print(" Previous Sensor: ");
    Serial.print(prev);
    Serial.print(" Present Sensor: ");
    Serial.print(present);
    Serial.print(" Distance Value: ");
    Serial.print(d);
  }
  Dloop = Dloop + 1; //need this for swiveling broooooo
}

void FourthGoStraight() {

  int prev = analogRead(Color); //color sensor

  sensors_event_t event;
  bno.getEvent(&event);

  if (event.orientation.x > 180 ) { //making anything above 180 into a negative number
    event.orientation.x = event.orientation.x * -1; //>>>>>>>>>>>>>>>>>>NEW LINE<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
  }

  if ( (event.orientation.x - e) > 0 ) { //if direction is more than 0, it would turn left
    Left();
    if (PROX() < min_dist) {
      avoid();
      Stop();
    }
    Serial.println(" Make Left ");
  }
  else if ((event.orientation.x + e) < 0) { //if direction is less than 0, it would turn right
    Right();
    if (PROX() < min_dist) {
      avoid();
      Stop();
    }
    Serial.println(" Make Right ");
  }
  else if ( (event.orientation.x + e) >= 0 && (event.orientation.x - e) <= 0) {
    Forward();
    if (PROX() < min_dist) {
      avoid();
      Stop();
    }
    Serial.println(" Go Straight ");
  }

  int present = analogRead(Color); //color sensor
  int diff1 = abs(prev - present); //difference between color sensors

  if ( diff1 >= threshold ) {
    d = d - distance;
    Serial.print(" Previous Sensor: ");
    Serial.print(prev);
    Serial.print(" Present Sensor: ");
    Serial.print(present);
    Serial.print(" Distance Value: ");
    Serial.print(d);
  }
  Dloop = Dloop + 1; //need this for swiveling broooooo
}

void SecondFollowApril() { //just FollowApril with a Bavoid() function

  if ( digitalRead(Pone) > 0 && digitalRead(Ptwo) == 0 && digitalRead(Pthree) == 0) {
    Serial.println("ID Turn Left ");
    Left();
    if (PROX() < min_dist) {
      Bavoid();
      Stop();
    }
  }
  else if ( digitalRead(Ptwo) > 0 && digitalRead(Pone) == 0 && digitalRead(Pthree) == 0) {
    Serial.println("ID Turn Right ");
    Right();
    if (PROX() < min_dist) {
      Bavoid();
      Stop();
    }
  }
  else if ( digitalRead(Pthree) > 0 && digitalRead(Pone) == 0 && digitalRead(Ptwo) == 0) {
    Serial.println("ID Straight ");
    Forward();
    if (PROX() < min_dist) {
      Bavoid();
      Stop();
    }
  }
  else {
    Stop();
  }
  if ( digitalRead(Pfive) > 0 ) { //pattern comes too close so it avoids it
    Serial.println("Too close ");
    Bavoid(); // <===== need to investigate this 8/28/2019 work ended around here >>FINISHED<<
    Stop();
  }

  int present = analogRead(Color); //color sensor
  int diff1 = abs(prev - present); //difference between color sensors

  if ( diff1 >= threshold ) {
    d = d - distance;
    Serial.print(" Previous Sensor: ");
    Serial.print(prev);
    Serial.print(" Present Sensor: ");
    Serial.print(present);
    Serial.print(" Distance Value: ");
    Serial.print(d);
  }
}

void ThirdGoStraight() {

  int prev = analogRead(Color); //color sensor

  sensors_event_t event;
  bno.getEvent(&event);

  if ( (event.orientation.x > 180 + e) ) { //if direction is more than 0, it would turn left
    Left();
    if (PROX() < min_dist) {
      Bavoid();
      Stop();
    }
    Serial.println(" Make Left ");
  }
  else if ( (event.orientation.x < 180 - e )) { //if direction is less than 0, it would turn right
    Right();
    if (PROX() < min_dist) {
      Bavoid();
      Stop();
    }
    Serial.println(" Make Right ");
  }
  else if ( event.orientation.x >= 180 - e && event.orientation.x <= 180 + e) {
    Forward();
    if (PROX() < min_dist) {
      Bavoid();
      Stop();
    }
    Serial.println(" Go Straight ");
  }

  int present = analogRead(Color); //color sensor
  int diff1 = abs(prev - present); //difference between color sensors

  if ( diff1 >= threshold ) {
    d = d + distance;
    Serial.print(" Previous Sensor: ");
    Serial.print(prev);
    Serial.print(" Present Sensor: ");
    Serial.print(present);
    Serial.print(" Distance Value: ");
    Serial.print(d);
  }
}

void SecondGoStraight() {

  int prev = analogRead(Color); //color sensor

  sensors_event_t event;
  bno.getEvent(&event);

  if ( (event.orientation.x > 180 + e) ) { //if direction is more than 0, it would turn left
    Left();
    if (PROX() < min_dist) {
      Bavoid();
      Stop();
    }
    Serial.println(" Make Left ");
  }
  else if ( (event.orientation.x < 180 - e )) { //if direction is less than 0, it would turn right
    Right();
    if (PROX() < min_dist) {
      Bavoid();
      Stop();
    }
    Serial.println(" Make Right ");
  }
  else if ( event.orientation.x >= 180 - e && event.orientation.x <= 180 + e) {
    Forward();
    if (PROX() < min_dist) {
      Bavoid();
      Stop();
    }
    Serial.println(" Go Straight ");
  }

  int present = analogRead(Color); //color sensor
  int diff1 = abs(prev - present); //difference between color sensors

  if ( diff1 >= threshold ) {
    d = d - distance;
    Serial.print(" Previous Sensor: ");
    Serial.print(prev);
    Serial.print(" Present Sensor: ");
    Serial.print(present);
    Serial.print(" Distance Value: ");
    Serial.print(d);
  }
}

void FirstBigLoop() {

  if ( digitalRead(Pzero) > 0 ) {
    FirstFollowApril();
    Serial.print("Sees ID 0 ");
  }
  else if ( digitalRead(Pfour) > 0 ) { //found tag ID = 1 Goes after ID 1 and if it gets too close it will go around you at least it will try to
    FirstFollowApril();
    Serial.print("Sees ID 1 ");
  }
  else if ( digitalRead(Psix) > 0 ) { //breaks loop when it sees ID 3 and also makes a Uturn
    FirstSeesThree(); //use key == 1 here
    Serial.print("Sees ID 3 ");
  }
  else if ( Dloop == 250 ) { //trying to find the tag if it cant find it for 300 loops loop = loop + 1 at magnetometer line section
    Swivel();
    Dloop = 0;
  }
  else {
    FirstGoStraight();
  }
}

void SecondBigLoop() {

  if ( digitalRead(Pzero) > 0 ) { //found tag ID = 0 Goes after ID zero
    SecondFollowApril();
    Serial.print("Sees ID 0 ");
  }
  else if ( digitalRead(Pfour) > 0 ) { //found tag ID = 1 Goes after ID 1 and if it gets too close it will go around you at least it will try to
    SecondFollowApril();
    Serial.print("Sees ID 1 ");
  }
  else if ( digitalRead(Psix) > 0 ) { //breaks loop when it sees ID 3 and also makes a Uturn
    SecondSeesThree();
    Serial.print("Sees ID 3 ");
  }
  else { //change this later and add GPS on it
    SecondGoStraight();
  }
}

void ThirdBigLoop() {

  if ( digitalRead(Pzero) > 0 ) { //found tag ID = 0 Goes after ID zero
    FirstFollowApril();
    Serial.print("Sees ID 0 ");
  }
  else if ( digitalRead(Pfour) > 0 ) { //found tag ID = 1 Goes after ID 1 and if it gets too close it will go around you at least it will try to
    FirstFollowApril();
    Serial.print("Sees ID 1 ");
  }
  else if ( digitalRead(Psix) > 0 ) { //breaks loop when it sees ID 3 and also makes a Uturn
    FirstSeesThree();
    Serial.print("Sees ID 3 ");
  }
  else if ( Dloop == 250 ) { //trying to find the tag if it cant find it for 300 loops loop = loop + 1 at magnetometer line section
    Bswivel();
    Dloop = 0;
  }
  else { //changed this to if statement maybe this can potentially work if not switch it back to else if{}
    ThirdGoStraight();
    Serial.print("Going Straight ");
  }
}

void FourthBigLoop() {
  if ( digitalRead(Pzero) > 0 ) {
    SecondFollowApril();
    Serial.print("Sees ID 0 ");
  }
  else if ( digitalRead(Pfour) > 0 ) { //found tag ID = 1 Goes after ID 1 and if it gets too close it will go around you at least it will try to
    SecondFollowApril();
    Serial.print("Sees ID 1 ");
  }
  else if ( digitalRead(Psix) > 0 ) { //breaks loop when it sees ID 3 and also makes a Uturn
    SecondSeesThree();
    Serial.print("Sees ID 3 ");
  }
  else {
    FourthGoStraight();
  }
}

void initiate() {
  for ( int i; i <= 3; i++ ) { //to calibrate the orientation sensor!!
    Serial.println("Swiveling for Orientation Sensor ");
    Swivel();
  }
  Stop();
}

void initiate2() {
  for ( int i; i <= 3; i++ ) { //to calibrate the orientation sensor!!
    Serial.println("Swiveling for Orientation Sensor ");
    Bswivel();
  }
  Stop();
}

int PROX() {
  int i;
  int value = 0;
  int numReadings = 10;

  for (i = 0; i < numReadings; i++) {
    // Read light sensor data.
    value = value + analogRead(Prox);

    // 1ms pause adds more stability between reads.
    delay(1);
  }

  // Take an average of all the readings.
  value = value / numReadings;

  // Scale to 8 bits (0 - 255).
  value = value / 4;

  return value;
}
