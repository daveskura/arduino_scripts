/*arduino:avr:mega

https://wiki.keyestudio.com/KS0499_KEYESTUDIO_Mega_2560_Plus_Board
Serial communication interface(4 channel): Serial(D0 =RX0, D1 =TX0), Serial1(D19 is RX1, D18 is TX1)
Serial2 (D17 is RX2, D16 equals to TX2), Serial3(D15 is RX3, D14 is TX3), D0 and D1 are connected to ATMEGA16U2-MU
PWM port(Pulse width modulation): D2-D13 and D44-D46
External interrupt pins:D2(interrupt 0), D3(interrupt 1), D21(interrupt 2), D20 (interrupt 3), D19(interrupt 4)and D18(interrupt 5)
SPI communication interface:D53 stands for SS, D51 is MOSI, D50 is MISO, D52 equals to SCK
IIC communication interface:D20 represents SDA, D21 is SCL



Arduino keyestudio Mega

layout:
---------------------------------------------------------------------------------------------------------------
|				A15	A14	A13	A12	A11	A10	A9	A8		A7	A6	A5	A4	A3	A2	A1	A0		Vin	GND	GND	5v	3v3	rest	5v			|
|				o		o		o		o		o		o		o		o			o		o		o		o		o		o		o		o			o		o		o		o		o		o			o				|
| GND	o	o																																																			|
| 53	o	o  52																																																	|
| 51	o	o  50																																																	|
| 49	o	o  48																																																	|
| 47	o	o  46																																																	|
| 45	o	o  44																																																	|
| 43	o	o  42																																																	|
| 41	o	o  40																																																	|
| 39	o	o  38																																																	|
| 37	o	o  36																																																	|
| 35	o	o  34																																																	|
| 33	o	o  32																																																	|
| 31	o	o  30																																																	|
| 29	o	o  28																																																	|
| 27	o	o  26																																																	|
| 25	o	o  24																																																	|
| 23	o	o  22		21	20	19	18	17	16	15	14			0	1	2	3	4	5	6	7	8	9	10	11	12	13	GND	AREF	SDA	SCL			|
|		o	o  NA		o		o		o		o		o		o		o		o				o	o	o	o	o	o	o	o	o	o	o		o		o		o		o		o			o		o					|
---------------------------------------------------------------------------------------------------------------


	PIN	D38	<--> VMA409 IN3 - Grey
	PIN	D40	<--> VMA409 IN4 - Black
	PIN	D42	<--> VMA409 ENB - Brown
	PIN	D44	<--> VMA409 ENA - Purple
	PIN	D45	<--> Speaker
	PIN	D46	<--> VMA409 IN2 - Orange
	PIN	D47	<--> connected to white port for switch
	PIN	D48	<--> VMA409 IN1 - Blue
	PIN	D49	<--> NRF24L01 - CE
	PIN	D50	<--> NRF24L01 - MISO
	PIN	D51	<--> NRF24L01 - MOSI
	PIN	D52	<--> NRF24L01 - SCK
	PIN	D53	<--> NRF24L01 - CSN


(VMA409) Motor drive controller 
	ENA   ->  44	- Purple
	IN1   ->  48	- Blue  
	IN2   ->  46	- Orange  
	IN3   ->  38	- Grey  
	IN4   ->  40	- Black  
	ENB   ->  42	- Brown  
	GND	- Blue
	+5v	- White


(NRF24L01) Wireless Module Transceiver 
  CE    ->  49	- Yellow
  MISO  ->  50	- Brown
  MOSI  ->  51	- Blue
  SCK   ->  52	- Purple
  CSN   ->  53	- Orange
	+3vdc ->  Red
	GND		->  Green



	-- pins facing you
	-------------------------------
  | <blank> | 51  | 53 |  +3vdc |
  |---------|-----|----|---------
  |   50    | 52  | 49 |  GND   |

	---------------------------------
  | <blank> | MOSI  | CSN | +3vdc |
  |---------|-------|-----|--------
  |   MISO  | SCK   | CE  | GND   |

(Speaker)	
	PIN	45

*/
#include <RF24.h>
#include <nRF24L01.h>
#include <SoftwareSerial.h>

#include "pitches.h"
#include "L298N.h"
#include "L9110.h"


#define SPEAKER_PIN 45

#define NRF24L01_CE 49
#define NRF24L01_MISO 50
#define NRF24L01_MOSI 51 
#define NRF24L01_SCK 52
#define NRF24L01_CSN 53

RF24 radio(NRF24L01_CE, NRF24L01_CSN); // CE, CSN

const byte address[6] = "00001"; // radio channel
int RF_msg_len = 50;
String RF_Mode = ""; // TX for transmit or RX for receive

int melody[] = { NOTE_C5, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_G5, NOTE_A5, NOTE_B5, NOTE_C6};

String Device_Name = "Beetle";
String Version = "20220625";
const int transistor = 2;

int previous_turn_action = 0, current_turn_action = 0; 
int speed = 0;

#define ENA 6
#define ENB 7

#define MOTOR1_PIN1 13
#define MOTOR1_PIN2 12
#define MOTOR2_PIN1 11
#define MOTOR2_PIN2 10

L298N myL298N(ENA,ENB,MOTOR1_PIN1,MOTOR1_PIN2,MOTOR2_PIN1,MOTOR2_PIN2);

#define L9110_A_PWM 2
#define L9110_A_DIR 3
int ispinning = 0;
L9110 myL9110(L9110_A_PWM,L9110_A_DIR);

void startup_tone() {
	tone(SPEAKER_PIN, melody[0], 25);
	delay(100);
	tone(SPEAKER_PIN, melody[1], 25);
	delay(100);
	tone(SPEAKER_PIN, melody[2], 25);
	delay(100);
}
void setup() {
	//int speedpinA=5;
  Serial.begin(9600); 
	Serial1.begin(9600);

	pinMode (transistor, OUTPUT);
	digitalWrite(transistor,HIGH);// connect transistor so solar power charges while ON.

	myL298N.ReverseConnectMotor2();
	myL298N.SwapMotorsLeftRight();

	radio.begin();
	radio.openReadingPipe(0, address);
	radio.openWritingPipe(address);
	radio.setPALevel(RF24_PA_MIN);
	set_RX_mode();
	
	myL9110.stop_A();
	startup_tone();
}
void loop() {
//	DriverMotors(speed,speed);

	if (radio.available()) {
		// Serial.println("Loop:radio.available():Start");

		char RF_msg_array[RF_msg_len] = "";
		radio.read(&RF_msg_array, RF_msg_len);
		String szRF_MSG = String(RF_msg_array + '\0');
		//Serial.println(szRF_MSG);
		RF_RequestResponse(szRF_MSG);
		RF_HandleMoveCommand(szRF_MSG);

	}  // else Serial.println("no radio detected");

	
}

void RF_HandleMoveCommand(String RF_MSG) {
	int x = 0, y = 0;
  String RF_xspeed = "0"; // speed from 0 .. 255
  String RF_yspeed = "0"; // speed from 0 .. 255
  String RF_tmp = "",RF_xdir = "XS",RF_ydir = "YS"; // direction either STOP, XR,XL,YF,YR 
	int Local_xValue, Local_yValue; 

	int RF_MSG_xi = RF_MSG.indexOf(' ',RF_MSG.indexOf(' ')+1);
	int RF_MSG_xj = RF_MSG.indexOf(' ',RF_MSG_xi+1);
	String RF_msg_Actuator = RF_MSG.substring(RF_MSG_xi+1,RF_MSG_xj); 
	int RF_MSG_yi = RF_MSG.indexOf(' ',RF_MSG_xj+1);
	String RF_msg_Motor = RF_MSG.substring(RF_MSG_yi+1,RF_MSG.indexOf(' ',RF_MSG_yi+2)); 
	
	if (RF_msg_Actuator != "XS:0") {
		RF_xspeed = RF_msg_Actuator.substring(RF_msg_Actuator.indexOf(':')+1);
		RF_xdir = RF_msg_Actuator.substring(0,RF_msg_Actuator.indexOf(':'));
	}

	if (RF_msg_Motor != "YS:0") { //YS:0
		RF_yspeed = RF_msg_Motor.substring(RF_msg_Motor.indexOf(':')+1);
		RF_ydir = RF_msg_Motor.substring(0,RF_msg_Motor.indexOf(':'));
	}

	x = RF_xspeed.toInt();
	y = RF_yspeed.toInt();
	if (y > 255) {
		y = 255;
	}      
	if (x > 255) {
		x = 255;
	}      
	Serial.print(RF_ydir);
	Serial.print(":");
	Serial.println(RF_xdir);

	if ((RF_ydir == "YS") and (RF_xdir == "XR") ) { //TurnRight(x);
		//Serial.println("Right");			
		myL298N.DriverMotors(x,x*-1);

	} else if ((RF_ydir == "YS") and (RF_xdir == "XL") ) { //TurnLeft(x);
		//Serial.println("Left");
		myL298N.DriverMotors(x*-1,x);

	} else if ((RF_ydir == "YF") and (RF_xdir == "XS") ) { //moveforward(x);
		//Serial.println("Forward");
		myL298N.DriverMotors(y,y);

	} else if ((RF_ydir == "YR") and (RF_xdir == "XS") ) { 			//moveback(x);
		//Serial.println("Reverse");
		myL298N.DriverMotors(y*-1,y*-1);

	}	else if ((RF_ydir == "YF") and (RF_xdir == "XR") ) { 
		//Serial.println("Forward Right");
		myL298N.MoveForwardRight(y,x);

	} else if ((RF_ydir == "YF") and (RF_xdir == "XL") ) { 
		//Serial.println("Forward Left");
		myL298N.MoveForwardLeft(y,x);

	} else if ((RF_ydir == "YR") and (RF_xdir == "XR") ) { 
		//Serial.println("Reverse Right");
		myL298N.MoveReverseRight(y,x);

	} else if ((RF_ydir == "YR") and (RF_xdir == "XL") ) { 
		//Serial.println("Reverse Left");
		myL298N.MoveReverseLeft(y,x);

	} else {
		myL298N.stopmotor();      
	}

}

void RF_RequestResponse(String RF_Master_Control_Request_Msg) {
    char snd_text[RF_msg_len] = "";
/*
	- -Snd Hello
	- -What Version 
*/
		if ((RF_Master_Control_Request_Msg == "Hello") or
				(RF_Master_Control_Request_Msg == "What Version")) {  // send RF messages

			if (RF_Master_Control_Request_Msg == "Hello") { // received "Hello"
				//Serial.println("received text 'Hello'");
				String Reply_msg = "Beetle Says...Hello";
				Reply_msg.toCharArray(snd_text, RF_msg_len);
			} else if (RF_Master_Control_Request_Msg == "What Version") { 
				String Reply_msg = "Beetle Says..." + Version;
				Reply_msg.toCharArray(snd_text, RF_msg_len);
			}
			delay(500);
			set_TX_mode();
			radio.write(&snd_text, sizeof(snd_text));
			delay(500);
			set_RX_mode();

		} else { // else don't send an RF msg ... just do something
			
			if (RF_Master_Control_Request_Msg == "Buzzer Test") { 
				test_buzzer();
			} else if (RF_Master_Control_Request_Msg == "Power 100") { 
				speed = 100;
			} else if (RF_Master_Control_Request_Msg == "Power 150") { 
				speed = 150;
			} else if (RF_Master_Control_Request_Msg == "Power 200") { 
				speed = 200;
			} else if (RF_Master_Control_Request_Msg == "Power 250") { 
				speed = 250;
			} else if (RF_Master_Control_Request_Msg == "Spin") { 
				Serial.print("Spin ");
				Serial.println(ispinning);
				if (ispinning == 0) {
					ispinning = 1;
					myL9110.fastforward_A();
				} else {
					ispinning = 0;
					myL9110.stop_A();
				}
			} 
		}



}

void set_TX_mode(){
  if (RF_Mode != "TX") {
    RF_Mode = "TX";
    radio.stopListening(); // put radio in RX mode
  }
}

void set_RX_mode(){
  if (RF_Mode != "RX") {
    RF_Mode = "RX";
    radio.startListening(); // put radio in RX mode
  }
}


void test_buzzer(){
  // put your setup code here, to run once:
  for (int i=0; i<7; i++) {
    tone(SPEAKER_PIN, melody[i], 25);
    delay(100);
  }
  tone(SPEAKER_PIN, melody[7], 100);
}
