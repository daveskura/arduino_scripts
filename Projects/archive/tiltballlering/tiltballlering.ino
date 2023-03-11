#define GREEN_LED 5 
#define AETHDX 6 // SW-520D Roller Ball Tilt Switch

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN,OUTPUT);
  digitalWrite(LED_BUILTIN,LOW);
  pinMode(GREEN_LED,OUTPUT);
  dim_green_led_flkr();
  notilt_action();
  tilt_action();
  dim_green_led_flkr();
  
  pinMode(AETHDX,INPUT);      
  digitalWrite(AETHDX, HIGH); // tilt ball switch needs this
}

// the loop function runs over and over again forever
void loop() {
  int sw_tiltball = digitalRead(AETHDX);
  
  //Serial.println(sw_tiltball);
  if(sw_tiltball == HIGH) {
    tilt_action();
  } else {
    notilt_action();
  }
}

void notilt_action() {
    digitalWrite(GREEN_LED,0);
}
void tilt_action(){
    digitalWrite(GREEN_LED,15);
    delay(5000);
    digitalWrite(GREEN_LED,0);
}

void dim_green_led_flkr() {
  // #define GREEN_LED 5
  // uses PIN 5, because u need a digital PWM pin on the arduino such as 5 ... not all pins are PWM.
  analogWrite(GREEN_LED,15);
  delay(100);  
  analogWrite(GREEN_LED,0);
  
}
