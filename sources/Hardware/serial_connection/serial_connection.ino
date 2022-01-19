#include <Bounce.h>
#include <Mouse.h>
void setup()
{
    pinMode(13, OUTPUT);
  Serial.begin(345600); // COM4
  
 digitalWrite(13, HIGH);   // set the LED on

}

void loop()
{
  if (!Serial.available())
  return;
  
  int x = (int)Serial.read();
  int y = (int)Serial.read();
  Mouse.move(x, y);
 // Serial.write("WORKING!");
}
