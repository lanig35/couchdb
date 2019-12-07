
/*
  mqtt-logger

  ce croquis se connecte à un serveur Mosquitto pour publier la
  température et la valeur d'un potentiomètre.

  le potentiomètre permet de faire briller une LED

  Circuit:
   carte UNO WiFi Rev.2
   
  TMP36: connecté en A3
  potentiomètre: connecté en A0
  LED: connectée en D3

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

// variables pour la gestion des capteurs
const int potPin = A0;
const int tmpPin = A3;
const int ledPin = 3;

unsigned long tmpAvant = 0;
unsigned long potAvant = 0;
const long priseTemp = 5000; // 5 secondes
const long prisePot = 1000;

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ;
  }

  // vérification du module WIFI
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println(F("Communication avec le module Wifi en échec!"));
    // attente infinie
    while (true);
  }

  // vérification de la version du firmware
  String fv = WiFi.firmwareVersion();
  if (fv < "1.0.0") {
    Serial.println(F("Mise à jour firmware wifi requise"));
  }

  // tentative de connection au réseau
  while (status != WL_CONNECTED) {
    Serial.print(F("Tentative de connexion: "));
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
    // attente 10 seconds
    delay(10000);
  }
  Serial.println(F("Connexion réussie"));
  printWifiStatus();

  // initialisation du client MQTT
  mqttclient.setServer (mqtt_server, mqtt_port);

  // préparation des entrées-sorties
  pinMode (ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
}

void loop() {
  unsigned long maintenant = millis ();

  if ((maintenant - tmpAvant) >= priseTemp) {
    tmpAvant = maintenant;
    // récupération de la température
    int sensor = analogRead (tmpPin);
    float tmpVolt = (sensor / 1024.0) * 5.0;
    float temperature = (tmpVolt - 0.5) * 100;
    Serial.print (F("tmp: ")); Serial.println (temperature);

    // connexion au serveur mosquitto
    if (!mqttclient.connected()) {
      reconnect();
    }
    // publication de la température
    char message[8];
    dtostrf (temperature, 4, 2, message);
    if (mqttclient.publish(F("arduino/temp"), message)) {
      Serial.println (F("Température publiée"));
    } else {
      Serial.print(F("Echec publication, rc="));
      Serial.print(mqttclient.state());
    }
  }

  if ((maintenant - potAvant) >= prisePot) {
    potAvant = maintenant;
    // lecture du potentiomètre
    int sensor = analogRead (potPin);
    int ledValeur = map (sensor, 0, 1023, 0, 255);
    analogWrite (ledPin, ledValeur);
    Serial.print (F("pot: ")); Serial.println (ledValeur);

    // connexion au serveur mosquitto
    if (!mqttclient.connected()) {
      reconnect();
    }
    // publication du potentiomètre
    char message [4];
    itoa (ledValeur, message, 10);
    if (mqttclient.publish(F("arduino/pot"), message)) {
      Serial.println (F("Potentiomètre publié"));
    } else {
      Serial.print(F("Echec publication, rc="));
      Serial.print(mqttclient.state());
    }
  }
}

// connexion au serveur Mosquitto
void reconnect() {
  while (!mqttclient.connected()) {
    Serial.print(F("Tentative de connexion MQTT..."));
    if (mqttclient.connect(F("arduinoClient"), mqtt_user, mqtt_pass)) {
      Serial.println(F("Connexion réussie"));
    } else {
      Serial.print(F("connexion en échec, rc="));
      Serial.print(mqttclient.state());
      Serial.println(F("tentative dans 5 secondes"));
      delay(5000);
    }
  }
}

void printWifiStatus() {
  // affiche le nom de la box
  Serial.print(F("SSID: "));
  Serial.println(WiFi.SSID());

  // affiche l'adresse IP
  IPAddress ip = WiFi.localIP();
  Serial.print(F("Adresse IP: "));
  Serial.println(ip);

  // affiche la force du signal
  long rssi = WiFi.RSSI();
  Serial.print(F("Force du signal (RSSI): "));
  Serial.print(rssi);
  Serial.println(F(" dBm"));
}
