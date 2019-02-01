#include <SoftwareSerial.h>
#define RX 10
#define TX 11
const int RECV_PIN1 = 3;
const int RECV_PIN5 = 4;
const int redLED = 8;
const float distance = 24.0; //in cm
unsigned long timeTravel[2], timer;
String AP = "SSID HOTSPOT";       // CHANGE ME
String PASS = "PASSWORD"; // CHANGE ME
String API = "";   // CHANGE ME
String HOST = "api.thingspeak.com";
String PORT = "80";
String field = "field1";
int countTrueCommand;
int countTimeCommand; 
boolean found = false; 
int valSensor = 1;
SoftwareSerial esp8266(RX,TX); 
 

void setup() {
  Serial.begin(9600);
  esp8266.begin(115200);
  sendCommand("AT",10,"OK");
  sendCommand("AT+CWMODE=1",5,"OK");
  sendCommand("AT+CWJAP=\""+ AP +"\",\""+ PASS +"\"",20,"OK");
  pinMode(RECV_PIN1, INPUT);
  pinMode(RECV_PIN5, INPUT);
  pinMode(redLED, OUTPUT);
}
void loop() {
  Serial.print("LINK START \n");
 valSensor = getSensorData();
 String getData = "GET /update?api_key="+ API +"&"+ field +"="+String(valSensor);
sendCommand("AT+CIPMUX=1",2,"OK");
 sendCommand("AT+CIPSTART=0,\"TCP\",\""+ HOST +"\","+ PORT,2,"OK");
 sendCommand("AT+CIPSEND=0," +String(getData.length()+4),2,">");
 esp8266.println(getData);delay(100);countTrueCommand++;
 sendCommand("AT+CIPCLOSE=0",2,"OK");
}
float getSensorData(){
  float speedtest=0.0000;
  float speedlimit = 1.4400; //in km/h
  while(digitalRead(RECV_PIN1)==HIGH)
  {
    delay(100);
  }
  if(digitalRead(RECV_PIN1)==LOW)
  {
    timeTravel[0]=millis();
  while(digitalRead(RECV_PIN5)==HIGH)
  {
    delay(100);
  }
  if(digitalRead(RECV_PIN5)==LOW)
  {
    timeTravel[1]=millis();
  }
  }
  Serial.print("Waktu Awal : ");
  Serial.println(timeTravel[1]);
  Serial.print("Waktu Akhir : ");
  Serial.println(timeTravel[0]);
  timer = timeTravel[1]-timeTravel[0];
  speedtest = distance/(float) timer * 10.0*3.6; // in m/s
  Serial.println("");
  Serial.println(speedtest,5);
  Serial.println("");
  Serial.println("Speedtest");
  Serial.print(speedtest); //in km/h
  Serial.println(" km/h");
  Serial.println("Speedlimit");
  Serial.print(speedlimit); //in km/h
  Serial.println(" km/h");
  

  // COnvert to integer for if statement
  speedtest = (int) (speedtest*1000.0);
  speedlimit = (int) (speedlimit*1000.0);
  
   if(speedtest>speedlimit)
   {
      digitalWrite(redLED, HIGH);
      Serial.print("Slow down");
      Serial.print("\n");
      delay(4000);
      digitalWrite(redLED,LOW);
   }
   delay(100); // Replace with 
  speedtest = speedtest/1000.0;
  return speedtest;
}
void sendCommand(String command, int maxTime, char readReplay[]) {
  Serial.print(countTrueCommand);
  Serial.print(". at command => ");
  Serial.print(command);
  Serial.print(" ");
  while(countTimeCommand < (maxTime*1))
  {
    esp8266.println(command);//at+cipsend
    if(esp8266.find(readReplay))//ok
    {
      found = true;
      break;
    }
  
    countTimeCommand++;
  }
  
  if(found == true)
  {
    Serial.println("OYI");
    countTrueCommand++;
    countTimeCommand = 0;
  }
  
  if(found == false)
  {
    Serial.println("Fail");
    countTrueCommand = 0;
    countTimeCommand = 0;
  }
  
  found = false;
 }
