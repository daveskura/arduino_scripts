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

const String VERSION = "3.0.20230305";

#include <Servo.h>
// #include <RF24.h>

//RF24 radio(7, 8); // CE, CSN
//const byte address[6] = "00001";
//int RF_msg_len = 50;
//String RF_Mode = ""; // TX for transmit or RX for receive

const unsigned long WARMUP_TIME = 1000;  //the value is a number of milliseconds
const unsigned long ARMING_TIME = 5000;  //the value is a number of milliseconds
const int ALARM_THRESHOLD = 5;
const int SPRING_TRAP_POS = 115;

#define BUTTON1 A2
#define BUTTON2 A3
#define SERVO_PIN 2
#define IR_BEAM_PIN 4

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

  gbl_startMillis = millis();  //initial start time

  gbl_min_sensor_reading = 52;
  gbl_max_sensor_reading = 62;
  gbl_armed = false;
  gbl_ready_to_use = false;
  gbl_trap_sprung = false;

	myservo.attach(SERVO_PIN); 
  
  /*
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
	
  set_TX_mode();
  */
  logmsg("Warming up.");

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
      logmsg("Arming.");
		  gbl_ready_to_use = true;
	}
  
  if ((readiness_countdown > warm_arm_time) and (not gbl_armed) and (gbl_ready_to_use)) {
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
  /*
	char text[RF_msg_len] = "";
	M.toCharArray(text,RF_msg_len);
	bool ok = radio.write(&text,RF_msg_len);
  */
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
