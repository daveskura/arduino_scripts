/*arduino:avr:uno
  Dave Skura, 2022
	Ultrasonic Distance Sensor - HC-SR04


	Servo
	 +5vdc
	 GND
	 PIN  2

	BUTTON1
	 PIN  A2

	BUTTON2
	 PIN  A3
*/

const String VERSION = "3.0.20230302";

#include <Servo.h>


const int SENSOR_TRIG_PIN = 9;
const int SENSOR_ECHO_PIN = 10;

const unsigned long ARMING_TIME = 5000;  //the value is a number of milliseconds
const int ALARM_THRESHOLD = 5;
const int SPRING_TRAP_POS = 106;

#define BUTTON1 A2
#define BUTTON2 A3
#define SERVO_PIN 2

Servo myservo;  

// defines global variables

int gbl_servo_pos = 106;
int gbl_min_sensor_reading = -1;
int gbl_max_sensor_reading = -1;
bool gbl_armed = false;

unsigned long gbl_startMillis;  
unsigned long gbl_currentMillis;

void setup() {
  exit(0);
  Serial.begin(9600); // Starts the serial communication
  gbl_startMillis = millis();  //initial start time
  
	pinMode(BUTTON1,INPUT_PULLUP);
  pinMode(BUTTON2,INPUT_PULLUP);

	pinMode(SENSOR_TRIG_PIN, OUTPUT); // Sets the SENSOR_TRIG_PIN as an Output
  pinMode(SENSOR_ECHO_PIN, INPUT); // Sets the SENSOR_ECHO_PIN as an Input
	int sensor_reading = get_sensor_reading();
	gbl_min_sensor_reading = sensor_reading;
	gbl_max_sensor_reading = sensor_reading;

	myservo.attach(SERVO_PIN);  
}

void loop() {
	int btn1 = digitalRead(BUTTON1); 
	int btn2 = digitalRead(BUTTON2); 

	unsigned long count_down;

	gbl_currentMillis = millis();  
  long arming_time_countdown = gbl_currentMillis - gbl_startMillis;
	if (arming_time_countdown > ARMING_TIME) {
		gbl_armed = true;
	}

	if (btn1 == LOW) {
    if (gbl_servo_pos < 180) {
      gbl_servo_pos = gbl_servo_pos + 1; // goes from 0 degrees to 180 degrees
      myservo.write(gbl_servo_pos);              // tell servo to go to pos in variable 'gbl_servo_pos'
      delay(15);                       // waits 15 ms for the servo to reach the position
    }
  } else if (btn2 == LOW) {
    if (gbl_servo_pos > 0) {
      gbl_servo_pos = gbl_servo_pos - 1; // goes from 0 degrees to 180 degrees
      myservo.write(gbl_servo_pos);              // tell servo to go to position in variable 'gbl_servo_pos'
      delay(15);                       // waits 15 ms for the servo to reach the position
    }
  } else {
		check_sensor_ranges();
		delay(2000);
	}

}

void trigger_alarm() {
	gbl_armed = false;
	Serial.print("****** Alarm triggered ****** ");
	myservo.write(SPRING_TRAP_POS);
	exit(0);
}

void check_sensor_ranges() {
	int reading_drop = 0;
	int sensor_reading = get_sensor_reading();
	if (gbl_armed) {
		if (sensor_reading < gbl_min_sensor_reading) {
			reading_drop = gbl_min_sensor_reading - sensor_reading;
			if (reading_drop > ALARM_THRESHOLD){
				trigger_alarm();
			}
		}
	} 

	if (sensor_reading > gbl_max_sensor_reading) {
		gbl_max_sensor_reading = sensor_reading;
	}
	if (sensor_reading < gbl_min_sensor_reading) {
		gbl_min_sensor_reading = sensor_reading;
	}

	Serial.print("min ");
	Serial.print(gbl_min_sensor_reading);
	Serial.print(", ");
	Serial.print("max ");
	Serial.print(gbl_max_sensor_reading);
	Serial.print(", ");
	Serial.print("sensor_reading ");
	Serial.println(sensor_reading);

}

int get_sensor_reading() {
	long duration;
	int distance;

	// Clears the SENSOR_TRIG_PIN
  digitalWrite(SENSOR_TRIG_PIN, LOW);
  delayMicroseconds(2);
  // Sets the SENSOR_TRIG_PIN on HIGH state for 10 micro seconds
  digitalWrite(SENSOR_TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(SENSOR_TRIG_PIN, LOW);
  // Reads the SENSOR_ECHO_PIN, returns the sound wave travel time in microseconds
  duration = pulseIn(SENSOR_ECHO_PIN, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2;
}
