//
//  star.h
//  asteroids pt2
//
//  Created by Sam Trent on 3/15/16.
//  Copyright © 2016 Sam Trent. All rights reserved.
//

#ifndef star_h
#define star_h

#include "uiDraw.h"
#include "point.h"


class Star
{
private:
   Point point; // star point
   float transRate; // star transparancy 
   
   
   
   
   
   
public:
   // default construtor
   Star(Point point)
   {
      this->point.setX(point.getX());
      this->point.setY(point.getY());
      transRate = random(0.000000000000000001, 0.85);
   };
   
   // draw the star
   void draw();
   
   
   //getter
   Point getPoint() const { return point; };
   
   //setter
   void setPoint(Point point) { this->point = point; };
   
   
   
   
   
   
};


#endif /* star_h */
