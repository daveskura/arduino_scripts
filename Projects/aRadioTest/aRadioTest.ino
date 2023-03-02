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
const byte address[6] = "00001";
int RF_msg_len = 50;
String RF_Mode = ""; // TX for transmit or RX for receive

void setup() {
	Serial.begin(9600);           // Set comm speed for serial plotter window

  radio.begin();
  radio.openReadingPipe(0, address);
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
	set_TX_mode();

}

void loop() {
	String M = "Test RF";
	char text[RF_msg_len] = "";
	M.toCharArray(text,RF_msg_len);
	bool ok = radio.write(&text,RF_msg_len);
	delay(5000);
}

void set_TX_mode(){
  if (RF_Mode != "TX") {
    RF_Mode = "TX";
    radio.stopListening(); // put radio in TX mode
  }
}
void set_RX_mode(){
  if (RF_Mode != "RX") {
    RF_Mode = "RX";
    radio.startListening(); // put radio in RX mode
  }
}