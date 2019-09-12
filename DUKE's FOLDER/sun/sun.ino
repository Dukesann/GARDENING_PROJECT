#define Sun A8 //sunlight sensor analog pin

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
Serial.print("Sun Value: ");
Serial.println(analogRead(Sun));
}
