/*arduino:avr:nano:cpu=atmega328
  Dave Skura, 2022

	arduino:avr:nano:cpu=atmega328
	arduino:avr:nano:cpu=atmega328old
	arduino:avr:uno
	arduino:avr:mega

	https://www.best-microcontroller-projects.com/arduino-battery-charger.html


	AA and AAA charger
	Could use for others with different charge currents
	i.e. different emitter resistors.
*/

#include <SPI.h>

#define FLASH_DELAY 500
#define START_PIN 6
#define RLED_PIN A4
#define GLED_PIN A5

#define VBATT_PIN A0
#define CHRG_PIN 2
#define HIGH_PIN 3
#define SPI_SS_PIN 10

#define CHARGE_ON  pinMode(CHRG_PIN,OUTPUT);digitalWrite(CHRG_PIN,LOW);
#define CHARGE_OFF pinMode(CHRG_PIN,INPUT);
#define HIGH_ON    pinMode(HIGH_PIN,OUTPUT);digitalWrite(HIGH_PIN,LOW);
#define HIGH_OFF   pinMode(HIGH_PIN,INPUT);
#define RLED_ON    digitalWrite(RLED_PIN,HIGH);
#define RLED_OFF   digitalWrite(RLED_PIN,LOW);
#define GLED_ON    digitalWrite(GLED_PIN,HIGH);
#define GLED_OFF   digitalWrite(GLED_PIN,LOW);
#define SPC        Serial.print(' ');

#define VBATMAX 1.55 // Maximum and minimum acceptable battery.
#define VBATMIN 0.75 // voltage to allow charging to start.
#define VREFANA 5.0 // Reference voltage. Measure this with DMM.
#define CHRG_ms 30000 // Time period of normal charging time (ms).
#define HIGH_ms 120000 // Time period of normal higher power charging time (ms).

#define PREV_ADC 20  // Number of nearly same ADC readings to declare done.

typedef enum { IDLE, BATFND, CHARGE, HIGH_CHARGE, TEST,
               FINISHED, FAIL, WAIT_START } state_t;

enum { BATTFULL, BATTAVGSTABLE, BATTBAD, BATTNORISE };

///////////////////////////////////////////////////
void setup() {
	Serial.begin(9600);
	Serial.println("Battery Charger ");

	pinMode(VBATT_PIN,INPUT);
	pinMode(START_PIN,INPUT_PULLUP);

	pinMode(CHRG_PIN,OUTPUT);
	pinMode(HIGH_PIN,OUTPUT);
	pinMode(GLED_PIN,OUTPUT);
	pinMode(RLED_PIN,OUTPUT);

	CHARGE_OFF
	HIGH_OFF
	RLED_OFF
	GLED_OFF
	showStartVolts();
}

void flash_green(int flashcount) {
	for (int i=0; i < flashcount; i++) {
		GLED_ON
		delay(FLASH_DELAY);
		GLED_OFF
		delay(FLASH_DELAY);
	}
}

void flash_red(int flashcount) {
	for (int i=0; i < flashcount; i++) {
		RLED_ON
		delay(FLASH_DELAY);
		RLED_OFF
		delay(FLASH_DELAY);
	}
}

void startup_flash() {
	RLED_ON
	GLED_ON
	delay(2000);
	RLED_OFF
	GLED_OFF
}
///////////////////////////////////////////////////
// Print start volts to LCD & serial(for reference).
void showStartVolts(void) {
	int current_adc = analogRead(VBATT_PIN);
  float v = getVana( current_adc ) ;

  //lcd.setCursor(11, 0);
  //lcd.print( v,3 );
  Serial.print("\nStart volts: ");
  Serial.println( v,3 );

	//flash_green(adc_tens);
	//flash_red(adc_ones);

}

///////////////////////////////////////////////////
int reason(uint8_t c) {
	static int reasoncount = 0;
	Serial.print("End reason: ");
	switch(c) {
		case BATTFULL       : Serial.println("Max volts."); reasoncount = 1; break;
		case BATTAVGSTABLE  : Serial.println("No avg change."); reasoncount = 2; break;
		case BATTBAD        : Serial.println("Out of range."); reasoncount = 3; break;
		case BATTNORISE     : Serial.println("No Hchrg rise."); reasoncount = 4; 
	}
	return reasoncount;
	
}
///////////////////////////////////////////////////
void loop() {
  static uint32_t loop_time = 0;

  if (millis()-loop_time>250) {  // loop every 1/4 second
     action();
     loop_time = millis();
  }
}

///////////////////////////////////////////////////
float getVana_old(uint16_t adc) {
    return adc * VREFANA / 1024;
}
// new, from this guy https://www.youtube.com/watch?v=xI_qU2auVx8
float getVana(uint16_t adc) {
  float val0 = adc * 5.0 / 1024.0;

  // read correct supply voltage
  float supply = readVcc() / 1000.0;
  float val0Corrected = supply / 5 * val0;
	return val0Corrected;
}



///////////////////////////////////////////////////
uint8_t checkBattBad(float Vana) {
   if ( Vana<VBATMIN || Vana>VBATMAX ) return 1;
   return 0;
}

///////////////////////////////////////////////////
uint8_t checkBattFinished(float Vana) {
   if ( Vana>=VBATMAX ) return 1;
   return 0;
}

///////////////////////////////////////////////////
void showSerialElapsedTime(uint32_t r_time) {
   Serial.print("Time:");
   Serial.print(r_time/1000/60);
   Serial.println(" mins.");
}

///////////////////////////////////////////////////
void showState(state_t state) {

   // This state has no text, to leave FINISHED or FAIL on //lcd.
   if (state == WAIT_START) return;

   // Show current state
   //lcd.setCursor(0, 0);
   //lcd.print("           "); // 11 chars clear debug area.
   //lcd.setCursor(0, 0);
   switch( state) {
     case IDLE : //lcd.print("IDLE");break;
     case BATFND : //lcd.print("BATFND");break;
     case CHARGE : //lcd.print("CHRG");break;
     case HIGH_CHARGE : //lcd.print("H-CHRG");break;
     case TEST : //lcd.print("TEST");break;
     case FINISHED : //lcd.print  ("FINSHD");break;
     case FAIL : //lcd.print("FAIL");break;
//     case WAIT_START : //lcd.print("WAIT");break;
    default: break;
   }
}

///////////////////////////////////////////////////
void showTime(uint32_t timeVal_ms) { // could use sprinf - this = less memory.
  uint8_t lcdpos;
  uint16_t tmin,tsec;
  uint32_t t;

    t = timeVal_ms;
    tmin = (t/1000/60); // Could use sprintf but smaller code using:

    if (tmin >= 1) { // Print minutes since reset
       lcdpos = 12;
       if (tmin>99) lcdpos = 10;
          else if (tmin>9) lcdpos = 11;
       //lcd.setCursor(lcdpos, 1);

       //lcd.print(tmin);
       //lcd.print(':');
    }

    // Print seconds since reset.
    tsec = (t/1000) % 60;

    //lcd.setCursor(14, 1);
    //if (tsec<10)
       //lcd.print('0');
    //lcd.print(tsec);
}

///////////////////////////////////////////////////
// -1 resets the store
//
uint8_t test_avg(uint16_t adc) {
uint8_t i,match,allmatch;
static uint16_t prevADC[PREV_ADC];
uint16_t avg;

   if (adc==-1) {
      for(i=0;i<PREV_ADC;i++) prevADC[i]=0;
      return;
   }

   // Get average value
      avg = 0;
      Serial.print("AVG:");
      for (i=0;i<PREV_ADC;i++) {
         Serial.print(prevADC[i]); SPC;
         avg+=prevADC[i];
      }
      avg += 5;  // =0.5 after division by 10
      avg /=10;
      Serial.print("Avg ");Serial.println(avg);

      // If any are zero then not filled: set avg zero to stop err. match
      for (i=0;i<PREV_ADC;i++) {
          if (prevADC[i]==0) {
             avg = 0;
             Serial.println("Avg set ZERO");
             break;
          }
      }

      // Check if has not increased over last n readings - if so exit.
      allmatch = 1;

      for (i=0;i<PREV_ADC;i++) {
         match = 0;
         // Detect close matches: lowChrgADC+/-2 (+/-4.88mV)
         if ( avg-1 <= prevADC[i] && prevADC[i] <= avg+1 ) match=1;

         // Here if the value is one of 3: lowChrgADC+/-1, then match is high
         if (!match) { allmatch = 0; break; }
      }

      // Update rolling store. // Array indices 0 ~ (n-1) shift.
      for (i=0;i<PREV_ADC-1;i++) prevADC[i]=prevADC[i+1];
      prevADC[PREV_ADC-1] = adc;

      // allmatch is 1 if all have matched for all elements.
      if (allmatch && avg!=0) { // Zero is a special case.
         //reason(BATTAVGSTABLE);
         return reason(BATTAVGSTABLE); // All the same so indicate finished.
      }
      return 0;
}



///////////////////////////////////////////////////
void action(void) {
	static uint32_t s_time = millis();
	static uint32_t r_time = s_time; // Relative, start times.
	static uint32_t timewas = 0, timewas_state=r_time;
	static state_t state = IDLE;
	static uint16_t lowChrgADC=0;
	static uint8_t timer_on=1;
	uint16_t adc=0;
	float Vana;
	static uint8_t done_once = 0;
	static bool button_pushed = false;

	if (!done_once) {
		test_avg(-1); // Reset store.
		done_once=1;
	}

	r_time = millis() - s_time; // Use time relative to start
	if (timer_on) showTime(r_time); // Time since start: to //lcd.

	adc = analogRead(VBATT_PIN);
	Vana=getVana(adc);

	static int adc_hundreds = adc / 100;
	static int adc_tens = (adc % 100) / 10;
	static int adc_ones = adc % 10;
	static int volt_ones = (int)Vana;
	static float volt_decimals = (float)Vana - (float)volt_ones;
	static int volt_tenths = (int)(volt_decimals*10);
	static int volt_hundredths = (int)(volt_decimals*100) - (10 * volt_tenths);
	static int volt_thousandths = (int)(volt_decimals*1000) - (100 * volt_tenths) - (10 * volt_hundredths);
	static int reason_code = 0;

	if (digitalRead(START_PIN) ==0) {
		button_pushed = true;
		flash_green(volt_ones);
		flash_red(volt_tenths);
		flash_green(volt_hundredths);
		flash_red(volt_thousandths);

	} else
		button_pushed = false;


   switch( state) {

   case IDLE :
       state = BATFND;
       Serial.println("IDLE>BATFND");

       CHARGE_ON   // Start charge to detect battery.
       HIGH_OFF

       RLED_OFF
       GLED_OFF

       showSerialElapsedTime(r_time);

       break;

   case BATFND : // Battery ok?
       Serial.println("BATFND>CHARGE");

       if ( checkBattBad(Vana) ) {
          state = FAIL; // Battery is dead or not present.
          Serial.println("FAIL in BATFND");
       } else if (Vana>=VBATMIN && Vana<=VBATMAX) {
           state = CHARGE;
       }
       break;

   case CHARGE :
      //if ( checkBattBad(Vana) )      { reason_code = reason(BATTBAD) ; state = FAIL; }
      if ( checkBattFinished(Vana) ) { reason_code = reason(BATTFULL); state = FINISHED; }
      else if (r_time - timewas_state>CHRG_ms) {
         Serial.println("CHARGE>HCHARGE");

         lowChrgADC = adc; // Store value just before high charge time.

         HIGH_ON
         state = HIGH_CHARGE;
         timewas_state = r_time;
      }
      break;

   case HIGH_CHARGE :
      //if ( checkBattBad(Vana) )      { reason_code = reason(BATTBAD) ; state = FAIL; }
      if ( checkBattFinished(Vana) ) { reason_code = reason(BATTFULL); state = FINISHED; }
      else if ( r_time - timewas_state>HIGH_ms) {
         Serial.println("HCHARGE>TEST");
         state = TEST;
         timewas_state = r_time;
      }
      break;

   case TEST :
      Serial.println("TEST>IDLE/FINISHED");
      Serial.println("Cur adc,lowChrgADC");
      Serial.print(adc);       SPC; Serial.println(getVana(adc),3);
      Serial.print(lowChrgADC);SPC;Serial.println(getVana(lowChrgADC),3);

      if ( adc >= (lowChrgADC) ) {  // Gone up by > (2.5/1024)*4=9.77mV
         state= IDLE;
      } else { // No rise so battery charge maxed out.
         reason_code = reason(BATTNORISE);
         state = FINISHED;
      }

			
      if ( test_avg(lowChrgADC) ) {
				state = FINISHED;
			}

      break;

   case FAIL : // Error condition so stop charging battery.
      Serial.println("FAIL");
      CHARGE_OFF
      HIGH_OFF
      timer_on = 0;
      RLED_ON
      GLED_OFF
			while (digitalRead(START_PIN)!=0) {
				flash_red(reason_code);
				delay(2000);
			}
      state = WAIT_START;
      break;

   case FINISHED :
      Serial.print("FINISHED:");
      Serial.println(Vana);
      showSerialElapsedTime(r_time);

      CHARGE_OFF
      HIGH_OFF
      RLED_OFF
			if ((Vana > 1.5) or (reason_code == 1)) {
				while (digitalRead(START_PIN)!=0) {
		      flash_green(1);
					delay(2000);
				}
			} else {
				while (digitalRead(START_PIN)!=0) {
					flash_red(reason_code);
					delay(2000);
				}

			}
			state = WAIT_START;
      timer_on = 0;
      break;

   case WAIT_START :
      if ( button_pushed ) { // digitalRead(START_PIN)==0
         state = IDLE;

         s_time = millis();
         r_time  = millis()-s_time;
         timewas_state = r_time;

         timer_on = 1;
         test_avg(-1); // Reset store.
         //lcd.clear();
         showStartVolts();
      }
      break;
   }

   showState(state);

   // Show raw adc value
   //lcd.setCursor(7, 0);
   //lcd.print(adc);//lcd.print(' ');
}

long readVcc() {
  long result;
  // Read 1.1V reference against AVcc
#if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
  ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
#elif defined (__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
  ADMUX = _BV(MUX5) | _BV(MUX0);
#elif defined (__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
  ADMUX = _BV(MUX3) | _BV(MUX2);
#else
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
#endif
  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Convert
  while (bit_is_set(ADCSRA, ADSC));
  result = ADCL;
  result |= ADCH << 8;
  result = 1126400L / result; // Calculate Vcc (in mV); 1126400 = 1.1*1024*1000
  return result;
}


int readVcc_rando(void) // Returns actual value of Vcc (x 100)
    {
        
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
     // For mega boards
     const long InternalReferenceVoltage = 1115L;  // Adjust this value to your boards specific internal BG voltage x1000
        // REFS1 REFS0          --> 0 1, AVcc internal ref. -Selects AVcc reference
        // MUX4 MUX3 MUX2 MUX1 MUX0  --> 11110 1.1V (VBG)         -Selects channel 30, bandgap voltage, to measure
     ADMUX = (0<<REFS1) | (1<<REFS0) | (0<<ADLAR)| (0<<MUX5) | (1<<MUX4) | (1<<MUX3) | (1<<MUX2) | (1<<MUX1) | (0<<MUX0);
  
#else
     // For 168/328 boards
     const long InternalReferenceVoltage = 1056L;  // Adjust this value to your boards specific internal BG voltage x1000
        // REFS1 REFS0          --> 0 1, AVcc internal ref. -Selects AVcc external reference
        // MUX3 MUX2 MUX1 MUX0  --> 1110 1.1V (VBG)         -Selects channel 14, bandgap voltage, to measure
     ADMUX = (0<<REFS1) | (1<<REFS0) | (0<<ADLAR) | (1<<MUX3) | (1<<MUX2) | (1<<MUX1) | (0<<MUX0);
       
#endif
     delay(100);  // Let mux settle a little to get a more stable A/D conversion
        // Start a conversion  
     ADCSRA |= _BV( ADSC );
        // Wait for it to complete
     while( ( (ADCSRA & (1<<ADSC)) != 0 ) );
        // Scale the value
     int results = (((InternalReferenceVoltage * 1023L) / ADC) + 5L) / 10L; // calculates for straight line value 
     return results*10;
 
    }