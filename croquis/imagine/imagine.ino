// 74HC164 registre
const int horloge = 2;
const int data = 3;

const int tmpPIN = A2;
const int ldrPIN = A3;

const int boutonPIN = 4;
int dernierEtatBouton = LOW;
int etatBouton = LOW;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

unsigned long previousMillis = 0;
const long interval = 1000;

byte registre = 0;

#define NOTE_A4  440

float getTemperature (const int pin) {
  int sensor = analogRead (pin);
  float tmpVolt = (sensor / 1024.0) * 5.0;
  return ((tmpVolt - 0.5) * 100);
}

int getLumiere (const int pin) {
  float sensor = analogRead (pin);
  int lumiere = map (sensor, 0, 1023, 0, 255);
  return (lumiere);
}

bool getBouton (const int pin) {
  bool retour = false;
  
  int lecture = digitalRead (pin);
  if (lecture != dernierEtatBouton) {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (lecture != etatBouton) {
      etatBouton = lecture;
      if (etatBouton == HIGH) {
        retour = true;
      }
    }
  }
  return (retour);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin (9600);

  pinMode (5, OUTPUT);
  pinMode (boutonPIN, INPUT_PULLUP);
  pinMode (12, INPUT_PULLUP);
  
  pinMode (horloge, OUTPUT);
  pinMode (data, OUTPUT);
  shiftOut (data, horloge, LSBFIRST, B10101010);
  delay (5000);
}

void loop() {
  // put your main code here, to run repeatedly:
  // if (getBouton(boutonPIN) == true) {
  if (digitalRead (boutonPIN) == LOW) {
    Serial.println ("BOUTON");
  }

  if (digitalRead (12) == LOW) {
    Serial.println ("CAPACITY");
  }
  
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    
    Serial.print (F("tmp: ")); Serial.print (getTemperature(tmpPIN));
    Serial.print (F(" - lum: ")); Serial.print (getLumiere(ldrPIN));
    Serial.print (F(" - TCH: ")); Serial.println (analogRead(A1));
    
    shiftOut (data, horloge, MSBFIRST, 1 << registre);
    registre = registre + 1;
    if (registre >= 8) {
      registre = 0;
      tone (5,NOTE_A4,20);
    }
  }
}
