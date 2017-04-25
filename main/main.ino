/*
Aici se includ librariile.
dht e pentru senzorul de temperatura si umiditate
SoftwareSerial e pentru bluetooth
OneWire si DallasTemperature sunt pentru temperatura apei

*/

#include <dht.h>
#include <SoftwareSerial.h>// import the serial library
#include <OneWire.h>
#include <DallasTemperature.h>


//pin pentru umid si temperatura
#define dht_apin A0 

//pentru temperatura apei
#define ONE_WIRE_BUS 12
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);


int MOTOR2_PIN1 = 3;
int MOTOR2_PIN2 = 5;
int MOTOR1_PIN1 = 6;
int MOTOR1_PIN2 = 9;


//variabile debit
volatile int flow_frequency; // Pulsatii senzor debit
unsigned int l_hour; // Litrii pe ora
unsigned char flowsensor = 2; // Inputul senzorului
unsigned long currentTime;
unsigned long cloopTime;
void flow () // 
{
   flow_frequency++;
}

float Celsius = 0;


char data;

SoftwareSerial bluetooth(10, 11); // RX, TX

dht DHT;


void setup() {
  pinMode(MOTOR1_PIN1, OUTPUT);
  pinMode(MOTOR1_PIN2, OUTPUT);
  pinMode(MOTOR2_PIN1, OUTPUT);
  pinMode(MOTOR2_PIN2, OUTPUT);  
  bluetooth.begin(9600);
  
   sensors.begin();
   
   pinMode(flowsensor, INPUT);
   digitalWrite(flowsensor, HIGH);
   Serial.begin(9600);
   attachInterrupt(0, flow, RISING); 
   sei(); // Enable interrupts
   currentTime = millis();
   cloopTime = currentTime;
}

void loop() {  
  
  /*
  programul asculta constant pentru bluetooth
  daca ia 1 merge inainte
  2 intoarce la stanga (Sistem senilat)
  3 intoarce la dreapta (sistem senilat)
  4 intoarce inapoi 
  
  s = stop
  
  t = temperatura
  h = umiditate 
  w = temperatura apei
  d = debitul apei
  
  
  */
   if (bluetooth.available())
   {
     data=bluetooth.read();
     if(data=='1')
     {
       // if number 1 pressed ...
       while(!bluetooth.available()) {
         go(255,255);
       }
     }
  if (data=='2')
  {// if number 0 pressed ....
    while(!bluetooth.available())
      go(-255,255);
  }
  
  if(data=='3') //dreapta
  {
    while(!bluetooth.available())
      go(255,-255);
    
  }
  
  
  if(data=='4') //inapoi
  {
    while(!bluetooth.available())
      go(-255,-255);
  }
 
 if(data=='s') //opreste
 {
   while(!(bluetooth.available()))
     go(0,0);
 }
 
 if(data=='t')
 {
   bluetooth.println("Temperatura este ");
   bluetooth.println(temperature()-5);
 }
 
 if(data=='h')
 {
   bluetooth.println("Umiditatea este ");
   bluetooth.println(humidity()); 
 }
 
 if(data=='w')
 {
   bluetooth.println("Temperatura apei este:  ");
   bluetooth.println(waterTemperature()); 
  }
 }
 
 
 if(data=='d')
 {
   bluetooth.println("Debitul este: ");
  while(!bluetooth.available())
   bluetooth.println(debit());
}
}

void go(int speedLeft, int speedRight) {
  if (speedLeft > 0) {
    analogWrite(MOTOR1_PIN1, speedLeft);
    analogWrite(MOTOR1_PIN2, 0);
    Serial.print("pula");
  } 
  else {
    analogWrite(MOTOR1_PIN1, 0);
    analogWrite(MOTOR1_PIN2, -speedLeft);
  }
 
  if (speedRight > 0) {
    analogWrite(MOTOR2_PIN1, speedRight);
    analogWrite(MOTOR2_PIN2, 0);
  }else {
    analogWrite(MOTOR2_PIN1, 0);
    analogWrite(MOTOR2_PIN2, -speedRight);
  }
}


int temperature()
{
 
    DHT.read11(dht_apin); 
    return DHT.temperature;
}

int humidity ()
{
  DHT.read11(dht_apin);
  return DHT.humidity;
}

int waterTemperature()
{
  sensors.requestTemperatures();
  Celsius = sensors.getTempCByIndex(0);
  return Celsius;
  
}

int debit()
{
     currentTime = millis();
   if(currentTime >= (cloopTime + 1000))
   {
      cloopTime = currentTime;
      // Pulse frequency (Hz) = 7.5Q, Q is flow rate in L/min.
      l_hour = (flow_frequency * 60 / 7.5); // (Pulse frequency x 60 min) / 7.5Q = flowrate in L/hour
      flow_frequency = 0; // Reset Counter
   }
   
   return l_hour;
  
}

