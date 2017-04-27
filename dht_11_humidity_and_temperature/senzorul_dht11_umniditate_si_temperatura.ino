#include <dht.h>
#define dht_apin A0

dht DHT;

void setup() {
  pinMode(A0,OUTPUT);
  Serial.begin(9600);
}

void loop() {
   
    DHT.read11(dht_apin);
    
    Serial.print("Current humidity = ");
    Serial.print(DHT.humidity);
    Serial.print("%  ");
    Serial.print("temperature = ");
    Serial.print(DHT.temperature-6); 
    Serial.println("C  ");
}

