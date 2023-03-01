#define TOGGLE_1_BLUE A9
#define TOGGLE_2_YELLOW A11 
#define TOGGLE_3_GREEN A8
#define TOGGLE_4_ORANGE A7
#define PUSHBUTTON_1 A6
#define PUSHBUTTON_2 A5
#define PUSHBUTTON_3 A4

#define RELAY_SW_CHICKEN_RADIO A10

void setup() {
  // put your setup code here, to run once:
   Serial.begin(9600);  
   pinMode(RELAY_SW_CHICKEN_RADIO,OUTPUT);
   digitalWrite(RELAY_SW_CHICKEN_RADIO,LOW);

}

void loop() {
  // put your main code here, to run repeatedly:
  int val = analogRead(TOGGLE_2_YELLOW);  // read the input pin
  Serial.println(val);          // debug value
  delay(2000);
}
