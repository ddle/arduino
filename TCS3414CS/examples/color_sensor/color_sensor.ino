/******************************************************************************/

#include <TCS3414CS.h>
#include <Wire.h>

void setup()
{  
  Wire.begin(); // join i2c bus (address optional for master)
  Serial.begin(9600);
  TCS3414CS_init();
}
void loop()
{
  //TCS3414CS_test();
  readRGB();		  
  //printRGB();
  printColor(getColorSingle(2800,2300,2000)); 
  delay( getIntegrationTime()*8 );
}
