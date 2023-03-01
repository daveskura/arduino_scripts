/*arduino:avr:nano:cpu=atmega328old

Potentiometer connected to 5V and GND and pin A5

*/
#define POTENTIOMETER_PIN A5 // Potentiometer output connected to analog pin 0
float potVal = 0; // Variable to store the input from the potentiometer, [0 .. 879]

void setup() {
	Serial.begin(9600);           // Set comm speed for serial plotter window
}

void loop() {
  potVal = analogRead(POTENTIOMETER_PIN);   // read the potentiometer value at the input pin

	Serial.print("potVal: ");
	Serial.println(potVal);
	delay(5000);

}