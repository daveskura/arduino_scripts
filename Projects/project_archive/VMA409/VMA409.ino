/*arduino:avr:uno
  Dave Skura, 2022

these boards work and are listed using C:\arduino\Projects\arduino-cli board listall

	arduino:avr:nano:cpu=atmega328old
	arduino:avr:uno
	arduino:avr:mega

*/
int pin1=13;
int pin2=12;
int pin3=11;
int pin4=10;

int speedpinA=5;


void setup() {
	Serial.begin(9600);
	pinMode(pin1,OUTPUT);
	pinMode(pin2,OUTPUT);
	pinMode(pin3,OUTPUT);
	pinMode(pin4,OUTPUT);

	digitalWrite(pin1,HIGH);
	digitalWrite(pin2,HIGH);
	digitalWrite(pin3,HIGH);
	digitalWrite(pin4,HIGH);


}

void loop() {
	digitalWrite(pin1,HIGH);
	digitalWrite(pin2,LOW);
	analogWrite(speedpinA,500);
	delay(3000);
	digitalWrite(pin1,LOW);
	digitalWrite(pin2,HIGH);
	analogWrite(speedpinA,500);
	delay(3000);

}

