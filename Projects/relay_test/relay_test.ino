//arduino:avr:uno

#define RELAY_SW 10

void setup() {
  pinMode(RELAY_SW,OUTPUT);
	normally_off_relay_OFF();
}

void loop() {
}

void normally_off_relay_ON(){
	digitalWrite(RELAY_SW,HIGH);
}

void normally_off_relay_OFF(){
	digitalWrite(RELAY_SW,LOW);
}