#include <SoftwareSerial.h>// import the serial library

SoftwareSerial bluetooth(10, 11); // RX, TX
int ledpin=13; // led on D13 will show blink on / off
int BluetoothData; // the data given from Computer

void setup() {
  // put your setup code here, to run once:
  bluetooth.begin(9600);
  bluetooth.println("Bluetooth On please press 1 or 0 blink LED ..");
  pinMode(ledpin,OUTPUT);
  
}

void loop() {
  // put your main code here, to run repeatedly:
   if (bluetooth.available()){
     
BluetoothData=bluetooth.read();
   if(BluetoothData=='1'){   // if number 1 pressed ....
   digitalWrite(ledpin,1);
   bluetooth.println("LED  On D13 ON ! ");
   digitalWrite(13,HIGH);
   }
  if (BluetoothData=='0'){// if number 0 pressed ....
  digitalWrite(ledpin,0);
   bluetooth.println("LED  On D13 Off ! ");
   digitalWrite(13,LOW);
  }
}
delay(100);// prepare for next data ...
}
