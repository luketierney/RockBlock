#include <Arduino.h>
int ReadyR = 13;
int ReadyG = 12;
int NetReadyR = 11;
int NetReadyG = 10;
int MsgStateR = 9;
int MsgStateY = 8;
int MsgStateG = 7;
int SleepWake = 6;
int TXD = 5;
int NetAV = 4;
int RTS = 3;
int CTS = 2;
int RXD = 1;
String AT(String x){
  Serial.println(x + "\r");
  delay(100);
  return String(Serial.read());
}
void setup() {
  // put your setup code here, to run once:
  int out[] = {5,3,6,7,8,9,10,11,12,13};
  int in[] = {4,2,1};
  for(int outpin: out){
    pinMode(outpin, OUTPUT);
  }
  for(int inpin : in){
    pinMode(inpin, INPUT);
  }
  Serial.begin(19200);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(2, HIGH);
  delay(15000);
  if(AT("AT") == "OK"){
    Serial.println("Luke OK");
    digitalWrite(ReadyG, HIGH);
    digitalWrite(ReadyR, LOW);
  }
  else{
    digitalWrite(ReadyG, LOW);
    digitalWrite(ReadyR, HIGH);
    Serial.println(AT("AT"));
  }
  

}