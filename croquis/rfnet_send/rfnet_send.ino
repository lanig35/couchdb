#include <SPI.h>
#include <RF24.h>
#include <RF24Network.h>

RF24 radio (7,8);
RF24Network net (radio);

const uint16_t node = 02;
const uint16_t master = 00;

const unsigned long interval = 4000;
unsigned long last_sent;
unsigned long packets_sent;

struct payload_t {
  char location [16];
  unsigned long ms;
  unsigned long counter;
};

void setup() {
  SPI.begin ();
  radio.begin ();
  net.begin (90, node);
  packets_sent = 0;
}

void loop() {
  net.update ();

  unsigned long now = millis ();
  if (now - last_sent >= interval) {
    last_sent = now;
    payload_t msg = {"arduino", now, packets_sent};
    RF24NetworkHeader header (master);
    net.write (header, &msg, sizeof(msg));
    packets_sent++;
  }
}
