/*arduino:avr:uno
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

const String VERSION = "3.0.20230306";

#include <Servo.h>

const unsigned long WARMUP_TIME = 1000;  //the value is a number of milliseconds
const unsigned long ARMING_TIME = 5000;  //the value is a number of milliseconds
const int ALARM_THRESHOLD = 9;
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
bool gbl_armed;
bool gbl_ready_to_use;
bool gbl_trap_sprung;

unsigned long gbl_startMillis;  
unsigned long gbl_currentMillis;

void setup() {
  Serial.begin(9600); // Starts the serial communication
  pinMode(IR_BEAM_PIN, INPUT_PULLUP);     // INPUT ?
  
	pinMode(BUTTON1,INPUT_PULLUP);
  pinMode(BUTTON2,INPUT_PULLUP);

  pinMode(WHITE_LED,OUTPUT);
  pinMode(RED_LED,OUTPUT);

  gbl_startMillis = millis();  //initial start time

  gbl_min_sensor_reading = 52;
  gbl_max_sensor_reading = 62;
  gbl_armed = false;
  gbl_ready_to_use = false;
  gbl_trap_sprung = false;

	myservo.attach(SERVO_PIN); 
  
  logmsg("Warming up.");
	
	digitalWrite(RED_LED,HIGH);
	analogWrite(WHITE_LED,HIGH);


}

void loop() {
	int btn1 = digitalRead(BUTTON1); 
	int btn2 = digitalRead(BUTTON2); 

	unsigned long count_down;
  unsigned long count_down_arming;

	gbl_currentMillis = millis();  
  long warm_arm_time = WARMUP_TIME + ARMING_TIME;
  long readiness_countdown = gbl_currentMillis - gbl_startMillis;
	if (readiness_countdown > 86000000) { // one day has passed
		// reset counter to one day
		gbl_startMillis = gbl_currentMillis - warm_arm_time;
	}
  
	if ((readiness_countdown > WARMUP_TIME) and (not gbl_ready_to_use) and (not gbl_trap_sprung)) {
    digitalWrite(RED_LED,LOW);
  	analogWrite(WHITE_LED,LOW);
    delay(500);
    digitalWrite(RED_LED,HIGH);
  	analogWrite(WHITE_LED,HIGH);
    delay(500);

    logmsg("Arming.");
    gbl_ready_to_use = true;
	}
  
  if ((readiness_countdown > warm_arm_time) and (not gbl_armed) and (gbl_ready_to_use)) {
    digitalWrite(RED_LED,LOW);
    digitalWrite(WHITE_LED,HIGH);
    logmsg("Trap Armed.");
    gbl_armed = true;
  }  

	if (btn1 == LOW) {
    if (gbl_servo_pos < 180) {
      gbl_startMillis = gbl_currentMillis; 
      gbl_armed = false;
      gbl_ready_to_use = false;
      gbl_trap_sprung = false;

      gbl_servo_pos = gbl_servo_pos + 1; // goes from 0 degrees to 180 degrees
      myservo.write(gbl_servo_pos);              // tell servo to go to pos in variable 'gbl_servo_pos'
      delay(15);                       // waits 15 ms for the servo to reach the position
    }
  } else if (btn2 == LOW) {
    if (gbl_servo_pos > 0) {
      gbl_startMillis = gbl_currentMillis; 
      gbl_armed = false;
      gbl_ready_to_use = false;
      gbl_trap_sprung = false;

      gbl_servo_pos = gbl_servo_pos - 1; // goes from 0 degrees to 180 degrees
      myservo.write(gbl_servo_pos);              // tell servo to go to position in variable 'gbl_servo_pos'
      delay(15);                       // waits 15 ms for the servo to reach the position
    }
  } else if (gbl_armed) {
    if ( chk_IR_beam()) {
      trigger_alarm(); 
    }
	} else if (gbl_trap_sprung) {
    digitalWrite(RED_LED,LOW);
    delay(500);
    digitalWrite(RED_LED,HIGH);
    delay(500);
  }

}

bool chk_IR_beam() {
	int sensorState = digitalRead(IR_BEAM_PIN);
	bool isbroken = false;
	
	// check if the sensor beam is broken
	if (sensorState == LOW) {     
		digitalWrite(LED_BUILTIN, HIGH);  
		isbroken = true;
		//Serial.println("IR beam broken");
	} else {
		digitalWrite(LED_BUILTIN, LOW); 
		isbroken = false;
		//Serial.println("Good IR beam");
	}
	return isbroken;

}

void logmsg(String M) {
  Serial.println(M);
}

void trigger_alarm() {
	gbl_ready_to_use = false;
	gbl_armed = false;
  gbl_trap_sprung = true;
	logmsg(" Alarm triggered ");
  gbl_servo_pos = SPRING_TRAP_POS;
	myservo.write(gbl_servo_pos);
}
