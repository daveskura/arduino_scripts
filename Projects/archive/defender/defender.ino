//arduino:avr:mega

/*
https://wiki.keyestudio.com/KS0499_KEYESTUDIO_Mega_2560_Plus_Board
Serial communication interface(4 channel): Serial(D0 =RX0, D1 =TX0), Serial1(D19 is RX1, D18 is TX1)
Serial2 (D17 is RX2, D16 equals to TX2), Serial3(D15 is RX3, D14 is TX3), D0 and D1 are connected to ATMEGA16U2-MU
PWM port(Pulse width modulation): D2-D13 and D44-D46
External interrupt pins:D2(interrupt 0), D3(interrupt 1), D21(interrupt 2), D20 (interrupt 3), D19(interrupt 4)and D18(interrupt 5)
SPI communication interface:D53 stands for SS, D51 is MOSI, D50 is MISO, D52 equals to SCK
IIC communication interface:D20 represents SDA, D21 is SCL

Arduino keyestudio Mega


  PIN D40 <--> GREEN_LED
  PIN D42 <--> RED_LED
  PIN D44 <--> BUZZER_PIN
  
  (This represents Serial1 UART on the Mega)
  PIN D18 <--> DFPlayer TX
  PIN D19 <--> DFPlayer RX

  (far Side pins)
  PIN D33 <--> Orange
  PIN D35 <--> Blue
  PIN D37 <--> Yellow
  PIN D39 <--> Purple
  PIN D41 <--> Orange
  PIN D43 <--> Blue
  PIN D45 <--> Yellow
  PIN D47 <--> Purple
 

	PIN	D49	<--> NRF24L01 - CE
	PIN	D50	<--> NRF24L01 - MISO
	PIN	D51	<--> NRF24L01 - MOSI
	PIN	D52	<--> NRF24L01 - SCK
	PIN	D53	<--> NRF24L01 - CSN

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

	(DFPlayer) MP3 Player
		* DFPlayer Right pins not connected at all.
		* top of DFPlayer has writing

		DFPlayer +5VCC  top Left Pin 1  
    DFPlayer TX     top Left Pin 2 -> Arduino PIN 18 
		DFPlayer RX     top Left Pin 3 -> Arduino PIN 19 
		DFPlayer SPKR+  Bot Left Pin 6  
		DFPlayer GND    Bot Left Pin 7 
		DFPlayer SPKR-  Bot Left Pin 8  

*/
#include <RF24.h>
#include <nRF24L01.h>
#include "DFRobotDFPlayerMini.h"
#include <SoftwareSerial.h>

#include "pitches.h"
#define DFPLAYER_VOICE_VOLUME 5 //Set volume value. From 0 to 30

#define NRF24L01_CE 49
#define NRF24L01_MISO 50
#define NRF24L01_MOSI 51 
#define NRF24L01_SCK 52
#define NRF24L01_CSN 53

RF24 radio(NRF24L01_CE, NRF24L01_CSN); // CE, CSN

const byte address[6] = "00001"; // radio channel
int RF_msg_len = 50;
String RF_Mode = ""; // TX for transmit or RX for receive

DFRobotDFPlayerMini myDFPlayer;

#define GREEN_LED 40
#define RED_LED 42
#define BUZZER_PIN 44
int melody[] = { NOTE_C5, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_G5, NOTE_A5, NOTE_B5, NOTE_C6};

#define TEST_PIN 39

String Device_Name = "Defender";
String Version = "1.20220420.2";

void setup() {
  Serial.begin(9600); 
	Serial1.begin(9600);

	if (!myDFPlayer.begin(Serial1)) {  //
		Serial.println(F("Unable to begin:"));
		Serial.println(F("1.Please recheck the connection!"));
		Serial.println(F("2.Please insert the SD card!"));
		while(true);
	}
  myDFPlayer.volume(DFPLAYER_VOICE_VOLUME);  
	myDFPlayer.stop();
	
	radio.begin();
  radio.openReadingPipe(0, address);
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
	set_RX_mode();

}
void loop() {
  if (radio.available()) {
    char rec_text[RF_msg_len] = "";
    radio.read(&rec_text, RF_msg_len);
		String M = String(rec_text + '\0');
		//Serial.println(M);
		RF_RequestResponse(M);
	}
}

void RF_RequestResponse(String RF_Master_Control_Request_Msg) {
    char snd_text[RF_msg_len] = "";
/*
	- -Snd Hello
	- -Snd LED Test
	- -Snd Buzzer Test
	- -Snd Mute Speaker
	- -Snd Spkr vol 5       
	- -Snd Spkr vol 10
	- -Snd Spkr vol 20
	- -Snd Spkr vol 30
	- -Snd Play Growl
	- -Snd Play Owls
	- -What Version 
	- -Download Status
*/
		if (RF_Master_Control_Request_Msg == "Hello") { // received "Hello"
			//Serial.println("received text 'Hello'");
			String Reply_msg = "Defender Says...Hello";
			Reply_msg.toCharArray(snd_text, RF_msg_len);
		} else if (RF_Master_Control_Request_Msg == "LED Test") { 
			String Reply_msg = "Defender Says...LED tested";
			Reply_msg.toCharArray(snd_text, RF_msg_len);
			test_LED();
		} else if (RF_Master_Control_Request_Msg == "Buzzer Test") { 
			String Reply_msg = "Defender Says...Buzzer tested";
			Reply_msg.toCharArray(snd_text, RF_msg_len);
			test_buzzer();
		} else if (RF_Master_Control_Request_Msg == "Mute Speaker") {
			String Reply_msg = "Defender Says...Speaker Muted";
			Reply_msg.toCharArray(snd_text, RF_msg_len);
			myDFPlayer.volume(0); 
			myDFPlayer.stop();
		} else if (RF_Master_Control_Request_Msg == "Spkr vol 5") { 
			String Reply_msg = "Defender Says...Volume set to 5";
			Reply_msg.toCharArray(snd_text, RF_msg_len);
			myDFPlayer.volume(5); 
		} else if (RF_Master_Control_Request_Msg == "Spkr vol 10") { 
			String Reply_msg = "Defender Says...Volume set to 10";
			Reply_msg.toCharArray(snd_text, RF_msg_len);
			myDFPlayer.volume(10); 
		} else if (RF_Master_Control_Request_Msg == "Spkr vol 20") { 
			String Reply_msg = "Defender Says...Volume set to 20";
			Reply_msg.toCharArray(snd_text, RF_msg_len);
			myDFPlayer.volume(20); 
		} else if (RF_Master_Control_Request_Msg == "Spkr vol 30") { 
			String Reply_msg = "Defender Says...Volume set to 30";
			Reply_msg.toCharArray(snd_text, RF_msg_len);
			myDFPlayer.volume(30); 
		} else if (RF_Master_Control_Request_Msg == "Play Growl") { 
			String Reply_msg = "Defender Says...Growling";
			Reply_msg.toCharArray(snd_text, RF_msg_len);
			play_growl();
		} else if (RF_Master_Control_Request_Msg == "Play Owls") { 
			String Reply_msg = "Defender Says...Owling";
			Reply_msg.toCharArray(snd_text, RF_msg_len);
			play_owls(); 
		} else if (RF_Master_Control_Request_Msg == "What Version") { 
			String Reply_msg = "Defender Says..." + Version;
			Reply_msg.toCharArray(snd_text, RF_msg_len);

		}

		delay(500);
		set_TX_mode();
		radio.write(&snd_text, sizeof(snd_text));
		delay(500);
		set_RX_mode();

}

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

void play_growl() {
	myDFPlayer.playFolder(1,1); 
}

void play_owls() {
	myDFPlayer.playFolder(2,1); 
}

void test_LED() {
  for (int i=0; i<7; i++) {
		digitalWrite(GREEN_LED,LOW);
		digitalWrite(RED_LED,HIGH);
    delay(500);
		digitalWrite(GREEN_LED,HIGH);
		digitalWrite(RED_LED,LOW);
    delay(500);
  }
	digitalWrite(RED_LED,LOW);
	digitalWrite(GREEN_LED,LOW);

}

void test_buzzer(){
  // put your setup code here, to run once:
  for (int i=0; i<7; i++) {
    tone(BUZZER_PIN, melody[i], 25);
    delay(100);
  }
  tone(BUZZER_PIN, melody[7], 100);
}
