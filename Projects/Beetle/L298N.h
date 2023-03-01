/*
  Dave Skura, 2022

	using 6 PWM pins plug in the following
	ENA	-> 6
	IN1	-> 13
	IN2	-> 12
	IN3	-> 11
	IN4	-> 10
	ENB	-> 7

	When hooking up the motors, the red (+) is nearest the power source

	Motor1 (OUT1,OUT2)
	Motor2 (OUT4,OUT3)

	OUT1,OUT4 are gnd leads
	OUT2,OUT3 are hot leads

	L298N.ReverseConnectMotor1(); // swaps fwd/bck on motor1
	L298N.ReverseConnectMotor2(); // swaps fwd/bck on motor1
	
	L298N.SwapMotorsLeftRight();// swaps left and right motors

	L298N.DriverMotors(255,255); // range 0..255 for motor1 and motor2

	L298N.stopmotor(); // stops both

	****** REMEMBER TO SET DESIRED_MAX_SPEED *********

*/
#include "Arduino.h" 
class L298N {
public:
	String Version = "20220623";
	int MAX_SPEED = 255;
	int DESIRED_MAX_SPEED = 255; // ****** SET THIS to correspond to MAX value 255.  used in adjustment calc.

	
	int EXT_6v_PWR_2_1v = 10;
	int EXT_6v_PWR_2_3v = 20;
	int EXT_6v_PWR_2_5v = 30;
	int EXT_6v_PWR_2_8v = 40;
	int EXT_6v_PWR_4v = 100;

	int NO_EXT_PWR_2v = 40;
	int NO_EXT_PWR_3v = 115;
	int NO_EXT_PWR_3_8v = 215;

	int NO_SPEED = 0;
	int MIN_SPEED = 1;

	int EXT_MAX_VOLT = 6;
	int DESIRED_MAX_VOLT = 4;

	L298N(int MOTOR1_SPEED, int MOTOR2_SPEED, int in1, int in2, int in3, int in4);
	void ReverseConnectMotor1();
	void ReverseConnectMotor2();
	void SwapMotorsLeftRight();

	void fwd(int speed);// expects range 0..255
	void bck(int speed);// expects range 0..255

	void stopmotor();

	int getAdjustedSpeed(int speed);// expects range 0..255
	void DriverMotors(int iMotor1_speed, int iMotor2_speed); // expects range 0..255
	void MoveForwardRight(int fwdspeed, int turnspeed);
	void MoveForwardLeft(int fwdspeed, int turnspeed);
	void MoveReverseRight(int reversespeed, int turnspeed);
	void MoveReverseLeft(int reversespeed, int turnspeed);


private:
	int return_gtr(int a, int b);
	void setmotor1_fwd();
	void setmotor2_fwd();
	void setmotor1_bck();
	void setmotor2_bck();

	int MOTOR1_SPEED;
	int MOTOR2_SPEED;
	int MOTOR1_PIN1;
	int MOTOR1_PIN2;
	int MOTOR2_PIN1;
	int MOTOR2_PIN2;
	int RVRSE_MTR1 = 0;
	int RVRSE_MTR2 = 0;
	int SWP_MTRS = 0;

};

int L298N::getAdjustedSpeed(int ispeed) {
	return ispeed*DESIRED_MAX_SPEED/MAX_SPEED;
}

void L298N::ReverseConnectMotor1() {
	RVRSE_MTR1 = 1;
}

void L298N::ReverseConnectMotor2() {
	RVRSE_MTR2 = 1;
}

void L298N::SwapMotorsLeftRight() {
	SWP_MTRS = 1;
}


L298N::L298N(int mtr1_spd, int mtr2_spd, int in1, int in2, int in3, int in4) {
	MOTOR1_SPEED = mtr1_spd;
	MOTOR2_SPEED = mtr2_spd;
	MOTOR1_PIN1 = in1;
	MOTOR1_PIN2 = in2;
	MOTOR2_PIN1 = in3;
	MOTOR2_PIN2 = in4;

	pinMode(MOTOR1_PIN1,OUTPUT);
	pinMode(MOTOR1_PIN2,OUTPUT);
	pinMode(MOTOR2_PIN1,OUTPUT);
	pinMode(MOTOR2_PIN2,OUTPUT);
}

void L298N::setmotor1_fwd(){
	if (RVRSE_MTR1 == 0){
		digitalWrite(MOTOR1_PIN1,LOW);
		digitalWrite(MOTOR1_PIN2,HIGH);
	} else {
		digitalWrite(MOTOR1_PIN1,HIGH);
		digitalWrite(MOTOR1_PIN2,LOW);
	}
}

void L298N::setmotor1_bck(){
	if (RVRSE_MTR1 == 0){
		digitalWrite(MOTOR1_PIN1,HIGH);
		digitalWrite(MOTOR1_PIN2,LOW);
	} else {
		digitalWrite(MOTOR1_PIN1,LOW);
		digitalWrite(MOTOR1_PIN2,HIGH);
	}
}

void L298N::setmotor2_fwd(){
	if (RVRSE_MTR2 == 0){
		digitalWrite(MOTOR2_PIN1,HIGH);
		digitalWrite(MOTOR2_PIN2,LOW);
	} else {
		digitalWrite(MOTOR2_PIN1,LOW);
		digitalWrite(MOTOR2_PIN2,HIGH);
	}
}

void L298N::setmotor2_bck(){
	if (RVRSE_MTR2 == 0){
		digitalWrite(MOTOR2_PIN1,LOW);
		digitalWrite(MOTOR2_PIN2,HIGH);
	} else {
		digitalWrite(MOTOR2_PIN1,HIGH);
		digitalWrite(MOTOR2_PIN2,LOW);
	}
}


void L298N::fwd(int speed){
	setmotor1_fwd();
	setmotor2_fwd();
	
	analogWrite(MOTOR1_SPEED, speed);
	analogWrite(MOTOR2_SPEED, speed);
}  

void L298N::bck(int speed){
	setmotor1_bck();
	setmotor2_bck();
	
	analogWrite(MOTOR1_SPEED, speed);
	analogWrite(MOTOR2_SPEED, speed);
}  

void L298N::stopmotor(){
	digitalWrite(MOTOR1_PIN1,LOW);
	digitalWrite(MOTOR1_PIN2,LOW);
	digitalWrite(MOTOR2_PIN1,LOW);
	digitalWrite(MOTOR2_PIN2,LOW);
	analogWrite(MOTOR1_SPEED, 0);
	analogWrite(MOTOR2_SPEED, 0);

}

void L298N::DriverMotors(int p1, int p2) {
	int iMotor1_speed, iMotor2_speed;
	int m1,m2;
	if (SWP_MTRS == 0)	{
		iMotor1_speed = p1;
		iMotor2_speed = p2;
	} else {
		iMotor1_speed = p2;
		iMotor2_speed = p1;
	}

	if (iMotor1_speed > 0) {
		setmotor1_fwd();
	} else {
		setmotor1_bck();
	}

	if (iMotor2_speed > 0) {
		setmotor2_fwd();
	} else {
		setmotor2_bck();
	}
	m1 = getAdjustedSpeed(abs(iMotor1_speed));
	m2 = getAdjustedSpeed(abs(iMotor2_speed));

	analogWrite(MOTOR1_SPEED,m1);// Left Wheels.  possible range 0 ~ DESIRED_MAX_SPEED
	analogWrite(MOTOR2_SPEED,m2);// Right Wheels.  possible range 0 ~ DESIRED_MAX_SPEED
	
	Serial.print("(");
	Serial.print(iMotor1_speed);
	Serial.print(",");
	Serial.print(iMotor2_speed);
	Serial.println(")");

}



void L298N::MoveForwardRight(int fwdspeed, int turnspeed) {
	
	int LeftWheelSpeed = 0, RightWheelSpeed = 0;
	int SpeedRemainder = 0;
	float TurnRatio = 0.0;

	LeftWheelSpeed = return_gtr(turnspeed,fwdspeed);
	if (turnspeed <= 128) { // forward speed and turn speed is less than 128
		TurnRatio = ((128.0 - turnspeed)/128.0);
		RightWheelSpeed = TurnRatio * fwdspeed;
	} else {
		RightWheelSpeed = 0;
	}

	DriverMotors(LeftWheelSpeed,RightWheelSpeed);
};

void L298N::MoveForwardLeft(int fwdspeed, int turnspeed) {
	
	int LeftWheelSpeed = 0, RightWheelSpeed = 0;
	int SpeedRemainder = 0;
	float TurnRatio = 0.0;
	
	RightWheelSpeed = return_gtr(turnspeed,fwdspeed);
	if (turnspeed <= 128) { // forward speed and turn speed is less than 128
		TurnRatio = ((128.0 - turnspeed)/128.0);
		LeftWheelSpeed = TurnRatio * fwdspeed;
	} else {
		LeftWheelSpeed = 0;
	}
	DriverMotors(LeftWheelSpeed,RightWheelSpeed);
};

// void MoveReverseRight(y,x);
void L298N::MoveReverseRight(int reversespeed, int turnspeed) {
	
	int LeftWheelSpeed = 0, RightWheelSpeed = 0;
	int SpeedRemainder = 0;
	float TurnRatio = 0.0;

	LeftWheelSpeed = return_gtr(turnspeed,reversespeed) * -1;
	if (turnspeed <= 128) { // forward speed and turn speed is less than 128
		TurnRatio = ((128.0 - turnspeed)/128.0);
		RightWheelSpeed = TurnRatio * reversespeed * -1;
	} else {
		RightWheelSpeed = 0;
	}

	DriverMotors(LeftWheelSpeed,RightWheelSpeed);
};

// void MoveReverseLeft(y,x);
void L298N::MoveReverseLeft(int reversespeed, int turnspeed) {
	
	int LeftWheelSpeed = 0, RightWheelSpeed = 0;
	int SpeedRemainder = 0;
	float TurnRatio = 0.0;
	
	RightWheelSpeed = return_gtr(turnspeed,reversespeed) * -1;
	if (turnspeed <= 128) { // forward speed and turn speed is less than 128
		TurnRatio = ((128.0 - turnspeed)/128.0);
		LeftWheelSpeed = TurnRatio * reversespeed * -1;
	} else {
		LeftWheelSpeed = 0;
	}
	DriverMotors(LeftWheelSpeed,RightWheelSpeed);
};




int L298N::return_gtr(int a, int b) {
	if (a > b) {
		return a;
	} else {
		return b;
	}

}
