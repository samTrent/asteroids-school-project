//
//  alienShips.cpp
//  asteroids pt2
//
//  Created by Sam Trent on 3/19/16.
//  Copyright © 2016 Sam Trent. All rights reserved.
//

#include <stdio.h>
#include "alienShips.h"

/************************************************************************
* DRAW :
* Draw a single alien saucer!
 *************************************************************************/
void Saucer::drawShip(bool isPaused)
{
  // drawSaucer(getPoint(), 50, 0);
   drawAlienShipPart(getPoint(), spin, 0.2, 0.2, 0.2);
   drawGreenStrip(getPoint(), spin, 0.0, 1.0, 0.0);
   drawMediumAlienShipPart(getPoint(), slowSpin, 1.0, 1.0, 0.0);
   drawSmallAienShipPart(getPoint(), backSpin, 0.0, 1.0, 1.0);
   drawAlienShipPartSide(getPoint(), slowSpin, 1.0, 0.0, 0.0);
   drawSmallAienShipBigRing(getPoint(), spin, 1.0, blink1, 0.0);
  // drawGreenStrip(getPoint(), spin, 0.0, 1.0, 0.0);
  //   drawWeapon(getPoint() + 25, 5, 0, blink1, 1.0, blink3);
//   drawWeapon(getPoint() - 25, 5, 0, blink1, 1.0, blink3);
   blink1 = random(0.0,1.0);
   blink2 = random(0.0,1.0);
   blink3 = random(0.0,1.0);
   if (isPaused == false)
   {
      backSpin -= FAST_SPIN;
      spin += SPIN_RATE;
      slowSpin += SLOW_SPIN;
   }
}


/************************************************************************
 * kill :
 *
 *************************************************************************/
void Saucer::kill()
{
   setAlive(false);
}

/************************************************************************
* DRAW :
* Draw a single alien saucer!
*************************************************************************/
void AlienShip::kill()
{
   setAlive(false);
}


// ALIEN SCOUT SHIP
/************************************************************************
 * DRAW :
 * Draw a single alien saucer!
 *************************************************************************/
void ScoutShip::drawShip(bool isPaused)
{
   drawSmallAienShipPart(getPoint(), backSpin, 0.0, 1.0, 0.0);
   drawMediumAlienShipPart(getPoint(), spin, 0.0, 1.0, 1.0);
   drawAlienShipPartSolid(getPoint(), slowSpin, 0.0, 0.25, 1.0);
 
   if(isPaused == false)
   {
      slowSpin -= SLOW_SPIN;
      spin += FAST_SPIN;
      backSpin -= FAST_SPIN;
   }
   
   
   
   
   
   
   
}

// ALIEN BOMBER
/************************************************************************
 * DRAW :
 * Draw a single alien saucer!
 *************************************************************************/
void Bomber::drawShip(bool isPaused)
{
   drawAlienShipPart(getPoint(), spin, 1.0, 1.0, 0.0);
   drawGreenStrip(getPoint(), spin, blink1, blink2, blink3);
   drawMediumAlienShipPart(getPoint(), slowSpin, 1.0, 0.50, 0.0);
   drawSmallAienShipPart(getPoint(), backSpin, blink1, blink2, blink3);

   //drawWeapon(getPoint(), 5.0, 0, blink1, blink2, blink3);
   drawAlienShipPartSide(getPoint(), slowSpin, 0.0, 0.0, 1.0);
   drawSmallAienShipBigRing(getPoint(), spin, 1.0, blink1, 0.0);
 
   if (isPaused == false)
   {
      blink1 = random(0.0,1.0);
      blink2 = random(0.0,1.0);
      blink3 = random(0.0,1.0);
      backSpin -= FAST_SPIN;
      spin += SPIN_RATE;
      slowSpin += SLOW_SPIN;
   }

   if (point.getX() <= -760)
   {
      velocity.setDx(addVelPosX);
   }
   
   if (point.getX() >= 760)
   {
      velocity.setDx(addVelNegX);
   }
   
   
   
   if (point.getY() <= -560)
   {
      velocity.setDy(addVelPosY);
   }
   
   if (point.getY() >= 560)
   {
      velocity.setDy(addVelNegY);
   }

   

}
