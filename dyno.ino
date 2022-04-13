#include <Encoder.h>
#include <HX711.h>

#define ENC_A 2
#define ENC_B 3
#define LOADCELL_DOUT 7
#define LOADCELL_CLK 8
#define VOLT_SENSE A7
#define CURR_SENSE A6

#define ENC_PPR 48


Encoder enc(ENC_B, ENC_A);
HX711 load;

void setup()
{
    pinMode(VOLT_SENSE, INPUT);
    pinMode(CURR_SENSE, INPUT);

    pinMode(5, OUTPUT);
    pinMode(6, OUTPUT);

    load.begin(LOADCELL_DOUT, LOADCELL_CLK);
    load.set_scale(219);
    load.tare();
    delay(100);

    Serial.begin(9600);    

    // return;
    Serial.println("TIME\tLOAD\tSPEED\tVOLT\tCURRENT\tTORQUE\tMOTOR_POWER\tELEC_POWER\tEFFICIENCY\n");

    set_motor(100);
    while(millis() < 10000)
    {
        print_data();
        // delay(20);
    }

    

}

void set_motor(int pct)
{
    if (pct > 0)
    {
        analogWrite(5, pct * 255 / 100);
        digitalWrite(6, LOW);
    } else if (pct < 0)
    {
        analogWrite(6, -pct * 255 / 100);
        digitalWrite(5, LOW);
    } else
    {
        digitalWrite(5, LOW);
        digitalWrite(6, LOW);
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
    return (analogRead(CURR_SENSE) * -0.0256) + 13;
}

double get_force()
{
    return load.get_units() * 0.00220462; // milli-pounds
}

double get_torque()
{
    return get_force() * 5.5; // 5.5 inch lever arm
}

void print_data()
{
    double force = get_force();
    double rpm = get_rpm();
    double voltage = get_voltage();
    double current = get_current();
    double torque = get_torque();
    double motor_power = torque * (rpm / 60) * 1.36;
    double elec_power = current * voltage;
    double efficiency = motor_power / elec_power;

    Serial.print(millis() / 1000.0);
    Serial.print(", ");
    Serial.print(force, 4);
    Serial.print(", ");
    Serial.print(rpm);
    Serial.print(", ");
    Serial.print(voltage, 4);
    Serial.print(", ");
    Serial.print(current, 4);
    Serial.print(", ");
    Serial.print(torque, 4);
    Serial.print(", ");
    Serial.print(motor_power, 4);
    Serial.print(", ");
    Serial.print(elec_power, 4);
    Serial.print(", ");
    Serial.println(efficiency, 4);
}

void loop()
{    

    set_motor(0);

    // Serial.println(get_force(), 3);
    
    // Serial.println(enc.read());

    // Serial.println(enc.read());


    // Serial.println(get_current());

    // if((millis() / 10000) % 2 == 0)
    // {
    //     set_motor(100);
    // } else
    // {
    //     set_motor(0);
    // }

    delay(20);
}