 //
//  flyingObject.h
//  skeet
//
//  Created by Sam Trent on 2/26/16.
//  Copyright © 2016 Sam Trent. All rights reserved.
//

#ifndef flyingObject_h
#define flyingObject_h
#include "point.h"
#include "velocity.h"


/*******************************
 * CLASS: FlyingObject (hasA Point & Velocity)
 * base class for our asteroids
 *******************************/
class FlyingObject
{
private:
   //none
   
   
protected:

   Velocity velocity;
   
   
   
   
   
   
   
public:
  FlyingObject(float x, float y) : point(Point(x,y)) {};
  FlyingObject() : point(Point(10.0, 10.0)) {};
  FlyingObject(Point p) : point(p) {}
   
   //Methods
   void advance();
   
   virtual void draw() {};
   
   
   
   
   
   
   //Getter
   Point getPoint() const { return point;};
   Velocity getVelocity() const {return velocity; };
   
   
   //Setter
   void setPoint(Point point) { this->point = point; };
   void setVelocity(Velocity velocity) { this->velocity = velocity; };
   
   
   
   Point point;
   
   
};


#endif /* flyingObject_h */
