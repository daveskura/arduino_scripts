/*arduino:avr:nano
  Dave Skura, 2022

	Adafruit IR Break Beam Sensors with Premium Wire Header Ends - 3mm LEDs
	https://www.adafruit.com/product/2167

	IRBeam
		PIN 4

	Servo
	 +5vdc
	 GND
	 PIN  2

	BUTTON1
	 PIN  A2

	BUTTON2
	 PIN  A3

	White LED 6
	Red LED 7


*/

const String VERSION = "3.0.20230402";

#include <Servo.h>

const int SPRING_TRAP_POS = 115;

#define BUTTON1 A2
#define BUTTON2 A3
#define SERVO_PIN 2
#define IR_BEAM_PIN 4
#define WHITE_LED 6
#define RED_LED 7

Servo myservo;  

// defines global variables

int gbl_servo_pos = 106;
int gbl_min_sensor_reading; // 52
int gbl_max_sensor_reading; // 62
bool gbl_trap_sprung;

void setup() {
  Serial.begin(9600); // Starts the serial communication
  pinMode(IR_BEAM_PIN, INPUT_PULLUP);     // INPUT ?
  
	pinMode(BUTTON1,INPUT_PULLUP);
  pinMode(BUTTON2,INPUT_PULLUP);

  pinMode(WHITE_LED,OUTPUT);
  pinMode(RED_LED,OUTPUT);

  gbl_min_sensor_reading = 52;
  gbl_max_sensor_reading = 62;
  gbl_trap_sprung = false;

	myservo.attach(SERVO_PIN); 
  
	digitalWrite(RED_LED,LOW);
	digitalWrite(WHITE_LED,HIGH);

}
void loop() {
  
	int btn1 = digitalRead(BUTTON1); 
	int btn2 = digitalRead(BUTTON2); 


	if (btn1 == LOW) {
    if (gbl_servo_pos < 180) {
    	digitalWrite(RED_LED,HIGH);
      gbl_trap_sprung = false;

      gbl_servo_pos = gbl_servo_pos + 1; // goes from 0 degrees to 180 degrees
      myservo.write(gbl_servo_pos);              // tell servo to go to pos in variable 'gbl_servo_pos'
      delay(15);                       // waits 15 ms for the servo to reach the position
    }
  } else if (btn2 == LOW) {
    if (gbl_servo_pos > 0) {
    	digitalWrite(RED_LED,HIGH);
      gbl_trap_sprung = false;

      gbl_servo_pos = gbl_servo_pos - 1; // goes from 0 degrees to 180 degrees
      myservo.write(gbl_servo_pos);              // tell servo to go to position in variable 'gbl_servo_pos'
      delay(15);                       // waits 15 ms for the servo to reach the position
    }
  } else {
  	digitalWrite(RED_LED,LOW);
  }
  

  if ( chk_IR_beam()) {
    gbl_trap_sprung = true;
    gbl_servo_pos = SPRING_TRAP_POS;
    myservo.write(gbl_servo_pos);
  }

  if (gbl_trap_sprung) {
    digitalWrite(WHITE_LED,LOW);
    delay(10000);
    digitalWrite(WHITE_LED,HIGH);
    delay(10000);
  }

}

bool chk_IR_beam() {
	int sensorState = digitalRead(IR_BEAM_PIN);
	bool isbroken = false;
	
	// check if the sensor beam is broken
	if (sensorState == LOW) {     
		isbroken = true;
		//Serial.println("IR beam broken");
	} else {
		isbroken = false;
		//Serial.println("Good IR beam");
	}
	return isbroken;

}

void logmsg(String M) {
  Serial.println(M);
}

