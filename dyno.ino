#include <Encoder.h>
#include <HX711.h>

#define ENC_A 2
#define ENC_B 3
#define LOADCELL_DOUT 7
#define LOADCELL_CLK 8
#define VOLT_SENSE A7
#define CURR_SENSE A6

#define ENC_PPR 8


Encoder enc(ENC_A, ENC_B);
HX711 load;

void setup()
{
    pinMode(VOLT_SENSE, INPUT);
    pinMode(CURR_SENSE, INPUT);

    pinMode(5, OUTPUT);
    pinMode(6, OUTPUT);

    load.begin(LOADCELL_DOUT, LOADCELL_CLK);
    load.set_scale(219);
    // load.set_scale();
    load.tare();


    Serial.begin(9600);    
}

void set_motor(int pct)
{
    if (pct > 0)
    {
        analogWrite(5, pct * 255 / 100);
        analogWrite(6, 0);
    } else
    {
        analogWrite(6, -pct * 255 / 100);
        analogWrite(5, 0);
    }

}

double get_rpm()
{
    static double last_time = millis();
    static int last_ticks = enc.read();

    double time = millis();
    double ticks = enc.read();

    double out = ((ticks - last_ticks) / ENC_PPR) / ((time - last_time) / 60000);

    last_time = time;
    last_ticks = ticks;

    return out;
}

double get_voltage()
{
    return analogRead(VOLT_SENSE) / 148.6;
}

double get_current()
{
    return analogRead(CURR_SENSE);
}

void print_data()
{
    Serial.print(millis() / 1000.0);
    Serial.print(", ");
    Serial.print(load.get_units());
    Serial.print(", ");
    Serial.print(get_rpm());
    Serial.print(", ");
    Serial.print(get_voltage());
    Serial.print(", ");
    Serial.println(get_current());
}

void loop()
{    
    Serial.println(analogRead(CURR_SENSE));
    set_motor(100);
    delay(20);
}