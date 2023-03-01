/*
    PIN D43 <--> orange
    PIN D44                                         ** Not Used **
    PIN D45 <--> blue
    PIN D46                                         ** Not Used **
    PIN D47 <--> yellow

  PWM -> 45
  IN1 -> 47
  IN2 -> 43
*/

#define MOTOR_IN2 43
#define MOTOR_PWM 45
#define MOTOR_IN1 47
#define ACTUATOR_PWM 39
#define ACTUATOR_DIR 37

void setup() {
  // put your setup code here, to run once:
  pinMode(MOTOR_PWM,OUTPUT);
  pinMode(MOTOR_IN1,OUTPUT);
  pinMode(MOTOR_IN2,OUTPUT);
  pinMode(ACTUATOR_PWM,OUTPUT);
  pinMode(ACTUATOR_DIR,OUTPUT);

  digitalWrite(ACTUATOR_DIR,HIGH);
  analogWrite(ACTUATOR_PWM,255);

}

void loop() {
  // put your main code here, to run repeatedly:

}
