#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
const int PIN_CONTRAST = 7;
const int PIN_BUZZER = 9;
const int ON = 1;
const int OFF =0;

int alarm = 0;
int alarmButtonState = 0;
int alarmOff = 0;
int occupied = 0;

void setup() {
  Serial.begin(9600);

  // set up the LCD:
  lcd.begin(16, 2);  
  pinMode(PIN_CONTRAST, OUTPUT);
  digitalWrite(PIN_CONTRAST, LOW);
  lcd.setCursor(1, 0);
  lcd.print("LIBRE");
  
  //setup buzzer
  pinMode(PIN_BUZZER, OUTPUT);
  noTone(PIN_BUZZER);
}

void loop() {
  	checkAlarm();
    checkIfOccupied();
    
  
}  

void checkAlarm(){
  if(Serial.find("on"))
    {
      Serial.println("Alarme activee");
      alarm = ON;
      tone(PIN_BUZZER, 250); 

      lcd.clear();
      lcd.print("!!! ALERTE !!!");
    }
    if(Serial.find("off"))
    {
      noTone(PIN_BUZZER);

      Serial.println("Alarme desactivee");
      alarm = OFF;
      lcd.clear(); 

      if(occupied == 1){
        lcd.print("OCCUPE");
      }else 
      {
        lcd.print("LIBRE");
      }
    }
    
}

void checkIfOccupied()
{
  if(Serial.find("occ"))
    {
      occupied = 1;
      lcd.clear();
      lcd.print("OCCUPE");
    }
    if(Serial.find("lib"))
    {
      occupied = 0;
      lcd.clear();
      lcd.print("LIBRE");
    }
}
