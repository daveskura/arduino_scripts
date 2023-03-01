
/* 
  Dave Skura
  Jan 29,2022
  
  I want to setup timers to go off at different intervals and set off an action for a period of time in each case.

  using DS1302 for a clock.

pseudocode

init
  default variables
  get values from stored variables on chip and assign to variables

loop
  The looping logic is passing through one of the following:

  1) it's midnight, assign daily actions true/false.  update bool array iactions_to_complete_today[]
	2) all actions have happened today already
  3) an action is running, waiting to complete
  4) no action is currently happening, waiting to start an action


  Nano connections (uno and mega are different serial pins than nano):
     DS1302 CLK/SCLK --> 3
     DS1302 DAT/IO --> 2
     DS1302 RST/CE --> 4
     DS1302 VCC --> 5v
     DS1302 GND --> GND

LiquidCrystal Library 

The C1602A-V1.2 circuit:

		VSS  = GND 
		VDD  = 5v
		VO   =  <- 10k pentiometer <- 5v
		RS   = 7
		RW   = GND
		E    = 6
		D0   = nothing
		D1   = nothing
		D2   = nothing
		D3   = nothing
		D4   = 5 
		D5   = 10
		D6   = 9
		D7   = 8 

		BL+   -> <- 1k resistor <- 5v 
		BL-   -> GND


Nano Arduino Pinout:
	Nano D2 -> DS1302 DAT/IO 
	Nano D3 -> DS1302 CLK/SCLK 
	Nano D4 -> DS1302 RST/CE 
	Nano D5 -> LCDC1602A D4
	Nano D6 -> LCDC1602A E
	Nano D7 -> LCDC1602A RS
	Nano D8 -> LCDC1602A D7
	Nano D9 -> LCDC1602A D6
	Nano D10 -> LCDC1602A D5
*/


#define DEBUG_ON true
#define LCD_ON true
#define BUTTON_1 A2
#define BUTTON_2 A1
#define BUTTON_3 A0
#define PUMP1 A5

String PROGRAM = "The Timer Box";
String VERSION = "1.12";
bool BOOL_TEST = false;

#include<LiquidCrystal.h>
#include <ThreeWire.h>  
#include <RtcDS1302.h>
#include <EEPROM.h>

int CLK_SCLK=3;
int IO_DAT=2;
int RST_CE=4;

int icurrent_selection_lvl = 1;
int icurrent_main_selection = -1;
int icurrent_sub_selection = -1;
int imain_selection_max = 7;
int icurrent_sub_selection_max = 5;

const char *szmain_selections[] = {
    "test"  
    ,"Daily"  
    ,"Every Second Day"  
    ,"Every Third Day"  
    ,"Once a week"  
    ,"Time"             
    ,"Version"         
    };

const char *sztimer_selections[] = {
    "Manual"  
    ,"Start Hour (eg 13 for 1pm)"  
    ,"Start Minute (eg 15 for 1:15pm)"  
    ,"Duration (seconds)"  
    ,"Power (0-5)"             
    };

struct my_eeprom_object {
  long ts64_once_a_day_lr = 0;// timestamp of last run of this action
  long ts64_every_second_day_lr = 0;// timestamp of last run of this action
  long ts64_every_third_day_lr = 0;// timestamp of last run of this action
  long ts64_once_a_week_lr = 0;// timestamp of last run of this action
};

struct my_eeprom_object eeprom_obj;

uint32_t iseconds_in_day = 86400;

int iactions_hour[]								= {12,	12,	12,	12};// the hour to start the action each day
int iactions_min_start[]					= {0,	0,	0,	0};	// the start of the window in minutes, for running actions each day
int iactions_expected_durations[] = {5,		5,	5,	5};// how many seconds the action will take to complete.
int iactions_power[]							= {1,		2,	3,	5};// action power.
bool iactions_manual[]						= {false,		false,	false,	false};// manually on or off.

bool iactions_to_complete_today[] = {
        false, // "Once a day" 
        false, // "Every Second Day"
        false, // "Every Third Day"
        false, // "Once a week"
      };      // true/false to indicate if actions have run cycles for the current day

int iaction_max = 4;
const char *iMacro_Cycle_names[] = {
    "Once a day"        // iaction = 0
    ,"Every Second Day" // iaction = 1
    ,"Every Third Day"  // iaction = 2
    ,"Once a week"      // iaction = 3
    };

int icurrently_running_action_nbr = -1;  // currently running action [0..3] or -1 for no action running.
int iactions_duration_seconds[] = {0, 0, 0,  0};    // how many seconds have passed since the start of the action.
bool iactions_being_done[]      = {false,false,false,false};      // true/false to indicate if action is currently happening

uint32_t  tms_actions_started_on[]={0,0,0,0}; // TotalSeconds64() from when the action started.


ThreeWire myWire(IO_DAT,CLK_SCLK,RST_CE); // IO, SCLK, CE
RtcDS1302<ThreeWire> Rtc(myWire);

const int rs = 7, en = 6, d4 = 5, d5 = 10, d6 = 9, d7 = 8;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

uint32_t isecs_half_day = 0.5 * iseconds_in_day;
uint32_t isecs_day_and_half = 1.5 * iseconds_in_day;
uint32_t isecs_2day_and_half = 2.5 * iseconds_in_day;
uint32_t isecs_week_and_half = 6.5 * iseconds_in_day;

#define MOTOR_B_PWM 11 // Motor B PWM Speed
#define MOTOR_B_DIR 12 // Motor B Direction
#define PUMPOUT LOW

int imotor_power = 0;

void setup () 
{
    if (DEBUG_ON) {
      Serial.begin(9600);
    }
    pinMode( MOTOR_B_DIR, OUTPUT );
    pinMode( MOTOR_B_PWM, OUTPUT );
    
    digitalWrite( MOTOR_B_DIR, PUMPOUT );
    //analogWrite( MOTOR_B_PWM, 100 );
    //analogWrite( MOTOR_B_PWM, 0 );
    
    pinMode(BUTTON_1,INPUT_PULLUP);
    pinMode(BUTTON_2,INPUT_PULLUP);
    pinMode(BUTTON_3,INPUT_PULLUP);
    
    lcd.begin(16, 2);
    ShowMessage(PROGRAM + "   version " + VERSION);
      
    Rtc.Begin();
    RtcDateTime clocktime = Rtc.GetDateTime();
    RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);

    if (!Rtc.IsDateTimeValid()) {
      ShowMessage("DS1302 failing. chk battery.");
      Rtc.SetDateTime(compiled);
      exit(0);
    }
    
    read_eeprom();

    if (DEBUG_ON) {
      Serial.print(eeprom_obj.ts64_once_a_day_lr);
      Serial.println("");
      Serial.print(eeprom_obj.ts64_every_second_day_lr);
      Serial.println("");
      Serial.print(eeprom_obj.ts64_every_third_day_lr);
      Serial.println("");
      Serial.print(eeprom_obj.ts64_once_a_week_lr);
      Serial.println("");
    }
    switchon_todays_actions(clocktime);

    if (DEBUG_ON) {
      for (int i=0; i < 4; i++) {
        Serial.print("iactions_to_complete_today[");
        Serial.print(i);
        Serial.print("]=");
        if (iactions_to_complete_today[i])
          Serial.println("true");
         else
          Serial.println("false");
         
      }
    }
}


void loop () 
{
    RtcDateTime now = Rtc.GetDateTime();
		uint32_t current_ts64 = now.TotalSeconds64();
  /*
  The looping logic is passing through one of the following:

  A button is pressed
    Left button
    Right Button 1
    Right Button 2
    
  or 
  
  1) it's midnight, assign daily actions true/false.  update bool array iactions_to_complete_today[]
	2) all actions have happened today already
  3) an action is running, waiting to complete
  4) no action is currently happening, waiting to start an action

  */
    if (digitalRead(BUTTON_1) == LOW) {
			if (icurrent_selection_lvl == 1) {// when browsing lvl 1... left botton shows next selection from main

				icurrent_main_selection += 1;
				if (icurrent_main_selection >= imain_selection_max) {
					icurrent_main_selection = 0 ;
				}
				ShowMessage(szmain_selections[icurrent_main_selection]);
				delay(250);
			} else if (icurrent_selection_lvl == 2)	{
				icurrent_sub_selection += 1;
				if (icurrent_sub_selection >= icurrent_sub_selection_max) {
					icurrent_sub_selection = 0 ;
				}
				String logmsg = szmain_selections[icurrent_main_selection];
				logmsg += "; ";
				logmsg += sztimer_selections[icurrent_sub_selection];
				ShowMessage(logmsg);
				delay(250);

			}     
    } else if (digitalRead(BUTTON_2) == LOW) {

			if (icurrent_selection_lvl == 1) // when browsing lvl 1... pressing select button means go to lvl 2
			{
				if (szmain_selections[icurrent_main_selection] == "Daily" or 
					szmain_selections[icurrent_main_selection] == "Every Second Day" or 
					szmain_selections[icurrent_main_selection] == "Every Third Day" or 
					szmain_selections[icurrent_main_selection] == "Once a week" )	{
					
					icurrent_selection_lvl = 2;

					icurrent_sub_selection = 0; // start at the top of the sub selection list
					String logmsg = szmain_selections[icurrent_main_selection];
         logmsg += " ";
					logmsg += sztimer_selections[icurrent_sub_selection];
					ShowMessage_line1(logmsg);
					if (iactions_manual[icurrent_sub_selection])
						ShowMessage_line2("ON");
					else
						ShowMessage_line2("OFF");
				} else if (szmain_selections[icurrent_main_selection] == "Version") {
					ShowMessage(PROGRAM + "   version " + VERSION);
					delay(250);
				} else if (szmain_selections[icurrent_main_selection] == "Time") {
          showDateTime(now);
          delay(250);
        } else if (szmain_selections[icurrent_main_selection] == "test") {
          imotor_power += 25;
          if (imotor_power > 255) {
            imotor_power = 255;
          }
          analogWrite( MOTOR_B_PWM, imotor_power );
          ShowMessage(String(imotor_power));
          delay(250);
				}

			}

    } else if (digitalRead(BUTTON_3) == LOW) {
      if (szmain_selections[icurrent_main_selection] == "test") {
        imotor_power -= 25;
        if (imotor_power < 0) {
          imotor_power = 0;
        }
        analogWrite( MOTOR_B_PWM, imotor_power );
        ShowMessage(String(imotor_power));
        delay(250);
      }
         
      
    } else {
    
      // 0) it's midnight, assign daily actions true/false.  update bool array iactions_to_complete_today[]
      if (is_midnight(now)) {
        switchon_todays_actions(now);
  
      } else if (are_any_actions_left_today()) {
  			if (is_any_action_running())	{
  				//  3) an action is running, waiting to complete
  				iactions_duration_seconds[icurrently_running_action_nbr] = (current_ts64 - tms_actions_started_on[icurrently_running_action_nbr]);
  
  				// .... it's not finished doing it's thing      
  				if (DEBUG_ON) {
            String logmsg = "Action ";
            logmsg = logmsg + (icurrently_running_action_nbr + 1);
            logmsg = logmsg + " Running ";
            if (LCD_ON) {
              ShowMessage(logmsg);
            }
  					Serial.println(logmsg);
            delay(1000);
  				}
  
  				if (is_action_done(icurrently_running_action_nbr)) {
  					// action must be done
  					iactions_to_complete_today[icurrently_running_action_nbr] = false;
  					icurrently_running_action_nbr = -1;
  				}
  
  			} else {
  		    //  4) no action is currently happening, waiting to start an action
  				bool anyaction = false;
  				if (is_time_for_action(0,now)) {
  					anyaction = true;
  				} else if (is_time_for_action(1,now)) {
  					anyaction = true;
  				} else if (is_time_for_action(2,now)) {
  					anyaction = true;
  				} else if (is_time_for_action(3,now)) {
  					anyaction = true;
  				} 
          
  				if (anyaction)	{
  					iactions_being_done[icurrently_running_action_nbr] = true;
  					tms_actions_started_on[icurrently_running_action_nbr] = current_ts64;
  					iactions_duration_seconds[icurrently_running_action_nbr] = 0;
  
  					//it's time to start the action
  					do_start_action(icurrently_running_action_nbr,now);
  				} 

          /*
          Serial.println("exit(0);");
          delay(2500);
          exit(0);    
          */
  		
  
  
  			}
  		} else {
  		// 	2) all actions have happened today already
  		  if (false) {
          String logmsg = "all actions have completed today.";
          if (LCD_ON) {
            ShowMessage(logmsg);
          }
  				Serial.println(logmsg);
  				delay(5000);
  			}
  
      }
        
      
    } // no buttons pushed
}

void printDateTime(const RtcDateTime& dt)
{
    char datestring[20];

    snprintf_P(datestring, 
            (sizeof(datestring) / sizeof(datestring[0])),
            PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
            dt.Month(),
            dt.Day(),
            dt.Year(),
            dt.Hour(),
            dt.Minute(),
            dt.Second() );
    Serial.print(datestring);
    if (LCD_ON) {
      ShowMessage(datestring);
    }
}

void showDateTime(const RtcDateTime& dt)
{
    char datestring[25];

    snprintf_P(datestring, 
            (sizeof(datestring) / sizeof(datestring[0]))+5,
            PSTR("%02u/%02u/%04u      %02u:%02u:%02u"),
            dt.Month(),
            dt.Day(),
            dt.Year(),
            dt.Hour(),
            dt.Minute(),
            dt.Second() );
    if (LCD_ON) {
      ShowMessage(datestring);
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
long get_seconds_since_last_run(int timer_number, const RtcDateTime& dt) {
  uint32_t  seconds_since_run=0;
  if (timer_number == 1) {
    seconds_since_run = dt.TotalSeconds64()- eeprom_obj.ts64_once_a_day_lr;
  } else if (timer_number == 2) {
    seconds_since_run = dt.TotalSeconds64() - eeprom_obj.ts64_every_second_day_lr;
  } else if (timer_number == 3) {
    seconds_since_run = dt.TotalSeconds64()- eeprom_obj.ts64_every_third_day_lr;
  } else if (timer_number == 4) {
    seconds_since_run = dt.TotalSeconds64()- eeprom_obj.ts64_once_a_week_lr;
  }
  return seconds_since_run;
}

long get_seconds_till_todays_run(int timer_number, const RtcDateTime& dt){
  long seconds_till_todays_run = 0;
  if (iactions_hour[timer_number] == dt.Hour()) {
      if (iactions_min_start[timer_number] > dt.Minute() ) {
        seconds_till_todays_run = (iactions_min_start[timer_number] - dt.Minute())*60;
      }
  } else if (iactions_hour[1] > dt.Hour()) {
      seconds_till_todays_run = ((iactions_hour[timer_number] - dt.Hour())*60*60);
      if (iactions_min_start[timer_number] < dt.Minute()) {
        seconds_till_todays_run = seconds_till_todays_run + (iactions_min_start[timer_number]-dt.Minute())*60;
      }
  }
  return seconds_till_todays_run;
}
long get_seconds_till_next_run(int timer_number, const RtcDateTime& dt) {

  long seconds_till_next_run=0;
  if (timer_number == 1) {
     if (get_seconds_since_last_run(timer_number,dt) > isecs_half_day) {
        // will run in the next cycle.... 
        seconds_till_next_run = get_seconds_till_todays_run(timer_number,dt);
      
     } else { // otherwise
        //  will run the following cycle.
        seconds_till_next_run = iseconds_in_day;
     }
     
  } else if (timer_number == 2) {
     if (get_seconds_since_last_run(timer_number,dt) > isecs_day_and_half) {
        // will run in the next cycle.... 
        seconds_till_next_run = get_seconds_till_todays_run(timer_number,dt);
      
     } else { // otherwise
        //  will run the following cycle.
        seconds_till_next_run = iseconds_in_day*2;
     }

  } else if (timer_number == 3) {
     if (get_seconds_since_last_run(timer_number,dt) > isecs_2day_and_half) {
        // will run in the next cycle.... 
        seconds_till_next_run = get_seconds_till_todays_run(timer_number,dt);
      
     } else { // otherwise
        //  will run the following cycle.
        seconds_till_next_run = iseconds_in_day*3;
     }

  } else if (timer_number == 4) {
     if (get_seconds_since_last_run(timer_number,dt) > isecs_week_and_half) {
        // will run in the next cycle.... 
        seconds_till_next_run = get_seconds_till_todays_run(timer_number,dt);
      
     } else { // otherwise
        //  will run the following cycle.
        seconds_till_next_run = iseconds_in_day*7;
     }

  }
  return seconds_till_next_run;
  
}


void switchon_todays_actions(const RtcDateTime& dt) { // called at midnight.  Actions planned for noon..12 hours later
  bool once_a_day = false;  // run every day, always true here
  bool every_second_day = false;
  bool every_third_day = false;
  bool once_a_week = false;

  if ( get_seconds_till_next_run(1,dt) < iseconds_in_day ) { 
    once_a_day = true;
  }

  if ( get_seconds_till_next_run(2,dt) < iseconds_in_day ) { // don't run for a day and half
    every_second_day = true;
  }
  
  if ( get_seconds_till_next_run(3,dt) < iseconds_in_day) { // don't run for 2 days and half
    every_third_day = true;
  }

  if ( get_seconds_till_next_run(4,dt) < iseconds_in_day) { // don't run for 6 days and half
    once_a_week = true;
  }
  iactions_to_complete_today[0] = once_a_day;
  iactions_to_complete_today[1] = every_second_day;
  iactions_to_complete_today[2] = every_third_day;
  iactions_to_complete_today[3] = once_a_week;
  
}

bool is_midnight(const RtcDateTime& dt) {
  if (dt.Hour() == 1) {
    if (dt.Minute() == 1 and dt.Minute() == 1)  {
      return true; // IF in the first minnute passed midnight.
    }
  }
  return false;
}


bool is_any_action_running(){
  if (iactions_being_done[0] or 
      iactions_being_done[1] or 
      iactions_being_done[2] or 
      iactions_being_done[3] ) {
    return true;
  } else {
    return false;
  }
}

bool are_any_actions_left_today(){
  if (iactions_to_complete_today[0] or 
      iactions_to_complete_today[1] or 
      iactions_to_complete_today[2] or 
      iactions_to_complete_today[3] ) {
    return true;
  } else {
    return false;
  }

}

void do_start_action(int action_nbr,const RtcDateTime& dt) {
  iactions_being_done[action_nbr] = true;
  if (action_nbr == 0) {
    eeprom_obj.ts64_once_a_day_lr =  dt.TotalSeconds64();
  } else if (action_nbr == 1) {
    eeprom_obj.ts64_every_second_day_lr =  dt.TotalSeconds64();
  } else if (action_nbr == 2) {
    eeprom_obj.ts64_every_third_day_lr =  dt.TotalSeconds64();
  } else if (action_nbr == 3) {
    eeprom_obj.ts64_once_a_week_lr =  dt.TotalSeconds64();
  }
 
  if (DEBUG_ON) {
    String logmsg =  "Started Action ";
    logmsg += (action_nbr + 1);
    Serial.println(logmsg);
    if (LCD_ON) {
      ShowMessage(logmsg);
    }
  }
}

bool is_action_done(int action_nbr) {
  if (iactions_duration_seconds[action_nbr] > iactions_expected_durations[action_nbr]) {
    iactions_being_done[action_nbr] = false;
    iactions_to_complete_today[action_nbr] = false;
    save_eeprom();
    if (DEBUG_ON) {
      String logmsg =  "Action ";
      logmsg += (action_nbr + 1);
      logmsg += " Done ";
      Serial.println(logmsg);
      if (LCD_ON) {
        ShowMessage(logmsg);
      }
    }
    return true;
  } else {
    return false;
  }
}


bool is_time_for_action(int action_nbr,const RtcDateTime& dt) {
  if (iactions_to_complete_today[action_nbr] and
      not is_any_action_running() and 
      iactions_hour[action_nbr] == dt.Hour() and 
      dt.Minute() >= iactions_min_start[action_nbr])  {

    icurrently_running_action_nbr = action_nbr;
    return true; 
    
  } else {
    icurrently_running_action_nbr = -1;
    return false;

  }
}

void print_uint64_t(uint64_t num) {

  char rev[128]; 
  char *p = rev+1;

  while (num > 0) {
    *p++ = '0' + ( num % 10);
    num/= 10;
  }
  p--;
  /*Print the number which is now in reverse*/
  while (p > rev) {
    Serial.print(*p--);
  }
}
void save_eeprom()
{
  EEPROM.put(0, eeprom_obj);
}
void read_eeprom()
{
   EEPROM.get(0,eeprom_obj);
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
