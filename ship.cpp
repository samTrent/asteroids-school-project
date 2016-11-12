#include "ship.h"
#define PI = 3.145
// Put your ship methods here

/**************************************
 * Method: draw
 * function: creates the ship.
 ***************************************/
void Ship::draw(bool isPaused, bool shieldBat1, bool shieldBat2, bool plasmaRange, bool plasmaHeatSink, bool shotGunUp, bool laserRangeUp, bool laserDamageUp, bool deepScannerUp)
{
  

   
   if (laserRangeUp == true)
   {
      drawShipLaserShotUp(getPoint(), getAngle(), false);
   }

   if (shotGunUp == true)
   {
      drawShotGunUp(getPoint(), getAngle(), false);
   }
   
   if (plasmaHeatSink == true)
   {
      drawShipPlasmaHeatSink(Point(getPoint()), getAngle(), false);
   }
   
   drawShip(getPoint(), rotation, isThrusting());
   
   if (laserDamageUp == true)
   {
      drawShipLaserDamageUp(getPoint(), getAngle(), false);
   }


   if(plasmaRange == true)//else
   {
      drawBarrel(getPoint(), getAngle(), 1.0, 0.0, 1.0);
   }
   if(shieldBat1 == false)
   {
      drawCockpit(getPoint(), 4, 0);
   }
   if(shieldBat1 == true)//else
   {
      if (shieldBat2 == true)
      {
         drawWeapon(getPoint(), 4, 0, 0.0, 1.0, 1.0);
         drawWeapon(getPoint(), 3.5, 0, 0.0, 0.0, 1.0);
         drawWeapon(getPoint(), 1.5, 0, 1.0, 0.2, 0.5);
      }
      else
      {
         drawWeapon(getPoint(), 4, 0, 0.0, 0.0, 1.0);
         drawRectColor(getPoint(), 6, 2.0, getAngle(), 0.0, 1.0, 1.0);
      }
   }
   
   if (deepScannerUp == true)
   {
      drawPill(getPoint(), 6, 1, spin, 1.0, 1.0, 0.0);
      spin += 4;
      
   }
  
   spin += 4;

  // spin++;
 }

/**************************************
 * Method: deathAnimation
 * function: creates the ship.
 ***************************************/
void Ship::deathAnimation(int deathAnimationLen)
{
   // make the debris spin
   if (Alive == false)
   {
      for (int i = 0; i < 20; i++)
      {
         drawShipDeath(getPoint(), 25, 1, debrisSpin);
      }
   }
   
   
}


/**************************************
 * Method: Move
 * function: advances the ship
 ***************************************/
void Ship::move()
{
   advance();
//   point.setX(velocity.getDx()); // makes stuff move
//   point.setY(velocity.getDy());

}


// ***MOVEMENT*** //


/**************************************
 * Method: applyThrustBottom
 * function: Gives the ship the ability
 * to move forward.
 ***************************************/
void Ship::applyThrustBottom(int accelSpeed)
{
   if(canThrust())
   {
      if(accelSpeed == 0)
      {
         velocity.addDx(-sin((rotation * M_PI) / 180) * THRUST_AMOUNT_NORMAL);
         velocity.addDy(cos((rotation * M_PI) / 180) * THRUST_AMOUNT_NORMAL);
         
      }
      else if(accelSpeed == 1)
      {
         velocity.addDx(-sin((rotation * M_PI) / 180) * THRUST_AMOUNT_FAST);
         velocity.addDy(cos((rotation * M_PI) / 180) * THRUST_AMOUNT_FAST);
         
      }
      else if(accelSpeed == 2)
      {
         
         velocity.addDx(-sin((rotation * M_PI) / 180) * THRUST_AMOUNT_SLOW);
         velocity.addDy(cos((rotation * M_PI) / 180) * THRUST_AMOUNT_SLOW);
         
      }
     
   }
}

/**************************************
 * Method: applyThrustBottom
 * function: Gives the ship the ability
 * to move backwards.
 ***************************************/
void Ship::applyThrustTop(int accelSpeed)
{
   if(canThrust())
   {
      if(accelSpeed == 0)
      {
         velocity.addDx(sin((rotation * M_PI) / 180) * THRUST_AMOUNT_NORMAL);
         velocity.addDy(-cos((rotation * M_PI) / 180) * THRUST_AMOUNT_NORMAL);
         
      }
      else if(accelSpeed == 1)
      {
         velocity.addDx(sin((rotation * M_PI) / 180) * THRUST_AMOUNT_FAST);
         velocity.addDy(-cos((rotation * M_PI) / 180) * THRUST_AMOUNT_FAST);
         
      }
      else if(accelSpeed ==2)
      {
         
         velocity.addDx(sin((rotation * M_PI) / 180) * (THRUST_AMOUNT_SLOW + .25));
         velocity.addDy(-cos((rotation * M_PI) / 180) * (THRUST_AMOUNT_SLOW + .25));
         
         
      }
      
   }
}


/**************************************
 * Method: applyThrustLeft
 * function: Gives the ship the ability
 * to turn left.
 ***************************************/
void Ship::applyThrustLeft(int rotateSpeed)
{
   if (rotateSpeed == 0)
   {
      rotation += ROTATE_AMOUNT;
   }
   if (rotateSpeed == 1)
   {
      rotation += (ROTATE_AMOUNT + 2);
   }
   if (rotateSpeed == 2)
   {
      rotation += (ROTATE_AMOUNT - 2);
   }

   //angle += rotation;
}

/**************************************
 * Method: applyThrustRight
 * function: Gives the ship the ability
 * to turn right.
 ***************************************/
void Ship::applyThrustRight(int rotateSpeed)
{
   if (rotateSpeed == 0)
   {
      rotation -= ROTATE_AMOUNT;
   }
   if (rotateSpeed == 1)
   {
      rotation -= (ROTATE_AMOUNT + 2);
   }
   if (rotateSpeed == 2)
   {
      rotation -= (ROTATE_AMOUNT - 2);
   }

   
   // angle -= rotation;
}
