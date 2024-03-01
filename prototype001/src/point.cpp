#include <point.h>

//constructor
Point::Point(int x, int y, int z){
    xPos = x;
    yPos = y;
    zPos = z;
}

//destructor
Point::~Point(){

}

void Point::setPositions(int x, int y, int z){
    xPos = x;
    yPos = y;
    zPos = z;
}

int Point::getxPos(){
    return xPos;
}

int Point::getyPos(){
    return yPos;
}

int Point::getzPos(){
    return zPos;
}