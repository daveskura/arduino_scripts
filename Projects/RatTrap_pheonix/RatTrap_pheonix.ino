/*arduino:avr:uno
  Dave Skura, 2023

	(NRF24L01) Wireless Module Transceiver 
  CE    ->  9
  CSN   ->  10
  MOSI  ->  11
  SCK   ->  13
  IRQ   ->  Not Used  
  MISC  ->  12


  | <blank>    | MOSI = 11 | CSN = 10 | +3vdc |
  |------------|-----------|----------|--------
  |  MISC = 12 | SCK  = 13 | CE  = 9  | GND   |


	Servo
	 +5vdc
	 GND
	 PIN  2

	BUTTON1
	 PIN  6

	BUTTON2
	 PIN  5

	BUTTON3
	 PIN  A3

	BUTTON4
	 PIN  A4

	Infared motion sensor
	 +5vdc
	 GND
	 PIN  A2
	
*/
const int DEBUG_ON=1;

#define NRF24L01_CE 9
#define NRF24L01_CSN 10

#include <SPI.h>
#include <Servo.h>

#define MOTION_SENSOR_PIN A2

#define SERVO_PIN 2
#define BUTTON1 6
#define BUTTON2 5
#define BUTTON3 A3
#define BUTTON4 A4

Servo myservo;  

const String device_name = "Rat Trap";
const String Version = "2.1.20230301";
const int ispring_trap_position = 106; // This is the position of the servo which is deemed to spring the trap
const unsigned long WAIT_PERIOD = 30000;  //the value is a number of milliseconds
const unsigned long MOTION_DELTA = 20;  

const String RFMsg_AskRatTrap = "Ask about Rat Trap";

const int NUMBER_OF_ELEMENTS = 7;

const String info[NUMBER_OF_ELEMENTS] = { 
 { "BEGIN 1/7" }, 
 { "2/7: Status=" }, 
 { "3/7: Motion Detected=" }, 
 { "4/7: Servo Position=" }, 
 { "5/7: Device=" }, 
 { "6/7: Version=" }, 
 { "END 7/7" }
 };

//RF24 radio(NRF24L01_CE, NRF24L01_CSN); // CE, CSN

const byte address[6] = "00001"; // radio channel
const int RF_msg_len = 50;

const String szInfo = "All is well";

String gbl_RF_Mode = ""; // TX for transmit or RX for receive
int gbl_BROADCAST_RF = 0;
int gbl_pos = 106;    // variable to store the servo position
int gbl_just_once = 0;
int gbl_motion_detected = 0;
float gbl_previous_motion = 0;

unsigned long gbl_startMillis;  //some global variables available anywhere in the program
unsigned long gbl_currentMillis;

void setup() {
  if (DEBUG_ON == 1) {
    Serial.begin(9600);
    Serial.println("Initializing");
  }
	gbl_just_once = 0;

  pinMode(BUTTON1,INPUT_PULLUP);
  pinMode(BUTTON2,INPUT_PULLUP);
  pinMode(BUTTON3,INPUT_PULLUP);
  pinMode(BUTTON4,INPUT_PULLUP);
  myservo.attach(SERVO_PIN);  

  gbl_previous_motion = analogRead(MOTION_SENSOR_PIN); 
  gbl_startMillis = millis();  //initial start time

  //radio.begin();
  //radio.openReadingPipe(0, address);
  //radio.openWritingPipe(address);
  //radio.setPALevel(RF24_PA_MIN); // RF24_PA_HIGH
  //radio.setPALevel(RF24_PA_MAX);
  if (gbl_BROADCAST_RF == 1) {
    set_TX_mode();
  } else {
    set_RX_mode();
  }
}

void loop() {
	int btn1 = digitalRead(BUTTON1); 
	int btn2 = digitalRead(BUTTON2); 
	int btn3 = digitalRead(BUTTON3); 
	int btn4 = digitalRead(BUTTON4); 

   // button1 is pressed
  if ((btn1 == LOW) or (btn3 == LOW) ){
    if (gbl_pos < 180) {
      gbl_pos = gbl_pos + 1; // goes from 0 degrees to 180 degrees
      //Serial.println(pos);
      myservo.write(gbl_pos);              // tell servo to go to position in variable 'pos'
      //myservo.write(106);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15 ms for the servo to reach the position
    }

  } else if ((btn2 == LOW) or (btn4 == LOW)){
    if (gbl_pos > 0) {
      gbl_pos = gbl_pos - 1; // goes from 0 degrees to 180 degrees
      //Serial.println(pos);
      myservo.write(gbl_pos);              // tell servo to go to position in variable 'pos'
      //myservo.write(0);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15 ms for the servo to reach the position
    }

  } else {

			// no buttons pressed
      if (chk_for_motion() > 0) {            // if motion detected
        gbl_pos = ispring_trap_position;
        myservo.write(gbl_pos);              // tell servo to go to spring trap position 
        delay(15);
      }    
    
  }  
	delay(1000);
}


void set_TX_mode(){
  if (gbl_RF_Mode != "TX") {
    gbl_RF_Mode = "TX";
    //radio.stopListening(); // put radio in TX mode
		if (DEBUG_ON == 1) {
			Serial.println("radio.stopListening(); // put radio in TX mode");
		}
  }
}
void set_RX_mode(){
  if (gbl_RF_Mode != "RX") {
    gbl_RF_Mode = "RX";
    //radio.startListening(); // put radio in RX mode
		if (DEBUG_ON == 1) {
	    Serial.println("radio.startListening(); // put radio in RX mode");
		}
  }
}
void RF_Send_Info(){
	String currentinfo[NUMBER_OF_ELEMENTS];
  set_TX_mode();

	for (int i=0;i<NUMBER_OF_ELEMENTS;i++ ){
		currentinfo[i] = info[i];
	}

  currentinfo[1].concat("All is well");
  if (gbl_motion_detected == 0) {
    currentinfo[2].concat("False");
  } else {
    currentinfo[2].concat("True");
  }
  
  currentinfo[3].concat(gbl_pos);
	currentinfo[4].concat(device_name);
	currentinfo[5].concat(Version);

	
	for (int i=0;i<NUMBER_OF_ELEMENTS;i++ )
	{
    String M = currentinfo[i];
    char text[RF_msg_len] = "";
    M.toCharArray(text,RF_msg_len);
		//bool ok = radio.write(&text,RF_msg_len);
   //if (!ok) {
    Serial.println("Write failed");
   //}
    Serial.println(M);
		delay(500);
	}
  set_RX_mode();
  
}

int chk_for_motion() {
	int delta_motion = 0;
  gbl_currentMillis = millis();  
  unsigned long count_down;
  count_down = gbl_currentMillis - gbl_startMillis;

	float pirStat = analogRead(MOTION_SENSOR_PIN); 
  
	if (count_down > WAIT_PERIOD) {
		
    if ((DEBUG_ON == 1) and (gbl_just_once == 0)) {
      gbl_just_once = 1;
      Serial.print("Initialization period of ");
      Serial.print(WAIT_PERIOD);
      Serial.println("ms completed.  Sensing motion now...");
    }

    if (pirStat < gbl_previous_motion) {
      if ((gbl_previous_motion - pirStat) > MOTION_DELTA) {
        delta_motion = (gbl_previous_motion - pirStat);        
      } 
    } else if (pirStat > gbl_previous_motion) {
      if ((pirStat - gbl_previous_motion) > MOTION_DELTA) {
        delta_motion = (pirStat - gbl_previous_motion);        
      } 
    }

    if (DEBUG_ON == 1 and delta_motion > 0) {
      Serial.print("Motion Detected, pirStat=");
      Serial.print(pirStat);
      Serial.print(", ");
      Serial.print("delta_motion=");
      Serial.print(delta_motion);
      Serial.print(", ");
      Serial.print("gbl_previous_motion=");
      Serial.print(gbl_previous_motion);
      Serial.println(".");
    }

    gbl_previous_motion = pirStat;
      
  } else {
		// during wait period ... motion value is assigned to gbl_previous_motion
    gbl_previous_motion = pirStat;
  }

	if (count_down > 86400000) { // after a day, reset the start time
    gbl_startMillis = gbl_currentMillis - WAIT_PERIOD;
  }

	return delta_motion;
}
