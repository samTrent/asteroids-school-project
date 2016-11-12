/*****************************************************
 * File: Driver.cpp
 * Author: Br. Burton
 *
 * Description: This file contains the main function
 *  that starts the game and the callback function
 *  that specifies what methods of the game class are
 *  called each time through the game loop.
 ******************************************************/
#include "game.h"
#include "uiInteract.h"
#include "menu.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

//const int MY_X = 800;
//const int MY_Y = 600;



string configSettings[4]; // saving the setting to an array for the options


int theSwitch = 0; // activates certain menus
int selectorLoc = 0; // moves the selector icon on the main menu
int selectorLocOptions = 0; // moves the selector icon on the option screen
int selectorLocDeathScreen = 0; // moves the selector icon on the death screen
int selectorLocUpgradeScreen = 0;

//config

bool isInverted; // false
int gameSpeed; // 0
int accelSpeed; // 0
int rotaionSpeed; // 0


bool returnToDeathScreen = false; // lets us return to deathscreen from options menu.
bool returnToUpgradeScreen = false; // lets us return to Upgrade screen from options menu.
bool showUpgradeScreen = false;
bool resumeGame = false;

bool shieldsBelow25; // for the achievement "untouchable"

// *** Upgrades ***
int researchPoints;

bool shieldBat1;
bool shieldBat2;
bool plasmaRangeUp;
bool plasmaHeatSink;
bool sgAmmoUp;
bool ShotGunUp;
bool laserShotUp;
bool laserDamageUp;
bool deepScanUp;
bool pickupRangeUp;
int spin; // lets the deep scanner animate

// highScore management
int totalScore;
int highScore;
bool eraseHighScore = false;
int activateErase = 0;
bool areYouSure = false; // asks if the user is sure they want to erase the highscore
bool confirmErase = false;



Point point(random(-800, 800), random(-800, 800));

/*************************************
 * boolConvert
 *    This function changes a string to
 * a bool.
 **************************************/
bool boolConvert(string x)
{
   bool theBool = false;
   
   if (x == "0'")
   {
      theBool = false;
   }
   if (x == "1")
   {
      theBool = true;
   }
   return theBool;
}

// *** FILE READING ***

//config saving
/*************************************
 * writeFileConfig
 *    This function will save the high
 * score to a file.
 **************************************/
void writeFileConfig(string fileName, bool isInverted, int gameSpeed, int accelSpeed, int rotationSpeed)
{
   // open the stream
   ofstream fout(fileName);
  
  
   if(fout.fail())
      cout << "failed" << endl;
   
   //write the data
   fout << isInverted << endl;
   fout << gameSpeed << endl;
   fout << accelSpeed << endl;
   fout << rotationSpeed << endl;

   
   //close the file
   fout.close();
   
}


/*************************************
 * ReadFileConfig
 *    This function will save the option
 * settings to ta file.
 **************************************/
int readFileConfig(string configSettings[], bool &isInverted, int &gameSpeed, int &accelSpeed, int &rotationSpeed)
{
   // open the stream
   ifstream fin("config.txt");
   if(fin.fail())
   {
      cout << "read file config failed..." << endl;
      return -1;
   }
   
   
   //read the data
   string data;
   for (int i = 0; i < 4 ; i++)
   {
      //int gameSpeed = stoi(configSettings[1]); // 0
      
      getline(fin, data);
      configSettings[i] = data;
      
   }
   
   
   isInverted = boolConvert(configSettings[0]);
   gameSpeed = stoi(configSettings[1]);
   accelSpeed = stoi(configSettings[2]);
   rotaionSpeed = stoi(configSettings[3]); // 0

   
   //close the file
   fin.close();
   
   return 1;
}

// high score saving
/*************************************
 * writeFileHighScore
 *    This function will save the high
 * score to a file.
 **************************************/
void writeFileHighScore(string fileName, int highScore)
{
   // open the stream
   ofstream fout(fileName);
   if(fout.fail())
      cout << "failed" << endl;
   
   //write the data
   fout << highScore << endl;
   
   //close the file
   fout.close();
   
}

/*************************************
 * ReadFileHighScore
 *    This function will save the high
 * score to a file.
 **************************************/
int readFileHighScore()
{
   // open the stream
   ifstream fin("highScore.txt");
   if(fin.fail())
      return -1;
   
   //write the data
   int highScore;
   fin >> highScore;
   
   //close the file
   fin.close();
   return highScore;
   
}


//  *** RUNING THE GAME ***


/*************************************
 * All the interesting work happens here, when
 * I get called back from OpenGL to draw a frame.
 * When I am finished drawing, then the graphics
 * engine will wait until the proper amount of
 * time has passed and put the drawing on the screen.
 **************************************/
void callBack(const Interface *pUI, void *p)
{
   
   
   
   //call the main menu first
   Menu *pMenu = (Menu *)p;
   Game *pGame = (Game *)p;
   

   // load the config file
   if (readFileConfig(configSettings, isInverted, gameSpeed, accelSpeed, rotaionSpeed) == -1)
   {
      // if it fails, default to this
      isInverted = false;
      gameSpeed = 0; // 0
      accelSpeed = 0; // 0
      rotaionSpeed = 0; // 0
   }
   
   
   
   //loading the highScore from the file
   highScore = readFileHighScore();
   if (highScore == -1)
   {
      highScore = 0;
   }
  
   
   

   
   // main menu
   if (pMenu->getNewGameStart() == false)
   {
      pGame->resetAlienSpawn();
      
      shieldBat1 = false;
      shieldBat2 = false;
      plasmaRangeUp = false;
      plasmaHeatSink = false;
      sgAmmoUp = false;
      ShotGunUp = false;
      laserShotUp = false;
      laserDamageUp = false;
      deepScanUp = false;
      pickupRangeUp = false;
      shieldsBelow25 = false;
      researchPoints = 0; // 0
      returnToDeathScreen = false;
      selectorLocDeathScreen = 0;
      selectorLocOptions = 0;
      pGame->setTheSwitch(0);
      pGame->setCheck(false);
      pGame->setGameOver(true);
      
      // call the main menu screen
      pMenu->mainMenu(*pUI, theSwitch, selectorLoc, highScore);

   }
   
   //The upgrade screen!
   else if (pGame->getGameOver() == true && pMenu->getNewGameStart() == true &&
            pGame->getCheck() == true && pGame->getGameWin() == false && showUpgradeScreen ==true)
   {
      resumeGame = false;
      returnToDeathScreen = false;
      
      spin += 4;
      pMenu->upgradeScreen(*pUI, selectorLocUpgradeScreen, showUpgradeScreen, shieldBat1, researchPoints, shieldBat2, plasmaRangeUp, plasmaHeatSink, sgAmmoUp, ShotGunUp, laserShotUp, laserDamageUp, deepScanUp, spin, pickupRangeUp, pGame->getShotGunAmmo(), pGame->getLaserAmmo(), pGame->getCurrentSector(), pGame->getNumAsteroidsKilled(), pGame->getNumAlienKilled(), pGame->getNumRestarts(), resumeGame, theSwitch);
      
      returnToUpgradeScreen = true;
      
      if(showUpgradeScreen == false && resumeGame == true)
      {
         pGame->setGameOver(false);
         if(pGame->getCurrentSector() < 10)
         {
            pGame->nextLevel();
            pGame->reset();
            selectorLocUpgradeScreen = 0;
         }
      }
   }
   
   //displays game options
   else if (pMenu->getNewGameStart() == true && pGame->getGameOver() == true && pGame->getGameWin() == false && theSwitch == 3)
   {
    
      
      pMenu->optionsScreen(*pUI, theSwitch, selectorLocOptions, isInverted, gameSpeed, accelSpeed, returnToDeathScreen, rotaionSpeed, returnToUpgradeScreen, eraseHighScore, areYouSure, confirmErase, activateErase);
      pMenu->setRestart(false);
      writeFileConfig("config.txt", isInverted, gameSpeed, accelSpeed, rotaionSpeed);

     

      if(theSwitch == 0 && returnToUpgradeScreen == false && returnToUpgradeScreen == false)
      {
        

         pMenu->setNewGameStart(false);
      }
      
      // go back to main menu
      if(theSwitch == 0 && returnToDeathScreen == true && returnToUpgradeScreen == false) // return  to death screen
      {
        

         pGame->setCheck(true);
         returnToDeathScreen = false;
         pMenu->setNewGameStart(true);
                  
         
      }

      // go back to upgrade screen
      if (theSwitch == 0 && returnToDeathScreen == false && returnToUpgradeScreen == true)
      {
        

         returnToUpgradeScreen = false;
         showUpgradeScreen = true;
      }
      
   }

   
   //The death screen if the player loses
   else if (pGame->getGameOver() == true && pMenu->getNewGameStart() == true &&
            pGame->getCheck() == true && pGame->getGameWin() == false)
   {
      
      pMenu->deathScreen(*pUI, pGame->getTipNumber(), pGame->getDeathNumber(), selectorLocDeathScreen, theSwitch, pGame->getCheck());
      returnToDeathScreen = true;
      returnToUpgradeScreen = false;
      
      if (pMenu->getRestart() == true) // retry level
      {
         
         pMenu->setNewGameStart(true);
         pGame->setGameOver(false);
         pGame->reset();
         pGame->setCheck(false);
         pGame->setTip(0);
         pGame->setDeathNumber(0);
         pGame->setNumRestarts(pGame->getNumRestarts() + 1);
         selectorLocDeathScreen = 0;
        
      }
      if(theSwitch == 3)
      {
         selectorLocOptions = 0;
         pGame->setCheck(false); // go to options screen
      }
  
      
   }
   
   
//   // display the game goals/mission
   else if (pMenu->getNewGameStart() == true && pGame->getGameOver() == true && pGame->getGameWin() == false && theSwitch == 1)
   {
      pMenu->MissionScreen(*pUI, theSwitch);
   }
   

   
   //displays game information and controls
   else if (pMenu->getNewGameStart() == true && pGame->getGameOver() == true && pGame->getGameWin() == false && theSwitch == 2)
   {
      pMenu->controlScreen(*pUI, theSwitch, isInverted);
      pMenu->setRestart(false);
      
      
   }
   
//   //displays game options *** WAS RIGHT HERE! ***
   
   else if (pMenu->getNewGameStart() == true && pMenu->getRestart() == true && pGame->getGameOver() == true)
   {
      
      //setting up initial conditions of the game
      pGame->setCurrentSector(1); // 1
      pGame->setNumOfAsteroids(6); // 5 origonal
      pGame->setAsteroidsTotal(pGame->getNumOfAsteroids());
      
      pGame->setNumAilensKilled(0);
      pGame->setNumAsteroidsKilled(0);
      pGame->setNumRestarts(0);
      pGame->setNumShotsFired(0);
      pGame->setPillsCollected(0);
      
      
      pGame->reset(); // allows the game to rerun if they return to the main menu
      
      // display the game goals/mission
      
      // pMenu->MissionScreen(*pUI, theSwitch);
      pGame->setGameOver(false);
   
      
   }

   

   
   // If all else is false, run the game
   else if (pGame->getGameOver() == false && pMenu->getNewGameStart() == true)
   {
      //passing in upgrades
      pGame->setResearchPoints(researchPoints);
      
      pGame->setUpShieldBat1(shieldBat1);
      pGame->setUpShieldBat2(shieldBat2);
      pGame->setUpPlasmaRange(plasmaRangeUp);
      pGame->setUpPlasmaHeatSink(plasmaHeatSink);
      pGame->setUpSGAmmo(sgAmmoUp);
      pGame->setShotGunUp(ShotGunUp);
      pGame->setLaserShotUp(laserShotUp);
      pGame->setLaserDamageUp(laserDamageUp);
      pGame->setDeepScannerUp(deepScanUp);
      pGame->setPickUpRangeUp(pickupRangeUp);
      pGame->setShieldBelow25(shieldsBelow25);

      
      //run the game
      pGame->setCheckIsInverted(isInverted);
      pGame->setCheckAccelSpeed(accelSpeed);
      pGame->setCheckRotateSpeed(rotaionSpeed);
      
      pGame->advance(showUpgradeScreen, researchPoints, shieldsBelow25);
      pGame->handleInput(*pUI);
      pGame->draw(*pUI);
    
      
      //check for the end game condition
      if (pGame->getGameOver() == true)
      {
         pGame->clearLists();
         pGame->setGameOver(true);
         pMenu->setRestart(false);
         pMenu->setNewGameStart(true);
         pGame->setCheck(true);
         pGame->setTip(random(1,10));
         pGame->setDeathNumber(random(1,7));
         
      
      }

   }
   
   //Show the Win Screen
   else if (pGame->getGameOver() == true && pGame->getGameWin() == true)
   {
      totalScore = pMenu->WinScreen(*pUI, pGame->getNumAlienKilled(), pGame->getNumRestarts(), pGame->getNumShotsFired(), pGame->getNumAsteroidsKilled(), pGame->getPillsCollected(), shieldBat1, shieldBat2, plasmaRangeUp, plasmaHeatSink, sgAmmoUp, ShotGunUp, laserShotUp, laserDamageUp, deepScanUp, pickupRangeUp, researchPoints, pGame->getNumAsteroidsKilled(), pGame->getNumAlienKilled(), pGame->getPillsCollected(), shieldsBelow25, highScore);
      
      //make a new high score and save it to a file
      if (totalScore > highScore)
      {
         writeFileHighScore("highScore.txt", totalScore);
      }

    
      if (pMenu->getNewGameStart() == false)
      {
         pGame->setGameWin(false);
      }
   }

   
   if (eraseHighScore == true)
   {
      writeFileHighScore("highscore.txt", 0); // reset the score
      eraseHighScore = false;
   }

}


/*********************************
 * Main is pretty sparse.  Just initialize
 * the game and call the display engine.
 * That is all!
 *********************************/
int main(int argc, char ** argv)
{
   
   
   
   Point topLeft(-MY_X, MY_Y); // 800 600
   Point bottomRight(MY_X, -MY_Y); //800 600
   
   Interface ui(argc, argv, "Asteroids Redux", topLeft, bottomRight);
   Game game(topLeft, bottomRight);
  
   ui.run(callBack, &game);
   
     

   return 0;
}
