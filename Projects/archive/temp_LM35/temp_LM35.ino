
#include <LiquidCrystal.h>
#define ADCPIN 1

//Initialize variable to hold temperature
double temperature = 0;
double atemp = 0;

void setup(){
  Serial.begin(9600);
}


//Loop Block, this code runs continously
void loop()
{
  atemp = analogRead(ADCPIN);
  
  temperature = (atemp*500)/1023;
  
  Serial.println(temperature);

  //Wait 1 second for the next reading
  delay(1000);
}
