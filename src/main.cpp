#include <Arduino.h>
#include <IridiumSBD.h>
#include <dht_nonblocking.h>
#define IriduimSerial Serial3
#define DIAGNOSTICS true
#define DHT_SENSOR_TYPE DHT_TYPE_11
IridiumSBD modem(IriduimSerial);
int failLimit = 7;
int TempR = 23;
int TempG = 26;
int ReadyR = 13;
int ReadyG = 1;
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
static const int DHT_SENSOR_PIN = 22;
DHT_nonblocking dht_sensor( DHT_SENSOR_PIN, DHT_SENSOR_TYPE );

static bool measure_environment( float *temperature, float *humidity )
{
  static unsigned long measurement_timestamp = millis( );

  /* Measure once every four seconds. */
  if( millis( ) - measurement_timestamp > 0ul )
  {
    if( dht_sensor.measure( temperature, humidity ) == true )
    {
      measurement_timestamp = millis( );
      return( true );
    }
  }

  return( false );
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
  modem.adjustATTimeout(30);
  modem.adjustSendReceiveTimeout(300);
}
int count = 0;
int err;
void loop() {
  while (true){
  delay(500);
  count += 1;
  if (count > failLimit){
    count = 0;
    Serial.println("Sleeping");
    digitalWrite(ReadyG, LOW);
    digitalWrite(ReadyR, LOW);
    for(int i = 0; i <= 5; i+= 1){
     digitalWrite(ReadyR, HIGH); 
     delay(500);
     digitalWrite(ReadyR, LOW);
    }
    digitalWrite(ReadyR, HIGH);
    delay(100);
    modem.sleep();
    delay(600000); 
  }
  float temperature;
  float humidity;
  while(true){
  if(!measure_environment( &temperature, &humidity )){
    digitalWrite(TempR, HIGH);
    digitalWrite(TempG, LOW);
    continue;
  }
  else{
    break;
  }
  }
  digitalWrite(TempG, HIGH);
  digitalWrite(TempR, LOW);
  Serial.println(String(temperature) + " deg. C");
  if(modem.isAsleep()){
  if(modem.begin() != ISBD_SUCCESS){
    digitalWrite(ReadyR, HIGH);
    digitalWrite(ReadyG, LOW);
    Serial.println("Couldn't begin modem operations.");
    delay(6000);
    continue;
  }}
  digitalWrite(ReadyG, HIGH);
  digitalWrite(ReadyR, LOW);
  err = modem.getSignalQuality(SignalQuality);
  if ((err != ISBD_SUCCESS) or (SignalQuality <= 1)){
    Serial.println("SignalQuality failed: error");
    digitalWrite(NetReadyR, HIGH);
    digitalWrite(NetReadyG, LOW);
    delay(5000);
    continue;
  }
  digitalWrite(NetReadyG, HIGH);
  digitalWrite(NetReadyR, LOW);
  digitalWrite(MsgStateG, LOW);
  digitalWrite(MsgStateR, LOW);
  digitalWrite(MsgStateY, HIGH);
  String str = String(String(temperature) + " deg. C");
  const char * c = str.c_str();
  err = modem.sendSBDText(c);
  if(err != ISBD_SUCCESS){
    Serial.println("Send failed: error");
    digitalWrite(MsgStateY, LOW);
    digitalWrite(MsgStateR, HIGH);
    digitalWrite(MsgStateG, LOW);
    delay(500);
    continue;
  }
  digitalWrite(MsgStateY, LOW);
  digitalWrite(MsgStateG, HIGH);
  digitalWrite(MsgStateR, LOW);
  while (true){
    delay(100);
  }
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