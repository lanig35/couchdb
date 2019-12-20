// fonction appelée sur réception d'un message
void callback (char *topic, byte* message, unsigned int taille) {
  Serial.print ("message reçu ["); Serial.print (topic);
  Serial.print (" - "); Serial.print (taille);
  Serial.print("] ");
  for (int i = 0; i < taille; i++) {
    Serial.print((char)message[i]);
  }
  Serial.println();

  /*
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
  */
}

// connexion au serveur Mosquitto
void reconnect() {
  Serial.print("connecting...");
  while (!mqttClient.connect("eth-1", mqtt_user, mqtt_pass)) {
    Serial.println(".");
    delay(5000);
  }
  Serial.println("\nconnected!");
}

void printIPAddress () {
  // print your local IP address:
  Serial.print("My IP address: ");
  for (byte thisByte = 0; thisByte < 4; thisByte++) {
    // print the value of each byte of the IP address:
    Serial.print(Ethernet.localIP()[thisByte], DEC);
    Serial.print(".");
  }
  Serial.println();
}
