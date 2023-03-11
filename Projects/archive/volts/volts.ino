/*arduino:avr:uno

Voltage Measurement :
Arduino’s analog inputs can be used to measure DC voltage between 0 and 5V (when using the standard 5V analog reference voltage) 
and this range can be increased by using a voltage divider network. 
The voltage divider steps down the voltage being measured within the range of the Arduino analog inputs.

For a voltage divider circuit Vout = R2/(R1+R2) x Vin

Vin = (R1+R2)/R2 x Vout

The analogRead() function reads the voltage and converts it to a number between 0 and 1023

Calibration :
We’re going to read output value with one of the analog inputs of Arduino and its analogRead() function. 
That function outputs a value between 0 and 1023 that is 0.00488V for each increment (As 5/1024 = 0.00488V)

Vin = Vout*(R1+R2)/R2 ; R1=100k and R2=20k

Vin= ADC count*0.00488*(120/20) Volt // Highlighted part is Scale factor

Note: This leads us to believe that a reading of 1023 corresponds to an input voltage of exactly 5.0 volts. In practical you may not get 5V always from the Arduino pin 5V. So during calibration first measure the voltage between the 5v and GND pins of Arduino by using a multimeter, and use scale factor by using the below formula:

Scale factor = measured voltage/1024


arduino_raw_read = 645.00
arduino_volts_read = 3.15
battery_volts = 6.30

arduino_raw_read = 642.00
arduino_volts_read = 3.14
battery_volts = 6.28

arduino_raw_read = 635.00
arduino_volts_read = 3.10
battery_volts = 6.21



V = IR
5 = I * 20k ohms
I = 0.00025





*/
const float ARDUINO_VOLTAGE = 5.0;
const int VOLT_SENSOR_PIN = A0;
const float R1 = 10.0; // Resistor #1 value in k ohms (10k)
const float R2 = 10.0; // Resistor #2 value in k ohms (10k)


float battery_volts = 0.0; // the voltage we want to read
float arduino_raw_read = 0.0; // read from analogpin using analogRead
float arduino_volts_read = 0.0;
float analogpin_increments = 0.0; // measured voltage/1024... this is how analog pins work

void setup ()
{
  Serial.begin (9600);
}
void loop ()
{
  arduino_raw_read = analogRead(VOLT_SENSOR_PIN);

	analogpin_increments = ARDUINO_VOLTAGE/1023.0;
	arduino_volts_read = arduino_raw_read*analogpin_increments;
	battery_volts = arduino_volts_read*((R1+R2)/R2);

  Serial.print("arduino_raw_read = ");
  Serial.println(arduino_raw_read);
  Serial.print("arduino_volts_read = ");
  Serial.println(arduino_volts_read);
  Serial.print("battery_volts = ");
  Serial.println(battery_volts);
  delay(1500);
}
