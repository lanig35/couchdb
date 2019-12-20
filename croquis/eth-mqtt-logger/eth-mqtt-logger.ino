/*
  Basic MQTT example

  This sketch demonstrates the basic capabilities of the library.
  It connects to an MQTT server then:
  - publishes "hello world" to the topic "outTopic"
  - subscribes to the topic "inTopic", printing out any messages
    it receives. NB - it assumes the received payloads are strings not binary

  It will reconnect to the server if the connection is lost using a blocking
  reconnect function. See the 'mqtt_reconnect_nonblocking' example for how to
  achieve the same result without blocking the main loop.

*/

#include <SPI.h>
#include <Ethernet2.h>
#include <MQTT.h>

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = {
  0x90, 0xA2, 0xDA, 0x10, 0xF7, 0x05
};

EthernetClient ethClient;
MQTTClient mqttClient;

#include "secrets.h"
// adresse et port du serveur Mosquitto
char mqtt_server[] = MQTT_SERVER;
int mqtt_port = MQTT_PORT;
// nom et mot de passe pour connexion MQTT
char mqtt_user[] = MQTT_USER;
char mqtt_pass[] = MQTT_PASS;

// variables pour la gestion des capteurs
const int ldrPin = A0;

unsigned long ldrAvant = 0;
const long priseLdr = 5000; // 5 secondes

void setup()
{
  Serial.begin(9600);

  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    for (;;)
      ;
  }
  Serial.println(F("Connexion réussie"));
  // Allow the hardware to sort itself out
  delay(1500);
  printIPAddress ();

  mqttClient.begin (mqtt_server, mqtt_port, ethClient);

    // connexion au serveur mosquitto
    if (!mqttClient.connected()) {
    reconnect();
  }
}


void loop()
{
  unsigned long maintenant = millis ();

  // vérifie l'arrivée de message
  mqttClient.loop ();

  if ((maintenant - ldrAvant) >= priseLdr) {
    ldrAvant = maintenant;
    int sensor = analogRead (ldrPin);
    int ldrValeur = map (sensor, 0, 1023, 0, 255);
    Serial.print (F("lum: ")); Serial.println (ldrValeur);

    if (!mqttClient.connected()) {
      reconnect();
    }

    // publication
    // publication du potentiomètre
    char message [4];
    itoa (ldrValeur, message, 10);
    if (mqttClient.publish("lum/eth", message)) {
      Serial.println (F("luminosité publiée"));
    } else {
      Serial.print(F("Echec publication, rc="));
    }
  }
}
