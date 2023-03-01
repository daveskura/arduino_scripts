/*
 * Chicken Car Remote Control (RF), version 7.5
 * By Dave Skura, Aug 15,2021 
 
  ** The Chicken car also has the main component.  Covered here is only the chicken car remote itself.
  The main component is in another file.
  
  Description:
		The chicken car remote uses RF to control the movement of the chicken car.

  Power System: 
	  9v battery
 
  Equipment List: 
 
		Arduino Uno R3
			https://www.amazon.ca/gp/product/B01EWOE0UU/ref=ppx_od_dt_b_asin_title_s00?ie=UTF8&psc=1
	 
		(NRF24L01) Wireless Module Transceiver 
			https://www.amazon.ca/gp/product/B07XD4YW73/ref=ppx_yo_dt_b_asin_title_o04_s00?ie=UTF8&psc=1
	 
		analogue joystick
			https://www.amazon.ca/Dual-axis-Joystick-Module-2-Axis-arduino/dp/B07Z44NXRN/ref=sr_1_2?dchild=1&keywords=2pcs%2Flot+Dual-axis+Mini+xy+Joystick+Module+PSP+2-Axis+Analog+3V-5V+Thumb+Joystick+Module+for+arduino&qid=1627417134&sr=8-2

  Connectivity Specifications:

		Arduino Uno
			PIN	D0																					** Not Used **
			PIN	D1																					** Not Used **
			PIN	D2	<--> buzzer pin													** Not Used **
			PIN	D3																					** Not Used **
			PIN	D4																					** Not Used **
			PIN	D5																					** Not Used **
			PIN	D6																					** Not Used **
			PIN	D7	<--> NRF24L01 - CE
			PIN	D8	<--> NRF24L01 - CSN
			PIN	D9																					** Not Used **	
			PIN	D10																					** Not Used **
			PIN	D11	<--> NRF24L01 - MOSI
			PIN	D12	<--> NRF24L01 - MISC
			PIN	D13	<--> NRF24L01 - SCK

			PIN	D14	A0	Joystick Y Axis <--> Joytick VRy
			PIN D15	A1	Joystick X Axis	<--> Joytick VRx
			PIN D16	A2																			** Not Used **
			PIN D17	A3																			** Not Used **																				
			PIN D18	A4																			** Not Used **
			PIN D19	A5																			** Not Used **


		analogue joystick 

		VRx  -> A1 (Analogue input required for speed control)
		VRy  -> A0 (Analogue input required for speed control)
		SW   -> not used  *Used when joystick is clicked

		(NRF24L01) Wireless Module Transceiver 
		CE		->	7
		CSN		->	8
		MOSI	->	11
		SCK		->	13
		IRQ		->	Not Used	
		MISC	->	12

 */



#include "pitches.h"
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <EEPROM.h>

String Version = "1.1109";
String szDevice_Name = "Chicken Remote";
String szStatus = "All is well";

String RFMsg_AskChickenRemote = "Ask about Chicken Remote";
String RF_Mode = ""; // TX for transmit or RX for receive

const int NUMBER_OF_ELEMENTS = 5;

String info[NUMBER_OF_ELEMENTS] = { 
 { "BEGIN 1/5" }, 
 { "2/5: device_name=" }, 
 { "3/5: Version=" }, 
 { "4/5: Status=" }, 
 { "END 5/5" }
 };


RF24 radio(7, 8); // CE, CSN

const byte address[6] = "00001";
int RF_msg_len = 50;

#define BUZZER_PIN 2
int melody[] = { NOTE_C5, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_G5, NOTE_A5, NOTE_B5, NOTE_C6};

#define joyY A0		//  0 = min value, center about 520, 1023 = max value
#define joyX A1		//  0 = min value, center about 520, 1023 = max value

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
int Send_Info_Mode = 0; // 0 = Normal, 1 = Send Info
String Previous_RF_Msg = "";
int irepeat_send = 0;
int imax_repeat_send = 5;

void setup() {
  char* string = "Hello";

  Serial.begin(9600);

  radio.begin();
  radio.openReadingPipe(0, address);
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
	
	buzz_on();
}

void RF_Send_Info(){
  String currentinfo[NUMBER_OF_ELEMENTS];

  for (int i=0;i<NUMBER_OF_ELEMENTS;i++ ){
    currentinfo[i] = info[i];
  }

  currentinfo[1].concat(szDevice_Name);
  currentinfo[2].concat(Version);
  currentinfo[3].concat(szStatus);
  
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
  int xSpeed, ySpeed;
  int xValue, yValue; 
  String RF_msg = "Remote RF ";

  if (Send_Info_Mode == 1) {
    Serial.println("RF_Send_Info()");
    set_TX_mode();
    RF_Send_Info();
    Send_Info_Mode = 0;
    set_RX_mode();
  } else {
    if (radio.available()) {
      Serial.println("radio.available()");
      char RF_msg_array[RF_msg_len] = "";
      radio.read(&RF_msg_array, RF_msg_len);
      String RF_MSG = "Received: ";
      RF_MSG.concat(RF_msg_array); 
      String szMsg = RF_msg_array;
      Serial.println(RF_MSG);
      if (szMsg == RFMsg_AskChickenRemote) {
        Send_Info_Mode = 1;
        set_TX_mode();
        delay(3000);
      }
    }

    // Joystick inputs
    xValue = analogRead(joyX);
    xSpeed = calc_speed(xValue); // 0 = stopped, 255 = max speed
  
    yValue = analogRead(joyY);
    ySpeed = calc_speed(yValue); // 0 = stopped, 255 = max speed
  
    
    if (xValue > 540) {
      RF_msg +=  "XL:" + String(xSpeed, DEC); 
    } else {
      if (xValue < 500) {
        RF_msg += "XR:" + String(xSpeed, DEC);
      } else {
        RF_msg += "XS:0";
      }
    }
    RF_msg += " and ";
    
    if (yValue > 540) {
      RF_msg += "YR:" + String(ySpeed, DEC);
    } else {
      if (yValue < 500) {
        RF_msg += "YF:" + String(ySpeed, DEC);
      } else {
        RF_msg += "YS:0";
      }
    }
    RF_msg += "                                                                                                    ";
    char text[RF_msg_len];
    RF_msg.toCharArray(text, RF_msg_len);
  
    if (Previous_RF_Msg == RF_msg) {
      if (irepeat_send <= (imax_repeat_send+2)) {
        irepeat_send += 1;    
      }
      
    } else {
  
      Previous_RF_Msg = RF_msg;
      irepeat_send = 0;
    }
  
    if (irepeat_send < (imax_repeat_send)) {
      set_TX_mode();
      radio.write(&text, sizeof(text));
    } else {
      set_RX_mode();
    }
  }
}

int calc_speed(int parm) {
  float x,y;
  if (parm > 510 && parm < 530) {
    x = 0;
  } else if (parm >= 530) {
    x = parm - 493;
    y = (x/493)*255;
  } else {
    x = 510 - parm;
    y = (x/510)*255;
  }
  
  if (parm == 1023) {
    y = 255;
  }
  return y;
}

void buzz_on(void) {
  for (int i=0; i<7; i++) {
    tone(BUZZER_PIN, melody[i], 25);
    delay(25);
  }
  tone(BUZZER_PIN, melody[7], 100);
  delay(100);

}



void erase(void)
{
  for (int i = 0 ; i < EEPROM.length() ; i++)
  EEPROM.write(i, 0);
}

void printMessage(byte* first, size_t len)
{
  for (int i = 0; i < len; i++)
  {
  Serial.print((char)first[i]);
  }
}

void writeMsg(byte* first, size_t len)
{
  for(int i = 0; i < len; i++)
  {
  EEPROM.write(i, first[i]);
  }
}

void readMsg(size_t len)
{
  byte res;
 
  Serial.print("Message: ");
  for(int i = 0; i < len; i++)
  {
  res = EEPROM.read(i);
  Serial.print((char)res);
  }
  Serial.println("");
}
