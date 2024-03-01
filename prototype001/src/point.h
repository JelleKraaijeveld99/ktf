#ifndef POINT_H
#define POINT_H

#include <Arduino.h>

class Point {
  public:
  //constructor and destructor
  Point(int x, int y, int z);
  ~Point();
  
  //setters and getters
  void setPositions(int x, int y, int z);

  int getxPos();
  int getyPos();
  int getzPos();
  
  int xPos;
  int yPos;
  int zPos;
};

#endif //POINT_H