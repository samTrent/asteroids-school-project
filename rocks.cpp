#include "rocks.h"

// Put your Rock methods here


/**************************************
 * Method: hit
 * function: lets the game know if we
 *  hit the asteroid. Returns the number of
 * points you get for hitting the asteriods.
 ***************************************/
void Rock::hit()
{
   
   
}

/**************************************
 * Method: draw
 * function: Draws a regular asteroid
 ***************************************/
void Rock::draw(bool isPaused)
{
   // std::cout << "spinning: " << spin << std::endl;
  // drawLargeAsteroid(getPoint(), spin);
   
}



/**************************************
 * Method: move
 * function: moves the Asteroid around
 *  the screen.
 ***************************************/
void Rock::move()
{
  // std::cout << "in ROCK:MOVE\n";
   advance();
}






// * BIG ASTROID *


/**************************************
 * Method: draw
 * function: Draws a regular asteroid
 ***************************************/
void BigAsteroid::draw(bool isPaused)
{
   
   drawLargeAsteroid(getPoint(), spin);
   if(isPaused == false)
   {
      spin += BIG_ROCK_SPIN;
   }
}
/**************************************
 * Method: hit
 * function: lets the game know if we
 *  hit the asteroid. calls "createAsteroids"
 *  and passes the asteroid type so it knows
 *  what other asteroids it needs to 
 *  create in its place.
 ***************************************/
void BigAsteroid::hit()
{
   healthpoints--;
     if (healthpoints == 0)
   {
      setAlive(false) ;
   }
}







// **** MED ASTROID ****

/**************************************
 * Method: draw
 * function: Draws a regular asteroid
 ***************************************/
void MedAsteroid::draw(bool isPaused)
{
   // std::cout << "here\n";
   drawMediumAsteroid(getPoint(), spin);
   if (isPaused == false)
   {
      spin += MEDIUM_ROCK_SPIN;
   }
   
}

/**************************************
 * Method: hit
 * function: lets the game know if we
 *  hit the asteroid. calls "createAsteroids"
 *  and passes the asteroid type so it knows
 *  what other asteroids it needs to
 *  create in its place.
 ***************************************/
void MedAsteroid::hit()
{
   healthpoints--;
   if (healthpoints == 0)
   {
      setAlive(false) ;
   }
}





// * Small ASTROID *

/**************************************
 * Method: draw
 * function: Draws a regular asteroid
 ***************************************/
void SmallAsteroid::draw(bool isPaused)
{
   // std::cout << "here\n";
   drawSmallAsteroid(getPoint(), spin);
   if(isPaused == false)
   {
      spin += SMALL_ROCK_SPIN;
   }
}

/**************************************
 * Method: hit
 * function: lets the game know if we
 *  hit the asteroid. calls "createAsteroids"
 *  and passes the asteroid type so it knows
 *  what other asteroids it needs to
 *  create in its place.
 ***************************************/
void SmallAsteroid::hit()
{
   healthpoints--;
   if (healthpoints == 0)
   {
      setAlive(false) ;
   }
}





// * RockBits *

/**************************************
 * Method: draw
 * function: Draws a regular asteroid
 ***************************************/
void RockBits::draw(bool isPaused)
{
   drawDot(getPoint());
   if(isPaused == false)
   {
      rockBitTimeLeft(timeLeft--);
   }
}
/**************************************
 * Method: rockbitTimeleft
 * function: kills the rockbit if its been
 * on screen too long
 ***************************************/
void RockBits::rockBitTimeLeft(int timeLeft)
{
   if (timeLeft <= 0)
   {
      setAlive(false);
   }
   
}

// *** ALIEN ASTEROID
/**************************************
 * Method: draw
 * function: Draws a regular asteroid
 ***************************************/
void AlienRocks::draw(bool isPaused)
{
   // std::cout << "here\n";
   drawAlienAsteroid(getPoint(), spin);
   if (isPaused == false)
   {
      spin += 15;
   }
}

/**************************************
 * Method: hit
 * function: lets the game know if we
 *  hit the asteroid. calls "createAsteroids"
 *  and passes the asteroid type so it knows
 *  what other asteroids it needs to
 *  create in its place.
 ***************************************/
void AlienRocks::hit()
{
   healthpoints--;
   if (healthpoints == 0)
   {
      setAlive(false) ;
   }
}


/**************************************
 * Method: draw
 * function: Draws a regular asteroid
 ***************************************/
void AlienRockBits::draw(bool isPaused)
{
   // std::cout << "here\n";
   drawWeapon(getPoint(), 1, 0, 0.0, 1.0, 0.0);
   if(isPaused == false)
   {
      rockBitTimeLeft(timeLeft--);
   }
}

/**************************************
 * Method: rockbitTimeleft
 * function: kills the rockbit if its been
 * on screen too long
 ***************************************/
void AlienRockBits::rockBitTimeLeft(int timeLeft)
{
   if (timeLeft <= 0)
   {
      setAlive(false);
   }
   
}

///**************************************
// * Method: hit
// * function: lets the game know if we
// *  hit the asteroid. calls "createAsteroids"
// *  and passes the asteroid type so it knows
// *  what other asteroids it needs to
// *  create in its place.
// ***************************************/
//void AlienRockBits::hit()
//{
//   healthpoints--;
//   if (healthpoints == 0)
//   {
//      setAlive(false) ;
//   }
//}


