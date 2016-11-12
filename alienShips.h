//
//  alienShips.h
//  asteroids pt2
//
//  Created by Sam Trent on 3/19/16.
//  Copyright © 2016 Sam Trent. All rights reserved.
//

#ifndef alienShips_h
#define alienShips_h

#include "uiDraw.h"
#include "ship.h"

#define SAUCER_SPEED 5.0
#define SAUCER_HEALTH 200.0
#define SPIN_RATE 3
#define FAST_SPIN 16
#define SLOW_SPIN 1

#define SCOUT_SPEED 6.0
#define SCOUT_HEALTH 25.0

#define BOMBER_SPEED 5.0
#define BOMBER_HEALTH 100.0
#define BOMBER_SPEED_INC 0.10

#define BASE_LASER_DAMAGE 7 // 10

class AlienShip : public Ship
{
private:
protected:
   float hitPoints;
   int type;
   float blink1;
   float blink2;
   float blink3;
   bool laserDamageUp;
   
   float addVelPosX;
   float addVelNegX;
   float addVelPosY;
   float addVelNegY;
public:
   //methods
   virtual void hit(int type, bool laserDamageUp)
   {
      // if hit by laser, do ten damage instead
      if(laserDamageUp == false)
      {
         if (type == 1)
            hitPoints--;
         if (type == 2)
            hitPoints -= BASE_LASER_DAMAGE;
      }
      
      else if(laserDamageUp == true)
      {
         if (type == 1)
            hitPoints--;
         if (type == 2)
            hitPoints -= 200;
      }
   };
   
   void incSpeed()
   {
      addVelPosX += BOMBER_SPEED_INC;
      addVelNegX -= BOMBER_SPEED_INC;
      addVelPosY += BOMBER_SPEED_INC;
      addVelNegY -= BOMBER_SPEED_INC;
  
      
   };

   virtual void kill();
   virtual void drawShip(bool isPaused) {};
   //getters
   float getHitPoints() { return hitPoints; };
   int getType() { return type; };
   
   //setters
   void setHitPoints(float hitPoints) {this->hitPoints = hitPoints; };
   void setType(int type) {this->type = type; };

   
};

// *** alien saucer ship (Assimliator) ***
class Saucer : public AlienShip
{
private:
protected:
   int spin;
   int backSpin;
   int slowSpin;
public:
   Saucer() {}; // default
   
   Saucer(Point point, bool isPaused)
   {
      setType(2);
      setHitPoints(SAUCER_HEALTH);
      Alive = true;
      float angle = random(0.0, 360.0);
      this->point.setX(point.getX());
      this->point.setY(point.getY());
      this->isPaused = isPaused;
      velocity.setDx(SAUCER_SPEED * (-cos(M_PI / 180.0 * angle)));
      velocity.setDy((point.getY() > 0)? random(-SAUCER_SPEED, -SAUCER_SPEED + 2.0) : random(SAUCER_SPEED, SAUCER_SPEED + 2.0));

      
   }
  // void hit() { hitPoints--; };
   void drawShip(bool isPaused);
   void kill();
   
   
   
   
};

// *** ALIEN SHIP scout ship
class ScoutShip : public Saucer
{
private:
protected:

public:
   ScoutShip(Point point, bool isPaused)
   {
      setType(1);
      setHitPoints(SCOUT_HEALTH);
      Alive = true;
      float angle = random(0.0, 720.0);
      this->point.setX(point.getX());
      this->point.setY(point.getY());
      this->isPaused = isPaused;
      velocity.setDx(SCOUT_SPEED * (-cos(M_PI / 180.0 * angle)));
      velocity.setDy((point.getY() > 0)? random(-SCOUT_SPEED, -SCOUT_SPEED + 5.0) : random(SCOUT_SPEED, SCOUT_SPEED + 5.0));
      
      
   }
   void drawShip(bool isPaused);
   
   
   
   
   
   
};

// *** ALIEN SHIP scout ship
class Bomber : public Saucer
{
private:
protected:
   

   
public:
   Bomber(Point point, bool isPaused)
   {
      
      addVelPosX = BOMBER_SPEED;
      addVelNegX = -BOMBER_SPEED;
      addVelPosY = BOMBER_SPEED;
      addVelNegY = -BOMBER_SPEED;

      setType(3);
      setHitPoints(BOMBER_HEALTH);
      Alive = true;
      float angle = random(0.0, 720.0);
      this->point.setX(point.getX());
      this->point.setY(point.getY());
      this->isPaused = isPaused;
      velocity.setDx(BOMBER_SPEED * (-cos(M_PI / 180.0 * angle)));
      velocity.setDy((point.getY() > 0)? -BOMBER_SPEED : BOMBER_SPEED);
      
      
   }
   void drawShip(bool isPaused);

   
};












#endif /* alienShips_h */
