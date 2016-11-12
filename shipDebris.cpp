//
//  shipDebris.cpp
//  asteroids pt2
//
//  Created by Sam Trent on 3/18/16.
//  Copyright © 2016 Sam Trent. All rights reserved.
//

#include <stdio.h>
#include "shipDebris.h"
/**************************************
 * Method: draw
 * function: Draws a regular asteroid
 ***************************************/
void ShipDebris::draw(bool isPaused)
{
   if (isPaused == false)
   {
      debrisSpin += DEBRIS_SPIN_RATE;
   }
   drawShipDeath(getPoint(), 25, 1, debrisSpin);
}






//debrisBits
/**************************************
 * Method: draw
 * function: Draws a regular asteroid
 ***************************************/
void DebrisBits::draw(bool isPaused)
{
  
   // std::cout << "here\n";
   drawDebrisBits(getPoint());
 
 
}
/**************************************
 * Method: hit
 * function: lets the game know if we
 *  hit the asteroid. calls "createAsteroids"
 *  and passes the asteroid type so it knows
 *  what other asteroids it needs to
 *  create in its place.
 ***************************************/
//void DebrisBits::hit()
//{
//   healthpoints--;
//   if (healthpoints == 0)
//   {
//      setAlive(false) ;
//   }
//}


/**************************************
 * Method: draw
 * function: Draws a regular asteroid
 ***************************************/
void AlienShipDebris::draw(bool isPaused)
{
   if(isPaused == false)
   {
      debrisSpin += DEBRIS_SPIN_RATE;
   }
   drawAlienShipDeath(getPoint(), 25, 1, debrisSpin);
   // std::cout << deathAnimationLen << std::endl;
}






//debrisBits
/**************************************
 * Method: draw
 * function: Draws a regular asteroid
 ***************************************/
void AlienDebrisBits::draw(bool isPaused)
{
   
   // std::cout << "here\n";
   drawAlienDebrisBits(getPoint());
   
   
}


//Warp Field
/**************************************
 * Method: draw
 * function: Draws a regular asteroid
 ***************************************/
void WarpField::draw(bool isPaused)
{
   if (isPaused == false)
   {
      debrisSpin += DEBRIS_SPIN_RATE;
   }
   // std::cout << "here\n";
   drawWarpField(getPoint(), 25, 1, debrisSpin);
   
   
}
   
   
   
