/*arduino:avr:uno
  Dave Skura, 2022

	arduino:avr:nano:cpu=atmega328
	arduino:avr:nano:cpu=atmega328old
	arduino:avr:uno
	arduino:avr:mega

	https://www.best-microcontroller-projects.com/arduino-battery-charger.html


	AA and AAA charger
	Could use for others with different charge currents
	i.e. different emitter resistors.
*/

#include <SPI.h>

#define START_PIN 12
#define RLED_PIN A4
#define GLED_PIN A5

#define VBATT_PIN A0
#define CHRG_PIN 2
#define HIGH_PIN 3
#define SPI_SS_PIN 10

#define CHARGE_ON  pinMode(CHRG_PIN,OUTPUT);digitalWrite(CHRG_PIN,LOW);
#define CHARGE_OFF pinMode(CHRG_PIN,INPUT);
#define HIGH_ON    pinMode(HIGH_PIN,OUTPUT);digitalWrite(HIGH_PIN,LOW);
#define HIGH_OFF   pinMode(HIGH_PIN,INPUT);
#define RLED_ON    digitalWrite(RLED_PIN,HIGH);
#define RLED_OFF   digitalWrite(RLED_PIN,LOW);
#define GLED_ON    digitalWrite(GLED_PIN,HIGH);
#define GLED_OFF   digitalWrite(GLED_PIN,LOW);
#define SPC        Serial.print(' ');

#define VBATMAX 1.60 // Maximum and minimum acceptable battery.
#define VBATMIN 0.75 // voltage to allow charging to start.
#define VREFANA 4.92 // Reference voltage. Measure this with DMM.
#define CHRG_ms 30000 // Time period of normal charging time (ms).
#define HIGH_ms 30000 // Time period of normal higher power charging time (ms).

#define PREV_ADC 10  // Number of nearly same ADC readings to declare done.

typedef enum { IDLE, BATFND, CHARGE, HIGH_CHARGE, TEST,
               FINISHED, FAIL, WAIT_START } state_t;

enum { BATTFULL, BATTAVGSTABLE, BATTBAD, BATTNORISE };

///////////////////////////////////////////////////
void setup() {
   Serial.begin(9600);
   Serial.println("Battery Charger Test");

   pinMode(VBATT_PIN,INPUT);
   pinMode(START_PIN,INPUT_PULLUP);

   pinMode(CHRG_PIN,OUTPUT);
   pinMode(HIGH_PIN,OUTPUT);
   pinMode(GLED_PIN,OUTPUT);
   pinMode(RLED_PIN,OUTPUT);

   CHARGE_ON
   HIGH_OFF
   RLED_OFF
   GLED_OFF
   showStartVolts();
}

///////////////////////////////////////////////////
// Print start volts to LCD & serial(for reference).
void showStartVolts(void) {

  float v = getVana( analogRead(VBATT_PIN) ) ;

  //lcd.setCursor(11, 0);
  //lcd.print( v,3 );
  Serial.print("\nStart volts: ");
  Serial.println( v,3 );

}
///////////////////////////////////////////////////
float getVana(uint16_t adc) {
    return adc * VREFANA / 1024;
}

void loop() {
}