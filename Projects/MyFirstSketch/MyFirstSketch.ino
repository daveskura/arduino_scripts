//arduino:avr:uno

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
		//Try running `arduino-cli core install arduino:avr`
