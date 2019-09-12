#define BRAKE 0
#define CW    1
#define CCW   2
#define CS_THRESHOLD 15   // Definition of safety current (Check: "1.3 Monster Shield Example").

//MOTOR 1 THIS MIGHT BE INCORRECT 
#define MOTOR_A1_1_PIN 29
#define MOTOR_A1_2_PIN 23
#define MOTOR_B1_1_PIN 31
#define MOTOR_B1_2_PIN 33

//MOTOR 2
#define MOTOR_A2_1_PIN 43 
#define MOTOR_A2_2_PIN 37
#define MOTOR_B2_1_PIN 45
#define MOTOR_B2_2_PIN 47

#define PWM_MOTOR_1_1 25
#define PWM_MOTOR_1_2 27
#define PWM_MOTOR_2_1 39
#define PWM_MOTOR_2_2 41

#define CURRENT_SEN_1_1 A2
#define CURRENT_SEN_1_2 A3
#define CURRENT_SEN_2_1 A6
#define CURRENT_SEN_2_2 A7

#define EN_PIN_1_1 A0
#define EN_PIN_1_2 A1
#define EN_PIN_2_1 A4
#define EN_PIN_2_2 A5

#define MOTOR_1 0 // I don't know what this is yet
#define MOTOR_2 1 // This too 

short usSpeed = 150;  //default motor speed
unsigned short usMotor_Status = BRAKE;
 
void setup()                         
{
  pinMode(MOTOR_A1_1_PIN, OUTPUT);
  pinMode(MOTOR_A1_2_PIN, OUTPUT);
  pinMode(MOTOR_B1_1_PIN, OUTPUT);
  pinMode(MOTOR_B1_2_PIN, OUTPUT);

  pinMode(MOTOR_A2_1_PIN, OUTPUT);
  pinMode(MOTOR_A2_2_PIN, OUTPUT);
  pinMode(MOTOR_B2_1_PIN, OUTPUT);
  pinMode(MOTOR_B2_2_PIN, OUTPUT);

  pinMode(PWM_MOTOR_1_1, OUTPUT);
  pinMode(PWM_MOTOR_1_2, OUTPUT);
  pinMode(PWM_MOTOR_2_1, OUTPUT);
  pinMode(PWM_MOTOR_2_2, OUTPUT);

  pinMode(CURRENT_SEN_1_1, OUTPUT);
  pinMode(CURRENT_SEN_1_2, OUTPUT);
  pinMode(CURRENT_SEN_2_1, OUTPUT);  
  pinMode(CURRENT_SEN_2_2, OUTPUT);

  pinMode(EN_PIN_1_1, OUTPUT);
  pinMode(EN_PIN_1_2, OUTPUT);
  pinMode(EN_PIN_2_1, OUTPUT);
  pinMode(EN_PIN_2_2, OUTPUT);

  Serial.begin(9600);              // Initiates the serial to do the monitoring 
  Serial.println("Begin motor control");
  Serial.println(); //Print function list for user selection
  Serial.println("Enter number for control option:");
  Serial.println("1. STOP");
  Serial.println("2. FORWARD");
  Serial.println("3. REVERSE");
  Serial.println("4. READ CURRENT");
  Serial.println("+. INCREASE SPEED");
  Serial.println("-. DECREASE SPEED");
  Serial.println();

}

void loop() 
{
  digitalWrite(EN_PIN_1_1, HIGH);
  digitalWrite(EN_PIN_1_2, HIGH);
  digitalWrite(EN_PIN_2_1, HIGH);
  digitalWrite(EN_PIN_2_2, HIGH);  

  /*digitalWrite(MOTOR_A1_1_PIN, LOW); 
  digitalWrite(MOTOR_A1_2_PIN, LOW); 
  digitalWrite(MOTOR_B1_1_PIN, HIGH);
  digitalWrite(MOTOR_B1_2_PIN, HIGH);
  */
  
  
  digitalWrite(MOTOR_A1_1_PIN, LOW); 
  digitalWrite(MOTOR_A1_2_PIN, LOW); 
  digitalWrite(MOTOR_B1_1_PIN, HIGH);
  digitalWrite(MOTOR_B1_2_PIN, HIGH);


  /*digitalWrite(MOTOR_A2_1_PIN, HIGH);
  digitalWrite(MOTOR_A2_2_PIN, HIGH);
  digitalWrite(MOTOR_B2_1_PIN, LOW);
  digitalWrite(MOTOR_B2_2_PIN, LOW);
  */
 
      
  analogWrite(PWM_MOTOR_1_1, -255); 
  analogWrite(PWM_MOTOR_1_2, 255);
 //analogWrite(PWM_MOTOR_2_1, 255); 
 //analogWrite(PWM_MOTOR_2_2, 255); 
 
  
}

void Stop()
{
  Serial.println("Stop");
  usMotor_Status = BRAKE;
  motorGo(MOTOR_1, usMotor_Status, 0);
  motorGo(MOTOR_2, usMotor_Status, 0);
}

void Forward()
{
  Serial.println("Forward");
  usMotor_Status = CW;
  motorGo(MOTOR_1, usMotor_Status, usSpeed);
  motorGo(MOTOR_2, usMotor_Status, usSpeed);
}

void Reverse()
{
  Serial.println("Reverse");
  usMotor_Status = CCW;
  motorGo(MOTOR_1, usMotor_Status, usSpeed);
  motorGo(MOTOR_2, usMotor_Status, usSpeed);
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
}

void motorGo(uint8_t motor, uint8_t direct, uint8_t pwm)         //Function that controls the variables: motor(0 ou 1), direction (cw ou ccw) e pwm (entra 0 e 255);
{
  if(motor == MOTOR_1)
  {
    if(direct == CW)
    {
      digitalWrite(MOTOR_A1_1_PIN, LOW); 
      digitalWrite(MOTOR_A1_2_PIN, LOW); 
      digitalWrite(MOTOR_B1_1_PIN, HIGH);
      digitalWrite(MOTOR_B1_2_PIN, HIGH);
    }
    else if(direct == CCW) //WORKS
    {
      digitalWrite(MOTOR_A1_1_PIN, HIGH);
      digitalWrite(MOTOR_A1_2_PIN, HIGH);
      digitalWrite(MOTOR_B1_1_PIN, LOW);
      digitalWrite(MOTOR_B1_2_PIN, LOW);       
    }
    else
    {
      digitalWrite(MOTOR_A1_1_PIN, LOW);
      digitalWrite(MOTOR_A1_2_PIN, LOW);
      digitalWrite(MOTOR_B1_1_PIN, LOW); 
      digitalWrite(MOTOR_B1_2_PIN, LOW);           
    }
    
    analogWrite(PWM_MOTOR_1_1, pwm); 
    analogWrite(PWM_MOTOR_1_2, pwm); 
  }
  else if(motor == MOTOR_2) //GOOD WORKING
  {
    if(direct == CW) 
    {
      digitalWrite(MOTOR_A2_1_PIN, LOW);
      digitalWrite(MOTOR_A2_2_PIN, LOW);
      digitalWrite(MOTOR_B2_1_PIN, HIGH);
      digitalWrite(MOTOR_B2_2_PIN, HIGH);
    }
    else if(direct == CCW)
    {
      digitalWrite(MOTOR_A2_1_PIN, HIGH);
      digitalWrite(MOTOR_A2_2_PIN, HIGH);
      digitalWrite(MOTOR_B2_1_PIN, LOW);
      digitalWrite(MOTOR_B2_2_PIN, LOW);      
    }
    else
    {
      digitalWrite(MOTOR_A2_1_PIN, LOW);
      digitalWrite(MOTOR_A2_2_PIN, LOW);
      digitalWrite(MOTOR_B2_1_PIN, LOW);
      digitalWrite(MOTOR_B2_2_PIN, LOW);             
    }
    
    analogWrite(PWM_MOTOR_2_1, pwm);
    analogWrite(PWM_MOTOR_2_2, pwm);
  }
}
