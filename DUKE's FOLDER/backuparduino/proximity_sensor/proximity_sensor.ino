#define pin A10

void setup() {
  // put your setup code here, to run once:
pinMode(pin,INPUT);
Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
Serial.print("Sensor Value: ");
Serial.println(analogRead(pin));
}
