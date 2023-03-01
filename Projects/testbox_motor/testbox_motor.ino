

#define MOTOR_PWM 3
#define MOTOR_IN1 4
#define MOTOR_IN2 5
#define BUTTON_1 A3
#define BUTTON_2 A4

int imotor_power = 0;

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  pinMode(BUTTON_1,INPUT_PULLUP);
  pinMode(BUTTON_2,INPUT_PULLUP);  

  pinMode(MOTOR_PWM,OUTPUT);
  pinMode(MOTOR_IN1,OUTPUT);
  pinMode(MOTOR_IN2,OUTPUT);

  digitalWrite(MOTOR_IN1,HIGH);
  digitalWrite(MOTOR_IN2,LOW);
  analogWrite(MOTOR_PWM,imotor_power);
}
void loop() {
  
  // put your main code here, to run repeatedly:
  if (digitalRead(BUTTON_1) == LOW) {
    imotor_power += 50;
    if (imotor_power > 250) {
      imotor_power = 250;
    }
    analogWrite(MOTOR_PWM,imotor_power);
    Serial.println(imotor_power );
    delay(250);
  }

  if (digitalRead(BUTTON_2) == LOW) {
    imotor_power -= 50;
    if (imotor_power < 0) {
      imotor_power = 0;
    }
    analogWrite(MOTOR_PWM,imotor_power);
    Serial.println(imotor_power );
    delay(250);
  }



}
