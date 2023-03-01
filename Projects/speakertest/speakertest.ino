/*arduino:avr:mega

	DFPlayer 
		https://wiki.dfrobot.com/DFPlayer_Mini_SKU_DFR0299


	Arduino Uno
 		PIN	D10	< --> DFPlayer TX - Yellow		
 		PIN	D11	< --> DFPlayer RX - Blue

	(DFPlayer) MP3 Player
		* DFPlayer Right pins not connected at all.
		* top of DFPlayer has writing

		DFPlayer +5VCC	top Left Pin 1  
		DFPlayer RX			top Left Pin 2 -> Arduino PIN 11 - blue  
		DFPlayer TX			top Left Pin 3 -> Arduino PIN 10 - yellow

		DFPlayer SPKR+	Bot Left Pin 6  
		DFPlayer GND		Bot Left Pin 7 
		DFPlayer SPKR-	Bot Left Pin 8  
*/

#define DFPLAYER_VOICE_VOLUME 5 //Set volume value. From 0 to 30

#include "DFRobotDFPlayerMini.h"
#include <SoftwareSerial.h>

DFRobotDFPlayerMini myDFPlayer;

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);

 if (!myDFPlayer.begin(Serial1)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true);
  }
    myDFPlayer.volume(DFPLAYER_VOICE_VOLUME);  

	// Power_on_voice_notify(myDFPlayer);
    //myDFPlayer.playFolder(1,1);
		myDFPlayer.stop();

    delay(5000);

}

void loop() {
  if (digitalRead(BUTTON_1) == LOW) {
    myDFPlayer.stop();
    //myDFPlayer.playFolder(2,1);
    delay(500);
	}
  if (digitalRead(BUTTON_2) == LOW) {
		myDFPlayer.stop();
    myDFPlayer.playFolder(1,1);
    delay(500);
	}
}
