/*arduino:avr:uno
  Dave Skura, 2022

	interesting
	https://www.youtube.com/watch?v=8ZEQEV-Stkc

	transistor pinout
	https://components101.com/transistors/bc547-transistor-pinout-datasheet

	start with this
	https://create.arduino.cc/projecthub/SBR/switching-using-transistor-c9114a

	connect 9v battery as follows:
	(+) lead replaces the 5v lead from the arduino.  Still uses resistor to Collector (C)
	Nothing changes to the base
	(-) lead connects to Emmitter, which is arduino GND

*/

#define VOLT_PIN A0
const int transistor = 2;

void setup()
{
  pinMode (transistor, OUTPUT);
	digitalWrite(transistor,LOW);
  analogWrite(VOLT_PIN,0);
}

void loop()
{
	digitalWrite(transistor,HIGH);
  delay(2000);
	digitalWrite(transistor,LOW);
  delay(2000);

}