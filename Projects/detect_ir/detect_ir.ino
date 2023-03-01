/*arduino:avr:uno
  Dave Skura, 2022

these boards work and are listed using C:\arduino\Projects\arduino-cli board listall

	arduino:avr:nano:cpu=atmega328old
	arduino:avr:uno
	arduino:avr:mega

*/

#define APIN A1
int ir_val = 0;

void setup() {
		Serial.begin(9600);
}

void loop() {
	ir_val = analogRead(APIN);
	if (ir_val > 30) {
		Serial.println(ir_val);
	}
	delay(100);
}

