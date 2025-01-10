#include <LiquidCrystal.h> 
int Contrast=75;
 LiquidCrystal lcd(0, 1, 8, 9, 10, 11); 

 void setup()
 {
    analogWrite(6,Contrast);
     lcd.begin(16, 2);
 } 
     void loop()
 {
   
     lcd.setCursor(0, 0);
     lcd.print("HELLO WORLD!");
   
 

   
}
