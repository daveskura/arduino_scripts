/*arduino:avr:uno
  Dave Skura, 2022

  IR Breakbeam sensor demo!

*/

#define SENSORPIN 4

void setup() {
  Serial.begin(9600);

	pinMode(LED_BUILTIN, OUTPUT);

  pinMode(SENSORPIN, INPUT_PULLUP);     // INPUT ?
  //digitalWrite(SENSORPIN, HIGH); // turn on the pullup
 
}

void loop(){
	bool beam_isbroken = chk_IR_beam();
	Serial.println(beam_isbroken);
	delay(2000);
  
}

bool chk_IR_beam() {
	int sensorState = digitalRead(SENSORPIN);
	bool isbroken = false;
	
	// check if the sensor beam is broken
	if (sensorState == LOW) {     
		digitalWrite(LED_BUILTIN, HIGH);  
		isbroken = true;
		Serial.println("IR beam broken");
	} else {
		digitalWrite(LED_BUILTIN, LOW); 
		isbroken = false;
		Serial.println("Good IR beam");
	}
	return isbroken;

}
