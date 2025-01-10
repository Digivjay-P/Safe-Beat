/*Safe beats:
Safe beats
This project intends to keep the users bpm in a healthy range.
It works using the pulse sensor. It cross checks the bpm readings with a carefully curated database consisting of bpm ranges for different age groups and individuals.
*/

#include <LiquidCrystal.h>
#include <Keypad.h>
#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#define REPORTING_PERIOD_MS 1000
PulseOximeter pox;
uint32_t tsLastReport = 0;  //unsigned int of 32 bit


int age, athlete, i;  // Use integer for age
float bpm;
int rgbRpin=36, rgbGpin=34, rgbBpin=24, buzzerpin=22, interuptpin=28;  // Assign pin numbers
int del1 = 100, del2 = 200, del3 = 600, del4 = 700;
int warn = 0;
int wait = 3000;
int check = 0;
int oximeterinitialisation = 0;
/*LCD Declaration*/
int contrast = 75;
LiquidCrystal lcd(2, 3, 8, 9, 10, 11);
int displaytime=500;
//keypad
byte rowpins[4] = { 46, 47, 48, 49 };
byte columnpins[4] = { 50, 51, 52, 53 };

char keymap[4][4] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};

Keypad k = Keypad(makeKeymap(keymap), rowpins, columnpins, 4, 4);
//function to take input
int getInput() {
  String input = "";  // Clear previous input

  while (true) {
    char key = k.getKey();
    if (key) {                      //checks if a key is pressed on keypad
      if (key == '#') {             // Confirm input with '#'
        int value = input.toInt();  // Convert input to integer
        lcd.clear();
        lcd.print("Input: ");
        lcd.print(value);
        Serial.println(value);  // Display input value on Serial Monitor
        delay(1000);
        return value;  // Return the input as an integer
      } else {         // Append key to input and display on LCD
        input += key;
        lcd.print(key);
        Serial.print(key);
      }
    }
  }
}
void onBeatDetected() {
  Serial.println("Beat!");
  digitalWrite(rgbBpin, HIGH);
  delay(100);
  digitalWrite(rgbBpin, LOW);
}
float bpmreader() {
  pox.update();
  if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
    Serial.print("Heart Rate:");
    Serial.print(pox.getHeartRate());
    Serial.print(", bpm/SpO2:");
    Serial.print(pox.getSpO2());
    Serial.println("%");
    tsLastReport = millis();
    return pox.getHeartRate();
  }
}
/*Making a structure to make the data easily understandable*/

struct ageGroups {
  int minAge;
  int maxAge;
  int minBPM;
  int maxBPM;
};

ageGroups Categories[] = {
  { 0, 1, 80, 160 },    // infant  -0
  { 1, 2, 80, 130 },    // toddler  -1
  { 2, 5, 80, 120 },    // preschool  -2
  { 5, 11, 70, 110 },   // schoolkid  -3
  { 11, 18, 60, 100 },  // adolescence  -4
  { 18, 90, 60, 100 }   // adult  -5
};

/*Function to check if bpm is out of range for a certain age group*/
int BPMoutofrange(int age, int bpm) {
  for (i = 0; i < 6; i++) {
    if (age >= Categories[i].minAge && age < Categories[i].maxAge) {
      if (bpm < Categories[i].minBPM || bpm > Categories[i].maxBPM) {
        return 1;
      } else {
        return 0;
      }
    }
  }
}

/*Function to check if bpm is in the range for a certain age group*/
int BPMinsideofrange(int age, int bpm) {
  for (i = 0; i < 6; i++) {
    if (age >= Categories[i].minAge && age < Categories[i].maxAge) {
      if (bpm >= Categories[i].minBPM && bpm <= Categories[i].maxBPM) {
        return 0;
      } else {
        return 1;
      }
    }
  }
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
    oximeterinitialisation = 1;
  }
}

void setup() {
  Serial.begin(115200);
  analogWrite(6, contrast);  //VO wale ko 26 mai dalna hai
  lcd.begin(16, 2);
  delay(100);
  lcd.print("Initialzing...");
  delay(1000);
  lcd.clear();
  pinMode(rgbRpin, OUTPUT);
  pinMode(rgbGpin, OUTPUT);
  pinMode(buzzerpin, OUTPUT);
  pinMode(interuptpin, OUTPUT);
}

void loop() {
  lcd.setCursor(0, 0);
  lcd.print("What is your ");
  lcd.setCursor(0, 1);
  lcd.print("Age:");
  Serial.println("Age prompt working:");
  lcd.setCursor(4, 1);
  //delay(1000);
  age = getInput();
  /*to stop the program if age above 90*/
  if (age >= 90) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("LOL!");
    delay(3000);
    lcd.clear();
    return;  // Exit loop, but continue running the program
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Are you an");
  lcd.setCursor(0, 1);
  lcd.print("an athlete");
  Serial.println("Athlete prompt working:");
  delay(3000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("YES=1");
  lcd.print(" or NO=0:");
  athlete = getInput();
  delay(100);
  Serial.print("Age:");
  Serial.println(age);
  Serial.print("Athlete:");
  Serial.println(athlete);
  if (oximeterinitialisation == 0) {
    pulseoximeterinitialization();
  }
  pox.setOnBeatDetectedCallback(onBeatDetected);
  lcd.clear();
  lcd.print("BPM:");
  lcd.setCursor(0, 1);
  lcd.print("SpO2:");
  while (true) {
    float bpmcheck = bpmreader();
    char button = k.getKey();
    if (button) {
      if (button == '*') {
        Serial.println("Starting...");
        check = 1;
        break;
      } else {
        check = 0;
      }
    }
  }

  if (check == 1) {

    bpm = bpmreader();  //reading bpm now

    if (athlete == 1) {
      if (bpm > 60 || bpm < 40) {
        warn = 1;
      }
    }

    if (athlete == 0) {
      warn = BPMoutofrange(age, bpm);
    }

    if (warn == 0) {  // Green light when everything is fine
      digitalWrite(rgbGpin, HIGH);
      digitalWrite(rgbRpin, LOW);
    }

    if (warn == 1) {
      // Alert Light
      digitalWrite(rgbRpin, HIGH);
      digitalWrite(rgbGpin, LOW);
      // Alert Buzzer
      digitalWrite(buzzerpin, HIGH);
      delay(del1);
      digitalWrite(buzzerpin, LOW);
      delay(del2);
      digitalWrite(buzzerpin, HIGH);
      delay(del3);
      digitalWrite(buzzerpin, LOW);
      delay(del4);
      lcd.setCursor(0, 0);
      lcd.print("Dont worry,follow");
      lcd.setCursor(0, 1);
      lcd.print("the instructions");


      while (warn == 1) {

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Breathe IN");
        delay(wait);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Breathe OUT");
        delay(wait);
        lcd.clear();
        warn = BPMinsideofrange(age, bpm);
      }
    }
  }
}