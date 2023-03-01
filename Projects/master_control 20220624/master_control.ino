/*arduino:avr:uno

  (NRF24L01) Wireless Module Transceiver 

  | <blank> | 11  | 10 |  +3vdc |
  |---------|-----|----|---------
  |   12    | 13  | 9  |  GND   |

  | <blank> | MOSI  | CSN | +3vdc |
  |---------|-------|-----|--------
  |   MISO  | SCK   | CE  | GND   |


Menus
===============================

  iMenu_level=0
		- Beetle
				- Back
				- Joystick
				- Power 100
				- Power 150
				- Power 200
				- Power 250
				- Snd Hello   
				- Snd Buzzer Test
				- What Version
				- Download Status
     - Defender 
         iMenu_level=1
				- -Back
				- -Snd Hello
				- -Snd LED Test
				- -Snd Buzzer test
				- -Snd Mute Speaker
				- -Snd Spkr vol 5       
				- -Snd Spkr vol 10
				- -Snd Spkr vol 20
				- -Snd Spkr vol 30
				- -Snd Play Growl
				- -Snd Play Owls
				- -What Version
				- -Download Status
     
     - View RF Traffic 
		 - Chicken Car
     

analogue joystick 

VRx  -> A3 (Analogue input required for speed control)
VRy  -> A4 (Analogue input required for speed control)
SW   -> not used  *Used when joystick is clicked

  
*/

#include "rotate_coords.h"
#include <LiquidCrystal.h>

#include <nRF24L01.h>
#include <RF24.h>

#define joyY A3		//  0 = min value, center about 520, 1023 = max value
#define joyX A4		//  0 = min value, center about 520, 1023 = max value

#define NRF24L01_CE 9
#define NRF24L01_CSN 10
#define NRF24L01_MOSI 11 
#define NRF24L01_MISO 12
#define NRF24L01_SCK 13

#define LT_BUTTON 8
#define RT_BUTTON A0

String Device_Name = "Master Control";
String Version = "20220624";

String RF_Mode = ""; // TX for transmit or RX for receive

RF24 radio(NRF24L01_CE, NRF24L01_CSN); // CE, CSN

const int NUMBER_OF_ELEMENTS = 10;

String info[NUMBER_OF_ELEMENTS];

const byte address[6] = "00001";      // radio channel
int RF_len = 50;

#define iMode_max 4
String iMode_names[iMode_max] = {
		"Beetle"									// iMode == 0
    ,"Defender"               // iMode == 1
    ,"View RF Traffic"        // iMode == 2
		,"Chicken Car"						// iMode == 3
		};


#define iMode_chickencar_mnu_max 2
String chickencar_mnu_names[iMode_chickencar_mnu_max] = {
    "Back"        
		,"What Version"
		};

#define iMode_beetle_mnu_max 4
String beetle_mnu_names[iMode_beetle_mnu_max] = {
    "Back"        
		,"Joystick"
    ,"Snd Buzzer Test"        
		,"What Version"
		};

#define iMode_defender_mnu_max 12
String defender_mnu_names[iMode_defender_mnu_max] = {
    "Back"        
    ,"Snd LED Test"        
		,"Snd Buzzer Test"
    ,"Snd Mute Speaker"        
    ,"Snd Spkr vol 5"        
    ,"Snd Spkr vol 10"        
    ,"Snd Spkr vol 20"        
    ,"Snd Spkr vol 30"        
    ,"Snd Play Growl"                
    ,"Snd Play Owls"           
		,"What Version"
    ,"Download Status"                
    };


int iMode = iMode_max;
int iMenu_level = 0;
int info_idx = -1;
int info_idx_max = 0;
int iMode_beetle_mnu = 0;						//  iMode == 0 
int iMode_defender_mnu = 0;					//  iMode == 1 
int iMode_view_rf_traffic = 0;      //  iMode == 2 
int iMode_chickencar_mnu = 0;				//  iMode == 3 

int Download_From_Remote_Device = 0;


int iDiagnostic = 0;
const int iDiagnostic_max = 23;

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
#define LCD_D7 2
#define LCD_D6 3
#define LCD_D5 4
#define LCD_D4 5
#define LCD_EN 6
#define LCD_RS 7

//const int rs = 7, en = 6, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
//LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

int iJoystick_control = 0;
String Previous_RF_Msg = "";
int irepeat_send = 0;
int imax_repeat_send = 5;

void setup() {
  Serial.begin(9600);

	randomSeed(analogRead(A1)); 

  pinMode(LT_BUTTON,INPUT_PULLUP);
  pinMode(RT_BUTTON,INPUT_PULLUP);
  
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  startup_msg();
}


void loop() {

  int ltbtn = 0, rtbtn = 0;
  ltbtn = digitalRead(LT_BUTTON);
  rtbtn = digitalRead(RT_BUTTON);
  String msg = ""; 

	if ((iMode_view_rf_traffic == -1) and (radio.available())) {
		char RF_array[RF_len] = "";
		radio.read(&RF_array, RF_len);
		String RF = RF_array; 
		ShowMessage(RF);
		delay(300);
	}
		
	if (ltbtn == LOW) { // LT_BUTTON
		if (iMenu_level == 0) {
			reset_submenus();
			
			iMode = iMode + 1; // cycle iMode
			if (iMode > (iMode_max-1)) {
				iMode = 0;
			}
			current_msg();
			delay(500);
		} else if ((iMenu_level == 1 and iMode == 0)) { // "Beetle" is iMode 0
			iMode_beetle_mnu = iMode_beetle_mnu + 1; // cycle menu items for beetle using iMode_beetle_mnu
			if (iMode_beetle_mnu > (iMode_beetle_mnu_max-1)) {
				iMode_beetle_mnu = 0;
			}
			ShowMessage_line1(iMode_names[iMode]);
			ShowMessage_line2(beetle_mnu_names[iMode_beetle_mnu]);

			delay(500);
		
		} else if ((iMenu_level == 1 and iMode == 1)) { // "Defender" is iMode 1
			iMode_defender_mnu = iMode_defender_mnu + 1; // cycle menu items for defender using iMode_defender_mnu
			if (iMode_defender_mnu > (iMode_defender_mnu_max-1)) {
				iMode_defender_mnu = 0;
			}
			ShowMessage_line1(iMode_names[iMode]);
			ShowMessage_line2(defender_mnu_names[iMode_defender_mnu]);

			delay(500);
			
		} else if ((iMenu_level == 1 and iMode == 2)) { // "View RF Traffic" is iMode == 2
			// if they hit Left button ... turn off RF traffic monitor
			reset_submenus();
			iMode = 0;
			iMenu_level = 0;
			radio.stopListening(); // put radio in TX mode
			current_msg();
			delay(500);

		} else if ((iMenu_level == 1 and iMode == 3)) { // "Chicken Car" is iMode 3
			iMode_chickencar_mnu = iMode_chickencar_mnu + 1; // cycle menu items for Chicken Car using iMode_chickencar_mnu
			if (iMode_chickencar_mnu > (iMode_chickencar_mnu_max-1)) {
				iMode_chickencar_mnu = 0;
			}
			ShowMessage_line1(iMode_names[iMode]);
			ShowMessage_line2(chickencar_mnu_names[iMode_chickencar_mnu]);

			delay(500);

		}

	}


	if (rtbtn == LOW) { // RT_BUTTON
			if (iMode == 0) { // beetle
				if (iJoystick_control == -1) { // in joystick mode
					RF_send("Spin");

				} else { // not in joystick mode
					if (iMenu_level == 1) {
						if (beetle_mnu_names[iMode_beetle_mnu] == "Back") {
							reset_submenus();
							iMode = 0;
							iMenu_level = 0;
							current_msg();
						} else if (beetle_mnu_names[iMode_beetle_mnu] == "Joystick") {
							iJoystick_control = -1;
							iMode = 0;
							iMenu_level = 0;

						} else { // a send command was selected
							if (beetle_mnu_names[iMode_beetle_mnu] == "Snd Buzzer Test") {
								RF_send("Buzzer Test");
							} else if (beetle_mnu_names[iMode_beetle_mnu] == "Download Status") {
								RF_send("status");
							} else if (beetle_mnu_names[iMode_beetle_mnu] == "What Version") {
								RF_send("What Version");
							} else if (beetle_mnu_names[iMode_beetle_mnu] == "Power 100") {
								RF_send("Power 100");
							} else if (beetle_mnu_names[iMode_beetle_mnu] == "Power 150") {
								RF_send("Power 150");
							} else if (beetle_mnu_names[iMode_beetle_mnu] == "Power 200") {
								RF_send("Power 200");
							} else if (beetle_mnu_names[iMode_beetle_mnu] == "Power 250") {
								RF_send("Power 250");
							}
							iMode_view_rf_traffic = -1;
						}
					} else {
						iMenu_level=1;
						iMode_beetle_mnu=1;
						ShowMessage_line1(iMode_names[iMode]);
						ShowMessage_line2(beetle_mnu_names[iMode_beetle_mnu]);
					}
				} // not in joystick mode
				delay(500);
			} else if (iMode == 1) { // defender
				if (iMenu_level == 1) {
					if (defender_mnu_names[iMode_defender_mnu] == "Back") {
						reset_submenus();
						iMode = 0;
						iMenu_level = 0;
						current_msg();
					} else { // a send command was selected
						if (defender_mnu_names[iMode_defender_mnu] == "Snd LED Test") {
							RF_send("LED Test");
						} else if (defender_mnu_names[iMode_defender_mnu] == "Snd Buzzer Test") {
							RF_send("Buzzer Test");
						} else if (defender_mnu_names[iMode_defender_mnu] == "Snd Mute Speaker") {
							RF_send("Mute Speaker");
						} else if (defender_mnu_names[iMode_defender_mnu] == "Snd Spkr vol 5") {
							RF_send("Spkr vol 5");
						} else if (defender_mnu_names[iMode_defender_mnu] == "Snd Spkr vol 10") {
							RF_send("Spkr vol 10");
						} else if (defender_mnu_names[iMode_defender_mnu] == "Snd Spkr vol 20") {
							RF_send("Spkr vol 20");
						} else if (defender_mnu_names[iMode_defender_mnu] == "Snd Spkr vol 30") {
							RF_send("Spkr vol 30");
						} else if (defender_mnu_names[iMode_defender_mnu] == "Snd Play Growl") {
							RF_send("Play Growl");
						} else if (defender_mnu_names[iMode_defender_mnu] == "Snd Play Owls") {
							RF_send("Play Owls");
						} else if (defender_mnu_names[iMode_defender_mnu] == "Download Status") {
							RF_send("status");
						} else if (defender_mnu_names[iMode_defender_mnu] == "What Version") {
							RF_send("What Version");
						}
						iMode_view_rf_traffic = -1;

					}
				} else {
					iMenu_level=1;
					ShowMessage_line1(iMode_names[iMode]);
					ShowMessage_line2(defender_mnu_names[iMode_defender_mnu_max-1]);
				}
				delay(500);

			} else if (iMode == 2) {
				// turn on RF monitor
				//iMenu_level = 0;
				iMode_view_rf_traffic = -1;  
				iMenu_level = 1;
				radio.startListening(); // put radio in RX mode

			} else if (iMode == 3) {// "Chicken Car" is iMode == 3
				if (iMenu_level == 1) {
					if (chickencar_mnu_names[iMode_chickencar_mnu] == "Back") {
						reset_submenus();
						iMode = 0;
						iMenu_level = 0;
						current_msg();
					} else { // a send command was selected
						if (chickencar_mnu_names[iMode_chickencar_mnu] == "What Version") {
							RF_send("CC:What Version");
							iMode_view_rf_traffic = -1;
							delay(500);
						}
					}
				} else {
					iMode_chickencar_mnu=1;
					iMenu_level=1;
					ShowMessage_line1(iMode_names[iMode]);
					ShowMessage_line2(chickencar_mnu_names[iMode_chickencar_mnu_max-1]);
				}
				delay(500);
			}
	
	}

	if (iJoystick_control == -1) {
		int xValue = 0, yValue = 0; 
		String RF = "";
		char text[50];

		// Joystick inputs
		xValue = analogRead(joyX);
		yValue = analogRead(joyY);

		RF = GetJoystickMsg(xValue, yValue);
    RF.toCharArray(text, 50);
  
    if (Previous_RF_Msg == RF) {
      if (irepeat_send <= (imax_repeat_send+2)) {
        irepeat_send += 1;    
      }
      
    } else {
  
      Previous_RF_Msg = RF;
      irepeat_send = 0;
    }

    if (irepeat_send < (imax_repeat_send)) {
      set_TX_mode();
      radio.write(&text, sizeof(text));
			ShowMessage(RF);
			delay(300);
    } else {
      set_RX_mode();
    }

		//Serial.println(RF);

	} 

} // loop

String GetJoystickMsg(int joy_x, int joy_y) {
		int xSpeed = 0, ySpeed = 0;
		String joyRFmsg = "Remote RF ";

		xSpeed = calc_speed(joy_x); // 0 = stopped, 255 = max speed
		ySpeed = calc_speed(joy_y); // 0 = stopped, 255 = max speed

		if (joy_x > 540) {
			joyRFmsg +=  "XL:" + String(xSpeed, DEC); 
		} else {
			if (joy_x < 500) {
				joyRFmsg += "XR:" + String(xSpeed, DEC);
			} else {
				joyRFmsg += "XS:0";
			}
		}

		joyRFmsg += " and ";
		
		if (joy_y > 540) {
			joyRFmsg += "YF:" + String(ySpeed, DEC);
		} else {
			if (joy_y < 500) {
				joyRFmsg += "YR:" + String(ySpeed, DEC);
			} else {
				joyRFmsg += "YS:0";
			}
		}
		return joyRFmsg;

}
void RF_send(String RFMsgtoSend) {
		ShowMessage_line1("Sending ");
		ShowMessage_line2(RFMsgtoSend);
		delay(250);
		
		char text[RF_len];
		RFMsgtoSend.toCharArray(text, RF_len);

		set_TX_mode();
		radio.write(&text, sizeof(text));
		set_RX_mode();
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
void ShowInfo() {
  String M_line1 = info[info_idx].substring(0,info[info_idx].indexOf("="));
  String M_line2 = info[info_idx].substring(info[info_idx].indexOf("=")+1);

  ShowMessage_line1(M_line1);
  ShowMessage_line2(M_line2);
}

String get_diagnostic(int p) {
  String M = "0=0 diagnostic_array[iDiagnostic]";
  String M_1 = M.substring(0,M.indexOf("="));
  String M_2 = M.substring(M.indexOf("=")+1);
  if (p == 0) {
    return M_1;
  } else {
    return M_2;
  }
}

void ShowMessage_line1(String msg) {
  lcd.setCursor(0, 0);
  for (int i=0; i< 16; i++) {
    if (msg.length() > i) {
      lcd.print(msg[i]);
    } else {
      lcd.print(" ");
    }
  }
}

void ShowMessage_line2(String msg) {
  lcd.setCursor(0, 1);
  for (int i=0; i< 16; i++) {
    if (msg.length() > i) {
      lcd.print(msg[i]);
    } else {
      lcd.print(" ");
    }
  }
}


void ShowMessage(String msg) {

  lcd.setCursor(0, 0);
  for (int i=0; i< 16; i++) {
    if (msg.length() > i) {
      lcd.print(msg[i]);
    } else {
      lcd.print(" ");
    }
  }

  lcd.setCursor(0, 1);
  for (int i=16; i< 32; i++) {
    if (msg.length() > i) {
      lcd.print(msg[i]);
    } else {
      lcd.print(" ");
    }
  }

}


void Reveal(String msg) {

  lcd.setCursor(0, 0);
  for (int i=0; i< 16; i++) {
    if (msg.length() > i) {
      lcd.print(msg[i]);
      delay(200);
    } else {
      lcd.print(" ");
    }
  }

  lcd.setCursor(0, 1);
  for (int i=16; i< 32; i++) {
    if (msg.length() > i) {
      lcd.print(msg[i]);
      delay(200);
    } else {
      lcd.print(" ");
    }
  }

}


int calc_speed(int parm) {
  float x,y;
  if (parm > 510 && parm < 530) {
    x = 0;
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
  return y;
}


void reset_submenus() {
  iMode_view_rf_traffic = 0; 
  iMode_defender_mnu = 0;
  Download_From_Remote_Device = 0;
	iJoystick_control = 0;
}

void current_msg(){
	String msg = "";
	msg.concat("iMode ");
	msg.concat(iMode);
	ShowMessage_line1(msg);
	ShowMessage_line2(iMode_names[iMode]);
}

void startup_msg(){
  ShowMessage_line1(Device_Name);
  ShowMessage_line2(Version);
}
