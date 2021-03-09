#include <SPI.h>
#include <RF24.h>

RF24 radio (7,8);
const byte address[6] = "tube1";

void setup() {
  radio.begin ();
  radio.openWritingPipe (address);
  radio.stopListening();
}

void loop() {
  const char msg[] = "Salut!";
  radio.write (&msg, sizeof(msg));
  delay (2000);
}
