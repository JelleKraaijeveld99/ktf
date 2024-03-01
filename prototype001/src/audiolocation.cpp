#include <audiolocation.h>

//constructor
AudioLoc::AudioLoc() {

}

//destructor
AudioLoc::~AudioLoc(){
    delete LiveTagLocPtr;
}

void AudioLoc::addAnchor(const Point& anchor){
    anchors.push_back(anchor);
}

void AudioLoc::addLiveTag(Point& tag){
    LiveTagLocPtr = &tag;
}

void AudioLoc::addAudioLoc(Point& audioloc){
    AudioLocCenterPtr = &audioloc;
}

void AudioLoc::addSnapshotTag(int x, int y, int z){
    SnapshotTagLocPtr = new Point(x, y, z);
}

void AudioLoc::generateCenter(){
    
    //section for putting all the coordinates in an array so its more clear code
    int xCoordinates[4] = {};
    int yCoordinates[4] = {};
    int zCoordinates[4] = {};

    for (int i = 0; i < 4; ++i) {
        xCoordinates[i] = anchors[i].getxPos();
        yCoordinates[i] = anchors[i].getyPos();
        zCoordinates[i] = anchors[i].getzPos();
    }

    //section for actually generating a new coordinate
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    // Genereer een willekeurig getal tussen 0 en RAND_MAX
    int randomU = std::rand();
    int randomV = std::rand();

    // Generate a random number between 0 and 1
    float u = static_cast<double>(randomU) / RAND_MAX;
    float v = static_cast<double>(randomV) / RAND_MAX;

    //barysentric coordinates:
    float w1 = (1-u)*(1*v);
    float w2 = u * (1-v);
    float w3 = u * v;
    float w4 = (1-u)*v;

    //calculate the x, y and z of a random point as an int
    int newX = (w1*xCoordinates[0]) + (w2*xCoordinates[1]) + (w3*xCoordinates[2]) + (w4*xCoordinates[3]);
    int newY = (w1*yCoordinates[0]) + (w2*yCoordinates[1]) + (w3*yCoordinates[2]) + (w4*yCoordinates[3]);
    int newZ = (w1*zCoordinates[0]) + (w2*zCoordinates[1]) + (w3*zCoordinates[2]) + (w4*zCoordinates[3]);

    AudioLocCenterPtr = new Point(newX, newY, newZ);
}

float AudioLoc::calculateSnapshotDistance(){
    //AudioLocCenterPtr
    int x1 = AudioLocCenterPtr->getxPos();
    int y1 = AudioLocCenterPtr->getyPos();
    int z1 = AudioLocCenterPtr->getzPos();
    //TagPtr 
    int x2 = SnapshotTagLocPtr->getxPos();
    int y2 = SnapshotTagLocPtr->getyPos();
    int z2 = SnapshotTagLocPtr->getzPos();

    float distance = std::sqrt(std::pow(x2 - x1, 2) + std::pow(y2 - y1, 2) + std::pow(z2 - z1, 2));
    return distance;
}

float AudioLoc::calculateLiveDistance(){
    //AudioLocCenterPtr
    int x1 = AudioLocCenterPtr->getxPos();
    int y1 = AudioLocCenterPtr->getyPos();
    int z1 = AudioLocCenterPtr->getzPos();
    //TagPtr 
    int x2 = LiveTagLocPtr->getxPos();
    int y2 = LiveTagLocPtr->getyPos();
    int z2 = LiveTagLocPtr->getzPos();

    float distance = std::sqrt(std::pow(x2 - x1, 2) + std::pow(y2 - y1, 2) + std::pow(z2 - z1, 2));
    return distance;
}

bool AudioLoc::liveAudioLocProximity(){
    float rangeSize = 8.0f;
    float distance = calculateLiveDistance();
    bool inRange = false;
   
    if(distance > rangeSize){
        Serial.println("NOT IN RANGE ");
        distance = calculateLiveDistance();
        inRange = false;
    }else{
    inRange = true;
    Serial.println("IN RANGE ");
    }
    
    return inRange;
}


void AudioLoc::processNewCenter(){
    float distance = 0.0f;
    float minDistance = 7.0f;
    while (distance <= minDistance)
    {
        Serial.println("DISTANCE TOO SMALL, GENERATING NEW CENTER");
        generateCenter();
        distance = calculateSnapshotDistance();
    } 

    Serial.println("NEW CENTER GENERATED!");
    getAudioLocCoordinates();  
}

void AudioLoc::getAnchorCoordinates(int index){ //index: 0 = anchor1, 1 = anchor2, 2 = anchor3, 3 = anchor4. xyz: 0 = x, 1 = y, 2 = z
    
    int xPosition = anchors[index].getxPos();
    int yPosition = anchors[index].getyPos();
    int zPosition = anchors[index].getzPos();
    int anchorNumber = index + 1;
    Serial.println("\nthese are the coordinates of anchor " + String(anchorNumber));
    Serial.println("x: " + String(xPosition));
    Serial.println("y: " + String(yPosition));
    Serial.println("z: " + String(zPosition));
}

void AudioLoc::getAudioLocCoordinates(){
   
    int xPosition = AudioLocCenterPtr->getxPos();
    int yPosition = AudioLocCenterPtr->getyPos();
    int zPosition = AudioLocCenterPtr->getzPos();

    Serial.println("\nthese are the coordinates of the audio location: ");
    Serial.println("x: " + String(xPosition));
    Serial.println("y: " + String(yPosition));
    Serial.println("z: " + String(zPosition));

}

void AudioLoc::getTagCoordinates(){
    
    int xPosition = SnapshotTagLocPtr->getxPos();
    int yPosition = SnapshotTagLocPtr->getyPos();
    int zPosition = SnapshotTagLocPtr->getzPos();

    Serial.println("\nthese are the coordinates of the tag: ");
    Serial.println("x: " + String(xPosition));
    Serial.println("y: " + String(yPosition));
    Serial.println("z: " + String(zPosition));
}

void AudioLoc::printLiveTagCoordinates(){
    
    int xPosition = LiveTagLocPtr->getxPos();
    int yPosition = LiveTagLocPtr->getyPos();
    int zPosition = LiveTagLocPtr->getzPos();

    Serial.println("\nthese are the coordinates of the tag: ");
    Serial.println("x: " + String(xPosition));
    Serial.println("y: " + String(yPosition));
    Serial.println("z: " + String(zPosition));
}
