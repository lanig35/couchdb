/*
  mqtt-logger

  ce croquis se connecte à un serveur Mosquitto pour publier la
  température, la luminosité et la valeur d'un potentiomètre.

  le potentiomètre permet de faire briller une LED

  Circuit:
   carte UNO WiFi Rev.2

  TMP36: connecté en A2
  LDR: connecté en A1
  potentiomètre: connecté en A0
  LED: connectée en D3 (brillance gérée par potentiomètre)
  LED connectée en D2: on/off selon dernier message MQTT reçu
  LED connectée en D4:
    clignote sans interruption en cas de problème
    clignote pour signaler l'envoi de message
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

// variables pour la gestion des capteurs et LEDs
const int potPin = A0;
const int tmpPin = A2;
const int ldrPin = A1;
const int ledPin = 3;
const int lampe = 2;
const int indicateur = 4;

float tempAvant = 0.0;
int lumAvant = 0;
int ledAvant = 0;

// variables pour les intervalles de capture
unsigned long capteurAvant = 0;
unsigned long potAvant = 0;
const long priseCapteur = 5000; // 5 secondes
const long prisePot = 1000; // 1 secondes

// compteur pour connexion Wifi et MQTT
int tentatives = 0;

void setup() {
  Serial.begin(9600);

  // préparation des entrées-sorties
  pinMode (ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  pinMode (lampe, OUTPUT);
  digitalWrite (lampe, LOW);
  pinMode (indicateur, OUTPUT);
  digitalWrite (indicateur, LOW);

  // vérification du module WIFI
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println(F("Communication avec le module Wifi en échec!"));
    // attente infinie
    while (true) {
      digitalWrite (indicateur, HIGH);
      delay (100);
      digitalWrite (indicateur, LOW);
    }
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
    while (true) {
      digitalWrite (indicateur, HIGH);
      delay (300);
      digitalWrite (indicateur, LOW);
    }
  }
  Serial.println(F("Connexion réussie"));
  printWifiStatus();

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
    for (int i = 0; i < 5; i++) {
      digitalWrite (indicateur, HIGH);
      delay (500);
      digitalWrite (indicateur, LOW);
    }
  }
  // abonnement au topic "lampe"
  if (!mqttclient.subscribe ("lampe/salon")) {
    Serial.println (F("Erreur abonnement"));
    for (int i = 0; i < 5; i++) {
      digitalWrite (indicateur, HIGH);
      delay (400);
      digitalWrite (indicateur, LOW);
    }
  }
}

void loop() {
  unsigned long maintenant = millis ();

  // vérifie l'arrivée de message
  mqttclient.loop ();

  if ((maintenant - capteurAvant) >= priseCapteur) {
    capteurAvant = maintenant;
    // récupération de la température
    int sensor = analogRead (tmpPin);
    float tmpVolt = (sensor / 1024.0) * 5.0;
    float temperature = (tmpVolt - 0.5) * 100;
    Serial.print (F("tmp: ")); Serial.println (temperature);

    // récupération de la luminosité
    sensor = analogRead (ldrPin);
    int lumiere = map (sensor, 0, 1023, 0, 255);
    Serial.print (F("lum: ")); Serial.println (lumiere);

    // connexion au serveur mosquitto
    tentatives = 0;
    while ((!mqttclient.connected()) and (tentatives < 5)) {

      reconnect();
      tentatives += 1;
    }

    if (tentatives >= 5) {
      Serial.print (F("Erreur conexion MQTT"));
      for (int i = 0; i < 5; i++) {
        digitalWrite (indicateur, HIGH);
        delay (500);
        digitalWrite (indicateur, LOW);
      }
    } else {
      if (temperature != tempAvant) {
        tempAvant = temperature;
        // publication de la température
        char message[8];
        dtostrf (temperature, 4, 2, message);
        if (mqttclient.publish(F("temperature/salon"), message, true)) {
          Serial.println (F("Température publiée"));
          digitalWrite (indicateur, HIGH); delay (200); digitalWrite (indicateur, LOW);
        } else {
          Serial.print(F("Echec publication, rc="));
          Serial.print(mqttclient.state());
        }
      }

      if (lumiere != lumAvant) {
        lumAvant = lumiere;
        // publication de la lumière
        char message [4];
        itoa (lumiere, message, 10);
        if (mqttclient.publish(F("lumiere/salon"), message, true)) {
          Serial.println (F("Lumière publiée"));
          digitalWrite (indicateur, HIGH); delay (200); digitalWrite (indicateur, LOW);
        } else {
          Serial.print(F("Echec publication, rc="));
          Serial.print(mqttclient.state());
        }
      }
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
    tentatives = 0;
    while ((!mqttclient.connected()) and (tentatives < 5)) {
      reconnect();
      tentatives += 1;
    }

    if (tentatives >= 5) {
      Serial.print (F("Erreur conexion MQTT"));
      for (int i = 0; i < 5; i++) {
        digitalWrite (indicateur, HIGH);
        delay (500);
        digitalWrite (indicateur, LOW);
      }
    } else {
      if (ledValeur != ledAvant) {
        ledAvant = ledValeur;
        // publication du potentiomètre
        char message [4];
        itoa (ledValeur, message, 10);
        if (mqttclient.publish(F("pot/salon"), message, true)) {
          Serial.println (F("Potentiomètre publié"));
          digitalWrite (indicateur, HIGH); delay (200); digitalWrite (indicateur, LOW);
        } else {
          Serial.print(F("Echec publication, rc="));
          Serial.print(mqttclient.state());
        }
      }
    }
  }
}
