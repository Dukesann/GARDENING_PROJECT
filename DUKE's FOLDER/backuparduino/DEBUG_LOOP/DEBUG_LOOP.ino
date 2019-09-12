int num = 1;
int looop = 0;

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
while( num != 100 ){
  Serial.print("Number: ");
  Serial.print(num);
  Serial.print("Loop: ");
  Serial.println(looop);
  looop = looop + 1;
  if( looop == 300 ){
    change();
  } 
}
Serial.print("Number: ");
Serial.print(num);
Serial.print("Loop: ");
Serial.print(looop);
delay(10000000);
}

void change(){
  if( num == 1 ){
    num = 100;
  }
}
