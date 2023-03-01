
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

	0) it's midnight, assign daily actions true/false
	1) no action is currently happening:
		a) waiting to start an action
		b) starting an action
		c) all actions have happened today already
	2) an action is running but not completed
	3) an action is completing



	Nano connections (uno and mega are different serial pins than nano):
		 DS1302 CLK/SCLK --> 3
		 DS1302 DAT/IO --> 2
		 DS1302 RST/CE --> 4
		 DS1302 VCC --> 5v
		 DS1302 GND --> GND
*/

#define DEBUG_ON true

#include <ThreeWire.h>  
#include <RtcDS1302.h>

int CLK_SCLK=3;
int IO_DAT=2;
int RST_CE=4;

const long iseconds_in_day = 86400;

int iaction_hour = 16;               // the hour to start the action each day
int iaction_min_start = 20;           // the start of the window in minutes, for running actions each day

int iexpected_durations[]       = {5, 10,15, 20};   // how long the action will take to complete.
bool iactions_to_complete_today[] = {
        false, // "Once a day" 
        false, // "Every Second Day"
        false, // "Every Third Day"
        false, // "Once a week"
      };      // true/false to indicate if actions have run cycles for the current day

int iaction_max = 4;
const char *iMacro_Cycle_names[] = {
    "Once a day"        // iaction = 0
    ,"Every Second Day"	// iaction = 1
    ,"Every Third Day"	// iaction = 2
    ,"Once a week"      // iaction = 3
    };

int irun_action_number = -1;  // currently running action [0..3]
int iactions_duration_seconds[] = {0, 0, 0,  0};    // how many seconds have passed since the start of the action.
bool iactions_being_done[]      = {false,false,false,false};      // true/false to indicate if action is currently happening

unsigned long tms_actions_started_on[]={0,0,0,0};

unsigned long ts64_once_a_day_lr=0;        // timestamp of last run of this action
unsigned long ts64_every_second_day_lr=0;  // timestamp of last run of this action
unsigned long ts64_every_third_day_lr=0;   // timestamp of last run of this action
unsigned long ts64_once_a_week_lr=0;       // timestamp of last run of this action

void switchon_todays_actions(const RtcDateTime& dt) { // called at midnight.  Actions planned for noon..12 hours later
  bool once_a_day = true;  // run every day, always true here
  bool every_second_day = false;
  bool every_third_day = false;
  bool once_a_week = false;

  int seconds_since_run=0;
  long isecs_day_and_half = 1.5 * iseconds_in_day;
  long isecs_2day_and_half = 2.5 * iseconds_in_day;
  long isecs_week_and_half = 6.5 * iseconds_in_day;
  seconds_since_run = dt.TotalSeconds64()- ts64_every_second_day_lr;
  if ( seconds_since_run > isecs_day_and_half ) { // don't run for a day and half
    every_second_day = true;
  }
  
  seconds_since_run = dt.TotalSeconds64()- ts64_every_third_day_lr;
  if ( seconds_since_run > isecs_2day_and_half) { // don't run for 2 days and half
    every_third_day = true;
  }

    seconds_since_run = dt.TotalSeconds64()- ts64_once_a_week_lr;
  if ( seconds_since_run > isecs_week_and_half) { // don't run for 6 days and half
    once_a_week = true;
  }

  iactions_to_complete_today[0] = once_a_day;
  iactions_to_complete_today[1] = every_second_day;
  iactions_to_complete_today[2] = every_third_day;
  iactions_to_complete_today[3] = once_a_week;
  
}


bool isTimeForAction(const RtcDateTime& dt) {

  if (dt.Hour() == iaction_hour)  { // it's the hour to start actions
    if (dt.Minute() >= iaction_min_start) {// it's passed the minute to start actions

      // only start actions if none are currently running  
      if (not iactions_being_done[0] 
        and not iactions_being_done[1] 
        and not iactions_being_done[2] 
        and not iactions_being_done[3]) {

          irun_action_number = -1;
          if (iactions_to_complete_today[0]) {
            irun_action_number = 0;
            return true; 
          } else if (iactions_to_complete_today[1]) {
            irun_action_number = 1;
            return true; 
          } else if (iactions_to_complete_today[2]) {
            irun_action_number = 2;
            return true; 
          } else if (iactions_to_complete_today[3]) {
            irun_action_number = 3;
            return true; 
          }
          
        }
    }
  }
  return false;
}

bool is_midnight(const RtcDateTime& dt) {
  if (dt.Hour() == 1) {
    if (dt.Minute() == 1 and dt.Minute() == 1)  {
      return true; // IF in the first minnute passed midnight.
    }
  }
  return false;
}

void do_start_action_0(const RtcDateTime& dt){
  ts64_once_a_day_lr = dt.TotalSeconds64();
  
  if (DEBUG_ON) {
    Serial.println("Starting the the action.");
  }
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

bool is_action_0_done() {
  if ( iactions_duration_seconds[irun_action_number] >= iexpected_durations[irun_action_number] ) {
    return false;
  } else {
    return true;
  }
}

ThreeWire myWire(IO_DAT,CLK_SCLK,RST_CE); // IO, SCLK, CE
RtcDS1302<ThreeWire> Rtc(myWire);

void setup () 
{
		if (DEBUG_ON)	{
	    Serial.begin(9600);
		}
    Rtc.Begin();
    RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
    Rtc.SetDateTime(compiled);
    switchon_todays_actions(compiled);
}


void loop () 
{
    //unsigned long current_tms = millis();
    RtcDateTime now = Rtc.GetDateTime();
	/*
loop
	The looping logic is passing through one of the following:

	0) it's midnight, assign daily actions true/false
	1) no action is currently happening:
		a) waiting to start an action
		b) starting an action
		c) all actions have happened today already
	2) an action is running but not completed
	3) an action is completing

	*/
    
		// 0) it's midnight, assign daily actions true/false
    // determine what actions are we doing today ?
    // update bool array iactions_to_complete_today[]
		if (is_midnight(now)) {
      switchon_todays_actions(now);

		} else if (not is_any_action_running()){)
		//	1) no action is currently happening:
		//		a) waiting to start an action
		//		b) starting an action
		//		c) all actions have happened today already
		
		
		} else {
		//	2) an action is running but not completed
		//	3) an action is completing

		}

    // are we in the action window now ?  as in ...is is passed noon, can we start running actions ?
		if (isTimeForAction(now)){

      if (irun_action_number > -1) {
        iactions_being_done[irun_action_number] = true;
        tms_actions_started_on[irun_action_number] = millis();
        iactions_duration_seconds[irun_action_number] = 0;
      }

      //it's time to start the action
      if (irun_action_number ==0) {
        do_start_action_0(now);
        Serial.println("Started Action 0");
      } else {
        Serial.println("Code actions 1,2,3");
      }
		}

    if (is_any_action_running()){
      iactions_duration_seconds[irun_action_number] = (current_tms - tms_actions_started_on[irun_action_number])/1000;
      if (irun_action_number == 0) {

        // .... it's not finished doing it's thing      
        if (DEBUG_ON) {
          Serial.println("Action 0 Running ");
        }

        if (is_action_0_done()) {
          // action must be done
          iactions_to_complete_today[irun_action_number] = false;
          irun_action_number = -1;
        }
      } else {
        Serial.println("Code actions 1,2,3");
      }
    }
    
    if (DEBUG_ON) {
  		printDateTime(now);
  		Serial.println();
  		delay(5000); // 5 seconds
    }

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
}
