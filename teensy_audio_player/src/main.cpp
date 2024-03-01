#include "audio_core.h"
#include "string.h"
#include <string>
#include <Arduino.h>

using namespace std;

const int bufferSize = 64;  // Kies een geschikte grootte voor de buffer voor het uitlezen van de berichten
char receivedString[bufferSize];  // Buffer voor ontvangen tekens
boolean newData = false;  // boolean voor nieuwe gegevens in de buffer

int espStarted = 0;

void receiveSerialData();

AudioCore audio;
// AudioCore audio2;

String filetemponame;
int range;
int beatIndicator;
const int size = 4;
String bpm100[] = {"DRM1.WAV", "DRM2.WAV", "DRM3.WAV", "DRM4.WAV"};
String bpm85[] = {"DRM5.WAV", "DRM6.WAV", "DRM7.WAV", "DRM8.WAV"};
String bpm70[] = {"DRM9.WAV", "DRM10.WAV", "DRM11.WAV", "DRM12.WAV"};
String wavFileList[size];

int amountOfFoundSounds; //variable for checking how many sounds are found

void setup()
{   
    Serial.begin(115200);  // Seriële communicatie met de computer
    Serial1.begin(115200);  // Seriële communicatie met de ESP32
    
    range = 20;
    beatIndicator = 0;
    audio.setupAudio();
    audio.setSoundFound(0);
    // audio2.setupAudio();
}

void loop()
{   
  amountOfFoundSounds = audio.getSoundsFound();//function for getting the amount of sounds found
  Serial.print("AMOUNT OF SOUNDS FOUND: ");
  Serial.println(amountOfFoundSounds);

  if(amountOfFoundSounds == 0){

    audio.setGainMix1(0.0); //function for setting the amp of the master to 0;
    Serial.print("STAGE 1!");
    audio.setWavNames(amountOfFoundSounds);//function for setting the right wav names for the second track
    audio.playMaster("FND1.WAV");
  }

  if(amountOfFoundSounds == 1){
    audio.setGainMix1(0.5);
    Serial.print("STAGE 2!");
    audio.setWavNames(amountOfFoundSounds);//function for setting the right wav names for the second track
    audio.playMaster("FND1.WAV");
  }

  if(amountOfFoundSounds == 2){
    audio.setGainMix1(0.5);
    Serial.print("STAGE 3!");
    audio.setWavNames(amountOfFoundSounds);//function for setting the right wav names for the second track
    audio.playMaster("FND2.WAV");
  }

  if(amountOfFoundSounds == 3){
    audio.setGainMix1(0.5);
    Serial.print("STAGE 4!");
    audio.playAudio("FND3.WAV");
  }


    //     if (Serial1.available() > 0) {
    //         espStarted = 1;
    //         Serial.println("Gegevens beschikbaar op Serial.");
    //      } else {
    //         Serial.println("Geen gegevens beschikbaar op Serial.");
    //         espStarted = 0;
    //      }

    // //zet ontvangen gegevens in een string
    //     receiveSerialData();
    
    // // Controleer of er nieuwe gegevens zijn ontvangen
    //     if (newData) {
    //         Serial.println("Ontvangen: " + String(receivedString));

    //         // Reset qde buffer en de indicator voor nieuwe gegevens
    //         memset(receivedString, 0, bufferSize);
    //         newData = false;
    //     }

    // if(espStarted == 1){
    //     Serial.println("GELUIDJE");
    //     audio.playAudio("76K2.WAV");
    //     espStarted = 0;
    // }

    //  audio.soundMaster();

    // if (range > 0 && range < 10)
    // {
    //     for (int i = 0; i < size; ++i)
    //     {
    //         wavFileList[i] = bpm100[i];
    //     }
    // }

    // if (range > 9 && range < 20)
    // {
    //     for (int i = 0; i < size; ++i)
    //     {
    //         wavFileList[i] = bpm85[i];
    //     }
    // }

    // if (range > 19 && range < 30)
    // {
    //     for (int i = 0; i < size; ++i)
    //     {
    //         wavFileList[i] = bpm70[i];
    //     }
    // }
    // const char* wavName = wavFileList[beatIndicator].c_str();
    // Serial.println("PLAYING: ");
    // Serial.println(wavName);
    // audio.playAudio(wavName);
    // beatIndicator++;
    // if (beatIndicator > 3){
    //     beatIndicator = 0;
    // }
    // Serial.println(beatIndicator);
    // audio.playAudio("DRM3.WAV");
    // range = 25;
    // audio.soundFinder(range);

    // range = 15;
    // audio.soundFinder(range);

    // range = 5; 
    // audio.soundFinder(range);

    // audio.testAudio("DRM2.WAV", "DRM3.WAV");
    // delay(5000);

    // audio2.playAudio("DRM2.WAV");
    // audio.playFoundAudio("DRM2.WAV", "DRM3.WAV");

    // delay(2000);

    // audio.playAudio("DRM4.WAV");
    // delay(5000);
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

