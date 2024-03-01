#ifndef AUDIO_CORE_H
#define AUDIO_CORE_H

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <string>
#include <string.h>

using namespace std;

#define SIZE 4


class AudioCore {
public:
    //constructor and destructor 
    AudioCore();
    ~AudioCore();
    //functions 
    void setupAudio(); //function for setting up the audio
    
    void playAudio(const char *filename); //function for playing an audio file 
    void playFinalAudio(const char *filename);
 
    void testAudio(const char* filename1, const char* filename2); //function for testing if two sounds can play at the same time
    void playFoundAudio(const char* filename1, const char* filename2); //function for playing the found audio
    void playNarratorAudio(int soundsfound); //function that confirms that audio is found and tells you what audio to find next

    void soundFinder(int range); //function for playing a sound on the right bpm and right amplitude. If the range is smaller then a certain range, the sound is found
    void soundMaster(); //function that processes the audio. This function is playing the master track with the found sounds. In the loop of this master track the soundfinder and soundIsFound Function are being called
    void soundIsFound(int range); //function that checks if a sound is actually found or not, if so stop all tracks, let the confirmation audio play, change the wav files of some tracks and soundsfound++
    void setSoundFound(int amount); //function that sets the amount of found sounds
    void setGainMix1(float amp);

    void playMaster(const char *filename);

    void setWavNames(int soundsfound); //function that changes the list of the wavefilenames
    int getSoundsFound();

    void setPlaybackSpeed(float speed); //AINT GONNA HAPPEN

    void receiveSerialData(); //function for receiving the serial data
    void processSerialData(); //function for processing the serial data

    
    void setAmplitude(float amp);
    void setRange(int rng);

private:

    AudioMixer4              mixer1; //voor rechterkanaal
    AudioMixer4              mixer2; //voor linkerkanaal
    AudioPlaySdWav           playWav1;
    AudioPlaySdWav           playWav2;
    AudioOutputI2S           audioOutput;

    AudioConnection          patchCord1{playWav1, 0, mixer1, 0};
    AudioConnection          patchCord2{playWav1, 1, mixer2, 0};
    AudioConnection          patchCord3{playWav2, 0, mixer1, 1};
    AudioConnection          patchCord4{playWav2, 1, mixer2, 1};
    AudioConnection          patchCord5{mixer1, 0, audioOutput, 0};
    AudioConnection          patchCord6{mixer2, 0, audioOutput, 1};


    int beatIndicator = 0; //variable for checking on which beat we are so we know what wav to play
    const int size = 4;
    String bpm124[SIZE] = {"DRM1.WAV", "DRM2.WAV", "DRM3.WAV", "DRM4.WAV"};
    String bpm100[SIZE] = {"DRM5.WAV", "DRM6.WAV", "DRM7.WAV", "DRM8.WAV"};
    String bpm76[SIZE] = {"DRM9.WAV", "DRM10.WAV", "DRM11.WAV", "DRM12.WAV"};
    String wavFileList[SIZE];


    AudioControlSGTL5000     sgtl5000_1;
    // Use these with the Teensy Audio Shield
    #define SDCARD_CS_PIN    10
    #define SDCARD_MOSI_PIN  7   // Teensy 4 ignores this, uses pin 11
    #define SDCARD_SCK_PIN   14  // Teensy 4 ignores this, uses pin 13

    int soundsFound = 0;

    float amplitude;
    float playbackSpeed;
    float range;
    bool inRange = false;
    int maxRange = 40;
    float maxAmp = 0.5;

  static const int bufferSize = 64;  // Kies een geschikte grootte voor de buffer voor het uitlezen van de berichten
  char receivedString[bufferSize];  // Buffer voor ontvangen tekens
  boolean newData;  // boolean voor nieuwe gegevens in de buffer

};

#endif // AUDIO_CORE_H