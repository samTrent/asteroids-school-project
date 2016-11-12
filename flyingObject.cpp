//
//  flyingObject.cpp
//  skeet
//
//  Created by Sam Trent on 2/26/16.
//  Copyright © 2016 Sam Trent. All rights reserved.
//

#include <stdio.h>
#include "flyingObject.h"

/**************************************
 * Method: Advance
 * function: updates the objects posision
 ***************************************/
void FlyingObject::advance()
{
  // std::cout << "does advance work?\n";
   // changing the point acording to the velocity
   point.addX(velocity.getDx()); // makes stuff move
   point.addY(velocity.getDy());
   
//   point.setX(velocity.getDx()); // makes stuff move
//   point.setY(velocity.getDy());
  
}
