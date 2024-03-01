#include "audio_core.h"

AudioCore::AudioCore()
{
}

AudioCore::~AudioCore()
{
}

void AudioCore::setupAudio()
{
  AudioMemory(12);

  amplitude = 0.5;
  sgtl5000_1.enable();
  sgtl5000_1.volume(amplitude);

  SPI.setMOSI(SDCARD_MOSI_PIN);
  SPI.setSCK(SDCARD_SCK_PIN);

  if (!(SD.begin(SDCARD_CS_PIN)))
  {
    while (1)
    {
      Serial.println("Unable to access the SD card");
      delay(500);
    }
  }
}

void AudioCore::playAudio(const char *filename)
{
  Serial.print("Playing file: ");
  Serial.println(filename);

  playWav1.play(filename);

  delay(25);

  while (playWav1.isPlaying())
  {
    processSerialData();
    delay(100);
    // float scaledAmp = maxAmp - (static_cast<float>(range) / static_cast<float>(maxRange)) * maxAmp; // scale 0-maxrange to max amp - 0
    // if (scaledAmp > maxAmp || scaledAmp < 0)
    // { // this piece of code functions as a limiter for the amplitude
    //   scaledAmp = 0.25;
    // }
    // // Serial.print("SCALED AMP: " + String(scaledAmp));
    // setAmplitude(scaledAmp);
    // delay(25);
  }
}

void AudioCore::playFinalAudio(const char *filename){
  Serial.print("Playing file: ");
  Serial.println(filename);

  playWav1.play(filename);

  delay(25);

  while (playWav1.isPlaying())
  {

  }
}


void AudioCore::setGainMix1(float amp)
{
  mixer1.gain(1, amp);
  mixer2.gain(1, amp);
}

void AudioCore::soundMaster()
{
  string filenamesf = ""; // sf stands for sounds found

  if (soundsFound == 0)
  { // change what kind of audio file the master track is playing
    filenamesf = "DRM1.WAV";
  }
  if (soundsFound == 1)
  {
    filenamesf = "DRM1.WAV";
  }
  if (soundsFound == 2)
  {
    filenamesf = "DRM1.WAV";
  }

  const char *filenamesfchar = filenamesf.c_str();
  Serial.println("Now playing: ");
  Serial.println(filenamesfchar);

  playWav2.play(filenamesfchar);

  delay(25);

  while (playWav2.isPlaying())
  { // this loop functions almost as the main loop. Everything happens if the mastertrack (playWav2) is playing an audio file
    // check if the sound is found with the range constantly changing
    Serial.println("HALLOOOO");
    delay(3000);
    Serial.println("BAM");
    range = 25;
    soundFinder(range);  // this functions checks what the range is and plays an audiofile according to the range
    soundIsFound(range); // this function checks if the sound is found according to the range. If it is found, stop the audio files playing and change the bpm lists.
    delay(3000);

    Serial.println("FLAM");
    range = 4;
    soundFinder(range);
    soundIsFound(range);
    // delay(3000);
    // Serial.println("RAM");
    // range = 5;
    // soundFinder(range);
    delay(25);
  }
}

void AudioCore::setAmplitude(float amp)
{
  amplitude = amp;
  mixer1.gain(0, amp);
}

void AudioCore::setPlaybackSpeed(float speed)
{
  playbackSpeed = speed;
}

void AudioCore::setRange(int rng)
{
  range = rng;
}

void AudioCore::soundFinder(int range)
{
  setRange(range); // set the range to the class. Only check the range after a wav file is played

  if (range < 500)
  {
    Serial.print("STOP THE WAV FILE");
    playWav2.stop();
  }
  else
  {
    // this section of code checks what the range between the player and the audio point is. Based on this range the right audio files with matched bpm are being played
    if (range > 500 && range < 1500)
    {
      for (int i = 0; i < size; ++i)
      {
        wavFileList[i] = bpm124[i];
      }
    }
    else if (range > 1499 && range < 3000)
    {
      for (int i = 0; i < size; ++i)
      {
        wavFileList[i] = bpm100[i];
      }
    }
    else if (range > 2999)
    {
      for (int i = 0; i < size; ++i)
      {
        wavFileList[i] = bpm76[i];
      }
    }

    const char *wavName = wavFileList[beatIndicator].c_str();
    Serial.println("PLAYING: ");
    Serial.println(wavName);
    playAudio(wavName);
    beatIndicator++;
  }

  if (beatIndicator > 3)
    {
      beatIndicator = 0;
    }
}

void AudioCore::playMaster(const char *filename)
{

  Serial.print("Playing file: ");
  Serial.println(filename);
  playWav2.play(filename);

  delay(25);

  while (playWav2.isPlaying())
  {
    delay(50);
    processSerialData();
    soundFinder(range);
  
  }
}

void AudioCore::receiveSerialData()
{
  static byte index = 0; // Houd de huidige positie in de buffer bij

  while (Serial1.available() > 0)
  {
    char receivedChar = Serial1.read();

    // Controleer op het einde van de zin
    if (receivedChar == '\n')
    {
      receivedString[index] = '\0'; // Voeg een null-terminator toe aan het einde van de zin
      index = 0;                    // Reset de positie in de buffer
      newData = true;               // Markeer dat er nieuwe gegevens zijn
    }
    else
    {
      receivedString[index] = receivedChar; // Voeg het teken toe aan de buffer
      index = (index + 1) % bufferSize;     // Ga naar de volgende positie in de buffer
    }
  }
}

void AudioCore::processSerialData()
{

  if (Serial1.available() > 0)
  {
    // Lees de gegevens van de ESP32
    for (int i = 0; i < 60; ++i) //this forloop "empties" the serial buffer
    {
      String fakeData = Serial1.readStringUntil('\n');
    }

    String data = Serial1.readStringUntil('\n');

    // Splits de gegevens op basis van het komma-teken
    int commaIndex = data.indexOf(',');

    if (commaIndex != -1)
    {
      // Extract inRange en distance
      String inRangeStr = data.substring(0, commaIndex);
      String distanceStr = data.substring(commaIndex + 1);

      // Zet de ontvangen waarden om naar de juiste types
      inRange = inRangeStr.toInt();
      range = distanceStr.toFloat();

      // Gebruik inRange en distance zoals nodig
      if (inRange)
      {
        Serial.println("ESP32: IN RANGE");
        soundsFound++;
        playWav2.stop();
        playWav1.stop(); 
        delay(2000);
      }
      else
      {
        Serial.println("ESP32: NOT IN RANGE");
      }

      Serial.print("ESP32: DISTANCE = ");
      Serial.println(range);

      // Voeg hier verdere logica toe op basis van inRange en distance
      // ...

      // Reset de buffer en de indicator voor nieuwe gegevens
      memset(receivedString, 0, bufferSize);
      newData = false;
    }
  }
}

void AudioCore::setWavNames(int soundsfound)
{
  Serial.print("WAVNAMES");

  if (soundsfound == 0)
  {
    // Verander de strings in de array
    for (int i = 0; i < size; ++i)
    {
      bpm124[i] = "124K" + String(i + 1) + ".WAV";
      bpm100[i] = "100K" + String(i + 1) + ".WAV";
      bpm76[i] = "76K" + String(i + 1) + ".WAV";
    }
  }

  if (soundsfound == 1)
  {
    // Verander de strings in de array
    for (int i = 0; i < size; ++i)
    {
      bpm124[i] = "124B" + String(i + 1) + ".WAV";
      bpm100[i] = "100B" + String(i + 1) + ".WAV";
      bpm76[i] = "76B" + String(i + 1) + ".WAV";
    }
  }

  if (soundsfound == 2)
  {
    // Verander de strings in de array
    for (int i = 0; i < size; ++i)
    {
      bpm124[i] = "124D" + String(i + 1) + ".WAV";
      bpm100[i] = "100D" + String(i + 1) + ".WAV";
      bpm76[i] = "76D" + String(i + 1) + ".WAV";
    }
  }

  Serial.println("bpm124:");
  for (int i = 0; i < size; ++i)
  {
    Serial.println(bpm124[i]);
  }

  Serial.println("\nbpm100:");
  for (int i = 0; i < size; ++i)
  {
    Serial.println(bpm100[i]);
  }

  Serial.println("\nbpm76:");
  for (int i = 0; i < size; ++i)
  {
    Serial.println(bpm76[i]);
  }
}

void AudioCore::setSoundFound(int amount)
{
  soundsFound = amount;
}

int AudioCore::getSoundsFound()
{
  return soundsFound;
}

// void AudioCore::soundIsFound(int range)
// {
//   if (range < 5)
//   {
//     soundsFound++; // add one to the amount of sounds that are found
//     if (soundsFound < 4)
//     {
//       Serial.println("STOP"); // just to check if we reach this part of the code
//       playWav1.stop();        // stop the audio files
//       playWav2.stop();
//       delay(3000);
//       // playNarratorAudio(soundsFound);
//       setWavNames(soundsFound);
//       delay(3000);
//     }
//   }
// }

// void AudioCore::testAudio(const char *filename1, const char *filename2)
// {
//   Serial.print("Playing first file: ");
//   Serial.print(filename1);
//   Serial.print("Playing second file: ");
//   Serial.print(filename2);

//   playWav1.play(filename1);
//   playWav2.play(filename2);

//   Serial.println("playWav1 afspeelstatus: " + String(playWav1.isPlaying()));
//   Serial.println("playWav2 afspeelstatus: " + String(playWav2.isPlaying()));

//   delay(25);

//   while (playWav1.isPlaying() || playWav2.isPlaying())
//   {
//   }
// }

// void AudioCore::playFoundAudio(const char *filename1, const char *filename2)
// {
//   Serial.print("Playing files: ");

//   // // Controleer of bestanden bestaan voordat je ze probeert af te spelen
//   if (SD.exists(filename1))
//   {
//     Serial.println(filename1);
//     playWav1.play(filename1);
//   }
//   else
//   {
//     Serial.println("File not found: " + String(filename1));
//   }

//   if (SD.exists(filename2))
//   {
//     Serial.println(filename2);
//     playWav2.play(filename2);
//   }
//   else
//   {
//     Serial.println("File not found: " + String(filename2));
//   }

//   // Wacht tot het afspelen is voltooid
//   while (playWav1.isPlaying() || playWav2.isPlaying())
//   {
//     delay(25);
//   }
// }
