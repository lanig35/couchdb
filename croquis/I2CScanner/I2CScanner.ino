#include <Wire.h>

int TWBR_val [] = {152,72,32, 12};
int vitesse [] = {50,100,200,400};

void setup()
{
  Wire.begin();
 
  Serial.begin(9600);
  
  Serial.println("\nScanner I2C");
  // Serial.print ("TWBR: "); Serial.println (TWBR);
}
 
void loop()
{
  byte erreur, adresse;
  int nbPeripheriques;
 
  Serial.println("Recherche...");
 
  nbPeripheriques = 0;
  for(adresse = 8; adresse < 127; adresse++ )
  {
    Wire.beginTransmission(adresse);
    erreur = Wire.endTransmission();
 
    if (erreur == 0)
    {
      Serial.print("Périphérique I2C présent: adresse 0x");
      if (adresse<16)
        Serial.print("0");
      Serial.print(adresse,HEX);
      Serial.println(" - Vérification Vitesse");
 /*     
      for (int i=0; i<4; i++) {
        TWBR = TWBR_val [i];
        Wire.beginTransmission (adresse);
        if (Wire.endTransmission() == 0) {
          Serial.print (vitesse[i]); Serial.println (" Mhz");
        }
        delay (10); // attente 10 ms 
      }
 */
      nbPeripheriques++;
    }    
  }
  if (nbPeripheriques == 0) {
    Serial.println("Aucun périphérique I2C trouvé\n");
  }
  Serial.println("Fini\n");
 
  delay(10000);           // Attente 10 secondes avant prochaine recherche
}
