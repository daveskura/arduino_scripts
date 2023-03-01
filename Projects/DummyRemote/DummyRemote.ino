/*
 * 
   (NRF24L01) Wireless Module Transceiver 
  CE    ->  9
  CSN   ->  10
  MOSI  ->  11
  SCK   ->  13
  IRQ   ->  Not Used  
  MISC  ->  12


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

String Version = "1.1108";
String device_name = "Dummy Remote";

String RFMsg_AskDummyRemote = "Ask about Dummy Remote";
String RF_Mode = ""; // TX for transmit or RX for receive

const int NUMBER_OF_ELEMENTS = 5;
const int MAX_SIZE = 12;

String info[NUMBER_OF_ELEMENTS] = { 
 { "BEGIN 1/5" }, 
 { "2/5: device_name=" }, 
 { "3/5: Version=" }, 
 { "4/5: Status=" }, 
 { "END 5/5" }
 };


RF24 radio(NRF24L01_CE, NRF24L01_CSN); // CE, CSN

const byte address[6] = "00001"; // radio channel
int RF_msg_len = 50;

String szInfo = "All is well";

void set_TX_mode(){
  if (RF_Mode != "TX") {
    RF_Mode = "TX";
    radio.stopListening(); // put radio in TX mode
    Serial.println("radio.stopListening(); // put radio in TX mode");
  }
}
void set_RX_mode(){
  if (RF_Mode != "RX") {
    RF_Mode = "RX";
    radio.startListening(); // put radio in RX mode
    Serial.println("radio.startListening(); // put radio in RX mode");
    
  }
}
int Send_Info_Mode = 0;
void setup() {
  Serial.begin(9600);
  Serial.println("Start ...");
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  set_RX_mode();
}

void RF_Send_Info(){
	String currentinfo[NUMBER_OF_ELEMENTS];

	for (int i=0;i<NUMBER_OF_ELEMENTS;i++ ){
		currentinfo[i] = info[i];
	}

	currentinfo[1].concat(device_name);
	currentinfo[2].concat(Version);
	currentinfo[3].concat("All is well");

	
	for (int i=0;i<NUMBER_OF_ELEMENTS;i++ )
	{
    String M = currentinfo[i];
    char text[RF_msg_len] = "";
    M.toCharArray(text,RF_msg_len);
		bool ok = radio.write(&text,RF_msg_len);
   if (!ok) {
    Serial.println("Write failed");
   }
		delay(500);
	}
  
}
void loop() {

  if (Send_Info_Mode == 1) {
    Serial.println("RF_Send_Info()");
    RF_Send_Info();
    Send_Info_Mode = 0;
    set_RX_mode();
  } else {
    
    // put your main code here, to run repeatedly:
    if (radio.available()) {
      char RF_msg_array[RF_msg_len] = "";
      radio.read(&RF_msg_array, RF_msg_len);
      String RF_MSG = "Received: ";
      RF_MSG.concat(RF_msg_array); 
      String szMsg = RF_msg_array;
      Serial.println(RF_MSG);
      if (szMsg == RFMsg_AskDummyRemote) {
        Send_Info_Mode = 1;
        set_TX_mode();
        delay(3000);
      }
    }
  }
}
