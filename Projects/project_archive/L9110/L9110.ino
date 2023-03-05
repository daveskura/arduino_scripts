/*arduino:avr:mega
  Dave Skura, 2022
     
*/
#include "L9110.h"

// wired connections
#define L9110_A_PWM 2
#define L9110_A_DIR 3
/*
	Motor driver VCC to operating voltage 5V.
  Motor driver GND to common ground.
  Motor driver MOTOR B screw terminals to a small motor.
*/
L9110 myL9110(L9110_A_PWM,L9110_A_DIR);

void setup()
{
	//myL9110.forward_A(50);
	//myL9110.fastforward_A();
	myL9110.stop_A();
}
 
void loop()
{

}
