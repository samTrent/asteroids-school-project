/*********************************************************************
 * File: game.h
 * Description: Defines the game class for Asteroids
 *
 *********************************************************************/

#ifndef GAME_H
#define GAME_H

#include <vector>
#include <list>
#include <iostream>

#include "uiDraw.h"
#include "uiInteract.h"
#include "rocks.h"
#include "ship.h"
#include "point.h"
#include "velocity.h"
#include "bullet.h"
#include "flyingObject.h"
#include "star.h"
#include "shipDebris.h"
#include "pills.h"
#include "alienShips.h"
#include "alienBullet.h"


/*****************
 * SCREEN SIZE *
 *****************/
#define MY_X 800 //800
#define MY_Y 600 //600

#define STAR_COUNT 2000

//TODO:: get rid of the asteroid sutter on the edge of the screen



/*******************************
* CLASS: Game 
* Runs the whole program!
*******************************/

class Game
{
private:
   // The coordinates of the screen
   Point topLeft;
   Point bottomRight;
   bool gameOver;
   bool gameWin;
   float shields; //ship health
   int currentSector;
   int numOfAsteroids;
   int asteroidsTotal;
   bool check;
   int getTip; // lets us get a random tip
   int deathNumber; // get a number to generate a random death story
   int theSwitch;
   bool checkIsInverted;
   bool isPaused;
   int checkAccelSpeed; // set the rate at how fast the ship moves
   //int *pTheSwitch;
   int checkRotateSpeed;
   
   bool shieldsBelow25;
   
   
   
   // *** Win Screen Stats ***
   
   int numAlienKilled;
   int numRestarts;
   int numShotsFired;
   int numAsteroidsKilled;
   int pillsCollected;
   
   
   // *** Upgrades ***
   int theResearchPoints;

   
   
   bool upShieldBat1;
   bool upShieldBat2;
   bool upPlasmaRange;
   bool upPlasmaHeatSink;
   bool upSGAmmo;
   bool shotGunUp;
   bool laserShotUp;
   bool laserDamageUp;
   bool deepScannerUp;
   bool pickupRangeUp;
   
   

   
   // *** Our list of bullets, ships, and asteroids (our containers)
//   std::list<Bullet*> bullets;
//   std::vector<Ship*> ships;
//  // std::list<Rock*> asteroids;
//   
//   //our list of stars
//   std::vector<Star> stars;
   
   
   // TODO: declare your asteroid and ship here (e.g., "Ship * ship = new Ship;")
  // Rock *asteroid; //= new Rock;
   //Ship *ship;
   
  
   
   
   
   /*************************************************
    * Private methods to help with the game logic.
    *************************************************/
   bool isOnScreen(const Point & point);
   void advanceBullets();
   void advanceAlienBullets();
   void advanceShip();
   void advanceAliens();
   void advancePills();
   
   void advanceAlienDebris();
   void advanceWarpField();
   void createWarpField(Point point); // makes a spawn point for aliens
   void advanceDebris(); // make ship wreakage move
   void createAlienDebris(Point point);
   void createDebris(Point point); // make the debris
   void advanceAsteroids();
   void handleCollisions();
   void shipCollisions();
   void deleteTheDead();
   void pillCollisions(int &theResearchPoints);
   void handleAlienCollision();
   
   void handleAlienBulletRockCollision();
   void handleAlienBulletShipCollision();

   float getClosestDistance(const FlyingObject &obj1, const FlyingObject &obj2) const;

  
   //create your ship and asteroid here
   
   void createAsteroid(int type, Point point);
   void createAlienAsteroids(int type, Point point);
   void createPills(Point point);
   void createAliens(Point point);
   
   void warpInAliens();
   
   
protected:
   
   
   
   
   
   
public:
   /**************************************************
    * Constructor:
    * Initializes the game, with the screen resolution
    **************************************************/
   Game(Point tl, Point br);
  
   ~Game();
   
   
   
   
   /*********************************************
    * Function: handleInput
    * Description: Takes actions according to whatever
    *  keys the user has pressed.
    *********************************************/
   void handleInput(const Interface & ui);
   
   /*********************************************
    * Function: advance
    * Description: Move everything forward one
    *  step in time.
    *********************************************/
   void advance(bool &showUpgradeScreen, int &researchPoints, bool &shieldsBelow25);
   
   /*********************************************
    * Function: draw
    * Description: draws everything for the game.
    *********************************************/
   void draw(const Interface & ui);
   
   /*********************************************
    * GAME :: RESET
    * Resets all values for a new game
    *********************************************/
   void reset();
   
   
   void resetAlienSpawn();
   int getShotGunAmmo();
   int getLaserAmmo();
   
   
   std::list<Bullet*> bullets;
   std::vector<Ship*> ships;
   
   
   //our list of stars
   std::vector<Star> stars;
   std::list<Rock*> asteroids;
   
   //list of debris that are "rocks" lol
   std::list<Rock*> warpField;
   std::list<Rock*> debris;
   std::list<Rock*> alienDebris;
   std::list<Pills*> pills;
   
   //the aliens
   std::list<AlienShip*> alienShips;
   std::list<AlienBullet*> alienBullets;
   
   Ship *alien;
   Rock *asteroid;
   
   bool getGameOver() {return gameOver; };
   void setGameOver(bool gameOver) {this-> gameOver = gameOver; };
   
   bool getGameWin() {return gameWin; };
   void setGameWin(bool gameWin) {this-> gameWin = gameWin; };
   
   float getShields() {return shields; };
   void setShields(float shields) {this-> shields = shields; };
   
  
   
   void nextLevel();
   void clearLists();
   
   bool getShieldBelow25() { return shieldsBelow25; };
   void setShieldBelow25(bool shieldsBelow25) {this->shieldsBelow25 = shieldsBelow25;};
   
   int getCurrentSector() {return currentSector; };
   void setCurrentSector(int currentSector) {this-> currentSector = currentSector; };
   
   int getNumOfAsteroids() {return numOfAsteroids; };
   void setNumOfAsteroids(int numOfAsteroids) {this-> numOfAsteroids = numOfAsteroids; };
   
   int getAsteroidsTotal() {return asteroidsTotal; };
   void setAsteroidsTotal(int asteroidsTotal) {this-> asteroidsTotal = asteroidsTotal; };
   
   bool getCheck() {return check; };
   void setCheck(bool check) {this-> check = check; };
  
   
   
   int getTipNumber() { return getTip; };// passes to the death screen to get a random tip
   void setTip(int getTip) { this->getTip = getTip;};
   
   
   int getDeathNumber() { return deathNumber; };// passes to the death screen to get a random tip
   void setDeathNumber(int deathNumber) { this->deathNumber = deathNumber;};
   
   // get to mission screen
    int getTheSwitch() { return theSwitch; };
   void setTheSwitch(int theSwitch) {this->theSwitch = theSwitch; };
  

   //inveted settings
   bool getCheckIsInverted() { return checkIsInverted; };
   void setCheckIsInverted(bool checkIsInverted) {this->checkIsInverted = checkIsInverted; };
   
   
   //ship movement setting
   int getCheckAccelSpeed() { return checkAccelSpeed;};
   void setCheckAccelSpeed(int checkAccelSpeed) {this->checkAccelSpeed = checkAccelSpeed;};
   
   //shop rotation speed
   int getCheckRotateSpeed() {return checkRotateSpeed;};
   void setCheckRotateSpeed(int checkRotateSpeed) {this->checkRotateSpeed = checkRotateSpeed;};
   
   // pause the game
   bool getIsPaused() { return isPaused; };
   void setIsPaused(bool isPaused) {this->isPaused = isPaused; };
   
   // upgrade screen
//   bool getShowUpgradeScreen() { return setShowUpgradeScreen;};
//   void setShowUpgradeScreen(bool showUpgradeScreen) { this->showUpgradeScreen = showUpgradeScreen;};
   
   //win screen stats

   int getNumAlienKilled() {return numAlienKilled; };
   void setNumAilensKilled(int numAlienKilled) {this->numAlienKilled = numAlienKilled; };
   
   int getNumRestarts() {return  numRestarts; };
   void setNumRestarts(int numRestarts) {this->numRestarts = numRestarts; };
   
   int getNumShotsFired() {return numShotsFired; };
   void setNumShotsFired(int numShotsFired) {this->numShotsFired = numShotsFired;};
   
   int getNumAsteroidsKilled() {return numAsteroidsKilled; };
   void setNumAsteroidsKilled(int numAsteroidsKilled) {this->numAsteroidsKilled = numAsteroidsKilled; };
   
   int getPillsCollected() {return pillsCollected; };
   void setPillsCollected(int pillsCollected) {this->pillsCollected = pillsCollected; };
   
   // UPGRADES SETTERS/GETTERS
   
   int getResearchPoints() { return theResearchPoints; };
   void setResearchPoints(int theResearchPoints) {this->theResearchPoints = theResearchPoints; };
   
   bool getUpShieldBat1() { return upShieldBat1; };
   void setUpShieldBat1(bool upShieldBat1) {this->upShieldBat1 = upShieldBat1;};
   
   bool getUpShieldBat2() { return upShieldBat2; };
   void setUpShieldBat2(bool upShieldBat2) {this->upShieldBat2 = upShieldBat2;};
   
   bool getupPlasmaRange() { return upPlasmaRange; };
   void setUpPlasmaRange(bool upPlasmaRange) { this->upPlasmaRange = upPlasmaRange; };
   
   bool getUpPlasmaHeatSink() { return upPlasmaHeatSink; };
   void setUpPlasmaHeatSink(bool upPlasmaHeatSink) {this->upPlasmaHeatSink = upPlasmaHeatSink; };
   
   bool getUpSGAmmo() { return upSGAmmo; };
   void setUpSGAmmo(bool upSGAmmo) {this->upSGAmmo = upSGAmmo; };
   
   bool getShotGunUp() { return shotGunUp; };
   void setShotGunUp(bool shotGunUp) {this->shotGunUp = shotGunUp; };
   
   bool getLaserShotUp() {return laserShotUp; };
   void setLaserShotUp(bool laserShotUp) {this->laserShotUp = laserShotUp; };
   
   bool getLaserDamageUp() { return laserDamageUp; };
   void setLaserDamageUp(bool laserDamageUp) {this->laserDamageUp = laserDamageUp; };
   
   bool getDeepScannerUp() { return deepScannerUp; };
   void setDeepScannerUp(bool deepScannerUp) {this->deepScannerUp = deepScannerUp; };

   bool getPickupRangeUp() { return pickupRangeUp; };
   void setPickUpRangeUp(bool pickupRangeUp) {this->pickupRangeUp = pickupRangeUp; };

   
};



#endif /* GAME_H */
