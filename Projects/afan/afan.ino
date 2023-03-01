/*arduino:avr:nano:cpu=atmega328
  Dave Skura, 2022

these boards work and are listed using C:\arduino\Projects\arduino-cli board listall

	arduino:avr:nano:cpu=atmega328
	arduino:avr:nano:cpu=atmega328old
	arduino:avr:uno
	arduino:avr:mega

*/

#define MY_PIN 5

void setup() {
		Serial.begin(9600);
    pinMode(MY_PIN, OUTPUT);
}

void loop() {
    digitalWrite(MY_PIN, HIGH);
    delay(5000);
    digitalWrite(MY_PIN, LOW);
    delay(5000);
		Serial.println("Hello");
}

