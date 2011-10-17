#include <NewSoftSerial.h>


int val;
NewSoftSerial serial(0,1);

void setup()
{
  //pinMode(0, INPUT);
  //pinMode(1, OUTPUT);
  pinMode(11, OUTPUT);
  serial.begin(9600);
  val=0;
  
}

void loop()
{
  
    char input = serial.read();
    //char input = 'a';
    
    int pin = 11;
    
    if (input == 'a') {
      for (int i = 0; i < 1000; i++)
          analogWrite(pin,255);
    } else if (input == 'b') {
          analogWrite(pin, 0); 
    }
  
}


