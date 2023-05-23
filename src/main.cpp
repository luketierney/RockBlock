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
int TXD = 18;
int NetAV = 4;
int RTS = 3;
int CTS = 2;
int RXD = 19;
String Respond(){
  const unsigned int MAX_MESSAGE_LENGTH = 12;
  while (IriduimSerial.available() > 0)
 {
   //Create a place to hold the incoming message
   static char message[MAX_MESSAGE_LENGTH];
   static unsigned int message_pos = 0;

   //Read the next available byte in the serial receive buffer
   char inByte = IriduimSerial.read();

   //Message coming in (check not terminating character) and guard for over message size
   if ( inByte != '\n' && (message_pos < MAX_MESSAGE_LENGTH - 1) )
   {
     //Add the incoming byte to our message
     message[message_pos] = inByte;
     message_pos++;
   }
   //Full message received...
   else
   {
     //Add null character to string
     message[message_pos] = '\0';

     //Print the message (or do other things)
     message_pos = 0;
     return(message);

     //Reset for the next message
   }
 }
 return "None";
}
String AT(String x){
  Serial.println(x + '\r');
  delay(1000);
  String y = String(IriduimSerial.read());
  Serial.println(y);
  return String(Serial.read());
}
uint8_t buffer[200] = 
{ 1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89 };  
int SignalQuality = -1;

void setup() {
  // put your setup code here, to run once:
  int out[] = {TXD,RTS,MsgStateG,MsgStateY,MsgStateR,NetReadyG,NetReadyR,ReadyR,ReadyG};
  int in[] = {NetAV,CTS,RXD};
  for(int outpin : out){
    pinMode(outpin, OUTPUT);
  }
  for(int inpin : in){
    pinMode(inpin, INPUT);
  }
  Serial.begin(115200);
  while (!Serial);
  IriduimSerial.begin(19200);
  digitalWrite(6, HIGH);
  modem.adjustATTimeout(10);
  modem.adjustSendReceiveTimeout(20);
}
//static bool messageSent = false;
int err;
void loop() {
  while (true){
  if (modem.begin() != ISBD_SUCCESS){
    Serial.println("Couldn't begin modem operations.");
    exit(0);
    delay(6000);
    continue;
  }
  err = modem.getSignalQuality(SignalQuality);
  if ((err != ISBD_SUCCESS) or (SignalQuality <= 1) ){
    Serial.println("SignalQuality failed: error");
    digitalWrite(NetReadyR, HIGH);
    digitalWrite(NetReadyG, LOW);
    delay(60000);
    continue;
  }
  digitalWrite(NetReadyG, HIGH);
  digitalWrite(NetReadyR, LOW);
  digitalWrite(MsgStateY, HIGH);
  size_t bufferSize = sizeof(buffer);
  err = modem.sendReceiveSBDText("Test", buffer, bufferSize);
  if(err != ISBD_SUCCESS){
    Serial.println("Send failed: error");
    digitalWrite(MsgStateY, LOW);
    digitalWrite(MsgStateR, HIGH);
    digitalWrite(MsgStateG, LOW);
    continue;
  }
  digitalWrite(MsgStateY, LOW);
  digitalWrite(MsgStateG, HIGH);
  digitalWrite(MsgStateR, LOW);
  Serial.print("Inbound buffer size is ");
  Serial.println(bufferSize);
  for (int i=0; i<bufferSize; ++i){
        Serial.print(buffer[i], HEX);
        if (isprint(buffer[i]))
        {
          Serial.print("(");
          Serial.write(buffer[i]);
          Serial.print(")");
        }
        Serial.print(" ");
      }
  delay(500);
}}
#if DIAGNOSTICS
void ISBDConsoleCallback(IridiumSBD *device, char c)
{
  Serial.write(c);
}

void ISBDDiagsCallback(IridiumSBD *device, char c)
{
  Serial.write(c);
}
#endif