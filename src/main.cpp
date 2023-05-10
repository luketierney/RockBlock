#include <Arduino.h>
int ReadyR = 13;
int ReadyG = 12;
int NetReadyR = 11;
int NetReadyG = 10;
int MsgStateR = 9;
int MsgStateY = 8;
int MsgStateG = 7;
int SleepWake = 6;
int TXD = 0;
int NetAV = 4;
int RTS = 3;
int CTS = 2;
int RXD = 1;
String AT(String x){
  Serial.println(x + '\r');
  Serial.println(Serial.read());
  delay(10000);
  return String(Serial.read());
}
void setup() {
  // put your setup code here, to run once:
  int out[] = {TXD,RTS,SleepWake,MsgStateG,MsgStateY,MsgStateR,NetReadyG,NetReadyR,ReadyR,ReadyG};
  int in[] = {NetAV,CTS,RXD};
  for(int outpin : out){
    pinMode(outpin, OUTPUT);
  }
  for(int inpin : in){
    pinMode(inpin, INPUT);
  }
  Serial.begin(19200);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(AT("AT") == "OK"){
    Serial.println("Luke OK");
    digitalWrite(ReadyG, HIGH);
    digitalWrite(ReadyR, LOW);
  }
  else{
    digitalWrite(ReadyG, LOW);
    digitalWrite(ReadyR, HIGH);
    Serial.println("Not OK");
    Serial.println("LUKE" + AT("AT"));
  }
  

}