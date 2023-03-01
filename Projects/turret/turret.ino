#define SERVO_PIN 5
#define BUTTON_1 A3
#define BUTTON_2 A4

#include <Servo.h>
Servo myservo;  
int iturret_angle = 10; 


void setup() {
  Serial.begin(9600);

  pinMode(BUTTON_1,INPUT_PULLUP);
  pinMode(BUTTON_2,INPUT_PULLUP);  

  myservo.attach(SERVO_PIN);  
	myservo.write(iturret_angle);    // Max Range (0 .. 180)
  delay(250);          

}


void loop() {
  
  // put your main code here, to run repeatedly:
  if (digitalRead(BUTTON_1) == LOW) {
    myservo.write(0);   // Max Range (0 .. 180) 
    delay(250);          
  }

  if (digitalRead(BUTTON_2) == LOW) {
    myservo.write(180);   // Max Range (0 .. 180)
    delay(250);          
  }

}
