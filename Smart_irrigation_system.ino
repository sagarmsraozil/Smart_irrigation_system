#include "DHTesp.h"
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#define BLYNK_PRINT Serial

// 
//
const char ssid[] = "Samsungs10";                                  
const char pass[] = "nepal123";                              
const char auth[] = "0qe4KEWPCduWBcZ-hKBiA2JWNdtLU5jz";

 

//Initializing Rain sensor
 
int sensorpin=A0;

int sensorvalue=0;
int outputvalue=0;


int trigPin = 5;
int echoPin = 4;
int pingTravelTime;
int distance=0;
int inches = 0;

int motor2 = 14;
int motor1 = 12;


 
DHTesp dht;
BlynkTimer timer;



void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println("Status\tHumidity (%)\tTemperature (C)");
  dht.setup(2, DHTesp::DHT22); 
//  pinMode(outputPin, OUTPUT);
  pinMode(trigPin,OUTPUT);
  pinMode(echoPin,INPUT);
 
  pinMode(motor2,OUTPUT);
 
  pinMode(motor1,OUTPUT);



  Blynk.begin(auth,ssid,pass);
  Blynk.notify("weather station ready to go!!!");
  Blynk.syncAll(); // Connect DHT sensor to GPIO 5
 
  timer.setInterval(1000L, myTimerEvent);
}





void loop()
{
  
   myTimerEvent();
   soilMoisture();
   distanceChecker();

   Blynk.run();//  Running Blynk app
  timer.run(); // Initiates BlynkTimer 
}



long readUltrasonicDistance(int tx, int rx) {
  
  digitalWrite(tx,LOW);
  delayMicroseconds(10);
  digitalWrite(tx,HIGH);
  delayMicroseconds(10);
  digitalWrite(tx,LOW);
  pingTravelTime = pulseIn(rx,HIGH);
  return pingTravelTime;
}

void distanceChecker()
{
 
   distance = 0.01723*(readUltrasonicDistance(5,4)/2);
 
  Serial.println("Distance is : ");
  Serial.print(distance);
  Serial.print("cm");
  delay(100);
}


void soilMoisture()
{
  sensorvalue=analogRead(sensorpin);
  outputvalue=map(sensorvalue,0,1023,0,100); //100,0
  delay(1000);

  if(outputvalue>74)
  {
       Serial.println("water your plant: ");
       Serial.print(outputvalue);
      Blynk.notify("water your plant");
      digitalWrite(motor1,LOW);
      digitalWrite(motor2,LOW);  

       
  }
  else if(outputvalue<45)
  {
       Serial.println("soil is wet enough to water: ");
         Blynk.notify("soil is wet enough to water");
       Serial.print(outputvalue);
       digitalWrite(motor1,HIGH);
      digitalWrite(motor2,HIGH);       
//      digitalWrite(16,LOW);
//      digitalWrite(4,HIGH);
      
      
      
  }
  else
  {
      Serial.println("soil is in normal condition");
         Blynk.notify("soil is in normal condition");
         digitalWrite(motor1,HIGH);  
      digitalWrite(motor2,HIGH); 
       Serial.print(outputvalue);
  }
}


void myTimerEvent(){
  float humidity = dht.getHumidity();
  float temperature = dht.getTemperature();
  
  Serial.print("Temperature: ");
  Serial.print(temperature, 1);
  Serial.print(" celcius, Humidity: ");
  Serial.print(humidity, 1);
  
//  rainValue = analogRead(rainSensor);//Reading data from rain sensor
//  Serial.print(" %, Rain: ");
//  Serial.println(rainValue); 
//  
//if (rainValue>=700){
//digitalWrite(outputPin, LOW);
//  }
//  else{
//  Blynk.notify("Huge rainfall is detected!!!");
//  Serial.print("rain is detected");
//  digitalWrite(outputPin, HIGH); 
//  }

Blynk.virtualWrite(V0, temperature);
Blynk.virtualWrite(V1, outputvalue);
Blynk.virtualWrite(V2, humidity); 
Blynk.virtualWrite(V3, distance); 
//Blynk.virtualWrite(V4, rainValue);
  }
