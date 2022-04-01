#include <HX711.h>
#include <Wire.h>

#define ENC_A 2
#define ENC_B 3
#define LOADCELL_DOUT 4
#define LOADCELL_CLK 2

#define LOADCELL_SCALE 1
#define ENC_PPR 8


void setup()
{

    Serial.begin(9600);    
}

void loop()
{

    delay(100);
}