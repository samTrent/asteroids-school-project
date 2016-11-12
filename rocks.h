  /************************
 * All three types of
 * asteroids here.
 ***********************/
#ifndef rocks_h
#define rocks_h

#include "uiDraw.h"
#include "flyingObject.h"


#define BIG_ROCK_SIZE 16
#define MEDIUM_ROCK_SIZE 8
#define SMALL_ROCK_SIZE 4

//Asteroid spin rate
#define BIG_ROCK_SPIN 2
#define MEDIUM_ROCK_SPIN 5
#define SMALL_ROCK_SPIN 10

//Asteroid speed
#define BIG_ROCK_SPEED 1
#define MEDIUM_ROCK_SPEED 5
#define SMALL_ROCK_SPEED 10
#define ALIEN_ROCK_SPEED 15
#define ROCK_BIT_SPEED 3

#define ROCK_BIT_LIFE 20



/*******************************
 * CLASS: Rock (isA FlyingObject)
 * base class for our asteroids
 *******************************/

class Rock : public FlyingObject
{
private:
protected:
   bool alive;
   int type;
   int spin;
   int healthpoints;
   bool isPaused;
  public:
   //Methods
   bool isAlive() {return alive; };
   void move();
   virtual void hit();
   virtual void draw(bool isPaused);
   //  void Kill() { hitPoints = 0; };
   
   //Getters
   int getType() { return type;};
   int getAnimationLen() { return getAnimationLen(); };
   
   
   //Setters
   void setAlive(bool alive) {this->alive = alive; };
};

/*******************************
 * CLASS: BigAsteroid (isA FlyingObject)
 * Drived class for our asteroids
 *******************************/

class BigAsteroid : public Rock
{
private:
protected:
   

public:
   BigAsteroid(Point point, bool isPaused)
   {
      healthpoints = 1;
      spin = 0;
      type = 1;
      alive = true;
      float angle = random(0.0, 360.0);
      this->point.setX(point.getX());
      this->point.setY(point.getY());
      this->isPaused = isPaused;
      velocity.setDx(BIG_ROCK_SPEED * (-cos(M_PI / 180.0 * angle)));
      velocity.setDy(BIG_ROCK_SPEED * (sin(M_PI / 180.0 * angle)));
   
      
   };
   void draw(bool isPaused);
   void hit();

   
   
   
   
};

/*******************************
 * CLASS: MedAsteroid (isA FlyingObject)
 * Drived class for our asteroids
 *******************************/
class MedAsteroid : public Rock
{
private:
protected:
   
   
public:
   MedAsteroid(Point point, bool isPaused)
   {
      healthpoints = 1;
      spin = 0;
      type = 2;
      alive = true;
      float angle = random(0.0, 360.0);
      this->point.setX(point.getX());
      this->point.setY(point.getY());
      this->isPaused = isPaused;
      velocity.setDx(MEDIUM_ROCK_SPEED * (-cos(M_PI / 180.0 * angle)));
      velocity.setDy(MEDIUM_ROCK_SPEED * (sin(M_PI / 180.0 * angle)));
      
      
   };
   void draw(bool isPaused);
   void hit();
   
   
   
   
};

/*******************************
* CLASS: SmallAsteroid (isA FlyingObject)
* Drived class for our asteroids
*******************************/
class SmallAsteroid : public Rock
{
private:
protected:
   
   
public:
   SmallAsteroid(Point point, bool isPaused)
   {
      healthpoints = 1;
      spin = 0;
      type = 3;
      alive = true;
      float angle = random(0.0, 360.0);
      this->point.setX(point.getX());
      this->point.setY(point.getY());
      this->isPaused = isPaused;
      velocity.setDx(SMALL_ROCK_SPEED * (-cos(M_PI / 180.0 * angle)));
      velocity.setDy(SMALL_ROCK_SPEED * (sin(M_PI / 180.0 * angle)));
      
      
   };
   void draw(bool isPaused);
   void hit();
   
   
   
   
};

/*******************************
 * CLASS: RockBits (isA FlyingObject)
 * Drived class for our asteroids
 *******************************/
class RockBits : public Rock
{
private:
protected:
   int timeLeft;
   
public:
   RockBits(Point point, bool isPaused)
   {
      timeLeft = ROCK_BIT_LIFE;
      type = 4;
      alive = true;
      float angle = random(-1000.0, 1000.0);
      this->point.setX(point.getX());
      this->point.setY(point.getY());
      this->isPaused = isPaused;
      velocity.setDx(random(0 - ROCK_BIT_SPEED, ROCK_BIT_SPEED) * (-cos(M_PI / 180.0 * angle)));
      velocity.setDy(random(0 - ROCK_BIT_SPEED, ROCK_BIT_SPEED) * (sin(M_PI / 180.0 * angle)));
      
      
   };
   void draw(bool isPaused);
   void rockBitTimeLeft(int timeLeft);
   
   
   
   
};



/*******************************
* CLASS: Alien rocks!
* Drived class for our asteroids
*******************************/
class AlienRocks : public Rock
{
private:
protected:
   int timeLeft;
   
public:
   AlienRocks(Point point, bool isPaused)
   {
      healthpoints = 1;
      spin = 0;
      type = 5;
      alive = true;
      float angle = random(0.0, 360.0);
      this->point.setX(point.getX());
      this->point.setY(point.getY());
      this->isPaused = isPaused;
      velocity.setDx(ALIEN_ROCK_SPEED * (-cos(M_PI / 180.0 * angle)));
      velocity.setDy(ALIEN_ROCK_SPEED * (sin(M_PI / 180.0 * angle)));
      
      
   }
   
   void draw(bool isPaused);
   void hit();
   
   
   
   
   
   
};


//rock bits
class AlienRockBits : public Rock
{
private:
protected:
   int timeLeft;
   
public:
   AlienRockBits(Point point, bool isPaused)
   {
      timeLeft = ROCK_BIT_LIFE;
      type = 6;
      alive = true;
      float angle = random(-1000.0, 1000.0);
      this->point.setX(point.getX());
      this->point.setY(point.getY());
      this->isPaused = isPaused;
      velocity.setDx(random(0 - ROCK_BIT_SPEED, ROCK_BIT_SPEED) * (-cos(M_PI / 180.0 * angle)));
      velocity.setDy(random(0 - ROCK_BIT_SPEED, ROCK_BIT_SPEED) * (sin(M_PI / 180.0 * angle)));
      
      
   };
   void draw(bool isPaused);
   void rockBitTimeLeft(int timeLeft);
   
   
   
   
};






#endif /* rocks_h */
