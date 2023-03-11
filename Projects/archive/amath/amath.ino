/*arduino:avr:uno
  Dave Skura, 2022

*/
#include "rotate_coords.h"

void setup() {
	Serial.begin(9600);
	Serial.println("(4,7)");

	Serial.println("rotate(45):");
	rotate_coords mathit(4,7);
	mathit.rotate(45);
	Serial.print(mathit.new_x);
	Serial.print(",");
	Serial.print(mathit.new_y);

	Serial.println("rotate(90):");
	mathit.rotate(90);
	Serial.print(mathit.new_x);
	Serial.print(",");
	Serial.print(mathit.new_y);

	Serial.println("rotate(180):");
	mathit.rotate(180);
	Serial.print(mathit.new_x);
	Serial.print(",");
	Serial.print(mathit.new_y);

}



void loop() {
}

