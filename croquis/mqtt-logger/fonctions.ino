// fonction appelée sur réception d'un message
void callback (char *topic, byte* message, unsigned int taille) {
  Serial.print ("message reçu ["); Serial.print (topic);
  Serial.print (" - "); Serial.print (taille);
  Serial.print("] ");
  for (int i = 0; i < taille; i++) {
    Serial.print((char)message[i]);
  }
  Serial.println();

  char cmd [5];
  memcpy (cmd, message, taille);
  cmd [taille] = '\0';
  int action = (int) strtol (cmd, (char**)NULL, 10);
  Serial.println (action);
  if (action == 0) {
    digitalWrite (lampe, LOW);
  } else if (action == 1) {
    digitalWrite (lampe, HIGH);
  } else {
    Serial.println ("erreur message");
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
