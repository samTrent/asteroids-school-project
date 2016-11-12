//
//  alienBullet.h
//  asteroids pt2
//
//  Created by Sam Trent on 3/19/16.
//  Copyright © 2016 Sam Trent. All rights reserved.
//

#ifndef alienBullet_h
#define alienBullet_h
#include "uiDraw.h"
#include "bullet.h"


class AlienBullet : public Bullet
{
private:
protected:
   int type;
public:
   AlienBullet() {}; // default
   
   AlienBullet(Point point, int rotation)
   {
      setType(1);
      //int rotation = random(0, 360);
      timeLeft = 30;
      this->point.setX(point.getX());
      this->point.setY(point.getY());
      velocity.setDx(sin((rotation * M_PI) / 180) * 20);
      velocity.setDy(-cos((rotation * M_PI) / 180) * 20);

   }
   int getTimeLeft() {return timeLeft;};
   void setTimeLeft(int timeLeft) {this->timeLeft = timeLeft; };
   
   int getType() { return type; };
   void setType(int type) {this->type = type; }; 
   void fire(Point point, float angle);
   virtual void draw();
};

// ALEIN SCOUT SHIP BULLET
class ScoutBullet : public AlienBullet
{
private:
protected:
   int type;
public:
   ScoutBullet(Point point, int rotation)
   {
      setType(2);
      //int rotation = random(0, 360);
      timeLeft = 30;
      this->point.setX(point.getX());
      this->point.setY(point.getY());
      velocity.setDx(sin((rotation * M_PI) / 180) * 20);
      velocity.setDy(-cos((rotation * M_PI) / 180) * 20);
      
   }
   void draw() { drawWeapon(getPoint(), 2, 0, 0.0, 1.0, 0.0); };
   
   
   
   
   
   
};

//Alien bomber bomb
class AlienBomb : public AlienBullet
{
private:
protected:
   int type;
public:
   AlienBomb(Point point, int rotation)
   {
      setType(3);
      //int rotation = random(0, 360);
      timeLeft = 150;
      this->point.setX(point.getX());
      this->point.setY(point.getY());

      
      
   }
   
   

   
   void draw();
};

class BombShrapnel : public AlienBullet
{
private:
protected:
   int type;
public:
   BombShrapnel(Point point, int rotation)
   {
      setType(4);
      //int rotation = random(0, 360);
      timeLeft = 20;
      this->point.setX(point.getX());
      this->point.setY(point.getY());
      velocity.setDx(sin((rotation * M_PI) / 180) * 20);
      velocity.setDy(-cos((rotation * M_PI) / 180) * 20);
      
      
      
   }
   
   
   
   
   void draw();
};


#endif /* alienBullet_h */
