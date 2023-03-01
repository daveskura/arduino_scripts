// Using 4 pin RGB LED with common Cathode
#define BLUE 6 // was 3
#define GREEN 5
#define RED 3 // was 6

// LM35 temperature sensor 
#define TEMPERATURE_SENSOR_PIN 1
#define ROOM_TEMPERATURE 21

float VLITTLE_COOL_TEMPERATURE;
float LITTLE_COOL_TEMPERATURE; 
float BIT_COOL_TEMPERATURE;
float COOL_TEMPERATURE;


float vLITTLE_WARM_TEMPERATURE;
float LITTLE_WARM_TEMPERATURE;
float BIT_WARM_TEMPERATURE;
float WARM_TEMPERATURE;

double temperature = 0;
double atemp = 0;

// put your setup code here, to run once:
void setup() {
  Serial.begin(9600);

  VLITTLE_COOL_TEMPERATURE = ROOM_TEMPERATURE - 2;
  LITTLE_COOL_TEMPERATURE = ROOM_TEMPERATURE - 3;
  BIT_COOL_TEMPERATURE = ROOM_TEMPERATURE - 4; 
  COOL_TEMPERATURE = ROOM_TEMPERATURE - 5; 
  
  vLITTLE_WARM_TEMPERATURE = ROOM_TEMPERATURE + 2; 
  LITTLE_WARM_TEMPERATURE = ROOM_TEMPERATURE + 3; 
  BIT_WARM_TEMPERATURE = ROOM_TEMPERATURE + 4; 
  WARM_TEMPERATURE = ROOM_TEMPERATURE + 5; 

  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE,OUTPUT);
  setColor(0, 25, 0); // green - to show on ... before taking a temp reading to adjust light
}

void loop() {
  atemp = analogRead(TEMPERATURE_SENSOR_PIN);
  temperature = (atemp*500)/1023;
  Serial.println(temperature);
  temp_color(temperature);
  delay(5000);
}


void setColor(int red, int green, int blue)
{
analogWrite(RED, red);
analogWrite(GREEN, green);
analogWrite(BLUE, blue);
}

void temp_color(float temperature)
{
  if ((temperature > VLITTLE_COOL_TEMPERATURE) and (temperature < vLITTLE_WARM_TEMPERATURE)) {
    setColor(0, 25, 0); // green - to show on ... before taking a temp reading to adjust light
  }
  if (temperature < VLITTLE_COOL_TEMPERATURE) {
    setColor(50, 0, 75); 
  }
  if (temperature < LITTLE_COOL_TEMPERATURE) {
    setColor(50, 0, 150); 
  }
  if (temperature < BIT_COOL_TEMPERATURE) {
    setColor(50, 0, 255); 
  }
  if (temperature < COOL_TEMPERATURE) {
    setColor(0, 0, 255); 
  }

  if (temperature > vLITTLE_WARM_TEMPERATURE) {
    setColor(75, 0, 50); 
  }
  if (temperature > LITTLE_WARM_TEMPERATURE) {
    setColor(150, 0, 50); 
  }
  if (temperature > BIT_WARM_TEMPERATURE) {
    setColor(255, 0, 50); 
  }
  if (temperature > WARM_TEMPERATURE) {
    setColor(255, 0, 0); 
  }

}
