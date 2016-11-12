//
//  star.cpp
//  asteroids pt2
//
//  Created by Sam Trent on 3/15/16.
//  Copyright © 2016 Sam Trent. All rights reserved.
//

#include <stdio.h>
#include "star.h"

/*********************************************
 * GAME :: drawStar
 * Draws a star to the screen
 *********************************************/
void Star::draw()
{
   drawStar(point, transRate);
}
