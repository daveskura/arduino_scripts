/*arduino:avr:nano:cpu=atmega328
  Dave Skura, 2022

*/
void setup() {
	Serial.begin(9600);           // Set comm speed for serial plotter window
}

void loop() {
	Serial.print("hello world");
	delay(50000); // 50 seconds
}