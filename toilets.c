// ARDUINO DU BOITIER TOILETTES

#include <LiquidCrystal.h>

// LCD init
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
const int PIN_CONTRAST = 13;
const int PIN_INCLI = 7;

int NB_PEE = 3; // Définis par l'utilisateur sur l'appli
int NB_POOP = 8;

int paperPin = 9;
int peePin = 8;
int poopPin = 6;
int motorPin = 10;

int paperButtonState = 0;
int peeButtonState = 0;
int poopButtonState = 0;
int lastButtonState = paperButtonState; // Empêche de rester appuyé

int lowPaper = 0; // 0 = il y a du papier, 1 = bout du rouleau
int mode = 0;     // 0 = inutilisé, 1 = petite commission, 2 = grosse commission
int paperCredit = -1;

int valueInclinaison = 0;
int alarmOn = 0;

void setup() {
  Serial.begin(9600);
  
  // LCD Screen setup
  lcd.begin(16, 2);
  pinMode(PIN_CONTRAST, OUTPUT);
  digitalWrite(PIN_CONTRAST, LOW);
  lcd.print("Bienvenue !");
  
  // Smartphone's simulation setup
  pinMode(peePin, INPUT);
  pinMode(poopPin, INPUT);
  pinMode(paperPin, INPUT);
  pinMode(motorPin, OUTPUT);
  
  pinMode(PIN_INCLI, INPUT);
}

void loop() {
  paperCredit = getCredit();
  
  checkBoxIntegrity();
  
  if(Serial.available())
  {
    if(Serial.find("paper"))
    {
      paperCredit--;
      givePaper();
    }
    if(Serial.find("done")) 
    {
      Serial.println("Envoi signifiant que les toilettes sont libres : lib");
      countPandemicPoints();
      reinit();
  	}
  }
  
  if(mode != 0) {
    printCredit();
  }
}

// Sélectionne le mode et compte le nombre de feuilles restantes
int getCredit() {
  peeButtonState = digitalRead(peePin);
  poopButtonState = digitalRead(poopPin);
  paperButtonState = digitalRead(paperPin);
  
  if(peeButtonState == HIGH && mode == 0) {
    Serial.println("Envoi signifiant que les toilettes sont occupées : occ");
    mode = 1;
    lcd.clear();
    return NB_PEE;
  } 
  else if(poopButtonState == HIGH && mode == 0) {
    Serial.println("Envoi signifiant que les toilettes sont occupées : occ");
    mode = 2;
    lcd.clear();
    return NB_POOP;
  }
  else if (paperButtonState == HIGH && paperButtonState != lastButtonState) {
    lastButtonState = paperButtonState;
    if(lowPaper == 1) {
      lowPaper = 0;
      Serial.println("Rouleau neuf bien arrivé");
    } else {
      lowPaper = 1;
      Serial.println("Changer le rouleau de papier !");
    }
  }
  else if (paperButtonState == LOW && paperButtonState != lastButtonState) {
    lastButtonState = paperButtonState;
  }
  
  return paperCredit;
}

// Fait tourner le moteur afin de distribuer une feuille 
void givePaper() {
  digitalWrite(motorPin, HIGH);
  delay(200);
  digitalWrite(motorPin, LOW);
}

// Affiche le nombre de feuilles restant
void printCredit() {
  lcd.setCursor(0, 0);
  lcd.print("Credit :");
  lcd.setCursor(0, 1);
  lcd.print(String(paperCredit));
}

// Réinitialise
void reinit() {
  mode = 0;
  paperCredit = -1;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Bienvenue !");
}

// Récupération des Pandemic Points
void countPandemicPoints() {
  Serial.begin(9600);
  if(paperCredit > 0) {
    // Calcul, envoi vers le smartphone, enregistrement en BDD, etc.
    Serial.print("Vous avez gagné ");
    Serial.print(paperCredit);
    Serial.print(" Pandemic Points !");
    Serial.println(" ");
  }
}
void checkBoxIntegrity(){
   valueInclinaison = digitalRead(PIN_INCLI); 
  if(valueInclinaison == 0 && alarmOn == 0 && lowPaper == 0){
   Serial.println("Envoi : ALEEEEERTE !!!!!");
    alarmOn = 1;
  }
  if(valueInclinaison == 1 && alarmOn == 1){
    alarmOn = 0;
  }

}

