#include <Wire.h>
#include "MAX30100_PulseOximeter.h"

#include <LiquidCrystal.h>

#define REPORTING_PERIOD_MS 1000
PulseOximeter pox;
uint32_t tsLastReport = 0;
int rgbBpin=24;
int contrast = 110;
int interuptpin=28;
int  oximeterinitialisation,bpmread;
int displaytime=100;
LiquidCrystal lcd(2, 3, 8, 9, 10, 11);

void onBeatDetected() {
  Serial.print("Beat!");
  digitalWrite(rgbBpin, HIGH);
  delay(100);
  digitalWrite(rgbBpin, LOW);
}

void pulseoximeterinitialization() {
  /*to interupt the unintitalised state*/
  digitalWrite(interuptpin, HIGH);
  delay(10);
  digitalWrite(interuptpin, LOW);
  if (!pox.begin()) {
    Serial.println("FAILED");
    for (;;)
      ;
  } else {
    Serial.println("SUCCESS");
    lcd.clear();
    lcd.print("Place your finger");
    delay(100);
    lcd.clear();
    oximeterinitialisation = 1;
  }
}

float bpmreader() {
  pox.update();
  if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
    Serial.print("Heart Rate:");
    Serial.print(pox.getHeartRate());
    lcd.setCursor(4, 0);
    lcd.print(pox.getHeartRate());
    lcd.setCursor(5, 1);
    lcd.print(pox.getSpO2());
    lcd.print("%");
    delay(displaytime);
    Serial.print(", bpm/SpO2:");
    Serial.print(pox.getSpO2());
    Serial.println("%");
    tsLastReport = millis();
    return pox.getHeartRate();
  }
}
void setup() {
  Serial.begin(115200);
  analogWrite(6, contrast);  //VO wale ko 26 mai dalna hai
  lcd.begin(16, 2);
}

void loop() {
  lcd.print("BPM:");
  lcd.setCursor(0, 1);
  lcd.print("SpO2:");
  if(oximeterinitialisation==0){
    pulseoximeterinitialization();
  }
 bpmread=bpmreader();
}