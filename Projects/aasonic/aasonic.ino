/*arduino:avr:uno
  Dave Skura, 2022

these boards work and are listed using C:\arduino\Projects\arduino-cli board listall

	arduino:avr:nano:cpu=atmega328
	arduino:avr:nano:cpu=atmega328old
	arduino:avr:uno
	arduino:avr:mega

*/

void setup() {
		Serial.begin(9600);
    pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);
		Serial.println("Hello");
}

