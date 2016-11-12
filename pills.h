//
//  pills.h
//  asteroids pt2
//
//  Created by Sam Trent on 3/18/16.
//  Copyright © 2016 Sam Trent. All rights reserved.
//

#ifndef pills_h
#define pills_h
#include "uiDraw.h"
#include "flyingObject.h"

#define PILL_SPEED 3

#define SHIELD_PILL_TIME 375 // time until shield pill expires
#define SHOT_GUN_PILL_TIME 350
#define LASER_PILL_TIME 325
#define RESEARCH_PILL_TIME 400

class Pills : public FlyingObject
{
private:
protected:
   bool alive;
   int timeAlive;
   int type;
   float fade;
   bool isPaused;
   bool upSGAmmo;

public:
   Pills() {};
   //methods
   void move();
   virtual void draw(bool isPaused, bool upSGAmmo) {};
   
   
   //Getters
   int getType() { return type;};
   bool isAlive() {return alive; };
   int getTimeAlive() { return timeAlive; };
   float getFade() { return fade; };
   
   
   //Setters
   void setAlive(bool alive) {this->alive = alive; };
   void setTimeAlive(int timeAlive) { this->timeAlive = timeAlive; };
   void setFade(float fade) { this->fade = fade; };
   
   
};



// recharge shields
class ShieldPill : public Pills
{
private:
protected:
   
public:
   ShieldPill() {};
   ShieldPill(Point point, bool isPaused, bool upSGAmmo)
   {
      fade = 1.0;
      type = 1;
      timeAlive = SHIELD_PILL_TIME;
      alive = true;
      float angle = random(-1000.0, 1000.0);
      this->point.setX(point.getX());
      this->point.setY(point.getY());
      this->isPaused = isPaused;
      this->upSGAmmo = upSGAmmo;
      velocity.setDx(PILL_SPEED * (-cos(M_PI / 180.0 * angle)));
      velocity.setDy(PILL_SPEED * (sin(M_PI / 180.0 * angle)));
      
   }
   void draw(bool isPaused, bool upSGAmmo);
   void timeToExpire(int timeAlive);
   void fadePill(int timeAlive);
   
   
   
};

//shot gun ammo
class ShotGunPill: public ShieldPill
{
private:
protected:
   
public:
   ShotGunPill(Point point, bool isPaused, bool upSGAmmo)
   {
      fade = 1.0;
      type = 2;
      timeAlive = SHOT_GUN_PILL_TIME;
      alive = true;
      float angle = random(-1000.0, 1000.0);
      this->point.setX(point.getX());
      this->point.setY(point.getY());
      this->isPaused = isPaused;
      this->upSGAmmo = upSGAmmo;
      velocity.setDx(PILL_SPEED * (-cos(M_PI / 180.0 * angle)));
      velocity.setDy(PILL_SPEED * (sin(M_PI / 180.0 * angle)));
      
   }
   void draw(bool isPaused, bool upSGAmmo);
   
   
   
   
   
   
};

// laser ammo
class LaserPill: public ShieldPill
{
   
private:
protected:
public:
   LaserPill(Point point, bool isPaused, bool upSGAmmo)
   {
      fade = 1.0;
      type = 3;
      timeAlive = LASER_PILL_TIME;
      alive = true;
      float angle = random(-1000.0, 1000.0);
      this->point.setX(point.getX());
      this->point.setY(point.getY());
      this->isPaused = isPaused;
      this->upSGAmmo = upSGAmmo;
      velocity.setDx(PILL_SPEED * (-cos(M_PI / 180.0 * angle)));
      velocity.setDy(PILL_SPEED * (sin(M_PI / 180.0 * angle)));
      
   }
   void draw(bool isPaused, bool upSGAmmo);
   
   
   
   
};


// RESEARCH PILL

class ResearchPill: public ShieldPill
{
private:
protected:
public:
   ResearchPill(Point point, bool isPaused, bool upSGAmmo)
   {
      fade = 1.0;
      type = 4;
      timeAlive = RESEARCH_PILL_TIME;
      alive = true;
      float angle = random(-1000.0, 1000.0);
      this->point.setX(point.getX());
      this->point.setY(point.getY());
      this->isPaused = isPaused;
      this->upSGAmmo = upSGAmmo;
      velocity.setDx(PILL_SPEED * (-cos(M_PI / 180.0 * angle)));
      velocity.setDy(PILL_SPEED * (sin(M_PI / 180.0 * angle)));
      
   }
   void draw(bool isPaused, bool upSGAmmo);
   
   

   
};

#endif /* pills_h */
