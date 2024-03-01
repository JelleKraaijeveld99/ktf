#ifndef AUDIOLOCATION_H
#define AUDIOLOCATION_H

#include <point.h>
#include <Arduino.h>
#include <vector>
#include <list>
#include <cstdlib>   // voor rand() en srand()
#include <ctime>     // voor time()
#include <cmath>

class AudioLoc {
    public:
    //constructor and destructor
    AudioLoc();
    ~AudioLoc();

    std::vector<Point> anchors; //vector for storing the anchors 

    //functions
    void setupAudioloc(); //function for setting up the anchors and tags 

    float calculateSnapshotDistance(); //function for checking if audio location center is far enough from the SNAPSHOT tag
    float calculateLiveDistance(); //function for checking if the audio location is in range of the LIVE tag 
     bool liveAudioLocProximity(); //function for checking if the live tag is in range with the audio location 
    
    void generateCenter(); //function for generating a middle point in between the boundaries of the anchors
    void generateArea(); //function for transforming the middle point to an area (circle)
    void processNewCenter(); //function for processing all functions together with a while loop to keep on checking if the distance between the tag and the new audio location point is big enough
   
    void addAnchor(const Point& anchor);//function for adding anchors from outside the class
    void addAudioLoc(Point& audioloc);
    void addLiveTag(Point& tag);//function for adding the tag from outside the class
    void addSnapshotTag(int x, int y, int z); //function for adding the coordinates of a tag from outside of the class, NOT THE ACTUAL OBJECT BUT THE COORDINATES OF 1 MOMENT.

    void printLiveTagCoordinates();//function for printing the live coordinates of the anchors
    
    //setters and getters
    void getAnchorCoordinates(int index); //function for getting and printing the coordinates of the anchors
    void getTagCoordinates(); //function for getting and printing the coordinates of the tag
    void getAudioLocCoordinates();



    private:
    Point* SnapshotTagLocPtr; //empty object for the tag location as a moment
    Point* AudioLocCenterPtr; //empty object for the audiolocation
    Point* LiveTagLocPtr; //empty object for the live location of the tag

};

#endif // AUDIOLOCATION_H