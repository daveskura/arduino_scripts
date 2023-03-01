/*
 * 
 	(NRF24L01) Wireless Module Transceiver 
	CE		->	9
	CSN		->	10
	MOSI	->	11
	SCK		->	13
	IRQ		->	Not Used	
	MISC	->	12


  | <blank> | 11  | 10 |  +3vdc |
  |---------|-----|----|---------
  |   12    | 13  | 9 |  GND   |


  | <blank> | MOSI  | CSN | +3vdc |
  |---------|-------|-----|--------
  |   MISC  | SCK   | CE  | GND   |

 */

#define NRF24L01_CE 9
#define NRF24L01_CSN 10

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

const RFMsg_AskDummyRemote = "Ask about Dummy Remote";
String RF_Mode = "RX"; // TX for transmit or RX for receive

RF24 radio(NRF24L01_CE, NRF24L01_CSN); // CE, CSN

const byte address[6] = "00001"; // radio channel
int RF_msg_len = 50;

void set_TX_mode(){
  if (RF_Mode != "TX") {
    RF_Mode = "TX";
    radio.stopListening(); // put radio in RX mode
  }
}
void set_RX_mode(){
  if (RF_Mode != "RX") {
    RF_Mode = "RX";
    radio.startListening(); // put radio in RX mode
  }
}
void setup() {
  Serial.begin(9600);
  Serial.println("Start ...");
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  set_RX_mode();
}

void loop() {
  // put your main code here, to run repeatedly:
  if (radio.available()) {
    char RF_msg_array[RF_msg_len] = "";
    radio.read(&RF_msg_array, RF_msg_len);
    String RF_MSG = "Received: ";
		RF_MSG.concat(RF_msg_array); 
    Serial.println(RF_MSG);
		if (RF_msg_array == "RFMsg_AskDummyRemote")	{
			//RF_Send_Info();
	    Serial.println("RF_msg_array == 'RFMsg_AskDummyRemote'");
		} else {
	    Serial.println("RF_msg_array != 'RFMsg_AskDummyRemote'");
		}
    delay(2000);
  }
}
