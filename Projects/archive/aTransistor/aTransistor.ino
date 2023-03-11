/*arduino:avr:nano:cpu=atmega328
  Dave Skura, 2022

*/
#define RELAY_PIN 2

void setup() {
	Serial.begin(9600);           // Set comm speed for serial plotter window
	pinMode(RELAY_PIN, OUTPUT);
}

void loop() {
	
	Serial.println("10"); // https://www.electricaltechnology.org/2020/04/pnp-transistor.html
	analogWrite(RELAY_PIN,10);
	//digitalWrite(RELAY_PIN,LOW);
	delay(10000); // 10 seconds

	Serial.println("25");
	analogWrite(RELAY_PIN,25);
	//digitalWrite(RELAY_PIN,HIGH);
	delay(10000); // 10 seconds

	Serial.println("100\n");
	analogWrite(RELAY_PIN,100);
	delay(10000); // 10 seconds
}