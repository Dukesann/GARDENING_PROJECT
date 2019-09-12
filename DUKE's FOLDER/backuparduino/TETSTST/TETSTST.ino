int x = 1;

void setup() {
  Serial.begin(9600);
}

void loop() {
  

  while(x<20){
    Serial.println(x);
    x = x+1;
    Serial.println(x);
    x = x+1;
    Serial.println(x);
    x = x+20;
    Serial.println(x);
    x = x+100;
    Serial.println(x);
  }

  delay(100000);

}
