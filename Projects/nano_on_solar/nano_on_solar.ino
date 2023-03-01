/*
 * Lets see if we can run a nano with a blinking light
 * powered by a mini 3.7v li-ion, charged with a solar cell,
 * without interuption.
 * 
 * 
 * Nano problems btw..If your device can not recognize it, 
 * you simply need to install the driver for the CH340G, 
 * you can find the driver 
 * here: https://sparks.gogo.co.nz/ch340.html
 */

#define DEBUG_ON true

/*
 * Using DS1302 Real Time Clock(RTC)
 * 
 * * looking at the DS3102, holding pins to left, battery to the right
 * * CONNECTIONS for nano
 * VCC -> 3.3v - 5v
 * GND -> GND
 * CLK -> PIN 3
 * DAT -> PIN 2
 * RST -> PIN 4
 * 
 */

int CLK_SCLK=3;
int IO_DAT=2;
int RST_CE=4;


#include <RTClib.h>

/* Init rtc object
   DS1302 rtc;
   DS1302 rtc(ce_pin, sck_pin, io_pin);

 * CLK -> PIN 3
 * DAT -> PIN 2
 * RST -> PIN 4
*/
DS1302 rtc(4,3,2);

// buffer for DateTime.tostr
char buf[20];

// Using 4 pin RGB LED with common Cathode
#define BLUE 6 // was 3
#define GREEN 5
#define RED 3 // was 6
unsigned long current_time;
int blink_interval = 60;
  
void setup() {
  // put your setup code here, to run once:
  if (DEBUG_ON) {
    Serial.begin(9600);
  }

  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE,OUTPUT);

  current_time = millis();
  setColor(100, 0, 0); // RED 

  rtc.begin();
  rtc.adjust(DateTime(2023, 2, 28, 0, 24, 0));
  
}

void loop() {
  unsigned long loop_time = millis();
  unsigned long milliseconds_passed;
  unsigned long seconds_passed;
  
  milliseconds_passed = loop_time - current_time;
  seconds_passed = milliseconds_passed/1000;
  if (seconds_passed > blink_interval) {
    setColor(0, 100, 0); // Green
    delay(2000);
    setColor(0, 0, 0); // off
    current_time = loop_time;
  }

  if (DEBUG_ON) {
    DateTime now = rtc.now();
  
    Serial.println(now.tostr(buf));
  
    delay(3000);
  }
}

void setColor(int red, int green, int blue)
{
  analogWrite(RED, red);
  analogWrite(GREEN, green);
  analogWrite(BLUE, blue);
}
