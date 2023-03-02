/*arduino:avr:nano:cpu=atmega328old

	(NRF24L01) Wireless Module Transceiver 
	CE		->	7
	CSN		->	8
	MOSI	->	11
	SCK		->	13
	IRQ		->	Not Used	
	MISC	->	12

*/

#include <RF24.h>

RF24 radio(7, 8); // CE, CSN

void setup() {
	Serial.begin(9600);           // Set comm speed for serial plotter window
}

void loop() {

}