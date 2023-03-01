
#include <TinkerKit.h>
#include <matrix_lcd_commands.h>
#include <TKLCD.h>
#include <LiquidCrystal.h>
#include <Wire.h>
 
TKLCD_Local lcd = TKLCD_Local();
TKThermistor therm(A0);
 
void setup() {
  lcd.begin();
}
 
void loop() {
 
  int temp = therm.readCelsius();
 
  lcd.clear();
  lcd.print("Temp: ");
  lcd.print(temp);
  delay(200); 

}
