/*arduino:avr:uno
  Dave Skura, 2022

these boards work and are listed using C:\arduino\Projects\arduino-cli board listall

	arduino:avr:nano:cpu=atmega328old
	arduino:avr:uno
	arduino:avr:mega

*/
#include <Stepper.h>

#define STEPPER_PIN1 12
#define STEPPER_PIN2 11
#define STEPPER_PIN3 10
#define STEPPER_PIN4 9

#define STEPS 2048 // the number of steps in one revolution of your motor (28BYJ-48)
	
Stepper stepper(STEPS, 12, 11, 10, 9);
	
void setup() {
// nothing to do
}
	
void loop() {
	stepper.setSpeed(15); // 1 rpm
	stepper.step(2048); // do 2038 steps -- corresponds to one revolution in one minute
	delay(5000); // wait for one second
	stepper.setSpeed(15); // 6 rpm
	stepper.step(-2048); // do 2038 steps in the other direction with faster speed -- corresponds to one revolution in 10 seconds
	delay(5000);
}