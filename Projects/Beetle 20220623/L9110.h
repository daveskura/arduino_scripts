/*arduino:avr:uno
  Dave Skura, 2022

  Connections:
   
    Arduino digital output D8 to motor driver input B-IA.
    Arduino digital output D9 to motor driver input B-IB.
    Arduino digital output D10 to motor driver input B-IA.
    Arduino digital output D11 to motor driver input B-IB.
    Motor driver VCC to operating voltage 5V.
    Motor driver GND to common ground.
    Motor driver MOTOR B screw terminals to a small motor.

		#define L9110_A_PWM 2
		#define L9110_A_DIR 3
		//	Motor driver VCC to operating voltage 5V.
		//  Motor driver GND to common ground.
		//  Motor driver MOTOR B screw terminals to a small motor.
		L9110 myL9110(L9110_A_PWM,L9110_A_DIR);

*/

#include "Arduino.h" 

class L9110 {
public:
	String Version = "20220623";
	L9110(int motor_a_pwm, int motor_a_dir);
	L9110(int motor_a_pwm, int motor_a_dir,int motor_b_pwm, int motor_b_dir);
	void stop_A();
	void fastforward_A(); 
	void forward_A(int speed); // speed 255..0 where 0 is fastest
	void back_A(int speed);// speed 255..0 where 0 is fastest

private:
	int MOTOR_A_PWM = 0;
	int MOTOR_A_DIR = 0;
	int MOTOR_B_PWM = 0;
	int MOTOR_B_DIR = 0;

	int DIR_DELAY = 1000; // brief delay for abrupt motor changes

};

L9110::L9110(int motor_a_pwm, int motor_a_dir) {
	MOTOR_A_PWM = motor_a_pwm;
	MOTOR_A_DIR = motor_a_dir;

  pinMode( MOTOR_A_DIR, OUTPUT );
  pinMode( MOTOR_A_PWM, OUTPUT );

  digitalWrite( MOTOR_A_DIR, LOW );
  digitalWrite( MOTOR_A_PWM, LOW );

}


L9110::L9110(int motor_a_pwm, int motor_a_dir,int motor_b_pwm, int motor_b_dir) {
	MOTOR_A_PWM = motor_a_pwm;
	MOTOR_A_DIR = motor_a_dir;
	MOTOR_B_PWM = motor_b_pwm;
	MOTOR_B_DIR = motor_b_dir;

  pinMode( MOTOR_A_DIR, OUTPUT );
  pinMode( MOTOR_A_PWM, OUTPUT );
  pinMode( MOTOR_B_DIR, OUTPUT );
  pinMode( MOTOR_B_PWM, OUTPUT );

  digitalWrite( MOTOR_A_DIR, LOW );
  digitalWrite( MOTOR_A_PWM, LOW );
  digitalWrite( MOTOR_B_DIR, LOW );
  digitalWrite( MOTOR_B_PWM, LOW );

}
void L9110::stop_A(){
	digitalWrite( MOTOR_A_DIR, LOW );
	digitalWrite( MOTOR_A_PWM, LOW );
}

void L9110::fastforward_A() {
	forward_A(0);
}
void L9110::forward_A(int speed){
	Serial.println( "forward..." );
	stop_A();
	delay( DIR_DELAY );
	digitalWrite( MOTOR_A_DIR, HIGH ); // direction 
	analogWrite( MOTOR_A_PWM, speed ); // PWM speed 
}

void L9110::back_A(int speed){
	Serial.println( "back..." );
	stop_A();
	delay( DIR_DELAY );
	digitalWrite( MOTOR_A_DIR, LOW ); // direction 
	analogWrite( MOTOR_A_PWM, speed ); // PWM speed 
}
