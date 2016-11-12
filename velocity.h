//
//  velocity.h
//  moonLander
//
//  Created by Sam Trent on 2/12/16.
//  Copyright © 2016 Sam Trent. All rights reserved.
//

#ifndef velocity_h
#define velocity_h
#include "point.h"
#include "uiInteract.h"




class Velocity
{
private:
   float dx;
   float dy;
   
public:
   //Constructor
   
   Velocity() : dx(0), dy(0) {}
   //Construtor #2               vvv including the default construtor
   //  Velocity(float dx, float dy): dx(dx), dy(dy), point (0.0, 0.0) {};
   
   void addDx(float dx)      { setDx(getDx() + dx);     }
   void addDy(float dy)      { setDy(getDy() + dy);     }
   
   //getter
   float getDx() { return dx; };
   float getDy() { return dy; };
   
   //setter
   void setDx(float dx) { this->dx = dx; };
   void setDy(float dy) { this->dy = dy; };
   
   
};


#endif /* velocity_h */
