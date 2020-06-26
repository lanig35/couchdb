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

//#include "dht.h"
// dht dht11;
#include <SimpleDHT.h>
SimpleDHT11 dht11 (3);

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
const int lampe = 2;

// variables pour les intervalles de capture
unsigned long capteurAvant = 0;
const long priseCapteur = 5000; // 5 secondes

int lumAvant = 0;

void setup()
{
  Serial.begin(9600);

  pinMode (lampe, OUTPUT);
  digitalWrite (lampe, LOW);

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
  mqttClient.onMessageAdvanced (callback);

  // connexion au serveur mosquitto
  if (!mqttClient.connected()) {
    reconnect();
  }
  mqttClient.subscribe ("lampe/salle");
}

void loop()
{
  unsigned long maintenant = millis ();

  // vérifie l'arrivée de message
  mqttClient.loop ();

  if ((maintenant - capteurAvant) >= priseCapteur) {
    capteurAvant = maintenant;

    // récupération de la température
    //    int chk = dht11.read (3);
    //    if (chk != DHTLIB_OK) {
    byte temperature = 0;
    byte humidity = 0;
    int err = SimpleDHTErrSuccess;
    if ((err = dht11.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
      Serial.print("Read DHT11 failed, err="); Serial.println(err);
      //      Serial.print ("Erreur DHT11: "); Serial.println(chk);
    } else {
      Serial.print ("temp: "); Serial.println ((float)temperature); //dht11.temperature);
      Serial.print ("humi: "); Serial.println ((float)humidity); //dht11.humidity);
    }

    // récupération de la luminosité
    int sensor = analogRead (ldrPin);
    int lumiere = map (sensor, 0, 1023, 0, 255);
    Serial.print (F("lum: ")); Serial.println (lumiere);

    if (!mqttClient.connected()) {
      reconnect();
    }

    //    if (chk == DHTLIB_OK) {
    if (temperature != 0) {
      char message[8];
      dtostrf (temperature, 4, 2, message);
      if (mqttClient.publish("temperature/salle", message, true, 0)) {
        Serial.println (F("temperature publiée"));
      } else {
        Serial.print(F("Echec publication, rc="));
      }
      dtostrf (humidity, 4, 2, message);
      if (mqttClient.publish("humidite/salle", message, true, 0)) {
        Serial.println (F("humididté publiée"));
      } else {
        Serial.print(F("Echec publication, rc="));
      }
    }

    if (lumiere != lumAvant) {
      lumAvant = lumiere;
      // publication de la lumiere
      char message [4];
      itoa (lumiere, message, 10);
      if (mqttClient.publish("lumiere/salle", message, true, 0)) {
        Serial.println (F("luminosité publiée"));
      } else {
        Serial.print(F("Echec publication, rc="));
      }
    }
  }
}
