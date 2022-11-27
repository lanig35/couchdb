#include <VirtualWire.h>

char message [40];

// pour gestion delai entre 2 acqusitions
const long delai_releve = 20000;
unsigned long dernier_releve = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin (9600);
  
  vw_set_tx_pin (7);
  vw_set_rx_pin (8);
  vw_set_ptt_pin (6);
  
  vw_setup (300);

}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned long maintenant = millis ();

  if (maintenant - dernier_releve > delai_releve) {
    dernier_releve = maintenant;

    sprintf (message, "%s", "Hello!");
    Serial.println (message);

    // émission message
    vw_send (message, strlen(message) + 1);
    vw_wait_tx ();
  }
}
