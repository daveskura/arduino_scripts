/*
  Chicken Car, version 10.3
	By Dave Skura, Sept 21,2021
 
 	** Upates **
	Version 8 was the last version to run on Uno.  
	Version 10 is the next version and is mapped to Arduino Mega pins

  ** The Chicken car also has an RF remote.  Covered here is only the chicken car itself.
  The remote is in another file.
  
  Description:
		The chicken car is a motorized, solar powered, remote controlled, semi-autonomous cage
		for 1-3 chickens to travel around the yard in.  With an open bottom for the chickens to scratch
		and peck the ground, and a spot for water and food.  The cage protects the chickens from predators
		such as hawks, while giving them access to lots of ground for various chicken activities ... 
		such as scratching and pecking.

	What is auto chicken ?
		Move forward every 3 minutes, 10 times, then back 10 times every 3 minutes
 
  Power System: 
  12v battery
		- driving 12vdc motors for drive and steering
		- Through converter from 12vdc to 5vdc to power arduino and peripheral circuits
		- battery is constantly charged with a low amp charge using a solar panel
 
  Equipment List: 
 
  Arduino Uno R3
 	https://www.amazon.ca/gp/product/B01EWOE0UU/ref=ppx_od_dt_b_asin_title_s00?ie=UTF8&psc=1
 
  (TOBSUN)		12V to 5V 3A Power Converter
		https://www.amazon.ca/gp/product/B077444WQY/ref=ppx_od_dt_b_asin_title_s01?ie=UTF8&psc=1
 
  (NRF24L01) Wireless Module Transceiver 
		https://www.amazon.ca/gp/product/B07XD4YW73/ref=ppx_yo_dt_b_asin_title_o04_s00?ie=UTF8&psc=1
 
  analogue joystick
		https://www.amazon.ca/Dual-axis-Joystick-Module-2-Axis-arduino/dp/B07Z44NXRN/ref=sr_1_2?dchild=1&keywords=2pcs%2Flot+Dual-axis+Mini+xy+Joystick+Module+PSP+2-Axis+Analog+3V-5V+Thumb+Joystick+Module+for+arduino&qid=1627417134&sr=8-2
 
  (MD10C R3) Cytron 13A, 5-30V Single DC Motor Controller 
		https://www.robotshop.com/en/cytron-13a-5-30v-single-dc-motor-controller.html
 
  (DRI0042)	DFRobot 15A Single DC Motor Driver Controller 
		https://www.robotshop.com/en/dfrobot-15a-single-dc-motor-driver.html
 
  PN01007-SPK ( 12vdc 50 RPM Gear Motor with Sprocket )
		https://makermotor.com/pn01007-spk-12vdc-50-rpm-gear-motor-with-sprocket/
 
  8 Inch 12vdc Linear Actuator
		https://www.amazon.ca/gp/product/B07L85ZM3M/ref=ppx_yo_dt_b_asin_title_o02_s00?ie=UTF8&psc=1
 
  #35 chain,sprocket,links,Wheels
		https://www.grainger.ca/product/p/GGF2YDW4
		https://www.grainger.ca/en/product/p/WWG6L778?cm_em=132014145&cm_mmc=EMT-_-CreateOrder_EN&rrid=132014145&rmid=CreateOrder_EN&e=daveskura@hotmail.com
		https://www.amazon.ca/gp/product/B004HKITPM/ref=ppx_yo_dt_b_asin_title_o03_s00?ie=UTF8&psc=1 
		https://www.amazon.ca/gp/product/B00PXM1RR0/ref=ppx_yo_dt_b_asin_image_o00_s00?ie=UTF8&psc=1
 
	DFPlayer 
		https://wiki.dfrobot.com/DFPlayer_Mini_SKU_DFR0299

  Connectivity Specifications:

	Arduino Uno
 		PIN	D0																											** Not Used **
 		PIN	D1																											** Not Used **
 		PIN	D2																											** Not Used **
 		PIN	D3																											** Not Used **	
 		PIN	D4																											** Not Used **	
 		PIN	D5																											** Not Used **	
 		PIN	D6																											** Not Used **	
 		PIN	D7																											** Not Used **	
 		PIN	D8																											** Not Used **
 		PIN	D9																											** Not Used **
 		PIN	D10	< --> DFPlayer TX - Yellow		
 		PIN	D11	< --> DFPlayer RX - Blue
 		PIN	D12																											** Not Used **
 		PIN	D13																											** Not Used **
		PIN	D14																											** Not Used **
		PIN	D15																											** Not Used **
		PIN	D16																											** Not Used **
		PIN	D17																											** Not Used **
		PIN	D18																											** Not Used **
		PIN	D19																											** Not Used **
		PIN	D20																											** Not Used **
		PIN	D21																											** Not Used **
		PIN	D22																											** Not Used **
		PIN	D23																											** Not Used **
		PIN	D24																											** Not Used **
		PIN	D25																											** Not Used **
		PIN	D26																											** Not Used **
		PIN	D27																											** Not Used **
		PIN	D28																											** Not Used **
		PIN	D29																											** Not Used **
		PIN	D30																											** Not Used **
		PIN	D31																											** Not Used **
		PIN	D32																											** Not Used **
		PIN	D33																											** Not Used **
		PIN	D34																											** Not Used **
		PIN	D35																											** Not Used **
		PIN	D36																											** Not Used **
		PIN	D37	<--> MD10C R3 - DIR - yellow
		PIN	D38																											** Not Used **
		PIN	D39	<--> MD10C R3 - PWM - blue
		PIN	D40																											** Not Used **
		PIN	D41	<--> not able to be used... broken pin 						  ** Not Used **
		PIN	D42																											** Not Used **
		PIN	D43	<--> DRI0042 - IN2 - orange
		PIN	D44																											** Not Used **
		PIN	D45	<--> DRI0042 - PWM - blue
		PIN	D46																											** Not Used **
		PIN	D47	<--> DRI0042 - IN1 - Yellow
		PIN	D48																											** Not Used **
		PIN	D49	<--> MEGA PIN D49 <--> NRF24L01 - CE
		PIN	D50	<--> MEGA PIN D50 <--> NRF24L01 - MISO
		PIN	D51	<--> MEGA PIN D51 <--> NRF24L01 - MOSI
		PIN	D52	<--> MEGA PIN D52 <--> NRF24L01 - SCK
		PIN	D53	<--> MEGA PIN D53 <--> NRF24L01 - CSN


 		PIN	A0	Joystick Y Axis <--> Blue <--> Joytick VRy 
 		PIN A1	Joystick X Axis	<--> Yellow <--> Joytick VRx
 		PIN A2																											** Not Used **
 		PIN A3																											** Not Used **
		PIN A4	<--> PUSHBUTTON_3.	MUSIC_VOLUME_PUSHBUTTON
		PIN A5	<--> PUSHBUTTON_2.	MUSIC_ON_OFF_PUSHBUTTON
		PIN A6	<--> PUSHBUTTON_1.  MUSIC_MODE_PUSHBUTTON
		PIN A7	<--> TOGGLE_4_ORANGE.																** Not Used **
		PIN A8	<--> TOGGLE_3_GREEN. SW_AUTO_CHICKEN_STRT_FWD_RVRSE
		PIN A9	<--> TOGGLE_1_BLUE.	RF_SWITCH_REMOTE_LOCAL
		PIN A10	<--> Relay Switch for Radio.  RELAY_SW_CHICKEN_RADIO 
		PIN A11	<--> TOGGLE_2_YELLOW. SW_AUTO_CHICKEN	


	(DFPlayer) MP3 Player
		* DFPlayer Right pins not connected at all.
		* top of DFPlayer has writing

		DFPlayer +5VCC top Left Pin 1  
		DFPlayer RX top Left Pin 2 -> Arduino PIN 11 - blue  
		DFPlayer TX top Left Pin 3 -> Arduino PIN 10 - yellow

		DFPlayer SPKR+ Bot Left Pin 6  
		DFPlayer GND Bot Left Pin 7 
		DFPlayer SPKR- Bot Left Pin 8  


  analogue joystick connected as follows:
  
  VRx  -> A1 (Analogue input required for speed control)
  VRy  -> A0 (Analogue input required for speed control)
  SW   -> not used  *Used when joystick is clicked

	(MD10C R3) Cytron 13A, 5-30V Single DC Motor Controller 

  PWM -> 39
  DIR -> 37

	(DRI0042)	DFRobot 15A Single DC Motor Driver Controller 

  PWM -> 45
  IN1 -> 47
  IN2 -> 43

 (NRF24L01) Wireless Module Transceiver 
  CE    ->  49
  MISO  ->  50
  MOSI  ->  51
  SCK   ->  52
  CSN   ->  53


  | <blank> | 51  | 53 |  +3vdc |
  |---------|-----|----|---------
  |   50    | 52  | 49 |  GND   |


  | <blank> | MOSI  | CSN | +3vdc |
  |---------|-------|-----|--------
  |   MISO  | SCK   | CE  | GND   |


*/
#define NRF24L01_CE 49
#define NRF24L01_CSN 53

#define joyY A0 //  0 = min value, center about 520, 1023 = max value
#define joyX A1 //  0 = min value, center about 520, 1023 = max value

#define MUSIC_ON_OFF_PUSHBUTTON A5
#define MUSIC_MODE_PUSHBUTTON A6
#define MUSIC_VOLUME_PUSHBUTTON A4

#define SW_AUTO_CHICKEN_STRT_FWD_RVRSE A8 // auto chicken to start forward or reverse
#define RF_SWITCH_REMOTE_LOCAL A9	// ONLY use LOCAL or RF Remote commands
#define RELAY_SW_CHICKEN_RADIO A10
#define SW_AUTO_CHICKEN A11	// auto chicken Enabled/Disabled

#define AUTO_CHICKEN_MVMNT_COUNT 10 // count of times to move forward and backward
#define AUTO_CHICKEN_MVMNT_INTERVAL (60000 * 3)	// 3 min. Time in milliseconds between movements
#define AUTO_CHICKEN_MVMNT_DURATION (1000 * 2)	// 2 secs.  Time in milliseconds of movement.

#define MOTOR_IN2 43
#define MOTOR_PWM 45
#define MOTOR_IN1 47

#define ACTUATOR_PWM 39
#define ACTUATOR_DIR 37

int gbl_music_mode = -1; // -1 = no music, 0 = Radio, 1=playlist #1, 2=playlist #2, 3=playlist #3 ...gbl_nbr_playlists

int gbl_music_volume_idx = 2; 
/*
	these values come from an array of possible values which are cycled as the button is pressed.
	eg. 2 means.... volume_range_array[2] which has a volume of 10
	also.. 0 is off ... 30 is max volume
*/

int chicken_radio_on_off = 0; //0 = radio OFF, 1 = radio ON
int gbl_chicken_music_on_off = 0; //0 = music OFF, 1 = music ON

long turn_position_milliseconds = 0; // this is the seconds turned to Left or right....to allow tracking of turns
long millisec_turn_range = 3000; // total milliseconds to allow actuator to move left or right
long millisec_of_current_turn = 0; // The time when the previous turn started
long millisec_of_previous_turn = 0; // The time when the previous turn started
int current_turn_action = 0; // 0 = stop, -1 = left, 1 = right
int previous_turn_action = 0; // 0 = stop, -1 = left, 1 = right

int last_checked_switch_auto_chicken = 0; //	0,1
int last_checked_switch_auto_chicken_fwd_rvrse = 0; //	0,1

int auto_chicken_mvmnt_direction = 0; //0 = Forward, 1 = Reverse
int auto_chicken_mvmnt_count = 0;   // 0 .. AUTO_CHICKEN_MVMNT_COUNT
unsigned long auto_chicken_last_move_time = 0;

int gbl_rf_vs_local = 0;

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <EEPROM.h>
#include "chicken_audio.h"
#include "Arduino.h"
#include "SoftwareSerial.h"

#define DFPLAYER_RX 10
#define DFPLAYER_TX 11
#define DFPLAYER_VOICE_VOLUME 20 //Set volume value. From 0 to 30

#include "DFRobotDFPlayerMini.h"

SoftwareSerial mySoftwareSerial(DFPLAYER_RX,DFPLAYER_TX); // RX, TX

DFRobotDFPlayerMini myDFPlayer;

RF24 radio(NRF24L01_CE, NRF24L01_CSN); // CE, CSN

const byte address[6] = "00001"; // radio channel
int RF_msg_len = 50;

void setup() {
	//Let's make it more random
	randomSeed(analogRead(A0)+analogRead(A2)+analogRead(A3)); 

  mySoftwareSerial.begin(9600);
  Serial.begin(9600);

 if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true);
  }
  Power_on_voice_notify(myDFPlayer);

  // Assume wheels are straight when switch is turned on.  0 is straight position.
  turn_position_milliseconds = 0;
  millisec_of_previous_turn = 0;
  millisec_of_current_turn = 0;
  previous_turn_action = 0;
  current_turn_action = 0; 

  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
 
  pinMode(ACTUATOR_PWM,OUTPUT);
  pinMode(ACTUATOR_DIR,OUTPUT);
  pinMode(MOTOR_PWM,OUTPUT);
  pinMode(MOTOR_IN1,OUTPUT);
  pinMode(MOTOR_IN2,OUTPUT);

  pinMode(RF_SWITCH_REMOTE_LOCAL,INPUT_PULLUP);

  pinMode(SW_AUTO_CHICKEN,INPUT_PULLUP);
  pinMode(SW_AUTO_CHICKEN_STRT_FWD_RVRSE,INPUT_PULLUP);

  pinMode(RELAY_SW_CHICKEN_RADIO,OUTPUT);
  
  pinMode(MUSIC_MODE_PUSHBUTTON,INPUT_PULLUP);
  pinMode(MUSIC_ON_OFF_PUSHBUTTON,INPUT_PULLUP);
	pinMode(MUSIC_VOLUME_PUSHBUTTON,INPUT_PULLUP);

  // when arduino is turned on, default radio to off
  chicken_radio_on_off = 0;
  gbl_chicken_music_on_off = 0;
  turn_music_on_or_off();
  last_checked_switch_auto_chicken = digitalRead(SW_AUTO_CHICKEN);
  last_checked_switch_auto_chicken_fwd_rvrse = digitalRead(SW_AUTO_CHICKEN_STRT_FWD_RVRSE);

}

void loop() {
  
	// Serial.println("Loop:Start");
  int RF_xspeed = 0;
  String RF_yspeed = "0"; // speed from 0 .. 255
  int local_yspeed = 0; // speed from 0 .. 255
  String RF_xdir = "STOP",RF_ydir = "STOP"; // direction either STOP, XR,XL,YF,YR 
	int Local_xValue, Local_yValue; 
  int RF_switch;

	unsigned long current_time = millis();
	unsigned long milliseconds_passed;
  int auto_chicken_switch;
	int auto_chicken_fwd_rvrse;

  int btn_music_mode;
  int btn_music_on_off;
  int btn_music_volume;
	
  btn_music_mode = digitalRead(MUSIC_MODE_PUSHBUTTON);
  btn_music_on_off = digitalRead(MUSIC_ON_OFF_PUSHBUTTON);
	btn_music_volume = digitalRead(MUSIC_VOLUME_PUSHBUTTON);

	// button is pressed
	// cycle gbl_music_volume = 10; // 5 - 30.  0 is off ... 30 is max volume
  if (btn_music_volume == LOW) {
		gbl_music_volume_idx += 1;
		if (gbl_music_volume_idx == gbl_nbr_volume_ranges)	{
			gbl_music_volume_idx = 0;
		}
		myDFPlayer.volume(volume_range_array[gbl_music_volume_idx]);
		delay(1000);
	}
	// volume_range_array[gbl_music_volume_idx]

	// button is pressed
  if (btn_music_mode == LOW) {
  
    //0 = radio OFF
    digitalWrite(RELAY_SW_CHICKEN_RADIO,LOW);
    // MP3player stop
    myDFPlayer.stop();

  	if (gbl_music_mode == gbl_nbr_playlists){
			gbl_music_mode = -1;
		} else {
			gbl_music_mode += 1;
		}

		music_voice_notify(myDFPlayer);
  }	


  if (btn_music_on_off == LOW) {
    turn_music_on_or_off();  	
    delay(2000);
	}

	
	RF_switch = digitalRead(RF_SWITCH_REMOTE_LOCAL);
	// toggle changed
	if (gbl_rf_vs_local != RF_switch)	{
		gbl_rf_vs_local =	RF_switch;
		RF_vs_Local_Toggle_voice_notify(myDFPlayer);
	}

  auto_chicken_switch = digitalRead(SW_AUTO_CHICKEN);
  auto_chicken_fwd_rvrse = digitalRead(SW_AUTO_CHICKEN_STRT_FWD_RVRSE);

	if ((last_checked_switch_auto_chicken != auto_chicken_switch) or (last_checked_switch_auto_chicken_fwd_rvrse != auto_chicken_fwd_rvrse))
	{ // switch has changed
		// assign new value and reset counter
		last_checked_switch_auto_chicken = auto_chicken_switch; 
    last_checked_switch_auto_chicken_fwd_rvrse = auto_chicken_fwd_rvrse; 
    auto_chicken_last_move_time = current_time;
		auto_chicken_mvmnt_count= 0;
    if (auto_chicken_fwd_rvrse == LOW)  {
      auto_chicken_mvmnt_direction = 1;
    } else {
      auto_chicken_mvmnt_direction = 0;
    }
		Autochicken_toggle_voice_notify(myDFPlayer);
	}

  milliseconds_passed = (current_time - auto_chicken_last_move_time);

  if (auto_chicken_switch == LOW) {
    
    if (milliseconds_passed > AUTO_CHICKEN_MVMNT_INTERVAL) {
      auto_chicken_last_move_time = current_time;
      if (auto_chicken_mvmnt_direction == 0) {
        // forward
        auto_chicken_mvmnt_count = auto_chicken_mvmnt_count + 1;
        auto_chicken_move();
        if (auto_chicken_mvmnt_count >= AUTO_CHICKEN_MVMNT_COUNT) {
          auto_chicken_mvmnt_count = 0;
          auto_chicken_mvmnt_direction = 1;
        }
      } // if (auto_chicken_mvmnt_direction == 0)
      else {
        // reverse
        auto_chicken_mvmnt_count = auto_chicken_mvmnt_count + 1;
        auto_chicken_move();
        if (auto_chicken_mvmnt_count >= AUTO_CHICKEN_MVMNT_COUNT) {
          auto_chicken_mvmnt_count = 0;
          auto_chicken_mvmnt_direction = 0;
        }
      } // if (auto_chicken_mvmnt_direction == 1)
      
    } //   if (milliseconds_passed > AUTO_CHICKEN_MVMNT_INTERVAL) {
    
  } //  if (auto_chicken_switch == LOW) {

  if (RF_switch == LOW) {
		//Serial.println("Loop:(RF_switch == LOW)");
    if (radio.available()) {
			// Serial.println("Loop:radio.available():Start");
      char RF_msg_array[RF_msg_len] = "";
      radio.read(&RF_msg_array, RF_msg_len);
      String RF_MSG = RF_msg_array; 

			//int chicken_radio_on_off = 0; //0 = radio OFF, 1 = radio ON
			if (RF_MSG == "radio:on+")
			{
        //if radio already on don't call again
        if (chicken_radio_on_off != 1) {
  				chicken_radio_on_off = 1;
          turn_music_on_or_off();
          //delay(3000);
        }
			} else if (RF_MSG == "radio:off") {
        //if radio already off don't call again
        if (chicken_radio_on_off != 0) {
          chicken_radio_on_off = 0;
          turn_music_on_or_off();
          //delay(3000);
        }
			} else {

				int RF_MSG_xi = RF_MSG.indexOf(' ',RF_MSG.indexOf(' ')+1);
				int RF_MSG_xj = RF_MSG.indexOf(' ',RF_MSG_xi+1);
				String RF_msg_Actuator = RF_MSG.substring(RF_MSG_xi+1,RF_MSG_xj); 
				int RF_MSG_yi = RF_MSG.indexOf(' ',RF_MSG_xj+1);
				String RF_msg_Motor = RF_MSG.substring(RF_MSG_yi+1,RF_MSG.indexOf(' ',RF_MSG_yi+2)); 
				
				if (RF_msg_Actuator == "XS:0") {
					RF_xspeed = 0;
				} else {
					RF_xspeed = RF_msg_Actuator.substring(RF_msg_Actuator.indexOf(':')+1).toInt();
					RF_xdir = RF_msg_Actuator.substring(0,RF_msg_Actuator.indexOf(':'));
				}
		
				if (RF_msg_Motor == "YS:0") { //YS:0
					RF_yspeed = "0";
				} else {
					RF_yspeed = RF_msg_Motor.substring(RF_msg_Motor.indexOf(':')+1);
					RF_ydir = RF_msg_Motor.substring(0,RF_msg_Motor.indexOf(':'));
				}

				int y =  RF_yspeed.toInt(); 
				if (y > 255) {
					y = 255;
				}      
				if (RF_ydir == "YF") {
					//Serial.println("Forward" + RF_yspeed);
					Forward(y);
				} else {
					if (RF_ydir == "YR") {
						//Serial.println("Reverse" + RF_yspeed);
						Reverse(y);    
					} else {
							stopmotor();      
							//Serial.println("stopmotor()");
					}
				}

				if (RF_xdir == "XR" and RF_xspeed  > 200)  {
					TurnRight(255); //speed
				} else {
					if (RF_xdir == "XL" and RF_xspeed  > 200) {
						TurnLeft(255); //speed    
					} else {
							stopturn();
							previous_turn_action = 0;
							current_turn_action = 0; 
					}
				}

			}  

    //Serial.println("Loop:radio.available():End");
    }  // else Serial.println("no radio detected");

      
	}		//  if (RF_switch == LOW) {
	else {
		//Serial.println("Loop:(RF_switch == HIGH)");
		// RF_switch == HIGH.... on board Joystick inputs
		
		Local_xValue = analogRead(joyX);
		Local_yValue = analogRead(joyY);
		local_yspeed = calc_speed(Local_yValue); // 0 = stopped, 255 = max speed
    
		if (Local_yValue > 540) {
      Forward(local_yspeed);
  	} else {
			if (Local_yValue < 500) {
        Reverse(local_yspeed);    
  		} else {
				stopmotor();      
			}
		}

		if (Local_xValue > 700)  {
			TurnRight(255); //speed
		} else {
			if (Local_xValue < 300) {
				TurnLeft(255); //speed    
			} else {
				stopturn();
        previous_turn_action = 0;
        current_turn_action = 0; 
			}
		}

	} //  if ((RF_remote_swtch == 0) 

	//Serial.println("Loop:End");
} // loop

void turn_music_on_or_off() {
  //int gbl_chicken_music_on_off = 0; //0 = music OFF, 1 = music ON
  if (gbl_chicken_music_on_off == 0) {
    gbl_chicken_music_on_off = 1;
  } else {
    gbl_chicken_music_on_off = 0;
  }

  if (gbl_chicken_music_on_off == 0)  {
    //0 = music OFF
    digitalWrite(RELAY_SW_CHICKEN_RADIO,LOW);
    myDFPlayer.stop();
  } else {
    
    //1 = music ON
    //int gbl_music_mode = 0; -1 = no music; 0 = Radio; 1=playlist #1; 2=playlist #2; 3=playlist #3.... gbl_nbr_playlists
    if (gbl_music_mode == -1) {
      //0 = radio OFF
      digitalWrite(RELAY_SW_CHICKEN_RADIO,LOW);
      // MP3player stop
      myDFPlayer.stop();

    } else if (gbl_music_mode == 0) {
      //1 = radio ON
      digitalWrite(RELAY_SW_CHICKEN_RADIO,HIGH);
      // MP3player stop
      myDFPlayer.stop();
    } else {
      //0 = radio OFF
      digitalWrite(RELAY_SW_CHICKEN_RADIO,LOW);
      // Play the selected playlist
      myDFPlayer.volume(volume_range_array[gbl_music_volume_idx]);  
      myDFPlayer.loopFolder(gbl_music_mode); //1=playlist #1; 2=playlist #2; 3=playlist #3.... gbl_nbr_playlists
    }
  }
}


int auto_chicken_move()
{
  //int auto_chicken_mvmnt_direction = 0; //0 = Forward, 1 = Reverse
  if (auto_chicken_mvmnt_direction == 0) {
    //Serial.println("Forward");
    Forward(100);
		delay(1000);    
		Forward(255);
		delay(AUTO_CHICKEN_MVMNT_DURATION);
		stopmotor();      
  } else {
    //Serial.println("Reverse");
		Reverse(100);
		delay(1000);    
		Reverse(255);
    delay(AUTO_CHICKEN_MVMNT_DURATION);
		stopmotor();      
   }
  return 0;
}

void Forward(int parm) {
	// less than 99 don't bother moving
	if (parm > 99)	{
		digitalWrite(MOTOR_IN1,HIGH);
		digitalWrite(MOTOR_IN2,LOW);
		analogWrite(MOTOR_PWM,parm);
	}
}

void Reverse(int parm) {
	// less than 99 don't bother moving
	if (parm > 99)	{
		digitalWrite(MOTOR_IN1,LOW);
		digitalWrite(MOTOR_IN2,HIGH);
		analogWrite(MOTOR_PWM,parm);
	}
}

void TurnRight(int parm) {
	current_turn_action = 1; 
	millisec_of_current_turn= millis();

	if (previous_turn_action == -1 ){ // 0 = stop, -1 = left, 1 = right
		// this means actuator has been turning left since millisec_of_previous_turn
		turn_position_milliseconds = turn_position_milliseconds - (millisec_of_current_turn - millisec_of_previous_turn);

	} else if (previous_turn_action == 1 ){ // 0 = stop, -1 = left, 1 = right

		// this means actuator has been turning right since millisec_of_previous_turn
		turn_position_milliseconds = turn_position_milliseconds + (millisec_of_current_turn - millisec_of_previous_turn);
	}

	if (turn_position_milliseconds < millisec_turn_range ){
		digitalWrite(ACTUATOR_DIR,HIGH);
		analogWrite(ACTUATOR_PWM,parm);
		previous_turn_action = current_turn_action;
		millisec_of_previous_turn = millis();
	} else {
		previous_turn_action = 0;
		millisec_of_previous_turn = millis();
	}

	//Serial.print("turn_position_milliseconds: ");
	//Serial.println(turn_position_milliseconds);

}

void TurnLeft(int parm) {
	current_turn_action = -1; 
	millisec_of_current_turn= millis();

	if (previous_turn_action == -1 ){ // 0 = stop, -1 = left, 1 = right
		// this means actuator has been turning left since millisec_of_previous_turn
		turn_position_milliseconds = turn_position_milliseconds - (millisec_of_current_turn - millisec_of_previous_turn);

	} else if (previous_turn_action == 1 ){ // 0 = stop, -1 = left, 1 = right

		// this means actuator has been turning right since millisec_of_previous_turn
		turn_position_milliseconds = turn_position_milliseconds + (millisec_of_current_turn - millisec_of_previous_turn);
	}

	if (turn_position_milliseconds > (millisec_turn_range * -1) ){
		digitalWrite(ACTUATOR_DIR,LOW);
		analogWrite(ACTUATOR_PWM,parm);

		previous_turn_action = current_turn_action;
		millisec_of_previous_turn = millis();
	} else {
		previous_turn_action = 0;
		millisec_of_previous_turn = millis();
	}

	Serial.print("turn_position_milliseconds: ");
	Serial.println(turn_position_milliseconds);

}

void stopturn() {
  analogWrite(ACTUATOR_PWM,0);
}

void stopmotor() {
  analogWrite(MOTOR_PWM,0);
}

int calc_speed(int parm) {
  float x,y;
  if (parm > 510 && parm < 530) {
    x = 0;
    y = 0;
  } else if (parm >= 530) {
    x = parm - 493;
    y = (x/493)*255;
  } else {
    x = 510 - parm;
    y = (x/510)*255;
  }
  
  if (parm == 1023) {
    y = 255;
  }

  if (y > 200) {
    y = 255;
  }

  return y;
}

void erase(void)
{
  for (int i = 0 ; i < EEPROM.length() ; i++)
  EEPROM.write(i, 0);
}

void printMessage(byte* first, size_t len)
{
  for (int i = 0; i < len; i++)
  {
  Serial.print((char)first[i]);
  }
}

void writeMsg(byte* first, size_t len)
{
  for(int i = 0; i < len; i++)
  {
  EEPROM.write(i, first[i]);
  }
}

void readMsg(size_t len)
{
  byte res;
 
  Serial.print("Message: ");
  for(int i = 0; i < len; i++)
  {
  res = EEPROM.read(i);
  Serial.print((char)res);
  }
  Serial.println("");
}


void printDetail(uint8_t type, int value){
  switch (type) {
    case TimeOut:
      Serial.println(F("Time Out!"));
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!"));
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Card Online!"));
      break;
    case DFPlayerPlayFinished:
      Serial.print(F("Number:"));
      Serial.print(value);
      Serial.println(F(" Play Finished!"));
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError:"));
      switch (value) {
        case Busy:
          Serial.println(F("Card not found"));
          break;
        case Sleeping:
          Serial.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          Serial.println(F("Get Wrong Stack"));
          break;
        case CheckSumNotMatch:
          Serial.println(F("Check Sum Not Match"));
          break;
        case FileIndexOut:
          Serial.println(F("File Index Out of Bound"));
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File"));
          break;
        case Advertise:
          Serial.println(F("In Advertise"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
}
