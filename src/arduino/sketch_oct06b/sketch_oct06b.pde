#include <Morse.h>

Morse morse(12);
int val;

void setup()
{
  val = 0;
}

void loop()
{
  //morse.dot(); morse.dot(); morse.dot();
  //morse.dash(); morse.dash(); morse.dash();
  //morse.dot(); morse.dot(); morse.dot();
  int pin = 11;
  
  val = (val + 1) % 256;
  analogWrite(pin,val);
  delay(30);
  
}


