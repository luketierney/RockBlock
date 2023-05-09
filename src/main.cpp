#include <Arduino.h>
String AT(String x){
  Serial.println(x + "\r");
  delay(100);
  return String(Serial.read());
}
void setup() {
  // put your setup code here, to run once:
  for(int x = 7; x <= 13; x += 1){
    pinMode(x, OUTPUT);
  }
  
  Serial.begin(19200);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(2, HIGH);
  if(AT("AT") == "OK"){
    Serial.println("Luke OK");
  }
  else{
    Serial.println(AT("AT"));
  }
}