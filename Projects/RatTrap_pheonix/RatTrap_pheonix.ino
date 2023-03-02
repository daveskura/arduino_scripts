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

	(NRF24L01) Wireless Module Transceiver 
	CE		->	7
	CSN		->	8
	MOSI	->	11
	SCK		->	13
	IRQ		->	Not Used	
	MISC	->	12
*/

const String VERSION = "3.0.20230302";

#include <Servo.h>


const int SENSOR_TRIG_PIN = 9;
const int SENSOR_ECHO_PIN = 10;

const unsigned long WARMUP_TIME = 1000;  //the value is a number of milliseconds
const unsigned long ARMING_TIME = 5000;  //the value is a number of milliseconds
const int ALARM_THRESHOLD = 5;
const int SPRING_TRAP_POS = 115;

#define BUTTON1 A2
#define BUTTON2 A3
#define SERVO_PIN 2

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
  gbl_startMillis = millis();  //initial start time

  gbl_min_sensor_reading = 52;
  gbl_max_sensor_reading = 62;
  gbl_armed = false;
  gbl_ready_to_use = false;
  gbl_trap_sprung = false;

	pinMode(BUTTON1,INPUT_PULLUP);
  pinMode(BUTTON2,INPUT_PULLUP);

	pinMode(SENSOR_TRIG_PIN, OUTPUT); // Sets the SENSOR_TRIG_PIN as an Output
  pinMode(SENSOR_ECHO_PIN, INPUT); // Sets the SENSOR_ECHO_PIN as an Input
	int sensor_reading = get_sensor_reading();

	myservo.attach(SERVO_PIN); 
  
  Serial.println("");
  Serial.println("Warming up.");
}

void loop() {
	int btn1 = digitalRead(BUTTON1); 
	int btn2 = digitalRead(BUTTON2); 

	unsigned long count_down;
  unsigned long count_down_arming;

	gbl_currentMillis = millis();  
  long warm_arm_time = WARMUP_TIME + ARMING_TIME;
  long readiness_countdown = gbl_currentMillis - gbl_startMillis;
  
	if ((readiness_countdown > WARMUP_TIME) and (not gbl_ready_to_use) and (not gbl_trap_sprung)) {
      Serial.println("");
      Serial.println("Arming.");
		  gbl_ready_to_use = true;
	}
  
  if ((readiness_countdown > warm_arm_time) and (not gbl_armed) and (not gbl_trap_sprung)) {
      Serial.println("Trap Armed.");
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
  } else if (not gbl_trap_sprung) {
		check_sensor_ranges();
		delay(2000);
	}

}

void trigger_alarm() {
	gbl_ready_to_use = false;
	gbl_armed = false;
  gbl_trap_sprung = true;
	Serial.print(" Alarm triggered ");
	myservo.write(SPRING_TRAP_POS);
}

void check_sensor_ranges() {
  int reading_spike = 0;
	int sensor_reading = get_sensor_reading();
  if (gbl_armed) {
    Serial.print("min ");
    Serial.print(gbl_min_sensor_reading);
    Serial.print(", ");
    Serial.print("max ");
    Serial.print(gbl_max_sensor_reading);
    Serial.print(", ");
    Serial.print("sensor_reading ");
    Serial.println(sensor_reading);

    if (sensor_reading < gbl_min_sensor_reading) {
      reading_spike = gbl_min_sensor_reading - sensor_reading;
    }
    if (sensor_reading > gbl_max_sensor_reading) {
      reading_spike = sensor_reading - gbl_max_sensor_reading;
    }

    if (reading_spike > ALARM_THRESHOLD){
      trigger_alarm();
    }
  }
  /* 
  if (gbl_ready_to_use) {
    if (sensor_reading > gbl_max_sensor_reading) {
      gbl_max_sensor_reading = sensor_reading;
    }
    if (sensor_reading < gbl_min_sensor_reading) {
      gbl_min_sensor_reading = sensor_reading;
    }
  }
  */

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
  return distance;
}
