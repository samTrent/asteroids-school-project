#include "bullet.h"
#include <iostream>

#include <stdio.h>
#include "bullet.h"
#include "ship.h"

/**************************************
 * Method: draw
 * function: draws a simple dot for
 * the bullet.
 ***************************************/
void Bullet::draw(bool laserDamageUp)
{
     // std::cout << "trans: " << trans << std::endl;
  
   drawShipBullet(getPoint(), 1.0, 0.0, 1.0, 1.0);
  
}


/**************************************
 * Method: fire
 * function: makes a bullet in the gun
 ***************************************/
void Bullet::fire(Point point, float rotation, Velocity v, bool shotGunUp)
{
   
   this->point.setX(point.getX());
   this->point.setY(point.getY());
   
   velocity.setDx(-sin((rotation * M_PI) / 180) * BULLET_SPEED + v.getDx());
   velocity.setDy(cos((rotation * M_PI) / 180) * BULLET_SPEED + v.getDy());
}

/**************************************
 * Method: draw
 * function: draws a simple dot for
 * the bullet.
 ***************************************/
void Bullet::move()
{
  
   //std::cout << "moving bullet\n";
   advance();
   bulletLifeTime(timeLeft--);
   
//   point.addX(5); // makes stuff move
//   point.addY(5);
}

/**************************************
 * Method: kill
 * function: kills the bullet if its been
 * on screen too long
 ***************************************/
void Bullet::kill()
{
   //std::cout << "moving bullet\n";
   Alive = false;
   //std::cout << "chech check" << std::endl;
}

/**************************************
 * Method: kill
 * function: kills the bullet if its been
 * on screen too long
 ***************************************/
void Bullet::bulletLifeTime(int timeLeft)
{
   if (timeLeft <= 0)
   {
      kill();
      
   }
   
}




// *** SHOT GUN ROUND ***

/**************************************
 * Method: fire
 * function: makes a bullet in the gun
 ***************************************/
void ShotGun::fire(Point point, float rotation, Velocity v, bool shotGunUp)
{
   int spread = 0;
   //This may need to be changed...
   this->point.setX(point.getX());
   this->point.setY(point.getY());
   
  
   
   if(shotGunUp == false)
   {
      spread = 10;
      
   }
   
   if (shotGunUp == true)
   {
      spread = 25;
   }
   
   float angle = random(-spread, spread);
   
   velocity.setDx(-sin(((rotation + angle)* M_PI) / 180) * random(SHOT_GUN_SPEED, SHOT_GUN_SPEED + 10) + v.getDx());
   velocity.setDy(cos(((rotation + angle) * M_PI) / 180) * random(SHOT_GUN_SPEED, SHOT_GUN_SPEED + 10) + v.getDy());
}

// *** LASER ***

/**************************************
 * Method: fire
 * function: makes a laser in the gun
 ***************************************/
void Laser::fire(Point point, float rotation, Velocity v, bool shotGunUp)
{
   //This may need to be changed...
   this->point.setX(point.getX());
   this->point.setY(point.getY());
   
//   velocity.setDx(-sin((rotation * M_PI) / 180) * random(LASER_SPEED, LASER_SPEED + 10) + v.getDx());
//   velocity.setDy(cos((rotation * M_PI) / 180) * random(LASER_SPEED, LASER_SPEED + 10) + v.getDy());
   
   velocity.setDx(-sin((rotation * M_PI) / 180) * LASER_SPEED+ v.getDx());
   velocity.setDy(cos((rotation * M_PI) / 180) * LASER_SPEED + v.getDy());
}


