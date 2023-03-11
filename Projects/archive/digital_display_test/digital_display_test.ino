/*arduino:avr:nano:cpu=atmega328old
  
	Nano using ATmega328P (Old Bootloader)

	-- Dave Skura, 2022

The 4 digit, 7 segment display (5641AS) has 12 pins.  6 on top, 6 on the bottom.
The 4 digit pins require a resistor so they don't burnout the LED


D1,D2,D3,D4
A,B,C,D,E,F,G,DP (decimal point)


TOP
D1	A		F		D2	D3	B
E		D		DP	C		G		D4
BOTTOM



Arduino Uno pins -> display pins
Pin 2	-> B
Pin 3 -> C
Pin 4 -> DP
Pin 5 -> D
Pin 6 -> E
Pin 7 -> G
Pin 8 -> F
Pin 9 -> A
Pin 10 -> D1
Pin 11 -> D2
Pin 12 -> D3
Pin 13 -> D4

*/

#include "SevSeg.h"
SevSeg sevseg; 
int idisplay_nbr = 2999;
long last_time = 0;
void setup(){

  byte numDigits = 4;
  byte digitPins[] = {10, 11, 12, 13};
  byte segmentPins[] = {9, 2, 3, 5, 6, 8, 7, 4};

  bool resistorsOnSegments = true; 
  bool updateWithDelaysIn = true;
  byte hardwareConfig = COMMON_CATHODE; 
  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments);
  sevseg.setBrightness(90);
	last_time = millis();
}

void loop(){
	long current_time = millis();
	int diff = current_time - last_time;

	sevseg.setNumber(idisplay_nbr, 0);
	sevseg.refreshDisplay(); 

	if (diff > 999) {
		idisplay_nbr = idisplay_nbr - 1;
		last_time = current_time;
	}

}
