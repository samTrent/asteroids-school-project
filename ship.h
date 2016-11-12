
#ifndef ship_h
#define ship_h

#include "uiDraw.h"
#include "flyingObject.h"

#define ANGLE_MAX 360
#define ANGLE_MIN 0
#define ANGLE_START 90


#define SHIP_SIZE 10

#define ROTATE_AMOUNT 6

#define TOP_SPEED 2.0

#define THRUST_AMOUNT_SLOW 0.2
#define THRUST_AMOUNT_NORMAL 0.5
#define THRUST_AMOUNT_FAST 0.65



// Put your Ship class here
class Ship : public FlyingObject
{
protected:
   int fuel;
   float rotation;
   float angle;
   bool Thrust;
   bool Alive;
   bool isThrustingNow;
   int deathAnimationLen;
   int debrisSpin;
   bool isPaused;
   int accelSpeed;
   int spin;
   int rotateSpeed;
   
   //upgrades
   bool shieldBat1;
   bool shieldBat2;
   bool plasmaRange;
   bool plasmaHeatSink;
   bool shotGunUp;
   bool laserRangeUp;
   bool deepScannerUp;
   
   
public:
   Ship() {};
   //Constructors
   //Default
   /* ship() */
   /* { */
   /*    // angel = ANGLE_START; */
   /*    // velocity = velocity(); */
   /*    // Point = point(); */
   /* }; */
   Ship(Point point, bool isPaused, int accelSpeed, bool shieldBat1, bool shieldBat2, bool plasmaRange, bool plasmaHeatSink, bool shotGunUp, bool laserRangeUp, bool deepScannerUp, int rotateSpeed)
   {
      spin = 0;
      isThrustingNow = false;
      angle = ANGLE_START;
      setCanThrust(true);
      Alive = true;
      this->point.setX(point.getX());
      this->point.setY(point.getY());
      this->isPaused = isPaused;
      this->accelSpeed = accelSpeed;
      this->shieldBat1 = shieldBat1;
      this->shieldBat2 = shieldBat2;
      this->plasmaRange = plasmaRange;
      this->plasmaHeatSink = plasmaHeatSink;
      this->shotGunUp = shotGunUp;
      this->laserRangeUp = laserRangeUp;
      this->deepScannerUp = deepScannerUp;
      this->rotateSpeed = rotateSpeed;
      
   };
   
   //check to see if the ship is alive!
   bool isAlive() { return Alive; };
   
   void deathAnimation(int deathAnimationLen); // draws a bunch of lines when you die
   
   
   
   /*******************************
    * SHIP ADVANCE
    *******************************/
   //updates the landers stats ingame
   virtual void move();
   virtual void draw(bool isPaused, bool shieldBat1, bool shieldBat2, bool plasmaRange, bool plasmaHeatSink, bool shotGunUp, bool laserRangeUp, bool laserDamageUp, bool deepScannerUp);
   //bool canThrust();
   
   
   /*******************************
    * SHIP MOVEMENT
    *******************************/
   void applyThrustBottom(int accelSpeed);
   void applyThrustLeft(int rotateSpeed);
   void applyThrustRight(int rotateSpeed);
   void applyThrustTop(int accelSpeed);
   
   
   
   //Getters
   bool canThrust() {return Thrust; };
   bool isThrusting() {return isThrustingNow; };
   int getFuel() { return fuel; };
   Velocity getVelocity() const { return velocity; };
   Point getPoint() const {return point; };
   float getAngle() { return rotation; };
   
   //Setters
   void setCanThrust(bool Thrust) {this->Thrust = Thrust;};
   void setisThrusting(bool isThrustingNow) {this->isThrustingNow = isThrustingNow;};
   void setFuel(int fuel) {this->fuel = fuel; };
   void setAlive(bool Alive) {this->Alive = Alive; };
   void setPoint(Point point) {this->point = point; };
   
   
};



#endif /* ship_h */
