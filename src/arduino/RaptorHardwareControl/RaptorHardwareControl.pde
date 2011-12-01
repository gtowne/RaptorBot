#include <RaptorWireProtocol.h>
#include <stdio.h>
#include <NewSoftSerial.h>

#define SWING_ITERATIONS 40

int val;
NewSoftSerial serial(0,1);
SerialPacket curPacket;

void setup()
{
  pinMode(11, OUTPUT);
  serial.begin(9600);
  val=0;
  
}

void loop()
{   
    
    if (readPacket(&curPacket)) {
  
       switch(curPacket.type) { 
           case SET_PIN:
           
             analogWrite(curPacket.data1, curPacket.data2);
              
             break;
             
           case TURN:
             
             if (curPacket.data2 == LEFT) {
                turnLeft(curPacket.data1, curPacket.data3);
             } else if (curPacket.data2 == RIGHT) {
                turnRight(curPacket.data1, curPacket.data3);
             }
             
             break;
             
       }
      
    }
  
  
  
    /*int pin = 11;
    
    if (input == 'a') {
      for (int i = 0; i < 1000; i++)
          analogWrite(pin,255);
    } else if (input == 'b') {
          analogWrite(pin, 0); 
    }*/
  
}

int readPacket(SerialPacket* packet) {
    while (!serial.available());
    packet->type = serial.read();
   
    while (!serial.available());
    packet->data1 = serial.read();
    
    while (!serial.available());
    packet->data2 = serial.read();
    
    while (!serial.available());
    packet->data3 = serial.read();
    
    char buff[100];
    
    sprintf(buff, "Got packet type=%u data1=%u data2=%u data3=%u\n", packet->type, packet->data1, packet->data2, packet->data3);
    
    serial.print(ACK_CHAR);
    //serial.flush();
 
  
  
  return 1;
}

int turnLeft (int pin, int d) {
    int pulse = 1500 - (10 * d);
    
    for (int i=0; i<SWING_ITERATIONS; i++){
      digitalWrite(pin, HIGH);
      delayMicroseconds(pulse);
      digitalWrite(pin, LOW); 
      delay(20);
    }

    return 1;
}

int turnRight(int pin, int d) {
    int pulse = 1500 + (10 * d);

    for (int i=0; i<SWING_ITERATIONS; i++){
      digitalWrite(pin, HIGH);
      delayMicroseconds(pulse);
      digitalWrite(pin, LOW); 
      delay(20);
    }
    
    return 1;
}


