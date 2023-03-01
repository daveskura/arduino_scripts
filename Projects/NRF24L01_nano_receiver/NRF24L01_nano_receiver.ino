/*
 * 
 (NRF24L01) Wireless Module Transceiver 
 */

#define NRF24L01_CE 7
#define NRF24L01_CSN 8

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
    Serial.println("radio.available()");
    char RF_msg_array[RF_msg_len] = "";
    radio.read(&RF_msg_array, RF_msg_len);
    String RF_MSG = RF_msg_array; 
    Serial.println(RF_MSG);
    delay(2000);
  }
}
