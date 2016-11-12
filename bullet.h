#ifndef bullet_h
#define bullet_h

#define BULLET_SPEED 10
#define BULLET_LIFE 80

#define SHOT_GUN_SPEED 18
#define SHOT_GUN_LIFE 8 

#define LASER_SPEED 40 // 40
#define LASER_LIFE 65
#define LASER_LEN 30


#include "flyingObject.h"
#include "uiDraw.h"

class Bullet : public FlyingObject
{
private:
protected:
   bool Alive;
   int angle;
   int timeLeft;
   int type; // what type of projectile will be shot.
   
   //upgrades
   bool plasmaRange;
   int rangeInc;
   
public:
   Bullet()
   {
      timeLeft = BULLET_LIFE;
      type = 1;
   };
   
   Bullet(bool plasmaRange)
   {
      rangeInc = 0;
      this->plasmaRange = plasmaRange;
      if (plasmaRange == true)
      {
         rangeInc = 20;
      }
      timeLeft = BULLET_LIFE + rangeInc;
      type = 1;
      
   };
   //Methods
   virtual void fire(Point point, float angle, Velocity v, bool shotGunUp);
   virtual void draw(bool laserDamageUp);
   void move();
   
   virtual void kill();
   
   void bulletLifeTime(int timeLeft);
   
   
   
   //Getter
   bool isAlive() { return Alive; }; // may need to change name to "isAlive"
   Point getPoint() { return point; };
   int getType() { return type;};
   
   
   //Setter
   void setAlive(bool Alive) { this->Alive = Alive; }; // may need to change
   
   
   
};

// *** New type of bullet ***
class ShotGun : public Bullet
{
private:
protected:

   
   
public:
   ShotGun()
   {
      timeLeft = SHOT_GUN_LIFE;
      type = 1;
   }
   
   void fire(Point point, float angle, Velocity v, bool shotGunUp);
   void draw(bool laserDamageUp) { drawWeapon(getPoint(), 1.5, 0, 1.0, 0.5, 0.0); };
   
   
   
   
};


// *** LASER ROUND ***
class Laser : public Bullet
{
private:
protected:
   float angleOfShip;
   
   
public:
   Laser(float ship_ang)
   {
      angleOfShip = ship_ang;
      type = 2;
      timeLeft = LASER_LIFE;
   }

   void fire(Point point, float angle, Velocity v, bool shotGunUp);
   void draw(bool laserDamageUp)
   {
      if (laserDamageUp == false)
      {
         int num = 2;
         int spin = 0;
         drawLaser(getPoint(), 65, 2, angleOfShip + 90, 1.0, 0.0, 0.0);
         spin += num;
      }
      
      if (laserDamageUp == true)
      {
         int num = 2;
         int spin = 0;
         drawLaser(getPoint(), 65, 2, angleOfShip + 90, 0.0, 1.0, 0.0);
         spin += num;
      }
   };
   
};






#endif /* bullet_h */
