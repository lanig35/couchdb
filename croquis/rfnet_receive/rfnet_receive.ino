#include <SPI.h>
#include <RF24.h>
#include <RF24Network.h>

RF24 radio (7,8);
RF24Network net (radio);

const uint16_t seeduino = 01;
const uint16_t arduino = 02;
const uint16_t master = 00;

struct payload_t {
  char location [16];
  unsigned long ms;
  unsigned long counter;
};

void setup() {
  Serial.begin (9600);
  Serial.println ("RF24 Network");

  SPI.begin ();
  radio.begin ();
  net.begin (90, master);
}

void loop() {
  net.update ();

  while (net.available ()) {
    RF24NetworkHeader header;
    payload_t msg;
    net.read (header, &msg, sizeof(msg));

    Serial.print ("#: "); Serial.print (msg.counter);
    Serial.print (" at "); Serial.print (msg.ms);
    Serial.print (" from "); Serial.println (msg.location);
  }
}
