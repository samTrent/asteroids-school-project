   /*********************************************************************
 * File: game.cpp
 * Description: Contains the implementaiton of the game class
 *  methods.
 *
 *********************************************************************/

#include "game.h"

#include "uiDraw.h"
#include "uiInteract.h"
#include "point.h"
#include <cassert>

// These are needed for the getClosestDistance function...
#include <limits>
#include <algorithm>
using namespace std;
#define OFF_SCREEN_BORDER_AMOUNT 5
#define NUM_START_SHIPS 1 
#define NUM_OF_ROCK_BITS 4 // x3
#define NUM_OF_DEBRIS 10
//setIsPaused(false);

// *** screen advance ***
#define CURRENT_SECTOR 1          // current level
#define END_SECTOR 10              // last level
#define DEATH_ANMIATION_LEN 1800      // how long you view the screen after you die
#define TIME_TILL_NEXT_SCREEN 120  // how long till death screen

double warningAnimate = 1.0;


// advance to next level animations
int nextSlide = TIME_TILL_NEXT_SCREEN;
int deathAnimation = DEATH_ANMIATION_LEN;

int currentWeapon = 1;


int lastSector = END_SECTOR;



#define ADV_TIME 120           // how long the warp field will advance
int advTime = ADV_TIME;
#define WARP_FIELD_SIZE 100 // 100
#define WARP_ANIMATION_LEN 10000 // 1
int warpAnimate = WARP_ANIMATION_LEN;

#define SHOW_GREET_TIME 100 // "Entering sector yadayaday ...
int showGreetTime = SHOW_GREET_TIME;


// *** BALANCE ***

#define STARTING_SHIELDS 100
#define NUM_START_ASTRO 5     // number of astroids in the beginning "Should be 5"


#define INVINCE_TIME 50          // keeps the player from being damaged by asteroids in the beginning
int invinceTime = INVINCE_TIME;

#define BIG_ASTRO_DAMG 20
#define MED_ASTRO_DAMG 10
#define SMALL_ASTRO_DAMG 5
#define ALIEN_ASTRO_DAMG 15
int astroHit = 0; // counts how many times your ship has been hit


#define CHANCE_OF_PILL 6 // 10
#define SHIELD_PILL 25

#define SHOT_GUN_AMMO 20 // 30
#define NUM_OF_BBS 5
#define SHOT_GUN_UP_CONSUMPSTION 3
#define SG_AMMO_UP 20

#define LASER_AMMO 10 // 10
#define LASER_DAMAGE_UP_CONSUMPTION 10

#define PLASMA_CANNON_OVERHEAT 100 // how long till the plasma cannon over heats
int plasmaCannonOverHeat = PLASMA_CANNON_OVERHEAT;
#define PLASMA_DISCHARGE_RATE 3

#define PICKUP_RANGE_UP 50


#define ALIENS_PER_LEVEL 1    // Number of aliens that can appear at the first level
#define ALIEN_SPAWN_TIME 500 // time it takes to spawn an alien (1000 is good)
int aliensPerLevel = ALIENS_PER_LEVEL;
int numOfAliens = 0;
#define SAUCER_FIRE_RATE 35 // less is faster 35
#define SAUCER_DAMAGE 10

#define SCOUT_FIRE_RATE 15 // less is faster
#define SCOUT_DAMAGE 1

#define BOMBER_FIRE_RATE 50
#define BOMB_DAMAGE 10
#define NUM_OF_SHRAP 15
#define SHRAP_DAMAGE 5

int shotGunAmmo = 0; // starting shot gun ammo
int LaserAmmo = 0; // starting laser ammo
#define CHANCE_OF_RS_DROP 3 // chance of an alien droping a research point


#define WHEN_ALIENS_SHOW 3  // what level the aliens start to show up
int aliensCreated = 0;     // keeps track of how many aliens have been created in the current sector
int spawnAlien = ALIEN_SPAWN_TIME;
int alienCount = 0; // keeps track of how many aliens are on the screen and displays in UI
int oneAlienPerWarp = 0; // makes sure that only one alien comes at a time


// plasma charge colors
float red2 = 0.0;
float green2 = 1.0;
float blue2 = 0.0;


/***************************************
 * GAME CONSTRUCTOR
 ***************************************/
Game :: Game(Point tl, Point br)
: topLeft(tl), bottomRight(br), asteroids()
{
   
}


/****************************************
 * GAME DESTRUCTOR
 ****************************************/
Game :: ~Game()
{
   if (gameOver == true)
   {
      delete ships[0];
      delete asteroid;
   }
}

/***************************************
 * GAME :: ADVANCE
 * advance the game one unit of time
 ***************************************/
void Game :: advance(bool &showUpgradeScreen, int &researchPoints, bool &shieldsBelow25)
{
   if (showGreetTime <= 0)
   {
     
      
      if (invinceTime > 0)
      {
         drawSheilds(ships[0]->getPoint(), 20.0, 1.0, 1.0, 0.0, 1.0);
      }
      if(getIsPaused() == false)
      {
         
         advanceAsteroids();
         if (invinceTime != -1)
         {
            invinceTime--; // nullifies damage from asteroids in the beginning
         }
         
         if (ships[0]->isAlive()) // stop if the ship is dead
         {
            advanceShip();
            advanceBullets();
            handleCollisions();
            pillCollisions(researchPoints);
            handleAlienCollision();
            handleAlienBulletShipCollision();
            warpInAliens();
            
            //check to see if shields fall below 25% for "unchouchable"
            if(shields < 25)
            {
               shieldsBelow25 = true;
            }
            
            //making sure that shields never go over 100
            if (getUpShieldBat1() == false)
            {
               if (shields > 100)
               {
                  shields = 100;
               }
            }
            else if (getUpShieldBat1() == true && getUpShieldBat2() == false)
            {
               if (shields >= 150)
               {
                  shields = 150;
               }
            }
            else if (getUpShieldBat2() == true && getUpShieldBat1() == true)
            {
               if (shields >= 200)
               {
                  shields = 200;
               }
               
            }
         }
         
         advanceWarpField();
         advanceAliens();
         advancePills();
         shipCollisions();
         advanceAlienBullets();
         advanceAlienDebris();
         handleAlienBulletRockCollision();
         deleteTheDead();
         
         
         if (shields <= 0) // game over condition
         {
            ships[0]->setAlive(false);
            
            //Moving our debris
            advanceDebris();
            
            if (nextSlide <= 0)
            {
               gameOver = true;
               
               //resetting screen advance times
               nextSlide = TIME_TILL_NEXT_SCREEN;
               debris.clear();
               shotGunAmmo = 0;
               LaserAmmo = 0;
               researchPoints = 0;
               currentWeapon = 1;
               
               deathAnimation = DEATH_ANMIATION_LEN;
               showGreetTime = SHOW_GREET_TIME;

            }
         }
         
         // go to the next level if its clear of all aliens and asteroids
         if (asteroids.size() == 0 && alienShips.size() <= 0)
         {
            
            if (currentSector < 10)
            {
               drawTitle(Point(-160,30), "MISSION ACCOMPLISHED", 0.0, 1.0, 0.0, 1.0);
               drawText(Point(-55,-55), "Returning to Base");
               
               // display this text if shields are not 100%
               if (shields < 100)
               {
                  drawTextColorTiny(Point(-60,-30), "Recharging Shields", 0.0, 1.0, 0.0, 1.0);
               }
               
            }
            else
            {
               drawTitle(Point(-60,30), "VICTORY!", 0.0, 1.0, 0.0, 1.0);
               drawText(Point(-80,-30), "Congratulations Commander");
            }
            nextSlide--; // timer till next level starts
            
            if (nextSlide <= 0)
            {
               
               if(currentSector >= 10)
               {
                  shotGunAmmo = 0;
                  LaserAmmo = 0;
                  currentWeapon = 1;
                  // if you win the game
                  showUpgradeScreen = false;
                  gameWin = true;
                  gameOver = true;
                  nextSlide = TIME_TILL_NEXT_SCREEN; // reseting value
               }
               
               showUpgradeScreen = true;
               gameOver = true;
              
               
               nextSlide = TIME_TILL_NEXT_SCREEN; // reseting value
               showGreetTime = SHOW_GREET_TIME;
               
               
            }
         }
         
      }
   }
   else if(showGreetTime > 0)
   {
      drawTitle(Point(-120,30), "ENTERING SECTOR", 0.0, 1.0, 0.0, 1.0);
      drawText(Point(-35,-50), "Get Ready!");
      drawSheilds(ships[0]->getPoint(), 20.0, 1.0, 1.0, 0.0, 1.0);
      showGreetTime--;
   }
}
/***************************************
 * GAME :: ADVANCE DEBRIS
 * makes the ship debris move accross the screen.
 ***************************************/
void Game :: advanceDebris()
{
   //*it is point to the current "debris" in the list
   //you need to derefrence it by putting in the "()"
   
   // Move each of the Debris forward if it is alive
   
   
   
   
   for (list <Rock*> :: iterator it = debris.begin();
        it != debris.end();
        ++it)
   {
      (*it)->setAlive(true);
      if ((*it)->isAlive())
      {
         
         // this Debris is "alive", so tell it to move forward
         (*it)->advance();
        
         
         if (!isOnScreen((*it)->getPoint()))
         {
            // the Debris has left the screen
            
            //make Debris wrap here
            (*it)->point.setX(- (*it)->point.getX());
            (*it)->point.setY(- (*it)->point.getY());
            
            
         } // *** if the Debrisleft the screem
         
      }  // *** move Debris forward
      
   }// *** if the Debris is alive
   
   nextSlide--; // time till we show the death screen
}// end

/***************************************
 * GAME :: ADVANCE WARP FIELD
 * makes the ship debris move accross the screen.
 ***************************************/
void Game :: advanceWarpField()
{
   if (advTime > 0)
   {
      
      
      for (list <Rock*> :: iterator it = warpField.begin();
           it != warpField.end();
           ++it)
      {
         (*it)->setAlive(true);
         if ((*it)->isAlive())
         {
            
            // this Debris is "alive", so tell it to move forward
            (*it)->advance();
            
            if (!isOnScreen((*it)->getPoint()))
            {
               // the Debris has left the screen
               
               //make Debris wrap here
               (*it)->point.setX(- (*it)->point.getX());
               (*it)->point.setY(- (*it)->point.getY());
               
               
            } // *** if the Debrisleft the screem
            
         }  // *** move Debris forward
         
      }// *** if the Debris is alive
      advTime--;
   }
}// end





/***************************************
 * GAME :: ADVANCE ALIEN DEBRIS
 * makes the ship debris move accross the screen.
 ***************************************/
void Game :: advanceAlienDebris()
{
   //*it is point to the current "debris" in the list
   //you need to derefrence it by putting in the "()"
   
   // Move each of the Debris forward if it is alive
   
   

   
   for (list <Rock*> :: iterator it = alienDebris.begin();
        it != alienDebris.end();
        ++it)
   {
      (*it)->setAlive(true);
      if ((*it)->isAlive())
      {
         
         // this Debris is "alive", so tell it to move forward
         (*it)->advance();
         
         
         if (!isOnScreen((*it)->getPoint()))
         {
            // the Debris has left the screen
            
            //make Debris wrap here
            (*it)->point.setX(- (*it)->point.getX());
            (*it)->point.setY(- (*it)->point.getY());
            
            
         } // *** if the Debrisleft the screem
         
      }  // *** move Debris forward
      
   }// *** if the Debris is alive
   
  // time till we show the death screen
}// end


/***************************************
 * GAME :: ADVANCE BULLETS
 * Go through each bullet and advance it.
 ***************************************/
void Game :: advanceBullets()
{
   
   for (list <Bullet*> :: iterator it = bullets.begin();
        it != bullets.end();
        ++it)
   {
      (*it)->setAlive(true);
      if ((*it)->isAlive())
      {
         
         // this bullet is alive, so tell it to move forward
         (*it)->move();
         
         
         if (!isOnScreen((*it)->getPoint()) && (*it)->getType() == 1)
         {
            // the bullet has left the screen
            
            //make bullets wrap here
            (*it)->point.setX(- (*it)->point.getX());
            (*it)->point.setY(- (*it)->point.getY());
            
            
         }
         if (!isOnScreen((*it)->getPoint()) && (*it)->getType() == 2 && getLaserShotUp() == true)
         {
            // the bullet has left the screen
            
            //make bullets wrap here
            (*it)->point.setX(- (*it)->point.getX());
            (*it)->point.setY(- (*it)->point.getY());
            
            
         }
         
         // *** if the bullet left the screem
         
      }  // *** move bullet forward
      
   }// *** if the bullet is alive
   
   
}// end

/***************************************
 * GAME :: ADVANCE ALIEN BULLETS
 * Go through each bullet and advance it.
 ***************************************/
void Game :: advanceAlienBullets()
{
   //*it is point to the current bullet in the list
   //you need to derefrence it by putting in the "()"
   
   // Move each of the bullets forward if it is alive
  
   for (list <AlienBullet*> :: iterator it = alienBullets.begin();
        it != alienBullets.end();
        ++it)
   {
      (*it)->setAlive(true);
      if ((*it)->isAlive())
      {
         
         // this bullet is alive, so tell it to move forward
         (*it)->move();
         
         if (!isOnScreen((*it)->getPoint()))
         {
            // the bullet has left the screen
            
            //make bullets wrap here
            (*it)->point.setX(- (*it)->point.getX());
            (*it)->point.setY(- (*it)->point.getY());
            
            
         } // *** if the bullet left the screem
         
      }  // *** move bullet forward
      
   }// *** if the bullet is alive
  
   
}// end


/**************************************************************************
 * GAME :: ADVANCE Asteroids
 *
 * 1. Create asteroids
 * 2. If there is an asteroid, and it's alive, advance it
 * 3. Check if the asteroid has gone of the screen, and if so, "wrap" it
 **************************************************************************/
void Game :: advanceAsteroids()
{
   
   for (list <Rock*> :: iterator it = asteroids.begin();
        it != asteroids.end();
        ++it)
   {
      (*it)->isAlive();
      if ((*it)->isAlive())
      {
         // this asteroid is alive, so tell it to move forward
         (*it)->advance();
         
         if (!isOnScreen((*it)->getPoint()))
         {
            // the asteroid has left the screen
//            
//            (*it)->point.setX(- (*it)->point.getX());
//            (*it)->point.setY(- (*it)->point.getY());
            
            if ((*it)->point.getX() < 0)
            {
               (*it)->point.setX((- (*it)->point.getX()) - 5);  
            }
            else
               (*it)->point.setX((- (*it)->point.getX()) + 5);

            if ((*it)->point.getY() < 0)
            {
               (*it)->point.setY((- (*it)->point.getY()) - 5);
            }
            else
               (*it)->point.setY((- (*it)->point.getY()) + 5);
            
            
         } // *** if the asteroid left the screem
         
      }  // *** move asteroid forward
      
   }// *** if the asteroid is alive

}


/**************************************************************************
 * GAME :: ADVANCE ALIENS!
 *
 **************************************************************************/
void Game :: advanceAliens()
{
   
   // move the alien
   for (list <AlienShip*> :: iterator it = alienShips.begin();
        it != alienShips.end();
        ++it)
   {
      (*it)->isAlive();
      if ((*it)->isAlive())
      {
         
         // this alien is alive, so tell it to move forward
         (*it)->move();
         if (ships[0]->isAlive()) // only shoot if the ship is alive
         {
            
            int alienType = (*it)->getType();
            
            //what alien bullet will be fired
            switch (alienType)
            {
                  
               case 1: // scout ship
               {
                  int rand = random(1, SCOUT_FIRE_RATE);
                  if (rand == 1)
                  {
                     alienBullets.push_back(new ScoutBullet((*it)->FlyingObject::getPoint(), ships[0]->getAngle()));
                     
                  }
               }
                  break;
               case 2: // assimilator
               {
                  int rand2 = random(1, SAUCER_FIRE_RATE);
                  if  (rand2 == 1)
                  {
                     alienBullets.push_back(new AlienBullet((*it)->FlyingObject::getPoint(), ships[0]->getAngle()));
                  }
                  
                  break;
               }
                  
               case 3: // bomber
                  {
                     int rand2 = random(1, BOMBER_FIRE_RATE);
                     if  (rand2 == 1)
                     {
                        alienBullets.push_back(new AlienBomb((*it)->FlyingObject::getPoint(), ships[0]->getAngle()));
                     }
                  }
                  break;
                  
               }
                  
                  
                  
                  
            }
            if (!isOnScreen((*it)->getPoint()))
            {
               int alienType = (*it)->getType();
               
               // the asteroid has left the screen
               switch (alienType)
               {
                  case 3:
                     //alien bomber
                  default:
                     if ((*it)->point.getX() < 0)
                     {
                        (*it)->point.setX((- (*it)->point.getX()) - 5);
                     }
                     else
                        (*it)->point.setX((- (*it)->point.getX()) + 5);
                     
                     if ((*it)->point.getY() < 0)
                     {
                        (*it)->point.setY((- (*it)->point.getY()) - 5);
                     }
                     else
                        (*it)->point.setY((- (*it)->point.getY()) + 5);
                     
                     break;
               }
               
               //            if ((*it)->point.getX() < 0)
               //            {
               //               (*it)->point.setX((- (*it)->point.getX()) - 5);
               //            }
               //            else
               //               (*it)->point.setX((- (*it)->point.getX()) + 5);
               //
               //            if ((*it)->point.getY() < 0)
               //            {
               //               (*it)->point.setY((- (*it)->point.getY()) - 5);
               //            }
               //            else
               //               (*it)->point.setY((- (*it)->point.getY()) + 5);
               
               
            } // *** if the alien left the screem
            
         }
         
      }
      // *** move alien forward
      
      // *** if the alien is alive
      
   }
   


/**************************************************************************
 * GAME :: warp in aliens!
 * warps in an alien if its sector 4 or greater and there are still asteroids 
 * on the screen.
 **************************************************************************/
void Game :: warpInAliens()
{
   if(getIsPaused() == false)
   {
      //create 1 alien group per warp-in
      if (alienShips.size() < aliensPerLevel  && aliensCreated < aliensPerLevel && currentSector >= WHEN_ALIENS_SHOW && asteroids.size() > 0) // for now we want only 1 alien at a time per level
      {
         
         Point point(random(-MY_X + 50, MY_X - 50), random(-MY_Y + 100, MY_Y - 50)); // get random point to warp in
         
         // after a certain duration, make an alien at the point of the warp in
         if (warpAnimate < 300 && oneAlienPerWarp == 0)
         {
            //make an alien at the point of the warp-in
            list <Rock*> :: iterator it = warpField.begin();
            {
               
               createAliens((*it)->getPoint());
               oneAlienPerWarp = 1;
               warpAnimate = WARP_ANIMATION_LEN;
               aliensCreated++;
            }
            
            
            
         }
         
         // for now we want only so many aliens per level and only if there are still asteroids on the screen
         if (warpField.size() == 0 && asteroids.size() > 0)
         {
            if (random(0, spawnAlien) == 0) // Makes alien spawn at a random time 1000
            {
               oneAlienPerWarp = 0;
               //draw the warp field
               createWarpField(point);
               numOfAliens++;
               
               
            }
         }
         
         
      }
   }
}


/**************************************************************************
 * GAME :: ADVANCE Pills
 *
 * 1. Create asteroids
 * 2. If there is an asteroid, and it's alive, advance it
 * 3. Check if the asteroid has gone of the screen, and if so, "wrap" it
 **************************************************************************/
void Game :: advancePills()
{
   
   for (list <Pills*> :: iterator it = pills.begin();
        it != pills.end();
        ++it)
   {
      (*it)->isAlive();
      if ((*it)->isAlive())
      {
         // this asteroid is alive, so tell it to move forward
         (*it)->advance();
         
         if (!isOnScreen((*it)->getPoint()))
         {
            // the asteroid has left the screen
//            (*it)->point.setX(- (*it)->point.getX());
//            (*it)->point.setY(- (*it)->point.getY());
            
            
            
            if ((*it)->point.getX() < 0)
            {
               (*it)->point.setX((- (*it)->point.getX()) - 5);
            }
            else
               (*it)->point.setX((- (*it)->point.getX()) + 5);
            
            if ((*it)->point.getY() < 0)
            {
               (*it)->point.setY((- (*it)->point.getY()) - 5);
            }
            else
               (*it)->point.setY((- (*it)->point.getY()) + 5);
            

            
         } // *** if the asteroid left the screem
         
      }  // *** move asteroid forward
      
   }// *** if the asteroid is alive
   
}

/**************************************************************************
 * GAME :: ADVANCE ship
 *
 * 1. Create the ship
 * 2. If the ship is alive, advance it
 * 3. Check if the ship has gone of the screen, and if so, "wrap" it
 **************************************************************************/
void Game :: advanceShip()
{
   
   
   ships[0]->advance();
  
   
   
   if (!isOnScreen(ships[0]->getPoint()))
   {
      // the asteroid has left the screen
      if (ships[0]->point.getX() < 0)
      {
         ships[0]->point.setX((- ships[0]->point.getX()) - 5);
      }
      else
         ships[0]->point.setX((- ships[0]->point.getX()) + 5);
      
      if (ships[0]->point.getY() < 0)
      {
         ships[0]->point.setY((- ships[0]->point.getY()) - 5);
      }
      else
         ships[0]->point.setY((- ships[0]->point.getY()) + 5);


   } // *** if the ship left the screen, make it wrap
   
}


/**************************************************************************
 * GAME :: CREATE Asteroid
 * Creates Meduim and Small asteroids if a big one is hit
 **************************************************************************/
void Game :: createAsteroid(int type, Point point)
{
   if (ships[0]->isAlive())
   {
      for (int i = 0; i < 3; i++)
      {
         switch (type)
         {
            case 1:
               asteroidsTotal++;
               if (i < 2)
                  asteroids.push_back(new MedAsteroid(point, getIsPaused()));
               else
                  asteroids.push_back(new SmallAsteroid(point ,getIsPaused()));
               break;
            case 2:
               if (i < 2)
               {
                  asteroidsTotal++;
                  asteroids.push_back(new SmallAsteroid(point, getIsPaused()));
               }
               else
                  ;
               break;
            case 3:
               for (int i = 0; i < NUM_OF_ROCK_BITS; i++)
               {
                  asteroids.push_back(new RockBits(point, getIsPaused()));
               }
               break;
               
               
         }
      }
   }
   else
   {
      //createDebris(ships[0]->getPoint());
   }
   
}

/**************************************************************************
 * GAME :: CREATE ALIEN Asteroid
 * Creates Meduim and Small asteroids if a big one is hit
 **************************************************************************/
void Game :: createAlienAsteroids(int type, Point point)
{
   if (ships[0]->isAlive())
   {
      for (int i = 0; i < 2; i++)
      {
         switch (type)
         {
            
            case 5:
             
             
                  asteroidsTotal++;
                  asteroids.push_back(new AlienRocks(point, getIsPaused()));
             
             
               
               break;
            case 6:
               for (int i = 0; i < 7; i++)
               {
                  asteroids.push_back(new AlienRockBits(point, getIsPaused()));
               }
               break;
               
               
         }
      }
   }
   
}


/**************************************************************************
 * GAME :: CREATE Debris
 * Creates debris after the ship dies
 **************************************************************************/
void Game :: createDebris(Point point)
{
   deathAnimation = DEATH_ANMIATION_LEN;
   for (int i = 0; i < NUM_OF_DEBRIS; i++)
   {
      debris.push_back(new ShipDebris(point, getIsPaused()));
      debris.push_back(new DebrisBits(point, getIsPaused()));
   }
   
   
}


/**************************************************************************
 * GAME :: CREATE Warp Field
 * makes a point where enemy ships spawn
 **************************************************************************/
void Game :: createWarpField(Point point)
{
   if(getIsPaused() == false)
   {
      advTime = 60;
      
      
      warpAnimate = WARP_ANIMATION_LEN;
      
      for (int i = 0; i < WARP_FIELD_SIZE; i++)
      {
         warpField.push_back(new WarpField(point, getIsPaused()));
         
      }
   }
   
}


/**************************************************************************
 * GAME :: CREATE Alien Debris
 * Creates debris after the ship dies
 **************************************************************************/
void Game :: createAlienDebris(Point point)
{
   
   deathAnimation = DEATH_ANMIATION_LEN;
   for (int i = 0; i < NUM_OF_DEBRIS; i++)
   {
      alienDebris.push_back(new AlienShipDebris(point, getIsPaused()));
      alienDebris.push_back(new AlienDebrisBits(point, getIsPaused()));
      
   }
   
   
}

/**************************************************************************
 * GAME :: CREATE pills
 * Creates different pills to power up the ship
 **************************************************************************/
void Game :: createPills(Point point)
{
   //add a random generator to get different pills
   int type = random(1,5);
   for (int i = 0; i < 1; i++)
   {
      switch (type)
      {
         case 0:
         case 1:
            pills.push_back(new ShieldPill(point, getIsPaused(), getUpSGAmmo()));
            break;
            
         case 2:
            pills.push_back(new ShotGunPill(point, getIsPaused(), getUpSGAmmo()));
            break;
         case 3:
            pills.push_back(new LaserPill(point, getIsPaused(), getUpSGAmmo()));
            break;
         case 4:
            pills.push_back(new ResearchPill(point, getIsPaused(), getUpSGAmmo()));
         
         
      }
     
   
   }
   
   
}

/**************************************************************************
 * GAME :: CREATE ALIENS!
 * Creates different alien randomly
 **************************************************************************/
void Game :: createAliens(Point point)
{
   if(getIsPaused() == false)
   {
      // we only want the scouts to show during the first 2 sectors of when aleins show
      if (currentSector == WHEN_ALIENS_SHOW || currentSector == (WHEN_ALIENS_SHOW + 1))
      {
         for (int i = 0; i < 3; i++)
         {
            
            alienShips.push_back(new ScoutShip(point, getIsPaused()));
            alienCount++;
            
         }
      }
      else // both aliens appeare at sector 5
      {
         int randomAlien = random(1,4);
         
         //int randomAlien = 3;
         
         //add a random generator to get different aliens
         if (randomAlien == 2)
         {
            for (int i = 0; i < 1; i++)
            {
               
               alienShips.push_back(new Saucer(point, getIsPaused()));
               alienCount++;
               
            }
            
         }
         else if (randomAlien == 1)
         {
            for (int i = 0; i < 3; i++)
            {
               
               alienShips.push_back(new ScoutShip(point, getIsPaused()));
               alienCount++;
               
            }
            
         }
         else if (randomAlien == 3)
         {
            for (int i = 0; i < 2; i++)
            {
               
               alienShips.push_back(new Bomber(point, getIsPaused()));
               alienCount++;
               
            }
            
         }

         
         
         
      }
   }
   
}


/**************************************************************************
 * GAME :: IS ON SCREEN
 * Determines if a given point is on the screen.
 **************************************************************************/
bool Game :: isOnScreen(const Point & point)
{
   return (point.getX() >= topLeft.getX() - OFF_SCREEN_BORDER_AMOUNT
           && point.getX() <= bottomRight.getX() + OFF_SCREEN_BORDER_AMOUNT
           && point.getY() >= bottomRight.getY() - OFF_SCREEN_BORDER_AMOUNT
           && point.getY() <= topLeft.getY() + OFF_SCREEN_BORDER_AMOUNT);
}


/**************************************************************************
 * GAME :: HANDLE COLLISIOn
 * Check for a collision between an asteroid and your ships bullet.
 **************************************************************************/
void Game :: handleCollisions()
{
   int hitRadious = 14;
   
   
   for (list <Bullet*> :: iterator it = bullets.begin();
        it != bullets.end();
        ++it)
      
   {
      for (list <Rock*> :: iterator itA = asteroids.begin();
           itA != asteroids.end();
           ++itA)
      {
         
         
         if ((*it)->isAlive())
         {
            // this bullet is alive, see if its too close
            
            // check if the asteroid is at this point (in case it was hit)
            if ((*itA) != NULL && (*itA)->isAlive())
            {
               
               //getting the correct hitRadius
               int checkType = (*itA)->getType();
               {
                  switch(checkType)
                  {
                     case 1:
                        hitRadious = 35; // 35
                        break;
                     case 2:
                        hitRadious = 25; // 25
                        break;
                     case 3:
                        hitRadious = 20; //20
                        break;
                     case 4:
                        hitRadious = 0;
                        break;
                     case 5:
                        hitRadious = 20; // 20
                        break;
                     case 6:
                        hitRadious = 0;

                  }
                  
                  int checkHit = getClosestDistance((FlyingObject)(*it), (FlyingObject)(*itA));
                  
                  if (fabs((*it)->getPoint().getX() - (*itA)->getPoint().getX()) < hitRadious
                      && fabs((*it)->getPoint().getY() - (*itA)->getPoint().getY()) < hitRadious)
                  {
                     if (checkHit == 0)
                     {
                        
                       
                        //we have a hit!
                        drawExplotion((*it)->getPoint(), 10.0, 0); //makes an explostion wh en you hit
                        // hit the asteroid
                        numAsteroidsKilled++;
                        
                        
                        int type = (*itA)->getType();
                        int num = 0;
                        if(deepScannerUp == false)
                        {
                           num = random(0, CHANCE_OF_PILL);
                        }
                        if(deepScannerUp == true)
                        {
                           num = random(0, (CHANCE_OF_PILL - 2));
                          
                        }

                        
                        if (type == 3 && num == 1)
                        {
                           //make a random pill
                           createPills((*itA)->getPoint());
                        }
                        
                        
                        Point point = (*itA)->getPoint();
                        createAsteroid(type, point);
                        if ((*itA)->getType() == 5)
                        {
                           createAlienAsteroids(6, point);
                        }
                        
                        (*itA)->setAlive(false);
                        if ((*itA)->isAlive() == false)
                           asteroidsTotal--;
                        if ((*it)->getType() == 1)
                        {
                           (*it)->kill(); // kill the bullet if its not a laser
                        }
                     }
                  }
                  
                  
               }
            }
         }
         
         
         
      }
   }
 //** getting the hitRadius

// if bullet is alive



} // for bullets

/**************************************************************************
 * GAME :: HANDLE Alien bullet to ship COLLISIONS
 * Check for a collision between an alien bullet and your ship .
 **************************************************************************/
void Game :: handleAlienBulletShipCollision()
{
   int hitRadious = 25;
  
   
   if (ships[0]->isAlive())
   {
      for (list <AlienBullet*> :: iterator itA = alienBullets.begin();
           itA != alienBullets.end();
           ++itA)
      {
         
         
         // check if the asteroid is at this point (in case it was hit)
         if ((*itA) != NULL && (*itA)->isAlive())
         {
            //cout << "isAlive? = "<< (*itA)->isAlive() << endl;

            int checkType = (*itA)->getType();
         
         
           
            
            if (fabs(ships[0]->getPoint().getX() - (*itA)->getPoint().getX()) < hitRadious
                && fabs(ships[0]->getPoint().getY() - (*itA)->getPoint().getY()) < hitRadious)
            {
                  
                  
               

               float stg = (getShields() / 100.0) * 2.0; // dims the shield barrier
               drawSheilds(ships[0]->getPoint(), hitRadious, 0.0, 1.0, 1.0, stg); //shows shields when you hit

               drawExplotion((*itA)->getPoint(), hitRadious - 15, 0); //makes an explostion wh en you hit
                  
               // *** shield drain
               if (checkType == 1)
               {
                  shields -= SAUCER_DAMAGE;
               }
               
               if (checkType == 2)
               {
                  shields -= SCOUT_DAMAGE;
               }
               
               if (checkType == 3)
               {
                  
                  shields -= BOMB_DAMAGE;
                  (*itA)->setTimeLeft(0);

                  //cout << "isAlive? = " << (*itA)->isAlive() << endl;

               }
               
               if (checkType == 4)
               {
                  
                  shields -= SHRAP_DAMAGE;
                  (*itA)->setTimeLeft(0);
                  
                  //cout << "isAlive? = " << (*itA)->isAlive() << endl;
                  
               }

               
               // if the ship is dead, make the debris
               if (shields <= 0)
               {
                  drawExplotion(ships[0]->getPoint(), 25.0, 0); //makes an explostion wh en you hit
                  createDebris(ships[0]->getPoint());
               }
              
               
              (*itA)->kill();
               //assert((*itA)->isAlive());
             
               
               
               
               //** getting the hitRadius
            }
            
          //  cout << "isAlive? = " << (*itA)->isAlive() << endl;
            if((checkType == 3 && (*itA)->getTimeLeft() == 0) || (checkType == 3 && !(*itA)->isAlive()))
            {
              
               for(int i = 0; i < NUM_OF_SHRAP; i++)
               {
                  alienBullets.push_back(new BombShrapnel((*itA)->FlyingObject::getPoint(), random(0, 360)));
               }
               (*itA)->kill();

               
            }

            
            
         } // if bullet is alive
       
         
      }
      
   }
   

} // for bullets

/**************************************************************************
 * GAME :: HANDLE Alien bullet asteroid colliosion
 * Check for a collision between an alien bullet and an asteroid
 **************************************************************************/
void Game :: handleAlienBulletRockCollision()
{
   int hitRadious = 14;
   
   for (list <AlienBullet*> :: iterator it = alienBullets.begin();
        it != alienBullets.end();
        ++it)
      
   {
      for (list <Rock*> :: iterator itA = asteroids.begin();
           itA != asteroids.end();
           ++itA)
      {
         
         
         if ((*it)->isAlive())
         {
            
            }
            // this bullet is alive, see if its too close
            
            // check if the asteroid is at this point (in case it was hit)
            if ((*itA) != NULL && (*itA)->isAlive())
            {
               
               //getting the correct hitRadius
                int bulletType = (*it)->getType(); // must be the Saucer bullet
               int RockType = (*itA)->getType();
               {
                  switch(RockType)
                  {
                     case 1:
                        hitRadious = 35;
                        break;
                     case 2:
                        hitRadious = 25;
                        break;
                     case 3:
                        hitRadious = 15;
                        break;
                     case 4:
                        hitRadious = 0;
                        break;
                        
                  }
                  
                  // BTW, this logic could be more sophisiticated, but this will
                  // get the job done for now...
                  
                  if (fabs((*it)->getPoint().getX() - (*itA)->getPoint().getX()) < hitRadious
                      && fabs((*it)->getPoint().getY() - (*itA)->getPoint().getY()) < hitRadious)
                  {
                     if ( RockType == 1 && bulletType == 1) // only effeced by Assimliator
                     {
                        //we have a hit!
                        drawExplotion((*it)->getPoint(), 10.0, 0); //makes an explostion wh en you hit
                        // hit the asteroid
                        
                        int type = (*it)->getType();
                        Point point = (*itA)->getPoint();
                        createAsteroid(type, point);
                        createAlienAsteroids(5, point);
                        
                        (*itA)->setAlive(false);
                        if ((*itA)->isAlive() == false)
                           asteroidsTotal--;
                        (*it)->kill();
                     }
                  }
                  
                  
               }
            }
         }
         
         
         
      }
   }
   //** getting the hitRadius
   
   // if bullet is alive
   


 // for bullets




/**************************************************************************
 * GAME :: HANDLE Alien Bullet COLLISIONS
 * Check for a collision between your bullet and an alien.
 **************************************************************************/
void Game :: handleAlienCollision()
{
   int hitRadious = 14;
   int div = 0; // shield drop rate
   int numOfRS = 0; //number of research points that spawn when alien dies

   
   for (list <Bullet*> :: iterator it = bullets.begin();
        it != bullets.end();
        ++it)
      
   {
      for (list <AlienShip*> :: iterator itAlien = alienShips.begin(); // comparing aliens to the bullets
           itAlien != alienShips.end();
           ++itAlien)
      {
         
         if ((*it)->isAlive())
         {
            //Alien collion
            if ((*itAlien) != NULL && (*itAlien)->isAlive())
            {
               int checkType = (*itAlien)->getType();
               {
                  switch(checkType)
                  {
                     case 1:
                        hitRadious = 30; // 25
                        div = 25;
                        break;
                     case 2:
                        hitRadious = 60; // 55
                        div = 200;
                        break;
                     case 3:
                        hitRadious = 60; // 55
                        div = 100;
                        break;

                        
                  }
                  
                  if (fabs((*it)->getPoint().getX() - (*itAlien)->getPoint().getX()) < hitRadious
                      && fabs((*it)->getPoint().getY() - (*itAlien)->getPoint().getY()) < hitRadious)
                  {
                     //we have a hit!
                     float stg = (((*itAlien)->getHitPoints() / div)); // dims the shield barrier
                     drawAlienSheilds((*itAlien)->getPoint(), hitRadious + 5, 0.25, 1.0, 1.0, stg); //shows shields when you hit
                     drawAlienSheilds((*itAlien)->getPoint(), hitRadious + 6, 0.25, 1.0, 1.0, stg); //shows shields when you hit

                     
                     drawExplotion((*it)->getPoint(), 10.0, 0); //makes an explostion wh en you hit
                     int type = (*it)->getType();
                     
                     // hit the alien
                     (*itAlien)->hit(type, getLaserDamageUp());
                     (*itAlien)->incSpeed();
                    
                     if ((*itAlien)->getHitPoints() <= 0)
                     {
                       
                        drawExplotion((*itAlien)->getPoint(), hitRadious + 10, 0); //makes an explostion wh en you hit
                        createAlienDebris((*itAlien)->getPoint());
                        
                        if ((*itAlien)->getType() == 1)
                        {
                           numOfRS = 1;
                        }
                        else if ((*itAlien)->getType() == 2)
                        {
                           numOfRS = 3;
                        }
                        else if ((*itAlien)->getType() == 3)
                        {
                           numOfRS = 2;
                        }
                        
                        for (int i = 0; i < numOfRS; i++)
                        {
                           int randInt = random(1, CHANCE_OF_RS_DROP);
                           
                           if (randInt == 1)
                           {
                           
                              pills.push_back(new ResearchPill((*itAlien)->getPoint(), getIsPaused(), getUpSGAmmo()));
                           }
                        }

                        
                        if (deathAnimation >= 0)
                        {
                           alienCount--;
                           numAlienKilled++;

                           (*itAlien)->kill();
                        }
                     }
                     if ((*it)->getType() == 1)
                     {
                        (*it)->kill(); // kill the bullet if its not a laser
                     }
                  }
               }
            }
            
            
            
         }
      }
   } //** getting the hitRadius
   
   // if bullet is alive
   
   
   
} // for bullets



/**************************************************************************
 * GAME :: SHIP COLLISIONS
 * Check for a collision between an asteroid and the ship.
 **************************************************************************/
void Game :: shipCollisions()
{
   
   
      int hitRadious = 14;
      
      if (ships[0]->isAlive())
      {
         for (list <Rock*> :: iterator itA = asteroids.begin();
              itA != asteroids.end();
              ++itA)
         {
            
            // check if the asteroid is at this point (in case it was hit)
            if ((*itA) != NULL && (*itA)->isAlive())
            {
               
               //getting the correct hitRadius for each asteroid
               int checkType = (*itA)->getType();
               {
                  switch(checkType)
                  {
                     case 1:
                        hitRadious = 35; // 35
                        break;
                     case 2:
                        hitRadious = 25; // 25
                        break;
                     case 3:
                        hitRadious = 20; // 20
                        break;
                     case 4:
                        hitRadious = 0; // 0
                        break;
                     case 5:
                        hitRadious = 20; // 20
                        break;
                     case 6:
                        hitRadious = 0; // 0
                        break;
                        
                  }
                  
                  
               }
               
               
               int checkHit = getClosestDistance((FlyingObject)(ships[0]), (FlyingObject)(*itA));
               
               if (fabs(ships[0]->getPoint().getX() - (*itA)->getPoint().getX()) < hitRadious
                   && fabs(ships[0]->getPoint().getY() - (*itA)->getPoint().getY()) < hitRadious)
               {
                 
                 
                  
                  if (checkHit == 0)
                  {
                     
                     float stg = (getShields() / 100.0) * 2.0; // dims the shield barrier
                     
              
                     if (invinceTime < 0)
                     {
                        drawSheilds(ships[0]->getPoint(), 20, 0.0, 1.0, 1.0, stg); //shows shields when you hit
                        // *** shield drain
                        if (checkType == 1)
                        {
                           shields -= BIG_ASTRO_DAMG;
                           astroHit++;
                        }
                        if (checkType == 2)
                        {
                           shields -= MED_ASTRO_DAMG;
                           astroHit++;
                        }
                        if (checkType == 3)
                        {
                           shields -= SMALL_ASTRO_DAMG;
                           astroHit++;
                        }
                        if (checkType == 5)
                        {
                           shields -= ALIEN_ASTRO_DAMG;
                           astroHit++;
                        }
                     }
                     // if the ship is dead, make the debris
                     if (shields <= 0)
                     {
                        drawExplotion(ships[0]->getPoint(), 45.0, 0); //makes an explostion wh en you hit
                        createDebris(ships[0]->getPoint());
                     }
                     
                     int type = (*itA)->getType();
                     Point point = (*itA)->getPoint();
                     createAsteroid(type, point);
                     if ((*itA)->getType() == 5)
                     {
                        createAlienAsteroids(6, point);
                     }
                     
                     (*itA)->setAlive(false); // astro is dead
                     
                     if ((*itA)->isAlive() == false) // lower the count of asteros
                        asteroidsTotal--;
                  }
               }
            }
            //** getting the hitRadius
         }
      } // if asteroid is alive
      
   
}

/**************************************************************************
 * GAME :: PILL COLLISIONS
 * Check for a collision between an asteroid and a BULLET.
 **************************************************************************/
void Game :: pillCollisions(int &theResearchPoints)
{
   int hitRadious = 0;
   
   if(getPickupRangeUp() == false)
   {
      hitRadious = 30; //25
   }
   
   if(getPickupRangeUp() == true)
   {
      hitRadious = PICKUP_RANGE_UP; // 100
   }
   
   
   
   if (ships[0]->isAlive())
   {
     
      for (list <Pills*> :: iterator it = pills.begin();
           it != pills.end();
           ++it)
         
      {
         
         if ((*it)->isAlive())
         {
            // this pill is alive, see if its too close
            
            // check if the pill is at this point (in case it was hit)
            if ((*it) != NULL && (*it)->isAlive())
            {
               
               
               
               
               if (fabs(ships[0]->getPoint().getX() - (*it)->getPoint().getX()) < hitRadious
                   && fabs(ships[0]->getPoint().getY() - (*it)->getPoint().getY()) < hitRadious)
               {
                  
                  //we have a hit!h
                  pillsCollected++;
                  
                  //bonus effects depending on type of pill
                  int pillType = (*it)->getType();
                  switch (pillType)
                  {
                        //shield pill adds shields
                     case 1:
                        if (shields <= 100 && getUpShieldBat1() == false)
                        {
                           shields += SHIELD_PILL;
                           drawSheilds(ships[0]->getPoint(), 20.0, 0.0,
                                       1.0, 0.0, 1.0); //shows shields when you hit
                        }
                        else if (shields <= 150 && getUpShieldBat1() == true && getUpShieldBat2() == false)
                        {
                           shields += SHIELD_PILL;
                           drawSheilds(ships[0]->getPoint(), 20.0, 0.0,
                                       1.0, 0.0, 1.0); //shows shields when you hit
                        }
                        else if (shields <= 200 && getUpShieldBat1() == true && getUpShieldBat2() == true)
                        {
                           shields += SHIELD_PILL;
                           drawSheilds(ships[0]->getPoint(), 20.0, 0.0,
                                       1.0, 0.0, 1.0); //shows shields when you hit
                        }


                        break;
                        
                        // shotgun pill adds ammo
                     case 2:
                        if(getUpSGAmmo() == false)
                        {
                        shotGunAmmo += SHOT_GUN_AMMO;
                        drawSheilds(ships[0]->getPoint(), 20.0, 1.0,
                                    0.50, 0.0, 1.0); //shows shields when you hit
                        }
                        else if (getUpSGAmmo() == true)
                        {
                           shotGunAmmo += SHOT_GUN_AMMO + SG_AMMO_UP;
                           drawSheilds(ships[0]->getPoint(), 20.0, 1.0,
                                       0.50, 0.0, 1.0); //shows shields when you hit
                        }
                        break;
                     case 3:
                        LaserAmmo += LASER_AMMO;
                        drawSheilds(ships[0]->getPoint(), 20.0, 1.0,
                                    0.0, 0.0, 1.0); //shows shields when you hit
                        break;
                     case 4:
                        theResearchPoints++;
                        drawSheilds(ships[0]->getPoint(), 20.0, 0.0,
                                    1.0, 0.0, 1.0); //shows shields when you hit
                        
                  
                  }
                  
                  (*it)->setAlive(false);
               }
               
              
               
               
            } //** getting the hitRadius
         }
      } // if pill is alive
      
      
   }
   
   
}


/**************************************************************************
 * GAME :: Delete the dead
 * Remove any dead objects (take bullets out of the list, deallocate bird)
 **************************************************************************/
void Game :: deleteTheDead()
{
   
   // check for dead asteroids
   
      
      list<Rock*>::iterator astroIt = asteroids.begin();
      while (astroIt != asteroids.end())
      {
         Rock pAsteroid = **astroIt;
         
         if (!pAsteroid.isAlive())
         {
            
            // remove from list and advance

            
            astroIt = asteroids.erase(astroIt);
         }
         else
         {
            astroIt++; // advance
         }
      }

  
   
   
   
   
      // check for dead aliens
  
  // delete after the death aninmation has played
   if (deathAnimation <= 0)
   {
      list<AlienShip*>::iterator AlienIt = alienShips.begin();
      while (AlienIt != alienShips.end())
      {
         Ship pAlien = **AlienIt;
         
         if (!pAlien.isAlive())
         {
            
            // remove from list and advance
         
            AlienIt = alienShips.erase(AlienIt);
            
         }
         else
         {
            AlienIt++; // advance
         }
      }
   }
   
   
   
   
   

   
   /************************
    * checking for dead bullets
    ************************/
   // Look for dead bullets
   list<Bullet*>::iterator bulletIt = bullets.begin();
   while (bulletIt != bullets.end())
   {
      Bullet bullet = **bulletIt;
      
      if (!bullet.isAlive())
      {
         // remove from list and advance
       
         bulletIt = bullets.erase(bulletIt);
      }
      else
      {
         bulletIt++; // advance
      }
   }
   
//   /************************
//    * checking for dead Alien bullets
//    ************************/
//   // Look for dead bullets
   list<AlienBullet*>::iterator AbulletIt = alienBullets.begin();
   while (AbulletIt != alienBullets.end())
   {
      AlienBullet alienBullet = **AbulletIt;
      
      if (!alienBullet.isAlive())
      {
         // remove from list and advance
         AbulletIt = alienBullets.erase(AbulletIt);
         //cout << "hello" << endl;
        
      }
      else
      {
         AbulletIt++; // advance
      }
   }
   
   
}

/***************************************
 * GAME :: HANDLE INPUT
 * accept input from the user
 ***************************************/
void Game :: handleInput(const Interface & ui)
{
   if((ui.isP() || ui.isEsc()) && getIsPaused() == false)
   {
      
      setIsPaused(true);
   }
   else if ((ui.isP() || ui.isEsc()) && getIsPaused() == true)
   {
      setIsPaused(false);
      
   }
   
   /************************
    * Ship Controls *
    ************************/
   if(showGreetTime <= 0)
   {
      if (getIsPaused() == false)
      {
         
         
         
         
         if (ui.isLeft())
         {
            ships[0]->applyThrustLeft(getCheckRotateSpeed());
         }
         
         if (ui.isDown())
         {
            if(getCheckIsInverted() == false)
            {
               ships[0]->applyThrustTop(getCheckAccelSpeed());
            }
            else if(getCheckIsInverted() == true)
            {
               ships[0]->setisThrusting(true);
               ships[0]->applyThrustBottom(getCheckAccelSpeed());
            }
         }
         else
            ships[0]->setisThrusting(false);
         
         if (ui.isUp())
         {
            if(getCheckIsInverted() == false)
            {
               ships[0]->setisThrusting(true);
               ships[0]->applyThrustBottom(getCheckAccelSpeed());
            }
            else if(getCheckIsInverted() == true)
            {
               ships[0]->applyThrustTop(getCheckAccelSpeed());
            }
         }
         
         
         
         if (ui.isRight())
         {
            ships[0]->applyThrustRight(getCheckRotateSpeed());
         }
         
         
         
         // Check for "Spacebar"
         if(getUpPlasmaHeatSink() == false)
         {
            if (ui.isSpace() && currentWeapon == 1)
            {
               numShotsFired++;
               
               Bullet * newBullet = new Bullet(getupPlasmaRange());
               newBullet->fire(ships[0]->getPoint(), ships[0]->getAngle(), ships[0]->getVelocity(), getShotGunUp());
               
               
               bullets.push_back(newBullet);// adds a "newBullet" to the vector
            }
         }
         if (getUpPlasmaHeatSink() == true)
         {
            if (ui.isSpaceHold() && currentWeapon == 1 && plasmaCannonOverHeat > 0)
            {
               numShotsFired++;
               
               Bullet * newBullet = new Bullet(getupPlasmaRange());
               newBullet->fire(ships[0]->getPoint(), ships[0]->getAngle(), ships[0]->getVelocity(), getShotGunUp());
               
               
               
               bullets.push_back(newBullet);// adds a "newBullet" to the vector
               plasmaCannonOverHeat -= PLASMA_DISCHARGE_RATE;
               if (plasmaCannonOverHeat < 0)
               {
                  plasmaCannonOverHeat = 0;
               }
               
               red2 += 0.05;
               green2 -= 0.05;
               
               if(red2 > 1.0)
                  red2 = 1.0;
               if(green2 < 0)
               {
                  green2 = 0;
               }
               
               
            }
            if (plasmaCannonOverHeat < PLASMA_CANNON_OVERHEAT && !ui.isSpaceHold())
            {
               plasmaCannonOverHeat++;
               
               red2 -= 0.05;
               green2 += 0.01;
               
               if(red2 < 0)
                  red2 = 0.0;
               if(green2 > 1.0)
               {
                  green2 = 1.0;
               }


            }
            //cout << plasmaCannonOverHeat << endl;
         }
         
         
         
         if (getShotGunUp() == false)
         {
            
            if (ui.isSpace() && currentWeapon == 2)
            {
               //only shoot if you have ammo
               if (shotGunAmmo > 0)
               {
                  numShotsFired++;
                  
                  // shooting shot gun
                  for (int i = 0; i < (NUM_OF_BBS - 1); i++)
                  {
                     
                     Bullet * newBullet = new ShotGun ();
                     newBullet->fire(ships[0]->getPoint(), ships[0]->getAngle(), ships[0]->getVelocity(), getShotGunUp());
                     
                     
                     bullets.push_back(newBullet);// adds a "newBullet" to the vector
                     
                  }
                  shotGunAmmo--;
                  if (shotGunAmmo <= 0)
                  {
                     currentWeapon = 1;
                  }
                  
               }
               
            }
         }
         
         if (getShotGunUp() == true)
         {
            
            if (ui.isSpace() && currentWeapon == 2)
            {
               //only shoot if you have ammo
               if (shotGunAmmo >= SHOT_GUN_UP_CONSUMPSTION)
               {
                  numShotsFired++;
                  
                  // shooting shot gun
                  for (int i = 0; i < (NUM_OF_BBS + NUM_OF_BBS + NUM_OF_BBS); i++)
                  {
                     
                     Bullet * newBullet = new ShotGun ();
                     newBullet->fire(ships[0]->getPoint(), ships[0]->getAngle(), ships[0]->getVelocity(), getShotGunUp());
                     
                     
                     bullets.push_back(newBullet);// adds a "newBullet" to the vector
                     
                  }
                  shotGunAmmo -= SHOT_GUN_UP_CONSUMPSTION;
                  if (shotGunAmmo < SHOT_GUN_UP_CONSUMPSTION)
                  {
                     currentWeapon = 1;
                  }
                  
               }
               
            }
         }

         
         if (laserDamageUp == false)
         {
            
            // the laser
            if (ui.isSpace() && currentWeapon == 3)
            {
               if (LaserAmmo > 0)
               {
                  numShotsFired++;
                  Bullet * newBullet = new Laser(ships[0]->getAngle());
                  newBullet->fire(ships[0]->getPoint(), ships[0]->getAngle(), ships[0]->getVelocity(), getShotGunUp());
                  bullets.push_back(newBullet);
                  LaserAmmo--;
                  if (LaserAmmo <= 0)
                  {
                     currentWeapon = 1;
                  }
                  
               }
               
            }
         }
         
         if (laserDamageUp == true)
         {
            
            // the laser
            if (ui.isSpace() && currentWeapon == 3)
            {
               if (LaserAmmo >= LASER_DAMAGE_UP_CONSUMPTION)
               {
                  numShotsFired++;
                  Bullet * newBullet = new Laser(ships[0]->getAngle());
                  newBullet->fire(ships[0]->getPoint(), ships[0]->getAngle(), ships[0]->getVelocity(), getShotGunUp());
                  bullets.push_back(newBullet);
                  LaserAmmo -= LASER_DAMAGE_UP_CONSUMPTION;
                  if (LaserAmmo < LASER_DAMAGE_UP_CONSUMPTION)
                  {
                     currentWeapon = 1;
                  }
                  
               }
               
            }
         }

      
      //cant change if game is paused or if level is starting
      
//         if (ui.isC())
//         {
//            if (currentWeapon == 1)
//            {
//               currentWeapon = 2;
//            }
//            else if (currentWeapon == 2)
//            {
//               currentWeapon = 3;
//            }
//            else if (currentWeapon == 3)
//            {
//               currentWeapon = 1;
//            }
//         
//            
//            
//         }
      }
   }
   
   if (ui.isC())
   {
      if (currentWeapon == 1)
      {
         currentWeapon = 2;
      }
      else if (currentWeapon == 2)
      {
         currentWeapon = 3;
      }
      else if (currentWeapon == 3)
      {
         currentWeapon = 1;
      }
      
      
      
   }

   
}

//stub


/*********************************************
 * GAME :: DRAW
 * Drawing everything on the screen
 *********************************************/
void Game :: draw(const Interface & ui)
{
   
   /*********************************************
    * This is possibly the right way
    *********************************************/
   //make it so that objs dont apeare in the UI
   Point ptTR(-600, -490); // top right
   Point ptTL(100, -490); // top left
   Point ptBR(-100, -600); // bottom right
   Point ptBL(-600, -600); // bottem left
   
   if(getIsPaused() == true)
   {
      drawTitle(Point(-38,30), "Paused", 1.0, 1.0, 0.0, 1.0);
   }
   
   
   if (asteroid != NULL)
   {
      // draw the asteroids, if they are alive
      for (list <Rock*> :: iterator it = asteroids.begin();
           it != asteroids.end();
           ++it)
      {
         if ((*it)->isAlive())
         {
            (*it)->draw(getIsPaused());
         }
      }
   }
   
   // drawing the pills!
   for (list <Pills*> :: iterator it = pills.begin();
        it != pills.end();
        ++it)
   {
      if ((*it)->isAlive())
      {
         (*it)->draw(getIsPaused(), getUpSGAmmo());
      }
   }
   
   // drawing the ALIENS!
   for (list <AlienShip*> :: iterator it = alienShips.begin();
        it != alienShips.end();
        ++it)
   {
      if ((*it)->isAlive())
      {
         (*it)->drawShip(getIsPaused());
      }
      else
      {
         for (list <Rock*> :: iterator it = alienDebris.begin(); // drawing death animation
              it != alienDebris.end();
              ++it)
         {
            if (deathAnimation >= 0)
            {
               (*it)->draw(getIsPaused());
               
               if(getIsPaused() == false)
               {
                  deathAnimation--;
               }
            }
            
         }
      }
   }
  


   // drawing the ALIENS BULLETS!
   for (list <AlienBullet*> :: iterator it = alienBullets.begin();
        it != alienBullets.end();
        ++it)
   {
      if ((*it)->isAlive())
      {
         (*it)->draw();
         
      }
   }
   







   // draw the bullets, if the ship is alive
   if (ships[0]->isAlive())
   {
      for (list <Bullet*> :: iterator it = bullets.begin();
           it != bullets.end();
           ++it)
      {
         if ((*it)->isAlive())
         {
            
            (*it)->draw(getLaserDamageUp());
         }
      }
   }
   
   
   
   
   //Drawing our stars
   for (int i = 0; i < stars.size(); i++)
   {
      stars[i].draw();
   }
   
   
   //draw the ship if its alive
   if (ships[0]->isAlive())
   {
      ships[0]->draw(getIsPaused(), getUpShieldBat1(), getUpShieldBat2(), getupPlasmaRange(), getUpPlasmaHeatSink(), getShotGunUp(), getLaserShotUp(),getLaserDamageUp(), getDeepScannerUp());
   }
   
   //drawing the debris
   if (!ships[0]->isAlive())
   {
      for (list <Rock*> :: iterator it = debris.begin();
           it != debris.end();
           ++it)
      {
         if (deathAnimation >= 0)
         {
            (*it)->draw(getIsPaused());
            if(getIsPaused() == false)
            {
               deathAnimation--;
            }
           
         }
      }
   }
   
   //drawing the WarpField
      for (list <Rock*> :: iterator it = warpField.begin();
           it != warpField.end();
           ++it)
      {
         if (warpAnimate >= 0)
         {
            (*it)->draw(getIsPaused());
            if(getIsPaused() == false)
            {
               warpAnimate--;
            }
         }
        
         
      }

   
   
   // clearing lists
   if (warpAnimate <= 0)
   {
      advTime = 60;
      warpField.clear();
      warpAnimate = WARP_ANIMATION_LEN;
   }
   
   if (deathAnimation <= 0)
   {
      alienDebris.clear();
   }
   
   

   
   
   /*********************************************
    * Game Interface UI
    *
    *********************************************/
   
   // *** UI BOX ***
   //left box
   drawLine(Point(-600, -490), Point(-600, -600), 0.0, 1.0, 0.0); // left
   drawLine(Point(-590, -480), Point(-600, -490), 0.0, 1.0, 0.0); // left diagnal line
   drawLine(Point(-590, -480), Point(-90, -480), 0.0, 1.0, 0.0); // far top
   drawLine(Point(-600, -490), Point(-100, -490), 0.0, 1.0, 0.0); // top
   drawLine(Point(-600, -599.9), Point(-100, -599.9), 0.0, 1.0, 0.0); // bottem
   drawLine(Point(-100, -599.9), Point(-90, -590), 0.0, 1.0, 0.0); // bottem right diagnal line
   drawLine(Point(-100, -490), Point(-100, -600), 0.0, 1.0, 0.0); // right
   drawLine(Point(-90, -480), Point(-90, -590), 0.0, 1.0, 0.0); // far right
   drawLine(Point(-100, -490), Point(-90, -480), 0.0, 1.0, 0.0); // right diagnal line
   drawLine(Point(-350, -490), Point(-350, -600), 0.0, 1.0, 0.0); // middle line
   drawLine(Point(-350, -490), Point(-340, -480), 0.0, 1.0, 0.0); // middle line
   
   //right box
   drawLine(Point(600, -490), Point(600, -600), 0.0, 1.0, 0.0); // right
  
   drawLine(Point(600, -490), Point(100, -490), 0.0, 1.0, 0.0); // top
   drawLine(Point(90, -480), Point(590, -480), 0.0, 1.0, 0.0); //  upper top
   drawLine(Point(600, -490), Point(590, -480), 0.0, 1.0, 0.0); // top right diagaonal
   drawLine(Point(600, -599.9), Point(100, -599.9), 0.0, 1.0, 0.0); // bottem
   drawLine(Point(100, -490), Point(100, -600), 0.0, 1.0, 0.0); // left
   drawLine(Point(90, -590), Point(90, -480), 0.0, 1.0, 0.0); // far right
   drawLine(Point(90, -590), Point(100, -600), 0.0, 1.0, 0.0); // bottem left diagaonal
    drawLine(Point(90, -480), Point(100, -490), 0.0, 1.0, 0.0); // top left diagaonal
   
   drawLine(Point(350, -490), Point(350, -600), 0.0, 1.0, 0.0); // middle
   drawLine(Point(350, -490), Point(340, -480), 0.0, 1.0, 0.0); // middle diagaonal
   
   
   
   
   
   //*** THE SHIELDS ***
   
   
   // dynamic positioning of shield number and percent sign
   int posNum = -502;
   int posPercent = -469;
   
   if (shields < 100)
   {
      posPercent = -471;
      posNum = -495;
   }
   if (shields < 10)
   {
      posPercent = -473;
      posNum = -487;
   }
   
   
   // Put the shields on the screen
   Point shieldsLocation;
   
   
   shieldsLocation.setX(topLeft.getX() + 0);
   shieldsLocation.setY(topLeft.getY() - 0);
   
   
  
   float red = 0.0;
   float gre = 1.0;
   float blue = 0.0;
   
   float red1 = 0.0;
   float gre1 = 1.0;
   float blue1 = 0.0;
   
   if (shields > 100)
   {
      red1 = 0.0;
      gre1 = 1.0;
      blue1 = 1.0;
   }

   if (shields <= 50)
   {
      red = 1.0;
      gre = 1.0;
      blue = 0.0;
      
      red1 = 1.0;
      gre1 = 1.0;
      blue1 = 0.0;
   }
   if (shields <= 25)
   {
      
      red = 1.0;
      gre = 0.0;
      blue = 0.0;
      
      red1 = 1.0;
      gre1 = 0.0;
      blue1 = 0.0;
      int warningAnimate = random(-1.0 , 2.0);
      if(getIsPaused() == true)
      {
         drawTextColor(Point(-525, -470), "WARNING!", red, 0.0, 0.0, 1.0);
      }
     
      drawTextColor(Point(-525, -470), "WARNING!", red, 0.0, 0.0, warningAnimate);
   }
   if(shields > 100)
   {
      drawTextColor(Point(-545, -470), "OVERCHARGED", 0.0, 1.0, 1.0, 1.0);

   }
   
   
   //drawing sheild number
   if (shields < 0)
      shields = 0;
   drawNumber(Point(posNum, -540), shields);
   
   drawTextColor(Point(-520, -520), "SHIELDS", red1, gre1, blue1, 1.0);
   drawLine(Point(-523, -525), Point(-439, -525), 0.0, 0.0, 1.0); // blue line under shields
   drawLine(Point(-523, -526), Point(-439, -526), 0.0, 0.0, 1.0);
   drawLine(Point(-523, -527), Point(-439, -527), 0.0, 0.0, 1.0);
   drawTextColor(Point(posPercent, -551), "%", 1.0, 1.0, 1.0, 1.0); // make this dynamic
   
   
   // the "shield bar"
   if (shields <= 100)
   {
      int len = 590 + -shields * 2.3; //rate of decrease
      int set = 590; // spot from the far left
      //drawLine(Point(-590, -580), Point(-360, -580), red, gre, blue);
      drawLine(Point(-set, -580), Point(-len, -580), red, gre, blue);
      drawLine(Point(-set, -579), Point(-len, -579), red, gre, blue);
      drawLine(Point(-set, -578), Point(-len, -578), red, gre, blue);
      drawLine(Point(-set, -577), Point(-len, -577), red, gre, blue);
      drawLine(Point(-set, -576), Point(-len, -576), red, gre, blue);
      drawLine(Point(-set, -575), Point(-len, -575), red, gre, blue);
      drawLine(Point(-set, -574), Point(-len, -574), red, gre, blue);
      drawLine(Point(-set, -573), Point(-len, -573), red, gre, blue);
   }
   
  
   

   // shield bat 1
   if(shields > 100)
   {
     // len = 590;
      int len1 = (590 + -shields * 2.3) + 225; //rate of increase
      int set1 = 590; // spot from the far left
      
      int set = 590;
      int len = 360;

      //green bar
      drawLine(Point(-set, -580), Point(-len, -580), red, gre, blue);
      drawLine(Point(-set, -579), Point(-len, -579), red, gre, blue);
      drawLine(Point(-set, -578), Point(-len, -578), red, gre, blue);
      drawLine(Point(-set, -577), Point(-len, -577), red, gre, blue);
      drawLine(Point(-set, -576), Point(-len, -576), red, gre, blue);
      drawLine(Point(-set, -575), Point(-len, -575), red, gre, blue);
      drawLine(Point(-set, -574), Point(-len, -574), red, gre, blue);
      drawLine(Point(-set, -573), Point(-len, -573), red, gre, blue);
      

      
      
      //blue bar
      drawLine(Point(-set1, -580), Point(-len1, -580), 0.0, 1.0, 1.0);
      drawLine(Point(-set1, -579), Point(-len1, -579), 0.0, 1.0, 1.0);
      drawLine(Point(-set1, -578), Point(-len1, -578), 0.0, 1.0, 1.0);
      drawLine(Point(-set1, -577), Point(-len1, -577), 0.0, 1.0, 1.0);
      drawLine(Point(-set1, -576), Point(-len1, -576), 0.0, 1.0, 1.0);
      drawLine(Point(-set1, -575), Point(-len1, -575), 0.0, 1.0, 1.0);
      drawLine(Point(-set1, -574), Point(-len1, -574), 0.0, 1.0, 1.0);
      drawLine(Point(-set1, -573), Point(-len1, -573), 0.0, 1.0, 1.0);
      
      
      
      
      
   }
   
   
   // *** SECTOR DISPLAY ***
   
   int spot = -27;      // location of first number
   int slashSpot = -3;
   int killsToWinSpot = 2; // location of last number
   if (currentSector <= 9)
   {
      spot = -18;
      slashSpot = -3;
      killsToWinSpot = 2;
   }

   drawTextColor(Point(-38, -502), "SECTOR", 0.5, 0.5, 1.0 , 1.0);
   drawTextColor(Point(slashSpot, -550), "/", 0.5, 0.5, 1.0, 1.0);
   
   drawNumber(Point(killsToWinSpot, -538), lastSector);
   drawNumber(Point(spot, -538), currentSector);
   
   int rSpot = 45; // moves the research number if its over 10
   
   if(getResearchPoints() < 10)
   {
      rSpot = 55;
   }
   
   drawTextColorTiny(Point(-65,-585), "Research Points :", 0.0, 1.0, 0.0, 1.0);
   drawNumber(Point(rSpot, -575), getResearchPoints());
   
   // *** ASTEROID COUNTER DISPLAY ***
   
   
   drawTextColor(Point(135, -520), "ASTEROID DENSITY", 0.5, 0.5, 1.0, 1.0);
   drawLine(Point(131, -525), Point(320, -525), 0.0, 0.0, 1.0); // blue line underneath
   drawLine(Point(131, -526), Point(320, -526), 0.0, 0.0, 1.0);
   drawLine(Point(131, -527), Point(320, -527), 0.0, 0.0, 1.0);

   if (asteroidsTotal <= 25)
   {
      drawTextColor(Point(200, -550), "LOW", 0.0, 1.0, 0.0, 1.0);
   }
   if (asteroidsTotal <= 40 && asteroidsTotal > 25)
   {
      drawTextColor(Point(190, -550), "MEDIUM", 1.0, 1.0, 0.0, 1.0);
   }
   if (asteroidsTotal > 40)
   {
      drawTextColor(Point(200, -550), "HIGH", 1.0, 0.0, 0.0, 1.0);
   }
   
   int numLocation = 185; // dynamically move the number
   if (asteroidsTotal >= 10)
   {
      numLocation = 175;
   }
   if (asteroidsTotal >= 100)
   {
      numLocation = 165;
   }

   drawText(Point(200, -580), "REMAINING");
   drawNumber(Point(numLocation, -570), asteroidsTotal);
   
   // *** WEAPONS DISPLAY ***
   
   drawTextColor(Point(-265, -520), "WEAPON", 0.5, 0.5, 1.0, 1.0);
   drawLine(Point(-268, -525), Point(-180, -525), 0.0, 0.0, 1.0); // blue line underneath
   drawLine(Point(-268, -526), Point(-180, -526), 0.0, 0.0, 1.0);
   drawLine(Point(-268, -527), Point(-180, -527), 0.0, 0.0, 1.0);
   
   //drawing pulse cannon
   if (currentWeapon == 1 && getUpPlasmaHeatSink() == false)
   {
      drawTextColorTiny(Point(-280, -545), "PLASMA CANNON", 1.0, 0.0, 1.0, 1.0);
      drawWeapon(Point(-268, -560), 2, 0, 1.0, 0.0, 1.0);
      drawWeapon(Point(-288, -560), 2, 0, 1.0, 0.0, 1.0);
      drawWeapon(Point(-248, -560), 2, 0, 1.0, 0.0, 1.0);
      drawWeapon(Point(-228, -560), 2, 0, 1.0, 0.0, 1.0);
      drawWeapon(Point(-208, -560), 2, 0, 1.0, 0.0, 1.0);
      drawWeapon(Point(-188, -560), 2, 0, 1.0, 0.0, 1.0);
      drawWeapon(Point(-168, -560), 2, 0, 1.0, 0.0, 1.0);
    
      
      drawTextColorTiny(Point(-270, -585), "AMMO :", 1.0, 1.0, 1.0, 1.0);
      drawTextColorTiny(Point(-210, -585), "Inf.", 0.0, 1.0, 0.0, 1.0);
      
      
      
   }
   
   if (currentWeapon == 1 && getUpPlasmaHeatSink() == true)
   {
//      float red2 = 0.0;
//      float green2 = 1.0;
//      float blue2 = 0.0;
      
      int set2 = 275;
      int len2 = 275 + -plasmaCannonOverHeat * 1.65; //110
      
      drawTextColorTiny(Point(-280, -545), "PLASMA CANNON", 1.0, 0.0, 1.0, 1.0);
      drawWeapon(Point(-268, -560), 2, 0, 1.0, 0.0, 1.0);
      drawWeapon(Point(-288, -560), 2, 0, 1.0, 0.0, 1.0);
      drawWeapon(Point(-248, -560), 2, 0, 1.0, 0.0, 1.0);
      drawWeapon(Point(-228, -560), 2, 0, 1.0, 0.0, 1.0);
      drawWeapon(Point(-208, -560), 2, 0, 1.0, 0.0, 1.0);
      drawWeapon(Point(-188, -560), 2, 0, 1.0, 0.0, 1.0);
      drawWeapon(Point(-168, -560), 2, 0, 1.0, 0.0, 1.0);
      
      
      drawTextColorTiny(Point(-340, -580), "Charge =", 1.0, 1.0, 1.0, 1.0);
      
      drawLine(Point(-set2, -580), Point(-len2, -580), red2, green2, blue2);
      drawLine(Point(-set2, -579), Point(-len2, -579), red2, green2, blue2);
      drawLine(Point(-set2, -578), Point(-len2, -578), red2, green2, blue2);
      drawLine(Point(-set2, -577), Point(-len2, -577), red2, green2, blue2);
      drawLine(Point(-set2, -576), Point(-len2, -576), red2, green2, blue2);
      drawLine(Point(-set2, -575), Point(-len2, -575), red2, green2, blue2);
      drawLine(Point(-set2, -574), Point(-len2, -574), red2, green2, blue2);
      drawLine(Point(-set2, -573), Point(-len2, -573), red2, green2, blue2);

      
      
      
      
   }
   
   if (currentWeapon == 2 && getShotGunUp() == false)
   {
      drawTextColorTiny(Point(-290, -545), "BALLISTIC SHOT GUN", 1.0, 0.5, 0.0, 1.0);
      drawWeapon(Point(-218, -560), 2, 0, 1.0, 0.5, 0.0); // drawing weapon dots
      drawWeapon(Point(-218, -555), 2, 0, 1.0, 0.5, 0.0);
      drawWeapon(Point(-218, -565), 2, 0, 1.0, 0.5, 0.0);
      drawWeapon(Point(-228, -560), 2, 0, 1.0, 0.5, 0.0);
      drawWeapon(Point(-228, -555), 2, 0, 1.0, 0.5, 0.0);
      drawWeapon(Point(-228, -565), 2, 0, 1.0, 0.5, 0.0);
      drawWeapon(Point(-238, -560), 2, 0, 1.0, 0.5, 0.0);
      drawWeapon(Point(-238, -555), 2, 0, 1.0, 0.5, 0.0);
      drawWeapon(Point(-238, -565), 2, 0, 1.0, 0.5, 0.0);
      drawWeapon(Point(-208, -560), 2, 0, 1.0, 0.5, 0.0);
      drawWeapon(Point(-208, -555), 2, 0, 1.0, 0.5, 0.0);
      drawWeapon(Point(-208, -565), 2, 0, 1.0, 0.5, 0.0);
      drawWeapon(Point(-248, -560), 2, 0, 1.0, 0.5, 0.0);
      drawWeapon(Point(-248, -555), 2, 0, 1.0, 0.5, 0.0);
      drawWeapon(Point(-248, -565), 2, 0, 1.0, 0.5, 0.0);
     
      

      
      drawTextColorTiny(Point(-270, -585), "AMMO :", 1.0, 1.0, 1.0, 1.0);
      if (shotGunAmmo > 0)
      {
         drawNumber(Point(-210, -576), shotGunAmmo);
      }
      else
         drawTextColorTiny(Point(-210, -585), "EMPTY", 1.0 , 0.0, 0.0, 1.0);
      
      
   }
   
   if (currentWeapon == 2 && getShotGunUp() == true)
   {
      drawTextColorTiny(Point(-290, -545), "BALLISTIC OVERLOAD", 1.0, 0.60, 0.0, 1.0);
      drawWeapon(Point(-218, -560), 2, 0, 1.0, 0.5, 0.0); // drawing weapon dots
      drawWeapon(Point(-218, -555), 2, 0, 1.0, 0.5, 0.0);
      drawWeapon(Point(-218, -565), 2, 0, 1.0, 0.5, 0.0);
      drawWeapon(Point(-228, -560), 2, 0, 1.0, 0.5, 0.0);
      drawWeapon(Point(-228, -555), 2, 0, 1.0, 0.5, 0.0);
      drawWeapon(Point(-228, -565), 2, 0, 1.0, 0.5, 0.0);
      drawWeapon(Point(-238, -560), 2, 0, 1.0, 0.5, 0.0);
      drawWeapon(Point(-238, -555), 2, 0, 1.0, 0.5, 0.0);
      drawWeapon(Point(-238, -565), 2, 0, 1.0, 0.5, 0.0);
      drawWeapon(Point(-208, -560), 2, 0, 1.0, 0.5, 0.0);
      drawWeapon(Point(-208, -555), 2, 0, 1.0, 0.5, 0.0);
      drawWeapon(Point(-208, -565), 2, 0, 1.0, 0.5, 0.0);
      drawWeapon(Point(-248, -560), 2, 0, 1.0, 0.5, 0.0);
      drawWeapon(Point(-248, -555), 2, 0, 1.0, 0.5, 0.0);
      drawWeapon(Point(-248, -565), 2, 0, 1.0, 0.5, 0.0);
      
      
      
      
      drawTextColorTiny(Point(-270, -585), "AMMO :", 1.0, 1.0, 1.0, 1.0);
      if (shotGunAmmo > 0)
      {
         drawNumber(Point(-210, -576), shotGunAmmo);
      }
      else
         drawTextColorTiny(Point(-210, -585), "EMPTY", 1.0 , 0.0, 0.0, 1.0);
      
      
   }

   
   
   if (currentWeapon == 3 && getLaserDamageUp() == false)
   {
      drawTextColorTiny(Point(-270, -545), "FUSION LASER", 1.0, 0.0, 0.0, 1.0);
    
      drawLine(Point(-180, -560), Point(-268, -560), 1.0, 0.0, 0.0); // drawing laser
      drawLine(Point(-180, -561), Point(-268, -561), 1.0, 0.0, 0.0);
      drawLine(Point(-180, -562), Point(-268, -562), 1.0, 0.0, 0.0);
      drawWeapon(Point(-268, -561), 5, 0, 1.0, 0.0, 0.0);
      
      
      drawTextColorTiny(Point(-270, -585), "AMMO :", 1.0, 1.0, 1.0, 1.0);
      if (LaserAmmo > 0)
      {
         drawNumber(Point(-210, -576), LaserAmmo);
      }
      else
         drawTextColorTiny(Point(-210, -585), "EMPTY", 1.0 , 0.0, 0.0, 1.0);
      
      
   }
   
   if (currentWeapon == 3 && getLaserDamageUp() == true)
   {
      drawTextColorTiny(Point(-310, -545), "FOCUSED FUSION LASER", 0.0, 1.0, 0.0, 1.0);
      
      drawLine(Point(-180, -560), Point(-268, -560), 0.0, 1.0, 0.0); // drawing laser
      drawLine(Point(-180, -561), Point(-268, -561), 0.0, 1.0, 0.0);
      drawLine(Point(-180, -562), Point(-268, -562), 0.0, 1.0, 0.0);
      drawWeapon(Point(-268, -561), 5, 0, 0.0, 1.0, 0.0);
      
      
      drawTextColorTiny(Point(-270, -585), "AMMO :", 1.0, 1.0, 1.0, 1.0);
      if (LaserAmmo > 0)
      {
         drawNumber(Point(-210, -576), LaserAmmo);
      }
      else
         drawTextColorTiny(Point(-210, -585), "EMPTY", 1.0 , 0.0, 0.0, 1.0);
      
      
   }


   
   // *** ALIEN DECTECTION ***
   
   drawTextColor(Point(385, -520), "THREATS DETECTED", 0.5, 0.5, 1.0, 1.0);
   drawLine(Point(385, -525), Point(575, -525), 0.0, 0.0, 1.0); // blue line underneath
   drawLine(Point(385, -526), Point(575, -526), 0.0, 0.0, 1.0);
   drawLine(Point(385, -527), Point(575, -527), 0.0, 0.0, 1.0);
   
   int numLoc = 400;
   if (alienCount > 9)
      numLoc = 390;
   
   
   if (alienCount == 0)
   {
      drawTextColor(Point(445, -550), "NONE", 0.0, 1.0, 0.0, 1.0);
   }
   if (alienCount == 1)
   {
      drawTextColor(Point(425, -560), "CAUTION!", 1.0, 0.0, 0.0, 1.0);
      drawText(Point(415, -580), "HOSTILE DETECTED");
      drawNumber(Point(400, -570), alienCount);
      
   }
   
   if (alienCount > 1)
   {
      drawTextColor(Point(425, -550), "CAUTION!", 1.0, 0.0, 0.0, 1.0);
      drawText(Point(415, -580), "HOSTILES DETECTED");
      drawNumber(Point(numLoc, -570), alienCount);
   }
   
   //shows warp warning
   if (warpAnimate < WARP_ANIMATION_LEN)
   {
      red = 1.0;
      gre = 0.0;
      blue = 0.0;
      int warningAnimate = random(-1.0 , 2.0);
      if (getIsPaused() == true)
      {
         drawTextColor(Point(425, -450), "WARP-IN", red, 0.0, 0.0, 1.0);
         drawTextColor(Point(420, -470), "DETECTED", red, 0.0, 0.0, 1.0);
      }
      
      drawTextColor(Point(425, -450), "WARP-IN", red, 0.0, 0.0, warningAnimate);
      drawTextColor(Point(420, -470), "DETECTED", red, 0.0, 0.0, warningAnimate);
      
      
      
   }
   


   
   
   
   
   
   
   
   
   
}

/*********************************************
 * GAME :: RESET
 * Resets all values for a new game
 *********************************************/
void Game :: reset()
{
 
   setGameWin(false);
   setShields(STARTING_SHIELDS);
   setGameOver(false);
   alienCount = 0;
   invinceTime = INVINCE_TIME;
   numOfAliens = 0;
   aliensCreated = 0;

   //making the initial number of astroids
   for (int i = 0; i < numOfAsteroids; i++)
   {
      // random staring points vvv
      Point point(random(-MY_X, MY_X - 30), random(-MY_Y, MY_Y - 30)); // hopefully this makes it so rocks dont start on ship
      
      asteroids.push_back(new BigAsteroid(point, getIsPaused()));
    
   }
   
   //Making our stars
   for (int i = 0; i < STAR_COUNT; i++)
   {
      Point point(random(-MY_X, MY_X), random(-MY_X, MY_X));
      stars.push_back(*new Star(point));
   }
   
   
   //making ship
   Point point(0.0, 0.0);
   ships.push_back(new Ship(point, getIsPaused(), getCheckAccelSpeed(), getUpShieldBat1(), getUpShieldBat2(), getupPlasmaRange(), getUpPlasmaHeatSink(), getShotGunUp(), getLaserShotUp(), getDeepScannerUp(), getCheckRotateSpeed()));
   //making the initial number of astroids
   
   
   
   
}


/*********************************************
 * GAME :: nextLevel
 * advances the game to the next level
 *********************************************/
void Game :: nextLevel()
{
   aliensCreated = 0;
   
   if (currentSector > 3)
   {
      aliensPerLevel += 2; // num of aliens that can appear each level
   }
   invinceTime = INVINCE_TIME;
   
   warpAnimate = WARP_ANIMATION_LEN;
   advTime = ADV_TIME;
   clearLists();
   currentSector += 1;
   numOfAsteroids += 6; // how many more asteroids will appear in the next level
   asteroidsTotal = numOfAsteroids;
   
   
}


/*********************************************
 * GAME :: clearLists
 * deletes all lists
 *********************************************/
void Game :: clearLists()
{
   setAsteroidsTotal(getNumOfAsteroids());
   asteroids.clear();
   ships.clear();
   warpField.clear();
   stars.clear();
   bullets.clear();
   pills.clear();
   alienDebris.clear();
   alienShips.clear();
   alienBullets.clear();
   
   
}


// You may find this function helpful...

/**********************************************************
 * Function: getClosestDistance
 * Description: Determine how close these two objects will
 *   get in between the frames.
 **********************************************************/

float Game :: getClosestDistance(const FlyingObject &obj1, const FlyingObject &obj2) const
{
   // find the maximum distance traveled
   float dMax = max(abs(obj1.getVelocity().getDx()), abs(obj1.getVelocity().getDy()));
   dMax = max(dMax, abs(obj2.getVelocity().getDx()));
   dMax = max(dMax, abs(obj2.getVelocity().getDy()));
   dMax = max(dMax, 0.1f); // when dx and dy are 0.0. Go through the loop once.
   
   float distMin = std::numeric_limits<float>::max();
   for (float i = 0.0; i <= dMax; i++)
   {
      Point point1(obj1.getPoint().getX() + (obj1.getVelocity().getDx() * i / dMax),
                     obj1.getPoint().getY() + (obj1.getVelocity().getDy() * i / dMax));
      Point point2(obj2.getPoint().getX() + (obj2.getVelocity().getDx() * i / dMax),
                     obj2.getPoint().getY() + (obj2.getVelocity().getDy() * i / dMax));
      
      float xDiff = point1.getX() - point2.getX();
      float yDiff = point1.getY() - point2.getY();
      
      float distSquared = (xDiff * xDiff) +(yDiff * yDiff);
      
      distMin = min(distMin, distSquared);
   }
 
   return sqrt(distMin);
}

/*********************************************
 * GAME :: clearLists
 * deletes all lists
 *********************************************/
void Game :: resetAlienSpawn()
{
   
   aliensPerLevel = ALIENS_PER_LEVEL;
   
}


/*********************************************
 * GAME :: clearLists
 * deletes all lists
 *********************************************/
int Game :: getShotGunAmmo()
{
   
   return shotGunAmmo;
   
}

/*********************************************
 * GAME :: clearLists
 * deletes all lists
 *********************************************/
int Game :: getLaserAmmo()
{
   
   return LaserAmmo;
   
}









