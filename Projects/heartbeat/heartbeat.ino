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


Potentiometer connected to 5V and GND and pin A5

*/
#include "SevSeg.h"
SevSeg sevseg; 

#define PULSE_SENSOR_PIN A7   // 'S' Signal pin connected to A0
#define POTENTIOMETER_PIN A5 // Potentiometer output connected to analog pin 0
#define MAX_POTENTIOMETER_VAL 879
#define THRESHOLD_MAX 1024
float potVal = 0; // Variable to store the input from the potentiometer, [0 .. 879]

int Signal;                // Store incoming ADC data. Value can range from 0-1024
int Threshold = 650;       // Determine which Signal to "count as a beat" and which to ignore.
float ms_diff = 0.0;
long Lastbeat = 0;
long PossibleLastbeat = 0;
int iBeat_Count = 0;
float hr = 0.0;

void setup() {
	Serial.begin(9600);           // Set comm speed for serial plotter window
  byte numDigits = 4;
  byte digitPins[] = {10, 11, 12, 13};
  byte segmentPins[] = {9, 2, 3, 5, 6, 8, 7, 4};

  bool resistorsOnSegments = true; 
  bool updateWithDelaysIn = true;
  byte hardwareConfig = COMMON_CATHODE; 

  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments);
  sevseg.setBrightness(90);

	long Lastbeat = millis();

}

void loop() {
	float pot_pct = 0.0;

	Signal = analogRead(PULSE_SENSOR_PIN); // Read the sensor value
  potVal = analogRead(POTENTIOMETER_PIN);   // read the potentiometer value at the input pin
	pot_pct = (potVal*100)/MAX_POTENTIOMETER_VAL;
	Threshold = (pot_pct/100) * THRESHOLD_MAX;

	if (Signal > Threshold) {                
		PossibleLastbeat = millis();
		ms_diff = PossibleLastbeat - Lastbeat;
		if (ms_diff > 400) {
			iBeat_Count += 1;
			Lastbeat = PossibleLastbeat;
			hr = 60000.0 / ms_diff;
			ms_diff = 0;

			Serial.print(" Heart Rate ");               
			Serial.print(hr);              
			Serial.print(" bpm - Threshold ");               

			Serial.print(pot_pct);
			Serial.print(" : ");
			Serial.println(Threshold);
		}
		//Serial.println(ms_diff);              

	}

	sevseg.setNumber(hr, 0);
	sevseg.refreshDisplay(); 

}