/*
  LiquidCrystal Library - Hello World

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
 D5   = was 4 .. now 10
 D6   = was 3 .. now 9
 D7   = was 2 .. now 8 

 BL+   -> <- 1k resistor <- 5v 
 BL-   -> GND

 */
#include<LiquidCrystal.h>

const int rs = 7, en = 6, d4 = 5, d5 = 10, d6 = 9, d7 = 8;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


void setup()
{
  Serial.begin(9600);
  lcd.begin(16, 2);

  ShowMessage("Hello this is a message");
  
}
int imsg = 0;
String msg[] = {"asdf","ddddd","ddddd","eeeeee"};
void loop()
{
  imsg++;
  if (4 <= imsg)
    imsg = 0;
  ShowMessage(msg[imsg]);
  delay(3000);
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
