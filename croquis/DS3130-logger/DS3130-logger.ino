/*
  mqtt-logger 

  ce croquis se connecte à un serveur Mosquitto pour publier la
  température acquise à partir d'un module RTC DS3231.

  le potentiomètre permet de faire briller une LED

  Circuit:
   carte UNO WiFi Rev.2
*/

#include <SPI.h>
#include <WiFiNINA.h>
#include <WiFiUdp.h>
#include <PubSubClient.h>
#include <NTPClient.h>
#include <TimeLib.h>

#include "arduino_secrets.h"
char ssid[] = SECRET_SSID;  // livebox
char pass[] = SECRET_PASS;  // clé WPA

int status = WL_IDLE_STATUS;
unsigned int localPort = 2390; // port ecoute UDP

// création du client WIFI
WiFiClient client;
WiFiUDP udp;

// création du client MQTT
PubSubClient mqttclient(client);
// création du client NTP
NTPClient ntp (udp);

// adresse et port du serveur Mosquitto
char mqtt_server[] = MQTT_SERVER;
int mqtt_port = MQTT_PORT;

// nom et mot de passe pour connexion MQTT
char mqtt_user[] = MQTT_USER;
char mqtt_pass[] = MQTT_PASS;

// compteur pour connexion Wifi et MQTT
int tentatives = 0;

// 
unsigned long derniereMesure = 0;
unsigned long priseMesure = 10000; // 10 s

void setup() {
  Serial.begin(9600);

  // vérification du module WIFI
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println(F("Communication avec le module Wifi en échec!"));
    // attente infinie
    while (true) {}
  }

  // vérification de la version du firmware
  String fv = WiFi.firmwareVersion();
  if (fv < "1.0.0") {
    Serial.println(F("Mise à jour firmware wifi requise"));
  }

  // tentative de connection au réseau
  tentatives = 0;
  while ((status != WL_CONNECTED) and (tentatives < 10)) {
    Serial.print(F("Tentative de connexion: "));
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
    // attente 10 secondes
    delay(10000);
    tentatives += 1;
  }
  if (tentatives >= 10) {
    Serial.print (F("Abandon Wifi"));
    while (true) {}
  }
  Serial.println(F("Connexion réussie"));
  printWifiStatus();
  delay (10000);

  // initialisation du client MQTT
  mqttclient.setServer (mqtt_server, mqtt_port);
  mqttclient.setCallback (callback);

  // connexion au serveur mosquitto
  tentatives = 0;
  while ((!mqttclient.connected()) and (tentatives < 5)) {
    reconnect();
    tentatives += 1;
  }

  if (tentatives >= 5) {
    Serial.print (F("Erreur conexion MQTT"));
  }
  
  // abonnement au topic "lampe"
  if (!mqttclient.subscribe ("lampe/salon")) {
    Serial.println (F("Erreur abonnement"));
  }

  udp.begin(localPort);
  ntp.begin ();
  ntp.update ();
  Serial.print (ntp.getFormattedTime()); Serial.print (" : ");
  time_t t = ntp.getEpochTime();
  Serial.print (day(t)); Serial.print (" - "); Serial.print (month(t));
  Serial.print (" - "); Serial.println (year(t));
  
}

void loop() {
  unsigned long maintenant = millis ();

  // vérifie l'arrivée de message
  mqttclient.loop ();

  if ((maintenant - derniereMesure) >= priseMesure) {
    derniereMesure = maintenant;
    // récupération de la température
    Serial.print (F("tmp: ")); Serial.println (10);

    // connexion au serveur mosquitto
    tentatives = 0;
    while ((!mqttclient.connected()) and (tentatives < 5)) {
      reconnect();
      tentatives += 1;
    }

    if (tentatives >= 5) {
      Serial.print (F("Erreur conexion MQTT"));
    } else {
      // publication de la température
      char message[8] = "10";
      // dtostrf (temperature, 4, 2, message);
      if (mqttclient.publish("temperature/salon", message, true)) {
        Serial.println (F("Température publiée"));
      } else {
        Serial.print(F("Echec publication, rc="));
        Serial.print(mqttclient.state());
      }
    }
  }
}
