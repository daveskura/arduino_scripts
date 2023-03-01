#include <Servo.h>


#define BUTTON1 3
#define BUTTON2 2

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position

void setup() {
  Serial.begin(9600);
  pinMode(BUTTON1,INPUT_PULLUP);
  pinMode(BUTTON2,INPUT_PULLUP);
  myservo.attach(4);  // attaches the servo on pin 9 to the servo object
  
}

void loop() {
  int btn1 = digitalRead(BUTTON1); 
  int btn2 = digitalRead(BUTTON2); 
   // button is pressed
  if (btn1 == LOW) {
    if (pos < 180) {
      pos = pos + 1; // goes from 0 degrees to 180 degrees
      Serial.println(pos);
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      //myservo.write(108);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15 ms for the servo to reach the position
    }
  }
      
  if (btn2 == LOW) {
    if (pos > 0) {
      pos = pos - 1; // goes from 0 degrees to 180 degrees
      Serial.println(pos);
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      //myservo.write(0);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15 ms for the servo to reach the position
    }
  }
}
