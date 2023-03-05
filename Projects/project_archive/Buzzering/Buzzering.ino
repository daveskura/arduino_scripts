/*
  Arduino Uno
    PIN D2  <--> buzzer pin  
*/

#include "pitches.h" // put *.h file in same dir as arduino *.ino source file
#define BUZZER_PIN 2
int melody[] = { NOTE_C5, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_G5, NOTE_A5, NOTE_B5, NOTE_C6};

void setup() {
  // put your setup code here, to run once:
  for (int i=0; i<7; i++) {
    tone(BUZZER_PIN, melody[i], 25);
    delay(25);
  }
  tone(BUZZER_PIN, melody[7], 100);
  delay(100);
}

void loop() {
  // put your main code here, to run repeatedly:

}
