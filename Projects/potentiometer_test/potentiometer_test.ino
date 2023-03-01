/*arduino:avr:nano:cpu=atmega328old
  Dave Skura, 2022
*/

// INPUT: Potentiometer should be connected to 5V and GND
#define POTENTIOMETER_PIN A5 // Potentiometer output connected to analog pin 0
#define MAX_POTENTIOMETER_VAL 879
#define THRESHOLD_MAX 1024
float potVal = 0; // Variable to store the input from the potentiometer, [0 .. 879]


void setup()
{
	Serial.begin(9600);
}

// Main program
void loop()
{
  potVal = analogRead(POTENTIOMETER_PIN);   // read the potentiometer value at the input pin
	float pot_pct = 0.0;
	int Threshold = 0;
	pot_pct = (potVal*100)/MAX_POTENTIOMETER_VAL;
	Threshold = (pot_pct/100) * THRESHOLD_MAX;
	Serial.print(pot_pct);
	Serial.print(" : ");
	Serial.println(Threshold);

	delay(2000);
}