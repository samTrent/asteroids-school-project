//
//  alienBullet.cpp
//  asteroids pt2
//
//  Created by Sam Trent on 3/19/16.
//  Copyright © 2016 Sam Trent. All rights reserved.
//

#include <stdio.h>
#include "alienBullet.h"

/**************************************
 * Method: fire
 * function: makes a bullet in the gun
 ***************************************/
void AlienBullet::fire(Point point, float rotation)
{
   //This may need to be changed...
   this->point.setX(point.getX());
   this->point.setY(point.getY());
   
   velocity.setDx(-sin((rotation * M_PI) / 180) * 40);
   velocity.setDy(cos((rotation * M_PI) / 180) * 40);
}

/**************************************
 * Method: draw
 * function: makes a bullet in the alien ship
 ***************************************/
void AlienBullet::draw()
{
   drawAlienBullet(getPoint(), 3 , 0);
   
}

//Alein bomb
/**************************************
 * Method: draw
 * function: makes a bullet in the alien ship
 ***************************************/
void AlienBomb::draw()
{
   float blink1 = random(0.0,1.0);
   float blink2 = 1.0;
   float blink3 = random(0.0,1.0);
   if (getTimeLeft() <= 50)
   {
      blink1 = random(0.0,1.0);
      blink2 = random(0.0,1.0);
      blink3 = random(0.0,1.0);
   }
  

   drawWeapon(getPoint(), 8, 0, blink1, blink2, blink3);
 }


//Alein bomb shrapnal
/**************************************
 * Method: draw
 * function: makes a bullet in the alien ship
 ***************************************/
void BombShrapnel::draw()
{
   drawWeapon(getPoint(), 4, 0, 0.0, 1.0, 1.0);
}