#include <SPI.h>
#include <RF24.h>
#include <RF24Network.h>

RF24 radio (7,8);
const byte address[6] = "tube1";

void setup() {
  Serial.begin (9600);
  radio.begin ();
  radio.openReadingPipe (0, address);
  radio.startListening();
}

void loop() {
  if (radio.available()) {
    char msg [32] = "";
    radio.read (&msg, sizeof(msg));
    Serial.println (msg);
  }
}
