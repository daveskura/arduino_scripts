/*
 * 
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

	Infared motion sensor
	 +5vdc
	 GND
	 PIN  A5
	
*/
const int DEBUG_ON=0;
const int BROADCAST_RF = 0;

#define NRF24L01_CE 9
#define NRF24L01_CSN 10

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

#define MOTION_SENSOR_PIN A5

#define SERVO_PIN 2
#define BUTTON1 6
#define BUTTON2 5

Servo myservo;  

String device_name = "Rat Trap";
String Version = "2.0.20211202";
int ispring_trap_position = 106; // This is the position of the servo which is deemed to spring the trap
int pos = 106;    // variable to store the servo position
float motion_detected = 0;
const unsigned long wait_period = 30000;  //the value is a number of milliseconds
const unsigned long motion_delta = 20;  


String RFMsg_AskRatTrap = "Ask about Rat Trap";
String RF_Mode = ""; // TX for transmit or RX for receive

const int NUMBER_OF_ELEMENTS = 7;

String info[NUMBER_OF_ELEMENTS] = { 
 { "BEGIN 1/7" }, 
 { "2/7: Status=" }, 
 { "3/7: Motion Detected=" }, 
 { "4/7: Servo Position=" }, 
 { "5/7: Device=" }, 
 { "6/7: Version=" }, 
 { "END 7/7" }
 };


RF24 radio(NRF24L01_CE, NRF24L01_CSN); // CE, CSN

const byte address[6] = "00001"; // radio channel
int RF_msg_len = 50;

String szInfo = "All is well";

int Send_Info_Mode = 0;

float previous_motion = 0;

unsigned long startMillis;  //some global variables available anywhere in the program
unsigned long currentMillis;

void set_TX_mode(){
  if (RF_Mode != "TX") {
    RF_Mode = "TX";
    radio.stopListening(); // put radio in TX mode
    //Serial.println("radio.stopListening(); // put radio in TX mode");
  }
}
void set_RX_mode(){
  if (RF_Mode != "RX") {
    RF_Mode = "RX";
    radio.startListening(); // put radio in RX mode
    //Serial.println("radio.startListening(); // put radio in RX mode");
    
  }
}
void RF_Send_Info(){
	String currentinfo[NUMBER_OF_ELEMENTS];
  set_TX_mode();

	for (int i=0;i<NUMBER_OF_ELEMENTS;i++ ){
		currentinfo[i] = info[i];
	}

  currentinfo[1].concat("All is well");
  if (motion_detected == 0) {
    currentinfo[2].concat("False");
  } else {
    currentinfo[2].concat("True");
  }
  
  currentinfo[3].concat(pos);
	currentinfo[4].concat(device_name);
	currentinfo[5].concat(Version);

	
	for (int i=0;i<NUMBER_OF_ELEMENTS;i++ )
	{
    String M = currentinfo[i];
    char text[RF_msg_len] = "";
    M.toCharArray(text,RF_msg_len);
		bool ok = radio.write(&text,RF_msg_len);
   if (!ok) {
    Serial.println("Write failed");
   }
    Serial.println(M);
		delay(500);
	}
  set_RX_mode();
  
}

void setup() {
  if (DEBUG_ON == 1) {
    Serial.begin(9600);
    Serial.println("Initializing");
    myservo.write(1);              // tell servo to go to position in variable 'pos'
    delay(1000);
    myservo.write(180);              // tell servo to go to position in variable 'pos'
    delay(1000);
    myservo.write(90);              // tell servo to go to position in variable 'pos'
    delay(1000);

  }

  pinMode(BUTTON1,INPUT_PULLUP);
  pinMode(BUTTON2,INPUT_PULLUP);
  myservo.attach(SERVO_PIN);  
  motion_detected = 0;

  float pirStat = analogRead(MOTION_SENSOR_PIN); 
  previous_motion = pirStat;
  startMillis = millis();  //initial start time

  radio.begin();
  radio.openReadingPipe(0, address);
  radio.openWritingPipe(address);
  //radio.setPALevel(RF24_PA_MIN); // RF24_PA_HIGH
  radio.setPALevel(RF24_PA_MAX);
  if (BROADCAST_RF == 1) {
    set_TX_mode();
  } else {
    set_RX_mode();
  }
}
int just_once = 0;

void chk_for_motion() {
  float pirStat;
  currentMillis = millis();  
  unsigned long count_down;
  count_down = currentMillis - startMillis;
  if (count_down > 86400000) { // after a day, reset the start time
    startMillis = currentMillis - wait_period;
  }
  pirStat = analogRead(MOTION_SENSOR_PIN); 
  if (count_down > wait_period) {
    if ((DEBUG_ON == 1) and (just_once == 0)) {
      just_once = 1;
      Serial.print("Initialization period of ");
      Serial.print(wait_period);
      Serial.println("ms completed.  Sensing motion now...");
    }
    if (pirStat < previous_motion) {
      if ((previous_motion - pirStat) > motion_delta) {
        motion_detected = (previous_motion - pirStat);        
      } 
    } else if (pirStat > previous_motion) {
      if ((pirStat - previous_motion) > motion_delta) {
        motion_detected = (pirStat - previous_motion);        
      } 
    }

    if (BROADCAST_RF == 1)  {
      String M = "pirStat=";
      M += pirStat;
      if (motion_detected > 0 ) {
        M += ", motion detected";
      }
      char atext[RF_msg_len] = "";
      M.toCharArray(atext,RF_msg_len);
      bool ok = radio.write(&atext,RF_msg_len);
      if (!ok) {
        if (DEBUG_ON == 1) {
          Serial.println("Write failed");
        }
      }
      if (motion_detected > 0 ) {
        delay(1000);
      }

    }

    
    if (DEBUG_ON == 1 and motion_detected > 0) {
      Serial.print("Motion Detected, pirStat=");
      Serial.print(pirStat);
      Serial.print(", ");
      Serial.print("previous_motion=");
      Serial.print(previous_motion);
      Serial.println(".");
    }
    previous_motion = pirStat;
    delay(500);
      
  } else {
    previous_motion = pirStat;
  }
}
void loop() {
	int btn1 = digitalRead(BUTTON1); 
	int btn2 = digitalRead(BUTTON2); 
  if (DEBUG_ON == 1) {
    Serial.print("btn1=");
    Serial.print(btn1);
    Serial.print(", btn2=");
    Serial.println(btn2);
    delay(1000);
  }  
   // button1 is pressed
  if (btn1 == LOW) {
    if (pos < 180) {
      pos = pos + 1; // goes from 0 degrees to 180 degrees
      //Serial.println(pos);
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      //myservo.write(106);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15 ms for the servo to reach the position
    }
  } else if (btn2 == LOW) {
    if (pos > 0) {
      pos = pos - 1; // goes from 0 degrees to 180 degrees
      //Serial.println(pos);
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      //myservo.write(0);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15 ms for the servo to reach the position
    }
  } else {

    if (Send_Info_Mode == 1) {
      RF_Send_Info();
      Send_Info_Mode = 0;
    } else {
      chk_for_motion();
  
      if (motion_detected > 0) {            // if motion detected
        pos = ispring_trap_position;
        myservo.write(pos);              // tell servo to go to spring trap position 
        delay(15);
        motion_detected = 0;
      }    
  
      if (radio.available()) {
        char RF_msg_array[RF_msg_len] = "";
        radio.read(&RF_msg_array, RF_msg_len);
        String RF_MSG = "Received: ";
        RF_MSG.concat(RF_msg_array); 
        String szMsg = RF_msg_array;
        //Serial.println(RF_MSG);
        if (szMsg == RFMsg_AskRatTrap) {
          Send_Info_Mode = 1;
          delay(3000);
        }
      }    
    } // Send_Info_Mode == 0
    
  } // btn1 == HIGH and btn2 == HIGH)  
}
