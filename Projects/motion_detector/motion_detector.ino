/*
 * 
  Infared motion sensor
   +5vdc
   GND
   PIN

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

*/
#define NRF24L01_CE 9
#define NRF24L01_CSN 10

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define MOTION_SENSOR_PIN A5

int BROADCAST_RF = 1;
int DEBUG = 0;

RF24 radio(NRF24L01_CE, NRF24L01_CSN); // CE, CSN

const byte address[6] = "00001"; // radio channel
int RF_msg_len = 50;

float motion_detected = 0;
String Version = "2.0.20211202";
String device_name = "Motion Sensor";

float previous_motion = 0;

unsigned long startMillis;  //some global variables available anywhere in the program
unsigned long currentMillis;
const unsigned long wait_period = 30000;  //the value is a number of milliseconds
const unsigned long motion_delta = 10;  


void setup() {
  if (DEBUG == 1) {
    Serial.begin(9600);
  }
  float pirStat = analogRead(MOTION_SENSOR_PIN); 
  previous_motion = pirStat;
  startMillis = millis();  //initial start time

	if (BROADCAST_RF == 1)	{
		radio.begin();
		radio.openReadingPipe(0, address);
		radio.openWritingPipe(address);
		//radio.setPALevel(RF24_PA_MIN); // RF24_PA_HIGH
		radio.setPALevel(RF24_PA_MAX);
		radio.stopListening(); // put radio in TX mode
	}
}

void loop() {
  float pirStat;
  currentMillis = millis();  
  unsigned long count_down;
  count_down = currentMillis - startMillis;
  if (count_down > wait_period) {
    if (DEBUG == 1) {
      Serial.print("Looking for motion.  ");
    }
    pirStat = analogRead(MOTION_SENSOR_PIN); 
    if (pirStat < previous_motion) {
      if ((previous_motion - pirStat) > motion_delta) {
        motion_detected = (previous_motion - pirStat);        
      } 
    } else if (pirStat > previous_motion) {
      if ((pirStat - previous_motion) > motion_delta) {
        motion_detected = (pirStat - previous_motion);        
      } 
    }

		if (BROADCAST_RF == 1)	{
			String M = "pirStat=";
			M += pirStat;
			if (motion_detected > 0 )	{
				M += ", motion detected";
			}
			char text[RF_msg_len] = "";
			M.toCharArray(text,RF_msg_len);
			bool ok = radio.write(&text,RF_msg_len);
			if (!ok) {
        if (DEBUG == 1) {
			  	Serial.println("Write failed");
        }
			}
      if (motion_detected > 0 ) {
        delay(1000);
      }

		}

    previous_motion = pirStat;
    if (DEBUG == 1) {
     
      Serial.print("pirStat=");
      Serial.print(pirStat);
      Serial.print(", ");
      Serial.print("previous_motion=");
      Serial.print(previous_motion);
    }
    if (motion_detected > 0) {            // if motion detected
      if (DEBUG == 1) {
        Serial.print(" Motion Detected");
      }
      motion_detected = 0;
    }
    if (DEBUG == 1) {  
      Serial.println(".");
    }
    delay(500);
    
  } else {
    if (DEBUG == 1) {
    
      Serial.print("Initializing over wait period ");
      Serial.print(count_down);
      Serial.print("/");
      Serial.print(wait_period);
      Serial.println("ms");
      delay(5000);
    }
  }
}
