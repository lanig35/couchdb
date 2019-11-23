
/*
  mqtt-test

  ce croquis se connecte à un serveur Mosquitto pour publier un message.

  Circuit:
   carte UNO WiFi Rev.2)
*/


#include <SPI.h>
#include <WiFiNINA.h>
#include <PubSubClient.h>

#include "arduino_secrets.h"
char ssid[] = SECRET_SSID;  // livebox
char pass[] = SECRET_PASS;  // clé WPA

int status = WL_IDLE_STATUS;

// création du client WIFI
WiFiClient client;
// création du client MQTT
PubSubClient mqttclient(client);
// adresse et port du serveur Mosquitto
char mqtt_server[] = MQTT_SERVER;
int mqtt_port = MQTT_PORT;
// nom et mot de passe pour connexion MQTT
char mqtt_user[] = MQTT_USER;
char mqtt_pass[] = MQTT_PASS;

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ;
  }

  // vérification du module WIFI
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication avec le module Wifi en échec!");
    // attente infinie
    while (true);
  }

  // vérification de la version du firmware
  String fv = WiFi.firmwareVersion();
  if (fv < "1.0.0") {
    Serial.println("Mise à jour firmware wifi requise");
  }

  // tentative de connection au réseau
  while (status != WL_CONNECTED) {
    Serial.print("Tentative de connexion: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
    // attente 10 seconds
    delay(10000);
  }
  Serial.println("Connexion réussie");
  printWifiStatus();

  // initialisation du client MQTT
  mqttclient.setServer (mqtt_server, mqtt_port);
}

void loop() {
  // connexion au serveur mosquitto
  if (!mqttclient.connected()) {
    reconnect();
  }
  // publication d'un message et attente 5 secondes
  if (mqttclient.publish("arduino", "Salut!")) {
    Serial.println ("Message publié");
  } else {
    Serial.print("Echec publication, rc=");
    Serial.print(mqttclient.state());
  }
  delay (5000);
}

// connexion au serveur Mosquitto
void reconnect() {
  while (!mqttclient.connected()) {
    Serial.print("Tentative de connexion MQTT...");
    if (mqttclient.connect("arduinoClient", mqtt_user, mqtt_pass)) {
      Serial.println("Connexion réussie");
    } else {
      Serial.print("connexion en échec, rc=");
      Serial.print(mqttclient.state());
      Serial.println(" tentative dans 5 secondes");
      delay(5000);
    }
  }
}

void printWifiStatus() {
  // affiche le nom de la box
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // affiche l'adresse IP
  IPAddress ip = WiFi.localIP();
  Serial.print("Adresse IP: ");
  Serial.println(ip);

  // affiche la force du signal
  long rssi = WiFi.RSSI();
  Serial.print("Force du signal (RSSI): ");
  Serial.print(rssi);
  Serial.println(" dBm");
}
