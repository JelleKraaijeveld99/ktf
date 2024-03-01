#include <Arduino.h>
#include <string> 

const int bufferSize = 64;  // Kies een geschikte grootte voor de buffer voor het uitlezen van de berichten
char receivedString[bufferSize];  // Buffer voor ontvangen tekens
boolean newData = false;  // boolean voor nieuwe gegevens in de buffer

void receiveSerialData();

void setup() {
  Serial.begin(115200);  // Seriële communicatie met de computer

  // RX_PIN en TX_PIN zijn de pinnen die je hebt gebruikt voor de verbinding met de ESP32
  int RX_PIN = 0;  // Stel dit in op de daadwerkelijke RX-pin
  int TX_PIN = 1;   // Stel dit in op de daadwerkelijke TX-pin

  pinMode(RX_PIN, INPUT);
  pinMode(TX_PIN, OUTPUT);

  Serial1.begin(115200);  // Seriële communicatie met de ESP32
}

void loop() {
  if (Serial1.available() > 0) {
    Serial.println("Gegevens beschikbaar op Serial.");
  } else {
    Serial.println("Geen gegevens beschikbaar op Serial.");
  }

  delay(100);

  //zet ontvangen gegevens in een string
  receiveSerialData();
  
  // Controleer of er nieuwe gegevens zijn ontvangen
  if (newData) {
    Serial.println("Ontvangen: " + String(receivedString));

    // Reset qde buffer en de indicator voor nieuwe gegevens
    memset(receivedString, 0, bufferSize);
    newData = false;
  }

  delay(1000);  // Voeg eventueel een wachttijd toe om de seriële monitor niet te overladen
}

void receiveSerialData(){
  static byte index = 0;  // Houd de huidige positie in de buffer bij

  while (Serial1.available() > 0) {
    char receivedChar = Serial1.read();

    // Controleer op het einde van de zin
    if (receivedChar == '\n') {
      receivedString[index] = '\0';  // Voeg een null-terminator toe aan het einde van de zin
      index = 0;  // Reset de positie in de buffer
      newData = true;  // Markeer dat er nieuwe gegevens zijn
    } else {
      receivedString[index] = receivedChar;  // Voeg het teken toe aan de buffer
      index = (index + 1) % bufferSize;  // Ga naar de volgende positie in de buffer
    }
  }
}


