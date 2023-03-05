/*arduino:avr:mega
  Dave Skura, 2022

these boards work and are listed using C:\arduino\Projects\arduino-cli board listall

	arduino:avr:nano:cpu=atmega328old
	arduino:avr:uno
	arduino:avr:mega

*/
#include "pitches.h"
#define BUZZER_PIN 45
int melody[] = { NOTE_C5, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_G5, NOTE_A5, NOTE_B5, NOTE_C6};

void setup() {
  for (int i=0; i<7; i++) {
    tone(BUZZER_PIN, melody[i], 25);
    delay(25);
  }
  tone(BUZZER_PIN, melody[7], 100);
  delay(100);

}

void loop() {
}
