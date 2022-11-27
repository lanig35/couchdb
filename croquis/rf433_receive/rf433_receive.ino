#include <VirtualWire.h>

void setup() {
  // put your setup code here, to run once:
  Serial.begin (9600);

  vw_set_rx_pin (7);
  vw_set_tx_pin(6);
  vw_set_ptt_pin (5);
  
  vw_setup (300);
  vw_rx_start ();

  Serial.println ("Lancement");
}

void loop() {
  // put your main code here, to run repeatedly:
  if (vw_have_message()) {
    byte taille = 12;
    byte message [12];

    if (vw_get_message (message, &taille)) {
      Serial.println ((char*)message);
    } else {
      Serial.print ("bad: "); Serial.println (vw_get_rx_bad());
    }
  }
}
