/*
 * 
 (NRF24L01) Wireless Module Transceiver 
  CE    ->  49
  MISO  ->  50
  MOSI  ->  51
  SCK   ->  52
  CSN   ->  53


  | <blank> | 51  | 53 |  +3vdc |
  |---------|-----|----|---------
  |   50    | 52  | 49 |  GND   |


  | <blank> | MOSI  | CSN | +3vdc |
  |---------|-------|-----|--------
  |   MISO  | SCK   | CE  | GND   |

 */

#define NRF24L01_CE 49
#define NRF24L01_CSN 53

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(NRF24L01_CE, NRF24L01_CSN); // CE, CSN

const byte address[6] = "00001"; // radio channel
int RF_msg_len = 50;

void setup() {
  Serial.begin(9600);
  Serial.println("Start ...");
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
  
}

void loop() {
  // put your main code here, to run repeatedly:
  if (radio.available()) {
    char RF_msg_array[RF_msg_len] = "";
    radio.read(&RF_msg_array, RF_msg_len);
    String RF_MSG = RF_msg_array; 
    Serial.println(RF_MSG);
    delay(2000);
  }
}
