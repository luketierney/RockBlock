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
String AT(String x){
  Serial.println(x + '\r');
  delay(1000);
  String y = String(IriduimSerial.read());
  Serial.println(y);
  return String(Serial.read());
}
uint8_t buffer[200] = 
{ 1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89 };
void setup() {
  // put your setup code here, to run once:
  int signalQuality = -1;
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
  modem.setPowerProfile(IridiumSBD::DEFAULT_POWER_PROFILE);
  if (modem.begin() != ISBD_SUCCESS){
    Serial.println("Couldn't begin modem operations.");
    exit(0);
  }
  Serial.print("LUKE" + IriduimSerial.read());
  int err = modem.getSignalQuality(signalQuality);
  if (err != 0)
  {
    Serial.print("SignalQuality failed: error ");
    Serial.println(err);
    exit(1);
  }
  Serial.print("Signal quality is ");
  Serial.println(signalQuality);

}
//static bool messageSent = false;

void loop() {
  digitalWrite(6, HIGH);
  // put your main code here, to run repeatedly:
  /*int err;
  if (!messageSent || modem.getWaitingMessageCount() > 0)
  {
    size_t bufferSize = sizeof(buffer);

    // First time through send+receive; subsequent loops receive only
    if (!messageSent)
      err = modem.sendReceiveSBDBinary(buffer, 11, buffer, bufferSize);
    else
      err = modem.sendReceiveSBDText(NULL, buffer, bufferSize);
      
    if (err != ISBD_SUCCESS)
    {
      Serial.print("sendReceiveSBD* failed: error ");
      Serial.println(err);
    }
    else // success!
    {
      messageSent = true;
      Serial.print("Inbound buffer size is ");
      Serial.println(bufferSize);
      for (int i=0; i<bufferSize; ++i)
      {
        Serial.print(buffer[i], HEX);
        if (isprint(buffer[i]))
        {
          Serial.print("(");
          Serial.write(buffer[i]);
          Serial.print(")");
        }
        Serial.print(" ");
      }
      Serial.println();
      Serial.print("Messages remaining to be retrieved: ");
      Serial.println(modem.getWaitingMessageCount());
    }
  }
*/
}
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