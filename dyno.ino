#include <Encoder.h>
#include <HX711.h>

// Hardware Setup
Encoder enc(2, 3);
HX711 loadcell;

void setup()
{
	loadcell.begin(4, 5);
    loadcell.set_scale(1);

    Serial.begin(9600);    
}

void loop()
{
	
}
