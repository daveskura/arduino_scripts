/*arduino:avr:nano
  Dave Skura, 2022
	Ultrasonic Distance Sensor - HC-SR04
*/

const int trigPin = 9;
const int echoPin = 10;

// defines variables
long gbl_duration;
int gbl_distance;

void setup() {
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  Serial.begin(9600); // Starts the serial communication
  Serial.print("hello");
}
void loop() {
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  gbl_duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  gbl_distance = gbl_duration * 0.034 / 2;
 
  Serial.print("gbl_duration: ");
  Serial.print(gbl_duration);
  Serial.print(", ");

  // Prints the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.println(gbl_distance);
	delay(2000);
}