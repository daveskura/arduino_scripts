/*arduino:avr:uno
  Dave Skura, 2022

	solar panel to battey needs a diode on (+) so it only goes one way 
	https://www.homemade-circuits.com/how-to-make-solar-battery-charger/

	need to shut off charging when battery is full...
	volt meter could sense this
	a transistor should be able to switch off the charger

	https://www.instructables.com/ARDUINO-PWM-SOLAR-CHARGE-CONTROLLER-V-202/#:~:text=The%20heart%20of%20the%20charge,battery%20and%20control%20the%20load.

*/

const int transistor = 2;

const float ARDUINO_VOLTAGE = 5.0;
const int VOLT_SENSOR_PIN = A0;
const float R1 = 1; // Resistor #1 value in k ohms (10k)
const float R2 = 1; // Resistor #2 value in k ohms (10k)


void setup()
{
	Serial.begin(9600);
  pinMode (transistor, OUTPUT);
	digitalWrite(transistor,LOW);
}

void loop()
{

	digitalWrite(transistor,HIGH);
	read_volts();
  delay(2000);

	digitalWrite(transistor,LOW);
	read_volts();
  delay(2000);

}

float read_volts() {
	float battery_volts = 0.0; // the voltage we want to read
	float arduino_raw_read = 0.0; // read from analogpin using analogRead
	float analogpin_increments = 0.0; // measured voltage/1024... this is how analog pins work

	arduino_raw_read = analogRead(VOLT_SENSOR_PIN);

	analogpin_increments = ARDUINO_VOLTAGE/1023.0;
	battery_volts = arduino_raw_read*analogpin_increments;

  Serial.print("battery_volts = ");
  Serial.println(battery_volts);

	return battery_volts;
}