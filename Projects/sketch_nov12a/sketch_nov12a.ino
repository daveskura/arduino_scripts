
#define MOTION_SENSOR_PIN 8

void setup() {
  Serial.begin(9600);
  pinMode(MOTION_SENSOR_PIN, INPUT);    
}

void loop() {
  // put your main code here, to run repeatedly:
  int pirStat = digitalRead(MOTION_SENSOR_PIN); 
  if (pirStat == HIGH) {            // if motion detected
    Serial.println("motion detected");
  } else {
    Serial.println("no motion detected");
  }
  delay(5000);
}
