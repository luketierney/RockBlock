#include <Arduino.h>
#include <IridiumSBD.h>
#define IriduimSerial Serial3
#define DIAGNOSTICS true
IridiumSBD modem(IriduimSerial);
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
  Serial.begin(115200);
  IriduimSerial.begin(19200);
  Serial.println("TEST");
  modem.setPowerProfile(IridiumSBD::DEFAULT_POWER_PROFILE);
  if (modem.begin() != ISBD_SUCCESS){
    Serial.println("Couldn't begin modem operations.");
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  if(AT("AT") == "OK"){
    IriduimSerial.println("Luke OK");
    digitalWrite(ReadyG, HIGH);
    digitalWrite(ReadyR, LOW);
  }
  else{
    digitalWrite(ReadyG, LOW);
    digitalWrite(ReadyR, HIGH);
    IriduimSerial.println("Not OK");
    Serial.println("LUKE" + AT("AT"));
  }
  

}