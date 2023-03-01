/*
  Arduino Uno
    PIN D1  <--> 
    PIN D2  <--> Push button pin  
    ...
    PIN D7  <--> LCD1602 -  RS
    PIN D8  <--> LCD1602 -  E
    PIN D9  <--> LCD1602 -  DB4
    PIN D10  <--> LCD1602 - DB5
    PIN D11  <--> LCD1602 - DB6
    PIN D12  <--> LCD1602 - DB7



  LCD1602   -> Arduino            -> Description
  VSS:      GND
  VDD:      +5vdc power supply.
  VO:       +5vdc power supply.   A pin that adjust the contrast of LCD1602
  RS:       D7                    A register select pin that controls where in the LCD’smemory you are writing data to. You can select either the data register, which holds what goes on the screen, or an instruction register, which is where the LCD’s controller looks for instructions on what to do next.
  R/W:      GND                   A Read/Write pin that selects reading mode or writingmode
  E:        D8                    An enabling pin that, when supplied with low-level energy, causesthe LDC module to execute relevantinstructions.
  DB0-DB3： * not used            Pins that read and write data
  DB4:      D9                    Pin that reads and writes data
  DB5:      D10                   Pin that reads and writes data
  DB6:      D11                   Pin that reads and writes data
  DB7:      D12                   Pin that reads and writes data
  A:        +5vdc power supply.   Pins that control the LED backlight
  K:        GND                   Pins that control the LED backlight

*/
#include <LiquidCrystal.h>
#include "pitches.h" // put *.h file in same dir as arduino *.ino source file

#define PUSH_ON_OFF_BUTTON 2
#define BACKLIGHT_PIN 5
#define DIGIT_DELAY 100
int push_on_off_button = 0;

int melody[] = { NOTE_C5, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_G5, NOTE_A5, NOTE_B5, NOTE_C6};
LiquidCrystal lcd(7, 8, 9, 10, 11,12);

int nbr_Texts = 20;

char *Magic8ballSays[] = {
"As I see it, yes",
"Ask again later",
"Better not tell you now",
"Cannot predict  now",
"Concentrate and ask again",
"Dont count on it",
"It is certain",
"It is decidedly so",
"Most likely",
"My reply is no",
"My sources say  no",
"Outlook good",
"Outlook not so  good",
"Reply hazy try  again",
"Signs point to  yes",
"Very doubtful",
"Without a doubt",
"Yes",
"Yes, definitely",
"You may rely on it"
    };

void setup() {
  Serial.begin(9600);
  Serial.println("Starting up.");
  pinMode(PUSH_ON_OFF_BUTTON,INPUT_PULLUP);
  pinMode(BACKLIGHT_PIN,OUTPUT);
  digitalWrite(BACKLIGHT_PIN,0);

  lcd.begin(16, 2);
  RevealMessage("Hello");
    
  //Let's make it more random
  randomSeed(analogRead(A0)); 
}

void loop() {
  // put your main code here, to run repeatedly:
  push_on_off_button = digitalRead(PUSH_ON_OFF_BUTTON); 
  if (push_on_off_button == LOW) {
    //Serial.println("Button pushed");
    
    cls();
    ShowRandomMessage();
    delay(1000);  
  }
}

void ShowRandomMessage() {
    int idx = random(0,nbr_Texts);
    String msg = Magic8ballSays[idx];
    RevealMessage(msg);
}
void cls() {
  lcd.setCursor(0, 0);
  lcd.print("                ");
  lcd.setCursor(0, 1);
  lcd.print("                ");

}
void RevealMessage(String msg) {
    String lcd_msg = "";
    String msg1 = msg;
    String msg2 = "                ";

    if (msg1.length() > 16) {
      msg2 = msg1.substring(16);
      if (msg2.length() > 16) {
        msg2 = "*Err:Msg too big";  
      }
      msg1 = msg1.substring(0,15);
    } else {
      msg1 = msg1 + "                ";
      msg1 = msg1.substring(0,15);
    }
    lcd.setCursor(0, 0);
    for (int i=0; i< msg1.length(); i++) {
      lcd.print(msg1[i]);
      delay(DIGIT_DELAY);
    }

    lcd.setCursor(0, 1);
    for (int i=0; i< msg2.length(); i++) {
      lcd.print(msg2[i]);
      delay(DIGIT_DELAY);
    }
  
}
