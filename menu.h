//
//  menu.h
//  asteroids pt2
//
//  Created by Sam Trent on 3/16/16.
//  Copyright © 2016 Sam Trent. All rights reserved.
//

#ifndef menu_h
#define menu_h

#include "uiDraw.h"
#include "uiInteract.h"
#include "game.h"
#include "star.h"
#include "point.h"
#include "pills.h"
#include <list>
#define STAR_COUNT1 1

class Menu
{
private:
   bool restart;
   bool newGameStart;
   bool goToControls;
   bool startingGame;
 //  int theSwitch;
   
protected:
   
   
   
   
public:
   /***************************************
   * Meun :: Default construtor
   ***************************************/
   Menu()
   {
    // Point point(random(-800, 800), random(-800, 800));
      //int num = random(0, 2);
     // setTheSwitch(0);
     // std::list<Star> menuStars;
      newGameStart = false;
      setRestart(false);
      setGoToControls(false);
      setStartingGame(false);
      
   };
   void mainMenu(const Interface & ui, int &theSwitch, int &selectorLoc, int highScore);
   void deathScreen(const Interface & ui, int num, int deathNumber, int &selectorLocDeathScreen, int &theSwitch, bool check);
   
   int WinScreen(const Interface & ui, int aliensKilled, int restarts, int shotsFired, int asteroidsKilled, int pillsCollected, bool shieldBat1, bool shieldBat2, bool plasmaRangeUp, bool plasmaHeatSink, bool sgAmmoUp, bool shotGunUp, bool laserShotUp, bool laserDamageUp, bool deepScanUp, bool pickupRangeUp, int researchPoints, int getAsteroidsKilled, int getAliensKilled, int getPillCollected, bool shieldsBelow25, int highScore);
   
   void controlScreen(const Interface & ui, int &theSwitch, bool isInverted);
   void MissionScreen(const Interface & ui, int &theSwitch);
   
   void optionsScreen(const Interface & ui, int &theSwitch, int &selecotLocOptions, bool &isInverted, int &gameSpeed, int &accelSpeed, bool returnToDeathScreen, int &rotationSpeed, bool returnToUpgradeScreen, bool &eraseHighScore, bool &areYouSure, bool &eraseConfirm, int &activateErase);
   
   void upgradeScreen(const Interface & ui, int &selectorLocUpgradeScreen, bool &showUpgradeScreen, bool &shieldBat1, int &sresearchPoints, bool &shieldBat2, bool &plasmaRangeUp, bool &plasmaHeatSink, bool &sgAmmoUp, bool &shotGunUp, bool &laserShotUp, bool &laserDamageUp, bool &deepScanUp, int spin, bool &pickupRangeUp, int getShotGunAmmo, int getLaserAmmo, int getCurrentSector, int getAsteroidsKilled, int getAliensKilled, int getNumRestarts, bool &resumeGame, int &theSwitch);
   
   
 // std::list<Star> menuStars;
   
   bool getRestart() { return restart; };
   void setRestart(bool restart) {this->restart = restart; };
   
   bool getGoToControls() { return goToControls; };
   void setGoToControls(bool goToControls) {this->goToControls = goToControls; };
   
   bool getStartingGame() { return startingGame; };
   void setStartingGame(bool startingGame) {this->startingGame = startingGame; };
   
   bool getNewGameStart() { return newGameStart; };
   void setNewGameStart(bool newGameStart) {this->newGameStart = newGameStart; };
   
  
};

#endif /* menu_h */
