/*arduino:avr:uno
  Dave Skura, 2022

	// LED_BUILTIN is PIN 13
*/

void setup() {
	Serial.begin(9600);
	pinMode(LED_BUILTIN, OUTPUT);
	digitalWrite(LED_BUILTIN, LOW);

}

void loop() {
	//digitalWrite(LED_BUILTIN, HIGH);
	//delay(1000);
	//digitalWrite(LED_BUILTIN, LOW);
	delay(1000);
}

