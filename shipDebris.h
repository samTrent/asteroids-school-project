//
//  shipDebris.h
//  asteroids pt2
//
//  Created by Sam Trent on 3/18/16.
//  Copyright © 2016 Sam Trent. All rights reserved.
//

#ifndef shipDebris_h
#define shipDebris_h
#include "uiDraw.h"
#include "rocks.h"


#define DEBRIS_SPIN_RATE 6
#define DEBRIS_MOVE_RATE 1.5

class ShipDebris : public Rock
{
private:
   

protected:
   bool alive;
   int deathAnimationLen;
   int healthpoints;
   int debrisSpin;
   
   
public:
   ShipDebris() {};
   ShipDebris(Point point, bool isPaused)
   {
      debrisSpin = random(0, 400);
      
      
      
      healthpoints = 1;
      
     // type = 1;
      alive = true;
      float angle = random(0.0, 360.0);
      this->point.setX(point.getX());
      this->point.setY(point.getY());
      this->isPaused = isPaused;
      velocity.setDx(random(0 - DEBRIS_MOVE_RATE, DEBRIS_MOVE_RATE) * (-cos(M_PI / 180.0 * angle)));
      velocity.setDy(random(0 - DEBRIS_MOVE_RATE, DEBRIS_MOVE_RATE) * (sin(M_PI / 180.0 * angle)));
      
      
   };

   ;
   //Methods
   bool isAlive() {return alive; };
   void move();
   

   //  void Kill() { hitPoints = 0; };

   //Getters
   


   //Setters
   void setPoint(Point point) {this->point = point; };
   void setAlive(bool alive) {this->alive = alive; };

   void draw(bool isPaused);
  



   
   
};

class DebrisBits : public Rock
{
private:
   
   
protected:
   int timeLeft;
   bool alive;

   
public:
   DebrisBits() {};
   DebrisBits(Point point, bool isPaused)
   {
     
     
      alive = true;
      float angle = random(-1000.0, 1000.0);
      this->point.setX(point.getX());
      this->point.setY(point.getY());
      this->isPaused = isPaused;
      velocity.setDx(random(0 - DEBRIS_MOVE_RATE, DEBRIS_MOVE_RATE) * (-cos(M_PI / 180.0 * angle)));
      velocity.setDy(random(0 - DEBRIS_MOVE_RATE, DEBRIS_MOVE_RATE) * (sin(M_PI / 180.0 * angle)));
      
   };
   void draw(bool isPaused);
 
   
   
   //Setters
   void setPoint(Point point) {this->point = point; };
   void setAlive(bool alive) {this->alive = alive; };

   
   
   
   
};


/********************************
 *
 * *** ALIEN DEBRIS ***
 *
 ********************************/

class AlienShipDebris : public ShipDebris
{
private:
protected:
public:
   
   AlienShipDebris(Point point, bool isPaused)
   {
      debrisSpin = random(0, 400);
      
      
      
      healthpoints = 1;
      
      // type = 1;
      alive = true;
      float angle = random(0.0, 360.0);
      this->point.setX(point.getX());
      this->point.setY(point.getY());
      this->isPaused = isPaused;
      velocity.setDx(random(0 - DEBRIS_MOVE_RATE, DEBRIS_MOVE_RATE) * (-cos(M_PI / 180.0 * angle)));
      velocity.setDy(random(0 - DEBRIS_MOVE_RATE, DEBRIS_MOVE_RATE) * (sin(M_PI / 180.0 * angle)));
      
      
   };

   void draw(bool isPaused);
   
   
   
};

class AlienDebrisBits :public DebrisBits
{
private:
protected:
public:
   
   AlienDebrisBits(Point point, bool isPaused)
   {
      
      
      alive = true;
      float angle = random(-1000.0, 1000.0);
      this->point.setX(point.getX());
      this->point.setY(point.getY());
      this->isPaused = isPaused;
      velocity.setDx(random(0 - DEBRIS_MOVE_RATE, DEBRIS_MOVE_RATE) * (-cos(M_PI / 180.0 * angle)));
      velocity.setDy(random(0 - DEBRIS_MOVE_RATE, DEBRIS_MOVE_RATE) * (sin(M_PI / 180.0 * angle)));
      
   };

   void draw(bool isPaused);
   
};



/********************************
*
* *** WARP FIELD ***
*
********************************/
class WarpField : public ShipDebris
{
private:
protected:
   
   
public:
   WarpField(Point point, bool isPaused)
   {
      
      debrisSpin = random(0, 400);
      
      
      
      healthpoints = 1;
      
      // type = 1;
      alive = true;
      float angle = random(0.0, 360.0);
      this->point.setX(point.getX());
      this->point.setY(point.getY());
      this->isPaused = isPaused;
      velocity.setDx(random(0 - DEBRIS_MOVE_RATE, DEBRIS_MOVE_RATE) * (-cos(M_PI / 180.0 * angle)));
      velocity.setDy(random(0 - DEBRIS_MOVE_RATE, DEBRIS_MOVE_RATE) * (sin(M_PI / 180.0 * angle)));
      
      
      
      
   }
   
   void draw(bool isPaused);
   
   Point getPoint() { return point; };
   
   
   
   
};

























#endif /* shipDebris_h */
