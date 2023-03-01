/*arduino:avr:nano:cpu=atmega328
  Dave Skura, 2022

these boards work and are listed using C:\arduino\Projects\arduino-cli board listall

	arduino:avr:nano:cpu=atmega328
	arduino:avr:nano:cpu=atmega328old
	arduino:avr:uno
	arduino:avr:mega

*/

// include <SPI.h>
#include <stdlib.h>

char buff[10];

void setup() {
  Serial.begin(9600);
  pinMode(A0, INPUT);
  int in0 = analogRead(A0);

	Serial.print("getvolts(): ");
	Serial.println(getvolts(in0));
  delay(500);

}

void loop() {

  // read normal Arduino value

	exit(0);
}
float getvolts(uint16_t adc) {
  float val0 = adc * 5.0 / 1024.0;

  // read correct supply voltage
  float supply = readVcc() / 1000.0;
  float val0Corrected = supply / 5 * val0;
	return val0Corrected;
}
long readVcc() {
  long result;
  // Read 1.1V reference against AVcc
#if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
  ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
#elif defined (__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
  ADMUX = _BV(MUX5) | _BV(MUX0);
#elif defined (__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
  ADMUX = _BV(MUX3) | _BV(MUX2);
#else
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
#endif
  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Convert
  while (bit_is_set(ADCSRA, ADSC));
  result = ADCL;
  result |= ADCH << 8;
  result = 1126400L / result; // Calculate Vcc (in mV); 1126400 = 1.1*1024*1000
  return result;
}