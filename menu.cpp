//
//  menu.cpp
//  asteroids pt2
//
//  Created by Sam Trent on 3/16/16.
//  Copyright © 2016 Sam Trent. All rights reserved.
//

#include "menu.h"

#include <list>
#include <vector>
#include <exception>
#include <cassert>


// *** Cost of Upgrades ***

#define SHIELD1_COST 6
#define SHIELD2_COST 8
#define PLASMA_RANGE_COST 4
#define PLASMA_HEAT_SINK_COST 7
#define SG_AMMO_COST 5
#define SHOT_GUN_UPGRADE 9
#define LASER_SHOT_COST 6
#define LASER_DAMAGE_UP_COST 11
#define DEEP_SCAN_COST 9
#define PICKUP_RANGE_COST 10

/// *** achievement score values ***

#define BASE_LINE_SCORE 1500
#define LEGENDARY_SCORE 2500
#define HOARDER_SCORE 1200
#define ECO_TRIGGER_SCORE 750
#define ASTRO_BLASTER_SCORE 500
#define RIPLEY_SCORE 650
#define THE_MOST_TOYS_SCORE 500
#define UNTOUCHABLE_SCORE 3500

// *** Score multipliers ***

#define ALIEN_MULT 10
#define ASTRO_MULT 0.5
#define POWERUP_MULT 5
#define DEATH_PENALTY 500

std::list<Star> menuStars;

/***************************************
* MainMenu:
* accept input from the user to start the
* game or to view the instuctions
***************************************/
void Menu::mainMenu(const Interface & ui, int &theSwitch, int &selectorLoc, int highScore)
{
   float trans = 1.0;
   
   // highScore
   drawTextColor(Point(-50, 400), "HIGH SCORE", 1.0, 1.0, 0.0, 1.0);
   if(highScore == 0)
   {
      drawNumber(Point(10, 380), highScore);

   }
   else
      drawNumber(Point(-5, 380), highScore);
   
   
   
   //title
   drawTextColor(Point(20, 170), "ASTEROIDS", 0.5, 0.5, 1.0, 1.0);
   drawLine(Point(15, 165), Point(140, 165), 0.5, 0.5, 1.0);
   
   drawTitle(Point(-100, 120), "Lone Armada", 0.0, 0.0, 1.0, trans);

   //Point point(random(-800, 800), random(-800, 800));
   
   for (int i = 0; i < STAR_COUNT1; i++)
   {
      Point point(random(-800, 800), random(-800, 800));

      menuStars.push_back(*new Star(point));
   }

   for (std::list <Star> :: iterator it = menuStars.begin();
        it != menuStars.end();
        ++it)
   {
      it->draw();
      
   }
   
//   for (int i = 0; i < menuStars.size(); i++)
//   {
//      menuStars[i].draw();
//   }

   
   //drawing title
//   drawTitle(Point(20, 120), "ASTEROIDS", 0.0, 0.0, 1.0, trans);
      
   //red line
   drawLine(Point(-100, 110), Point(160, 110), 1.0, 0.0, 0.0);
   drawLine(Point(-105, 108), Point(160, 108), 1.0, 0.0, 0.0);
   drawLine(Point(-110, 106), Point(160, 106), 1.0, 0.0, 0.0);
      
   //yellow line
   drawLine(Point(-115, 100), Point(160, 100), 1.0, 1.0, 0.0);
   drawLine(Point(-120, 98), Point(160, 98), 1.0, 1.0, 0.0);
   drawLine(Point(-125, 96), Point(160, 96), 1.0, 1.0, 0.0);
      
   //new game drawing
   drawTextColor(Point(-55,0), "Start New Game", 0.0, 1.0, 0.0, 1.0);

   
   drawTextColor(Point(-55, -100), "Options", 1.0, 1.0, 0.0, 1.0);

   
   drawTextColor(Point(-55,-50), "How to Play", 1.0, 1.0, 0.0, 1.0);
   
   drawTextColor(Point(-55, -150), "Quit", 1.0, 1.0, 0.0, 1.0);

   
   //making the selector
   if(selectorLoc == 0)
   {
      drawShip(Point(-80, 10), 0);
      drawCockpit(Point(-80, 10), 4, 0);
   }
   
   if(selectorLoc == 1)
   {
      drawShip(Point(-80, -40), 0);
      drawCockpit(Point(-80, -40), 4, 0);
   }
   if(selectorLoc == 2)
   {
      drawShip(Point(-80, -90), 0);
      drawCockpit(Point(-80, -90), 4, 0);
   }
   if(selectorLoc == 3)
   {
      drawShip(Point(-80, -140), 0);
      drawCockpit(Point(-80, -140), 4, 0);
   }


   if(ui.isSpace() || ui.isEnter())
   {
      switch(selectorLoc)
      {
         case 0:
            //start new game
            menuStars.clear();
            setNewGameStart(true);
            theSwitch = 1;
            break;
         case 1:
            setNewGameStart(true);
            theSwitch = 2;
            break;
         case 2:
            setNewGameStart(true);
            theSwitch = 3;
            break;
         case 3:
            exit(0); // quits the game
            break;
            
      }
   }
   
   
   if(ui.isNavDown() && selectorLoc < 3)
   {
      selectorLoc++;
   }
   if(ui.isNavUp() && selectorLoc > 0)
   {
      selectorLoc--;
   }

  
  
}
/***************************************
 * deathScreen:
 * Displays when the play dies, can we return 
 * to main menu or play again.
 ***************************************/
void Menu::deathScreen(const Interface & ui, int num, int deathNumber, int &selectorLocDeathScreen, int &theSwitch, bool check)
{
   
   drawTitle(Point(-40, 120), "MISSION FAILED", 1.0, 0.0, 0.0, 1.0);
   
   //red line
   drawLine(Point(-100, 110), Point(160, 110), 1.0, 0.0, 0.0);
   drawLine(Point(-105, 108), Point(160, 108), 1.0, 0.0, 0.0);
   drawLine(Point(-110, 106), Point(160, 106), 1.0, 0.0, 0.0);
   
   //Get a random death story
   switch (deathNumber)
   {
      case 1:
         drawTextColor(Point(-180,70), "You eject from your space craft just before it explodes but your", 0.50, 0.50, 1.0, 1.0);
         drawTextColor(Point(-200,40), "feeling of relief is short lived as you see an Alien Scout", 0.50, 0.50, 1.0, 1.0);
         drawTextColor(Point(-200,15), "coming to collect you...", 0.50, 0.50, 1.0, 1.0);
         break;
         
      case 2:
         drawTextColor(Point(-180,70), "The eject mechanism fails to function as your ship spirals", 0.50, 0.50, 1.0, 1.0);
         drawTextColor(Point(-200,40), "out of control, sending you crashing into a stray asteroid at", 0.50, 0.50, 1.0, 1.0);
         drawTextColor(Point(-200,15), "over 1,000 miles per hour...", 0.50, 0.50, 1.0, 1.0);
         break;
         
      case 3:
         drawTextColor(Point(-180,70), "Total system failure causes a fire to break out in your cockpit.", 0.50, 0.50, 1.0, 1.0);
         drawTextColor(Point(-200,40), "You frantically try to eject but the fire has fried the systems", 0.50, 0.50, 1.0, 1.0);
         drawTextColor(Point(-200,15), "and you slowly suffer death by fire as your ship drifts though space...", 0.50, 0.50, 1.0, 1.0);
         break;
      case 4:
         drawTextColor(Point(-180,70), "After the shields fail your ships hull begins to crush from the", 0.50, 0.50, 1.0, 1.0);
         drawTextColor(Point(-200,40), "pressure of space. You scream as the walls of the cockpit close in", 0.50, 0.50, 1.0, 1.0);
         drawTextColor(Point(-200,15), "on you and crumple every last bone in your body...", 0.50, 0.50, 1.0, 1.0);
         break;
         
      case 5:
         drawTextColor(Point(-180,70), "Life support systems fail as the shields fall. The ship eequalizes air pressure", 0.50, 0.50, 1.0, 1.0);
         drawTextColor(Point(-200,40), "from the vaccum of space and in your last moments you can feel your lungs", 0.50, 0.50, 1.0, 1.0);
         drawTextColor(Point(-200,15), "bursting and your eyes slowly being ejected from your skull...", 0.50, 0.50, 1.0, 1.0);
         break;
         
      case 6:
         drawTextColor(Point(-180,70), "Hull failure causes your ship to explode, launching bits of ship", 0.50, 0.50, 1.0, 1.0);
         drawTextColor(Point(-200,40), "pieces and human parts flying through space, destined to drift", 0.50, 0.50, 1.0, 1.0);
         drawTextColor(Point(-200,15), "endlessly to an unknown final destination...", 0.50, 0.50, 1.0, 1.0);
         break;
         
         
      default:
         break;
   }
   
   
   
   
   
   //play again prompt
   drawTextColor(Point(-30,-50), "Retry Level?", 0.0, 1.0, 0.0, 1.0);
   
   drawTextColor(Point(-30,-100), "Options", 1.0, 1.0, 0.0, 1.0);
   drawTextColor(Point(-30,-150), "Main Menu", 1.0, 1.0, 0.0, 1.0);

   
   //making the selector
   if(selectorLocDeathScreen == 0)
   {
      drawShip(Point(-55, -40), 0);
      drawCockpit(Point(-55, -40), 4, 0);
   }
   
   //options
   if(selectorLocDeathScreen == 1)
   {
      drawShip(Point(-55, -90), 0);
      drawCockpit(Point(-55, -90), 4, 0);
   }
   if(selectorLocDeathScreen == 2)
   {
      drawShip(Point(-55, -140), 0);
      drawCockpit(Point(-55, -140), 4, 0);
   }
   
   if(ui.isSpace() || ui.isEnter())
   {
      switch(selectorLocDeathScreen)
      {
         case 0:
            //restart level
            setRestart(true);
            break;
         //options
         case 1:
            check = false;
            setNewGameStart(true);
            theSwitch = 3;
            break;
         case 2:
            newGameStart = false;
            break;
            
      }
   }
   
   
   if(ui.isNavDown() && selectorLocDeathScreen < 2)
   {
      selectorLocDeathScreen++;
   }
   if(ui.isNavUp() && selectorLocDeathScreen > 0)
   {
      selectorLocDeathScreen--;
   }
   

   
   
   
   drawTextColor(Point(-10,-350), "TIPS:", 0.5, 0.5, 1.0, 1.0);
   drawLine(Point(-10, -356), Point(40, -356), 1.0, 1.0, 1.0); //straight line
   drawLine(Point(-10, -357), Point(40, -357), 1.0, 1.0, 1.0); // right line
   drawLine(Point(-10, -358), Point(40, -358), 1.0, 1.0, 1.0); // left line
   
   
   switch (num)
   {
      case 1:
         drawText(Point(-150, - 380), "Watch you speed! Loseing control of your ship is a quick");
         drawText(Point(-160, - 405), "to lose the game! just take it slow, don't fly faster");
         drawText(Point(-160, - 430), "than your bullets. Avoid the asteroids!");
         break;
      case 2:
         drawText(Point(-150, - 380), "Use your special Ammo! Don't be afraid to let loose with");
         drawText(Point(-160, - 405), "the shot gun or the laser. There is plenty of ammo floating");
         drawText(Point(-160, - 430), "around out there. Destroy the small asteroids to get more!");
         break;
      case 3:
         drawText(Point(-150, - 380), "Beware of the Aliens! Don't underestimate the strength of the Alien");
         drawText(Point(-160, - 405), "ships, use your special weapons on them to take them out quickly.");
         break;
      case 4:
         drawText(Point(-150, - 380), "Blast them away with the shot gun! The shot gun is a great weapon");
         drawText(Point(-160, - 405), "for clearing out big asteroids in the beginning of each sector. It");
         drawText(Point(-160, - 430), "also works wonders on Alien Scout ships.");
         break;
      case 5:
         drawText(Point(-150, - 380), "Unleash the laser! The laser is the most powerful weapon you have.");
         drawText(Point(-160, - 405), "Not only can it kill an Alien Scout in two hits or an Assimilator in five,");
         drawText(Point(-160, - 430), "but it can also pierce through asteroids and Aliens untill the end of the screen!");
         break;
      case 6:
         drawText(Point(-150, - 380), "Never stop shooting! Your plasma cannon has infinite ammo. Keep");
         drawText(Point(-160, - 405), "firing and the chances of an asteroid hitting you drop dramatically");
         break;
      case 7:
         drawText(Point(-150, - 380), "Switch it up! Don't forget that you can push \"x\" to change your weapon");
         drawText(Point(-160, - 405), "to something more powerful if you have ammo for it.");
         break;
      case 8:
         drawText(Point(-150, - 380), "Death has penalties...If you die you lose all the special ammo you've");
         drawText(Point(-160, - 405), "collected thus far. On the bright side you will always start off with");
         drawText(Point(-160, - 430), "full shields and on the last level that you died on.");
         break;
      case 9:
         drawText(Point(-150, - 380), "Ready to rock and roll! At the beginning of each sector you will always");
         drawText(Point(-160, - 405), "start off with full shields and a brief moment invincibility to make sure");
         drawText(Point(-160, - 430), "that none of the asteroids do any damage to you right off the bat");
         break;
         
   }
   
   
   
   

   // return to main menu
//   if (ui.isDown())
//   {
//      //setGoToControls(false);
//      newGameStart = false;
//   }

   
   
   
   
   
   
}

/***************************************
 * WinScreen:
 * Displays when the play dies, can we return
 * to main menu or play again.
 ***************************************/
int Menu::WinScreen(const Interface & ui, int aliensKilled, int restarts, int shotsFired, int asteroidsKilled, int pillsCollected, bool shieldBat1, bool shieldBat2, bool plasmaRangeUp, bool plasmaHeatSink, bool sgAmmoUp, bool shotGunUp, bool laserShotUp, bool laserDamageUp, bool deepScanUp, bool pickupRangeUp, int researchPoints, int getAsteroidsKilled, int getAliensKilled, int getPillsCollected, bool shieldsBelow25, int highScore)
{
   int totalScore = 0;
   
   drawTitle(Point(-175, 550), "MISSION ACCOMPLISHED!", 0.0, 1.0, 0.0, 1.0);
   
   //red line
   drawLine(Point(-175, 544), Point(160, 544), 0.0, 1.0, 0.0);
   drawLine(Point(-185, 540), Point(160, 540), 0.0, 1.0, 0.0);
   drawLine(Point(-195, 536), Point(160, 536), 0.0, 1.0, 0.0);
   drawLine(Point(-205, 532), Point(160, 532), 0.0, 1.0, 0.0);
   drawLine(Point(-215, 528), Point(160, 528), 0.0, 1.0, 0.0);
   
   // displaying win text
   drawTextColor(Point(-370, 490), "Well done Commander! You have successfully rid our sector of all the Aliens and Asteroids!", 0.50, 0.50, 1.0, 1.0);
    drawTextColor(Point(-400, 465), "Your efforts have been admirable and you have helped considerably with the reclaiming of", 0.50, 0.50, 1.0, 1.0);
    drawTextColor(Point(-400, 440), "sector from the Aliens. We hope that you will provide us with your continued service!", 0.50, 0.50, 1.0, 1.0);
   

   //stats
   drawTextColor(Point(-40, 390), "STATS:", 1.0, 1.0, 0.0, 1.0);
   drawLine(Point(-40, 386), Point(30, 386), 1.0, 1.0, 0.0);
   
   drawTextColorTiny(Point(-100, 355), "Aliens destroyed       - ", 0.0, 1.0, 1.0, 1.0);
   drawNumber(Point(60, 365), aliensKilled);
   drawTextColorTiny(Point(120, 355), "X 10", 1.0, 1.0, 1.0, 1.0);
   
   drawTextColorTiny(Point(-100, 325), "Asteroids obliterated  - ", 0.50, 0.50, 1.0, 1.0);
   drawNumber(Point(60, 335), asteroidsKilled);
   drawTextColorTiny(Point(120, 325), "X 0.5", 1.0, 1.0, 1.0, 1.0);
   
   drawTextColorTiny(Point(-100, 295), "Shots Fired              - ", 1.0, 1.0, 0.0, 1.0);
   drawNumber(Point(60, 305), shotsFired);
   drawTextColorTiny(Point(120, 295), "N/A", 1.0, 1.0, 1.0, 1.0);

   
   drawTextColorTiny(Point(-100, 265), "Power-ups collected - ", 0.0, 1.0, 0.0, 1.0);
   drawNumber(Point(60, 275), pillsCollected);
   drawTextColorTiny(Point(120, 265), "X 5", 1.0, 1.0, 1.0, 1.0);

   
   drawTextColorTiny(Point(-100, 235), "Deaths                     - ", 1.0, 0.0, 0.0, 1.0);
   drawNumber(Point(60, 245), restarts);
   drawTextColorTiny(Point(120, 235), "- 500", 1.0, 1.0, 1.0, 1.0);

   
   
   /***********************************
    *
    *      *** ACHIEVEMENTS ***
    *
    *************************************/
   
   
   drawTitle(Point(-110, 150), "ACHIEVEMENTS", 1.0, 1.0, 0.0, 1.0);
   drawLine(Point(-110,146), Point(105,146), 1.0, 1.0, 0.0);
   
   // baseLine model
   if(shieldBat1 == false && shieldBat2 == false && plasmaRangeUp == false && plasmaHeatSink == false && sgAmmoUp ==false && shotGunUp == false && laserShotUp == false && laserDamageUp == false && deepScanUp == false && pickupRangeUp == false)
   {
      drawTextColor(Point(-600,50), "Base Line Model", 0.0, 1.0, 0.0, 1.0);
      totalScore += BASE_LINE_SCORE;
   }
   if(shieldBat1 == true || shieldBat2 == true || plasmaRangeUp == true || plasmaHeatSink == true || sgAmmoUp == true || shotGunUp == true || laserShotUp == true || laserDamageUp == true || deepScanUp == true || pickupRangeUp == true)
   {
      drawTextColor(Point(-600,50), "Base Line Model", 0.25, 0.25, 0.50, 1.0);

   }
   drawText(Point(-550, 30), "+");
   drawNumber(Point(-540, 40), BASE_LINE_SCORE);
   drawText(Point(-590, 10), "Beat the game without");
   drawText(Point(-600, -10),"purchasing any upgrades!");
   
   
   
   
   // legendary
   if (restarts <= 0)
   {
      drawTextColor(Point(-250,50), "Legendary", 0.0, 1.0, 0.0, 1.0);
      totalScore += LEGENDARY_SCORE;
   }
   if (restarts > 0)
   {
      drawTextColor(Point(-250,50), "Legendary", 0.25, 0.25, 0.50, 1.0);
   }
   drawText(Point(-225, 30), "+");
   drawNumber(Point(-210, 40), LEGENDARY_SCORE);
   drawText(Point(-270, 10), "Beat the game without");
   drawText(Point(-240, -10),"any deaths!");

   
   //Hoarder
   if (researchPoints >= 40)
   {
      drawTextColor(Point(150,50), "Hoarder", 0.0, 1.0, 0.0, 1.0);
      totalScore += HOARDER_SCORE;
   }
   if( researchPoints < 40)
   {
      drawTextColor(Point(150,50), "Hoarder", 0.25, 0.25, 0.50, 1.0);
   }
   drawText(Point(160, 30), "+");
   drawNumber(Point(170, 40), HOARDER_SCORE);
   drawText(Point(100, 10), "Beat the game with 40 or more");
   drawText(Point(110, -10),"research points available!");

   
   
   //Econimical trigger
   if (shotsFired <= 5000)
   {
      drawTextColor(Point(500,50), "Economical Trigger", 0.0, 1.0, 0.0, 1.0);
      totalScore += ECO_TRIGGER_SCORE;
      
   }
   if (shotsFired > 5000)
   {
      drawTextColor(Point(500,50), "Economical Trigger", 0.25, 0.25, 0.50, 1.0);
      
   }
   drawText(Point(570, 30), "+");
   drawNumber(Point(585, 40), ECO_TRIGGER_SCORE);
   drawText(Point(535, 10), "Beat the game with");
   drawText(Point(520, -10),"5000 shots fired or less!");
   
   
   //ASTRO BLASTER
   if(getAsteroidsKilled >= 2500)
   {
      drawTextColor(Point(-580,-100), "Astro Blaster", 0.0, 1.0, 0.0, 1.0);
      totalScore += ASTRO_BLASTER_SCORE;
      
   }
   if(getAsteroidsKilled < 2500)
   {
      drawTextColor(Point(-580,-100), "Astro Blaster", 0.25, 0.25, 0.50, 1.0);
   }
   drawText(Point(-540, -120), "+");
   drawNumber(Point(-528, -110), ASTRO_BLASTER_SCORE);
   drawText(Point(-615, -140), "Beat the game having destroyed ");
   drawText(Point(-590, -160),"2500 Asteroids or more!");

   //Ripley
   if (getAliensKilled >= 35)
   {
      drawTextColor(Point(-230,-100), "Ripley", 0.0, 1.0, 0.0, 1.0);
      totalScore += RIPLEY_SCORE;
   }
   if (getAliensKilled < 35)
   {
       drawTextColor(Point(-230,-100), "Ripley", 0.25, 0.25, 0.50, 1.0);
   }
   drawText(Point(-225, -120), "+");
   drawNumber(Point(-210,-110), RIPLEY_SCORE);
   drawText(Point(-270, -140), "Beat the game having");
   drawText(Point(-290, -160),"destroyed 35 Aliens or more!");

   //the most toys
   if (getPillsCollected >= 200)
   {
      drawTextColor(Point(120,-100), "The Most Toys", 0.0, 1.0, 0.0, 1.0);
      totalScore += THE_MOST_TOYS_SCORE;
   }
   if(getPillsCollected < 200)
   {
      drawTextColor(Point(120,-100), "The Most Toys", 0.25, 0.25, 0.50, 1.0);
   }
   drawText(Point(165, -120), "+");
   drawNumber(Point(180, -110), THE_MOST_TOYS_SCORE);
   drawText(Point(95, -140), "Beat the game having collected");
   drawText(Point(115, -160),"200 or more power-ups!");
   
   //Untouchable
   if (shieldsBelow25 == false)
   {
      drawTextColor(Point(535,-100), "Untouchable", 0.0, 1.0, 0.0, 1.0);
      totalScore += UNTOUCHABLE_SCORE;
      
   }
   if (shieldsBelow25 == true)
   {
      drawTextColor(Point(535,-100), "Untouchable", 0.25, 0.25, 0.50, 1.0);
      
   }
   drawText(Point(570, -120), "+");
   drawNumber(Point(580, -110), UNTOUCHABLE_SCORE);
   drawText(Point(510, -140), "Beat the game without letting");
   drawText(Point(510, -160),"your shields fall below 25%!");
   
   
   // *** total points calculated ***
   totalScore += getAsteroidsKilled * ASTRO_MULT;
   totalScore += getAliensKilled * ALIEN_MULT;
   totalScore += getPillsCollected * POWERUP_MULT;
   totalScore -= restarts * DEATH_PENALTY;
   
   
   
   //Draw Scoreing
   drawTextColor(Point(-105,-250), "Your Score = ", 0.0, 1.0, 0.0, 1.0);
   drawNumber(Point(25, -238), totalScore);
   drawLine(Point(-105, -255), Point(75,-255), 0.0, 1.0, 0.0);
   drawLine(Point(-115, -260), Point(85,-260), 0.0, 1.0, 0.0);
   drawLine(Point(-125, -265), Point(95,-265), 0.0, 1.0, 0.0);


   
   if (totalScore >= highScore)
   {
      drawTextColor(Point(-135, -420), "*** NEW HIGHSCORE ***", 0.0, 1.0, 0.0, 1.0);
      
   }
   
   
   
   
   drawTextColor(Point(-105,-450), "High Score = ", 1.0, 1.0, 0.0, 1.0);
   drawNumber(Point(25, -438), highScore);
   
   drawTextColor(Point(-120, -510), "Press 'Space' or 'Enter' ", 1.0, 1.0, 1.0, 1.0);
   drawTextColor(Point(-120, -545), "to return to Main Menu", 1.0, 1.0, 1.0, 1.0);
   
   
   if (ui.isSpace() || ui.isEnter())
   {
       newGameStart = false;
   }
   return totalScore;
   
}

/***************************************
 * ControlScreen:
 * Displays the controls of the game.
 ***************************************/
void Menu::controlScreen(const Interface & ui, int &theSwitch, bool isInverted)
{
   // *** THE CONTROLS ***
   drawTitle(Point(-75, 550), "CONTROLS", 1.0, 1.0, 0.0, 1.0);
   drawLine(Point(-75, 546), Point(70, 546), 1.0, 1.0, 0.0);
   drawShip(Point(0,525), 0);
   drawCockpit(Point(0,525), 4, 0);
   
  
   drawRect(Point(-550,450), 60, 60, 0); // Pause key
   drawTitle(Point(-560,443), "P", 1.0, 1.0, 1.0, 1.0);
   drawTextColor(Point(-580, 395), "Pause", 1.0, 1.0, 1.0, 1.0);
   drawTextColor(Point(-580, 370), "Game", 1.0, 1.0, 1.0, 1.0);

   
   drawText(Point(-370, 445), "SPACE");
   drawRect(Point(-350,450), 160, 20, 0);
   drawTextColor(Point(-365, 395), "Fire", 1.0, 1.0, 1.0, 1.0);
   
   drawRect(Point(0,450), 60, 60, 0); // arrow key
   drawLine(Point(0, 468), Point(0, 430), 1.0, 1.0, 1.0); //straight line
   drawLine(Point(0, 430), Point(10, 445), 1.0, 1.0, 1.0); // right line
   drawLine(Point(0, 430), Point(-10, 445), 1.0, 1.0, 1.0); // left line
   if (isInverted == false)
   {
      drawTextColor(Point(-34, 395), "Reverse", 1.0, 1.0, 1.0, 1.0);
      drawTextColor(Point(-190, 395), "Accelerate", 1.0, 1.0, 1.0, 1.0);
   }
   else if(isInverted == true)
   {
      drawTextColor(Point(-34, 395), "Accelerate", 1.0, 1.0, 1.0, 1.0);
      drawTextColor(Point(-190, 395), "Reverse", 1.0, 1.0, 1.0, 1.0);
   }
   

   
   drawRect(Point(-150,450), 60, 60, 0); // arrow key
   drawLine(Point(-150, 468), Point(-150, 430), 1.0, 1.0, 1.0); //straight line
   drawLine(Point(-150, 468), Point(-140, 448), 1.0, 1.0, 1.0); // right line
   drawLine(Point(-150, 468), Point(-160, 448), 1.0, 1.0, 1.0); // left line
   
   drawRect(Point(150,450), 60, 60, 0); // arrow key
   drawLine(Point(130, 450), Point(166, 450), 1.0, 1.0, 1.0); //straight line
   drawLine(Point(143, 460), Point(130, 450), 1.0, 1.0, 1.0); // top line
   drawLine(Point(143, 440), Point(130, 450), 1.0, 1.0, 1.0); // bottem line
   drawTextColor(Point(105, 395), "Rotate Left", 1.0, 1.0, 1.0, 1.0);
   
   
   drawRect(Point(300,450), 60, 60, 0); // arrow key
   drawLine(Point(280, 450), Point(320, 450), 1.0, 1.0, 1.0); //straight line
   drawLine(Point(307, 460), Point(320, 450), 1.0, 1.0, 1.0); // top line
   drawLine(Point(307, 440), Point(320, 450), 1.0, 1.0, 1.0); // bottem line
   drawTextColor(Point(250, 395), "Rotate Right", 1.0, 1.0, 1.0, 1.0);
   
   drawRect(Point(450,450), 60, 60, 0); // arrow key
   drawTitle(Point(442,443), "X", 1.0, 1.0, 1.0, 1.0);
   drawTextColor(Point(416, 395), "Change", 1.0, 1.0, 1.0, 1.0);
   drawTextColor(Point(415, 370), "Weapon", 1.0, 1.0, 1.0, 1.0);
 
   
   // *** ENEMIES ***
   drawTitle(Point(-400, 300), "ENEMIES", 1.0, 0.0, 0.0, 1.0);
   drawLine(Point(-405, 290), Point(-290, 290), 1.0, 0.0, 0.0);
   
   //asteroids
   drawTextColor(Point(-385, 260), "Asteroids", 0.5, 0.5, 1.0, 1.0);
   drawText(Point(-480, 235), "While not necessarily \"enemies\", they can be deadly");
   drawText(Point(-520, 215), "nevertheless by exploding into smaller rocks when you shoot them.");
   
   drawLargeAsteroid(Point(-550,150), 0);
   drawTextColor(Point(-575, 100), "Large", 0.5, 0.5, 1.0, 1.0);
   drawTextColor(Point(-585, 80), "Asteroid", 0.5, 0.5, 1.0, 1.0);
   drawText(Point(-468, 150), "Becareful around these guys! They can ");
   drawText(Point(-468, 130), "drop your shields by 20% if they hit you.");
   
   
   drawMediumAsteroid(Point(-550, 25), 0);
   drawTextColor(Point(-580, -15), "Medium", 0.5, 0.5, 1.0, 1.0);
   drawTextColor(Point(-585, -35), "Asteroid", 0.5, 0.5, 1.0, 1.0);
   drawText(Point(-468, 25), "Not as deadly as the bigger ones but they");
   drawText(Point(-468, 5), "move faster and do 10% damage to your shields.");
   
   drawSmallAsteroid(Point(-550, -75), 0);
   drawTextColor(Point(-574, -105), "Small", 0.5, 0.5, 1.0, 1.0);
   drawTextColor(Point(-585, -125), "Asteroid", 0.5, 0.5, 1.0, 1.0);
   drawText(Point(-468, -75), "The least dangerous kind, dispite them being the");
   drawText(Point(-468, -95), "quickest of the three, they only do 5% damage.");
   drawText(Point(-468, -115), "They also have a chance to drop a power up.");
   
   // aliens
   drawTextColor(Point(-385, -175), "Aliens", 0.0, 1.0, 1.0, 1.0);
   drawText(Point(-520, -200), "The Aliens are a fearsom enemy. Having a fleet with durable shields");
   drawText(Point(-540, -220), "and tough armor, they should not be underestimated. Not only can their");
   drawText(Point(-540, -240), "ships warp in at any point but some ships also have powerful abilies.");
   
   // assimilater
   drawAlienShipPart(Point(-550, -300), 0, 0.2, 0.2, 0.2);
   drawGreenStrip(Point(-550, -300), 0, 0.0, 1.0, 0.0);
   drawMediumAlienShipPart(Point(-550, -300), 5, 1.0, 1.0, 0.0);
   drawSmallAienShipPart(Point(-550, -300), 10, 0.0, 1.0, 1.0);
   drawAlienShipPartSide(Point(-550, -300), 120, 1.0, 0.0, 0.0);
   drawSmallAienShipBigRing(Point(-550, -300), 0, 1.0, 1.0, 0.0);
   drawTextColor(Point(-595, -365), "Assimilator", 0.5, 0.5, 1.0, 1.0);
   drawTextColorTiny(Point(-575, -380), "HP: 200", 0.5, 0.5, 1.0, 1.0);
   drawText(Point(-468, -280), "The Assimilator is an extremely dangerous threat.");
   drawText(Point(-468, -300), "It's disruptor shots do massive damage and they are");
   drawText(Point(-468, -320), "capable of generating two small Alien asteroids if they");
   drawText(Point(-468, -340), "hit a large asteroid, which fly fast and do massive damage.");

   //Alien Scout
   drawSmallAienShipPart(Point(-550, -425), 0, 0.0, 1.0, 0.0);
   drawMediumAlienShipPart(Point(-550, -425), 10, 0.0, 1.0, 1.0);
   drawAlienShipPartSolid(Point(-550, -425), 20, 0.0, 0.25, 1.0);
   drawTextColor(Point(-580, -465), "Scout", 0.5, 0.5, 1.0, 1.0);
   drawTextColorTiny(Point(-575, -480), "HP: 50", 0.5, 0.5, 1.0, 1.0);
   drawText(Point(-468, -415), "The Alien Scout is small, weak and feeble. However,");
   drawText(Point(-468, -435), "it makes up for these fall backs with speed and a");
   drawText(Point(-468, -455), "blazing fast rate of fire.");

   
   
   
   // *** power ups ***
   drawTitle(Point(300, 300), "POWER UPS", 0.0, 0.0, 1.0, 1.0);
   drawLine(Point(447, 290), Point(290, 290), 0.0, 0.0, 1.0);
   
   drawTextColor(Point(315,260), "Shield Boost", 0.0, 0.45, 1.0, 1.0);
   
   drawPill(Point(370, 240), 40, 10, 0, 0.0, 0.0, 1.0);
   drawText(Point(370,240), "S");
   drawText(Point(300, 215), "Adds 25% to your shields!");
   
   drawTextColor(Point(305,170), "Shot Gun Ammo", 1.0, 0.50, 0.0, 1.0);
   drawPill(Point(370, 150), 40, 10, 0, 1.0, 0.50, 0.0);
   drawText(Point(370,150), "SG");
   drawText(Point(285, 125), "Gives you 20 Shot Gun rounds!");
   
   drawTextColor(Point(320,85), "Laser Ammo", 1.0, 0.0, 0.0, 1.0);
   drawPill(Point(370, 65), 40, 10, 0, 1.0, 0.0, 0.0);
   drawText(Point(370,65), "L");
   drawText(Point(300, 40), "Gives you 5 Laser ammo!");
   
   drawTextColor(Point(280,-5), "Alien Research Points", 0.0, 1.0, 0.0, 1.0);
   drawPill(Point(370, -25), 40, 10, 0, 0.0, 1.0, 0.0);
   drawText(Point(370,-25), "R");
   drawText(Point(225, -50), "Gives points to use towards upgrading your ship, small");
   drawText(Point(225, -75), "asteroids and Alien ships have a change to drop them!");
   
   
   //back button
   
   drawTextColor(Point(-770, -510), "'Enter'", 1.0, 1.0, 1.0, 1.0);
   
   drawRect(Point(-750,-550), 60, 60, 0); // left arrow key
   drawLine(Point(-770, -550), Point(-730, -550), 1.0, 1.0, 1.0); //straight line
   drawLine(Point(-770, -550), Point(-757, -540), 1.0, 1.0, 1.0); // top line
   drawLine(Point(-770, -550), Point(-757, -560), 1.0, 1.0, 1.0); // bottem line
   //return to main menu
   if (ui.isLeft() || ui.isSpace() || ui.isEnter() || ui.isEsc())
   {
      
      newGameStart = false;
      theSwitch = 0;
      
   }

   
   
   
   
   
   
}
/***************************************
 * MissionScreen:
 * Displays the goal of the game.
 ***************************************/
void Menu::MissionScreen(const Interface & ui, int &theSwitch)
{
   
   // mission discription
   drawTitle(Point(-100, 550), "YOUR MISSION", 1.0, 1.0, 0.0, 1.0);
   drawLine(Point(-95, 540), Point(90, 540), 1.0, 1.0, 0.0);
   
   //text
   
   drawTextColor(Point(-410, 490), "Commander, your mission is to clear all sectors of asteroids that are on a direct course with", 0.50, 0.50, 1.0, 1.0);
   drawTextColor(Point(-450, 455), "the colony base. The deeper you go into the proceeding sector, the more dangerous it will become.", 0.50, 0.50, 1.0, 1.0);
   drawTextColor(Point(-450, 420), "After each sector you will encounter more and more asteroids as their density will naturally", 0.50, 0.50, 1.0, 1.0);
   drawTextColor(Point(-450, 385), "increase. Beware, as we have also detected Alien ship signatures out in the deeper sectors. They", 0.50, 0.50, 1.0, 1.0);
   drawTextColor(Point(-450, 350), "may warp in at any time, so be watchful...", 0.50, 0.50, 1.0, 1.0);
   
   drawTextColor(Point(-410, 300), "Your ship is equiped with special weapons, you can find ammo for them amongst the asteroids. Your", 0.50, 0.50, 1.0, 1.0);
   drawTextColor(Point(-450, 265), "Plasma Cannon has infinte ammo but you will need to recharge your Ballistic Shot Gun and your Fusion", 0.50, 0.50, 1.0, 1.0);
   drawTextColor(Point(-450, 235), "Laser. Each weapon is best suited for certain situations. Use them wisely.", 0.50, 0.50, 1.0, 1.0);

   //objectives
   drawTitle(Point(-85, 150), "OBJECTIVES", 0.0, 1.0, 0.0, 1.0);
   drawLine(Point(-90, 140), Point(85, 140), 0.0, 1.0, 0.0);
   
   drawWeapon(Point(-150, 100), 5, 0, 1.0, 1.0, 1.0);
   drawWeapon(Point(-150, 65), 5, 0, 1.0, 1.0, 1.0);
   drawWeapon(Point(-150, 30), 5, 0, 1.0, 1.0, 1.0);
   drawText(Point(-70, 100), "Clear all ten sectors");
   drawText(Point(-70, 65), "Destroy all asteroids");
   drawText(Point(-110, 30), "Eliminate all Aliens you encounter");
   
   drawTitle(Point(-180, -400), "GOOD LUCK COMMANDER!", 0.0, 1.0, 0.0, 0.0);

   
   
   //back button
   
   drawTextColor(Point(-120, -510), "Press 'Space' or 'Enter' ", 1.0, 1.0, 1.0, 1.0);
   drawTextColor(Point(-50, -545), "to begin", 1.0, 1.0, 1.0, 1.0);
   
   if (ui.isSpace() || ui.isEnter())
   {
      setNewGameStart(true);
      setRestart(true);
      theSwitch = 0;
   }
   
   if(ui.isEsc())
   {
      newGameStart = false;
      theSwitch = 0;
   }

}

/***************************************
 * optionsScreen:
 * Displays the options and config of the game.
 ***************************************/
void Menu::optionsScreen(const Interface &ui, int &theSwitch, int &selectorLocOptions, bool &isInverted, int &gameSpeed, int &accelSpeed, bool returnToDeathScreen, int &rotationSpeed, bool returnToUpgradeScreen, bool &eraseHighScore, bool &areYouSure, bool &EraseConfirm, int &activateErase)
{
   drawTitle(Point(-60, 550), "OPTIONS", 1.0, 1.0, 0.0, 1.0);
   drawLine(Point(-60, 540), Point(60, 540), 1.0, 1.0, 0.0);
   
   //drawing different options
   drawTextColor(Point(-100, 200), "Invert 'X' Axis: ", 1.0, 1.0, 0.0, 1.0);
   if (isInverted == false)
   {
      drawTextColor(Point(80, 200), "No", 1.0, 1.0, 1.0, 1.0); // 0.50, 0.50, 1.0
   }
   if (isInverted == true)
   {
      drawTextColor(Point(80, 200), "Yes", 1.0, 1.0, 1.0, 1.0);
   }
   
   
   //not functioning
   drawTextColor(Point(-100, 150), "Game Speed: ", 1.0, 1.0, 0.0, 1.0);
   if (gameSpeed == 0)
   {
      drawTextColor(Point(80, 150), "Normal   (30 FPS)", 0.25, 0.25, 0.50, 1.0); // grayed out
   }
   if (gameSpeed == 1)
   {
      drawTextColor(Point(80, 150), "Fast       (60 FPS)", 0.5, 0.5, 1.0, 1.0);
   }
   if (gameSpeed == 2)
   {
      drawTextColor(Point(80, 150), "Slow      (20 FPS)", 0.5, 0.5, 1.0, 1.0);
   }

   
   // accel speed
   drawTextColor(Point(-100, 100), "Accleration Speed: ", 1.0, 1.0, 0.0, 1.0);
   
   if (accelSpeed == 0)
   {
      drawTextColor(Point(80, 100), "Normal", 1.0, 1.0, 1.0, 1.0);
   }
   if (accelSpeed == 1)
   {
      drawTextColor(Point(80, 100), "Fast", 1.0, 1.0, 1.0, 1.0);
   }
   if (accelSpeed == 2)
   {
      drawTextColor(Point(80, 100), "Slow", 1.0, 1.0, 1.0, 1.0);
   }
   
   //rotation speed
   drawTextColor(Point(-100, 50), "Rotation Speed: ", 1.0, 1.0, 0.0, 1.0);
   
   if (rotationSpeed == 0)
   {
      drawTextColor(Point(80, 50), "Normal", 1.0, 1.0, 1.0, 1.0);
   }
   if (rotationSpeed == 1)
   {
      drawTextColor(Point(80, 50), "Fast", 1.0, 1.0, 1.0, 1.0);
   }
   if (rotationSpeed == 2)
   {
      drawTextColor(Point(80, 50), "Slow", 1.0, 1.0, 1.0, 1.0);
   }

   //clear high score
   drawTextColor(Point(-100, 0), "Clear High Score", 1.0, 1.0, 0.0, 1.0);
   
   if(areYouSure == true)
   {
      drawTextColor(Point(80, 0), "Are you Sure?", 1.0, 0.0, 0.0, 1.0);
      drawTextColor(Point(225, 0), "(Press 'Enter' to confirm)", 1.0, 1.0, 1.0, 1.0);
   }
   if(areYouSure == false && EraseConfirm == true)
   {
      drawTextColor(Point(80, 0), "Reset!", 0.0, 1.0, 0.0, 1.0);
   }
   
   
   
   drawTextColor(Point(-100, -50), "Back", 1.0, 1.0, 0.0, 1.0);
   
   //making the selector
   if(selectorLocOptions == 0)
   {
      drawShip(Point(-125, 210), 0);
      drawCockpit(Point(-125, 210), 4, 0);
      
      //description
      drawTextColor(Point(-175,350), "This option changes the Up and Down arrow", 0.5, 0.5, 1.0, 1.0);
      drawTextColor(Point(-175,300), "keys to be inverted to eachother.", 0.5, 0.5, 1.0, 1.0);
   }
   
   
   if(selectorLocOptions == 1)
   {
      drawShip(Point(-125, 160), 0);
      drawCockpit(Point(-125, 160), 4, 0);
      
      //description
      drawTextColor(Point(-165,350), "This option changes the game speed,", 0.5, 0.5, 1.0, 1.0);
      drawTextColor(Point(-165,300), "'Normal' is highly recommended.", 0.5, 0.5, 1.0, 1.0);
   }
   if(selectorLocOptions == 2)
   {
      drawShip(Point(-125, 110), 0);
      drawCockpit(Point(-125, 110), 4, 0);
      
      //description
      drawTextColor(Point(-205,350), "This option changes the rate at which your ship", 0.5, 0.5, 1.0, 1.0);
      drawTextColor(Point(-205,300), "acclerates, 'Slow' is recommend for beginners.", 0.5, 0.5, 1.0, 1.0);
   }
   
   if(selectorLocOptions == 3)
   {
      drawShip(Point(-125, 60), 0);
      drawCockpit(Point(-125, 60), 4, 0);
      
      //description
      drawTextColor(Point(-205,350), "This option changes the rate at which your ship", 0.5, 0.5, 1.0, 1.0);
      drawTextColor(Point(-205,300), "rotate left and right, 'Normal is recommened.'", 0.5, 0.5, 1.0, 1.0);
   }
   if(selectorLocOptions == 4)
   {
      drawShip(Point(-125, 10), 0);
      drawCockpit(Point(-125, 10), 4, 0);
      
      //description
      drawTextColor(Point(-205,350), "This option will erease the current High Score", 0.5, 0.5, 1.0, 1.0);
      drawTextColor(Point(-205,300), "and change it back to 0.", 0.5, 0.5, 1.0, 1.0);
   }


   
   if(selectorLocOptions == 5)
   {
      drawShip(Point(-125, -40), 0);
      drawCockpit(Point(-125, -40), 4, 0);
   }
  
   
   //activaion
   if(ui.isSpace() || ui.isEnter())
   {
      switch(selectorLocOptions)
      {
         case 0:
            if (isInverted == false)
            {
               isInverted = true;
            }
            else if (isInverted == true)
            {
               isInverted = false;
            }
            break;
            
            /************************
             * Functionaliy
             * not currently working
             **************************/
//         case 1:
//            if (gameSpeed == 0)
//            {
//               gameSpeed = 1;
//            }
//            else if (gameSpeed == 1)
//            {
//               gameSpeed =2 ;
//            }
//            else if (gameSpeed == 2)
//            {
//               gameSpeed = 0;
//            }
//            break;
            
         case 2:
            if (accelSpeed == 0)
            {
               accelSpeed = 1;
            }
            else if (accelSpeed == 1)
            {
               accelSpeed =2 ;
            }
            else if (accelSpeed == 2)
            {
               accelSpeed = 0;
            }
            break;
         case 3:
            if (rotationSpeed == 0)
            {
               rotationSpeed = 1;
            }
            else if (rotationSpeed == 1)
            {
               rotationSpeed =2 ;
            }
            else if (rotationSpeed == 2)
            {
               rotationSpeed = 0;
            }
            break;
         case 4:
            areYouSure = true;
            activateErase++;
            
            if (activateErase >= 2)
            {
               areYouSure = false;
               EraseConfirm = true;
               eraseHighScore = true;
            }
            
            break;
         case 5:
            EraseConfirm = false;
            eraseHighScore = false;

            activateErase = 0;
            areYouSure = false;
            if(returnToDeathScreen == false)
            {
               setNewGameStart(false);
               theSwitch = 0;
            }
            
            if(returnToDeathScreen == true)
            {
               theSwitch = 0;
               setNewGameStart(true);
            }
            if(returnToUpgradeScreen == true)
            {
               theSwitch = 0;
               setNewGameStart(true);
            }
            break;
            
      }
   }
   
   
   
   
   if(ui.isNavDown() && selectorLocOptions < 5)
   {
      selectorLocOptions++;
   }
   if(ui.isNavUp() && selectorLocOptions > 0)
   {
      selectorLocOptions--;
   }
   if(ui.isEsc())
   {
      activateErase = 0;
      areYouSure = false;
      EraseConfirm = false;
      eraseHighScore = false;
      if(returnToDeathScreen == false)
      {
         setNewGameStart(false);
         theSwitch = 0;
      }
      
      if(returnToDeathScreen == true)
      {
         theSwitch = 0;
         setNewGameStart(true);
      }
      if(returnToUpgradeScreen == true)
      {
         theSwitch = 0;
         setNewGameStart(true);
      }
   }
   

   
   
}

/***************************************
 * upgradeScreen:
 * Displays upgrades that you can get for
 * your ship.
 ***************************************/
void Menu::upgradeScreen(const Interface &ui, int &selectorLocUpgradeScreen, bool &showUpgradeScreen, bool &shieldBat1, int &researchPoints, bool &shieldBat2, bool &plasmaRangeUp, bool &plasmaHeatSink, bool &sgAmmoUp, bool &shotGunUp, bool &laserShotUp, bool &laserDamageUp, bool &deepScanUp, int spin, bool &pickupRangeUp, int getShotGunAmmo, int getLaserAmmo, int getCurrentSector, int getAsteroidsKilled, int getAliensKilled, int restarts, bool &resumeGame, int &theSwitch)
{
   //colors
   float red = 0.25;
   float green = 0.25;
   float blue = 0.50;
   
   drawTitle(Point(-105, 550), "SHIP UPGRADES", 1.0, 1.0, 0.0, 1.0);
   drawLine(Point(-105, 540), Point(105, 540), 1.0, 1.0, 0.0);
   
   //total ammo
   if(selectorLocUpgradeScreen < 10)
   {
      drawText(Point(-70,-30), "Total Collected Ammo");
      drawLine(Point(-70, -34), Point(65,-34), 1.0, 1.0, 1.0);
      
      drawTextColorTiny(Point(-100, -60), "Shot Gun Ammo : ", 1.0, 0.50, 0.0, 1.0);
      drawNumber(Point(20,-50), getShotGunAmmo);
      drawText(Point(80, -56), "SG");
      drawPill(Point(80, -56), 40, 10, 0, 1.0, 0.50, 0.0);
      
      drawTextColorTiny(Point(-100, -85), "Laser Ammo      : ", 1.0, 0.0, 0.0, 1.0);
      drawNumber(Point(20,-75), getLaserAmmo);
      drawText(Point(80, -81), "L");
      drawPill(Point(80, -81), 40, 10, 0, 1.0, 0.0, 0.0);
   }
   
   
   //total research points
   drawPill(Point(-425, 523), 40, 10, 0, 0.0, 1.0, 0.0);
   drawText(Point(-425, 523), "R");
   drawNumber(Point(-470, 530), researchPoints);
   drawText(Point(-650, 520), "Research Points Available :");
   
  
   
   //Upgrade list
   drawTitle(Point(-650, 450), "Available Upgrades", 0.0, 1.0, 0.0, 1.0);
   drawLine(Point(-650, 446), Point(-435, 446), 0.0, 1.0, 0.0);
   drawLine(Point(-660, 442), Point(-425, 442), 0.0, 1.0, 0.0);
   drawLine(Point(-670, 438), Point(-415, 438), 0.0, 1.0, 0.0);
   
   if (researchPoints < SHIELD1_COST)
   {
      drawTextColor(Point(-660, 375), "Avd. Shield Battery Mk I", red, green, blue, 1.0);
   }
   if (researchPoints >= SHIELD1_COST)
   {
      drawTextColor(Point(-660, 375), "Avd. Shield Battery Mk I", 1.0, 1.0, 0.0, 1.0);
   }
   if (shieldBat1 == true)
   {
      drawTextColor(Point(-660, 375), "Avd. Shield Battery Mk I", 0.0, 1.0, 0.0, 1.0);
   }
   
   
   if (researchPoints < SHIELD2_COST)
   {
      drawTextColor(Point(-660, 325), "Avd. Shield Battery Mk II", red, green, blue, 1.0);
   }
   if (researchPoints >= SHIELD2_COST && shieldBat1 == false)
   {
      drawTextColor(Point(-660, 325), "Avd. Shield Battery Mk II", red, green, blue, 1.0);
   }
   if (researchPoints >= SHIELD2_COST && shieldBat1 == true)
   {
      drawTextColor(Point(-660, 325), "Avd. Shield Battery Mk II", 1.0, 1.0, 0.0, 1.0);
   }
   if (shieldBat2 == true)
   {
      drawTextColor(Point(-660, 325), "Avd. Shield Battery Mk II", 0.0, 1.0, 0.0, 1.0);
   }

   
   if (researchPoints < PLASMA_RANGE_COST)
   {
     drawTextColor(Point(-660, 275), "Plasma Cannon Barrel Augment", red, green, blue, 1.0);
   }
   if (researchPoints >= PLASMA_RANGE_COST)
   {
      drawTextColor(Point(-660, 275), "Plasma Cannon Barrel Augment", 1.0, 1.0, 0.0, 1.0);
   }
   if (plasmaRangeUp == true)
   {
      drawTextColor(Point(-660, 275), "Plasma Cannon Barrel Augment", 0.0, 1.0, 0.0, 1.0);
   }
   
   if (researchPoints < PLASMA_HEAT_SINK_COST)
   {
      drawTextColor(Point(-660, 225), "Plasma Heat Sink", red, green, blue, 1.0);
   }
   if (researchPoints >= PLASMA_HEAT_SINK_COST)
   {
      drawTextColor(Point(-660, 225), "Plasma Heat Sink", 1.0, 1.0, 0.0, 1.0);
   }
   if (plasmaHeatSink == true)
   {
      drawTextColor(Point(-660, 225), "Plasma Heat Sink", 0.0, 1.0, 0.0, 1.0);
   }
   
   if (researchPoints < SG_AMMO_COST)
   {
      drawTextColor(Point(-660, 175), "Improved Ballistic S.G. Ammo", red, green, blue, 1.0);
   }
   if (researchPoints >= SG_AMMO_COST)
   {
      drawTextColor(Point(-660, 175), "Improved Ballistic S.G. Ammo", 1.0, 1.0, 0.0, 1.0);
   }
   if (sgAmmoUp == true)
   {
      drawTextColor(Point(-660, 175), "Improved Ballistic S.G. Ammo", 0.0, 1.0, 0.0, 1.0);
   }
   
   if (researchPoints < SHOT_GUN_UPGRADE)
   {
      drawTextColor(Point(-660, 125), "Ballistic Overload", red, green, blue, 1.0);
   }
   if (researchPoints >= SHOT_GUN_UPGRADE)
   {
      drawTextColor(Point(-660, 125), "Ballistic Overload", 1.0, 1.0, 0.0, 1.0);
   }
   if (shotGunUp == true)
   {
      drawTextColor(Point(-660, 125), "Ballistic Overload", 0.0, 1.0, 0.0, 1.0);
   }
   
   
   if (researchPoints < LASER_SHOT_COST)
   {
      drawTextColor(Point(-660, 75), "Hyper Fusion Laser", red, green, blue, 1.0);
   }
   if (researchPoints >= LASER_SHOT_COST)
   {
      drawTextColor(Point(-660, 75), "Hyper Fusion Laser", 1.0, 1.0, 0.0, 1.0);
   }
   if (laserShotUp == true)
   {
      drawTextColor(Point(-660, 75), "Hyper Fusion Laser", 0.0, 1.0, 0.0, 1.0);
   }
   
   
   if (researchPoints < LASER_DAMAGE_UP_COST)
   {
      drawTextColor(Point(-660, 25), "Focused Fusion Capacitors", red, green, blue, 1.0);
   }
   if (researchPoints >= LASER_DAMAGE_UP_COST)
   {
      drawTextColor(Point(-660, 25), "Focused Fusion Capacitors", 1.0, 1.0, 0.0, 1.0);
   }
   if (laserDamageUp == true)
   {
      drawTextColor(Point(-660, 25), "Focused Fusion Capacitors", 0.0, 1.0, 0.0, 1.0);
   }

   
   if (researchPoints < DEEP_SCAN_COST)
   {
      drawTextColor(Point(-660, -25), "Deep Scanner", red, green, blue, 1.0);
   }
   if (researchPoints >= DEEP_SCAN_COST)
   {
      drawTextColor(Point(-660, -25), "Deep Scanner", 1.0, 1.0, 0.0, 1.0);
   }
   if (deepScanUp == true)
   {
      drawTextColor(Point(-660, -25), "Deep Scanner", 0.0, 1.0, 0.0, 1.0);
   }
   
   if (researchPoints < PICKUP_RANGE_COST)
   {
      drawTextColor(Point(-660, -75), "Recalibrated Harmonix", red, green, blue, 1.0);
   }
   if (researchPoints >= PICKUP_RANGE_COST)
   {
      drawTextColor(Point(-660, -75), "Recalibrated Harmonix", 1.0, 1.0, 0.0, 1.0);
   }
   if (pickupRangeUp == true)
   {
      drawTextColor(Point(-660, -75), "Recalibrated Harmonix", 0.0, 1.0, 0.0, 1.0);
   }
   


  
   drawLine(Point(-670, -125), Point(-415, -125), 0.0, 1.0, 0.0);
   
   drawTitle(Point(-660, -175), "Proceed to Next Sector...", 0.0, 0.0, 1.0, 0.0);
   
   drawTitle(Point(-660, -225), "Options", 1.0, 1.0, 0.0, 1.0);
   

   /***********************************
    * SELECTOR MOVEMNT
    *
    **********************************/
   
   // *** shield 1***
   if(selectorLocUpgradeScreen == 0)
   {
      drawShip(Point(-685, 385), 0);
      drawCockpit(Point(-685, 385), 4, 0);
      
      //description
      drawTextColor(Point(380, 450), "Avd. Shield Battery Mark I", 0.0, 1.0, 1.0, 1.0);
      
      if(shieldBat1 == false)
      {
         drawText(Point(440, 400), "Requires");
         drawNumber(Point(510, 410), SHIELD1_COST);
         
         drawPill(Point(550, 404), 40, 10, 0, 0.0, 1.0, 0.0);
         drawText(Point(550, 404), "R");
      }
      else if (shieldBat1 == true)
      {
         drawTextColor(Point(450, 400), "Purchased!", 0.0, 1.0, 0.0, 1.0);
      }
      
      drawTextColor(Point(260,350), "The Advanced Shield Battery will allow you to overcharge", 0.5, 0.5, 1.0, 1.0);
      drawTextColor(Point(240,300), "your shields by 50%, letting you increasing your total shield", 0.5, 0.5, 1.0, 1.0);
      drawTextColor(Point(240,250), "capacity to 150%!", 0.5, 0.5, 1.0, 1.0);
      
      //Draw Ship model
      drawBigShip(Point(0,200), 0, false);
      drawWeapon(Point(0,200), 80, 0, 0.0, 0.0, 1.0);

      int numOfRect = 40;
      int width = 125;
      int hight = 50;
      for(int i = 0; i < numOfRect; i++) // filling in rect
      {
         drawRectColor(Point(0,200), width, hight, 0, 0.0, 1.0, 1.0);
         width--;
         hight--;
      }
      
   }
   
   //*** shield 2 ***
   if(selectorLocUpgradeScreen == 1)
   {
      drawShip(Point(-685, 335), 0);
      drawCockpit(Point(-685, 335), 4, 0);
      
      //description
      
      drawTextColor(Point(380, 450), "Avd. Shield Battery Mark II", 1.0, 0.2, 0.5, 1.0);
      if(shieldBat1 == false)
      {
         drawTextColorTiny(Point(420, 375), "Requires Shield Battery Mk I", 1.0, 0.0, 0.0, 1.0);
      }
      
      
      if(shieldBat2 == false)
      {
         drawText(Point(440, 400), "Requires");
         drawNumber(Point(510, 410), SHIELD2_COST);
         
         drawPill(Point(550, 404), 40, 10, 0, 0.0, 1.0, 0.0);
         drawText(Point(550, 404), "R");
      }
      else if (shieldBat2 == true)
      {
         drawTextColor(Point(450, 400), "Purchased!", 0.0, 1.0, 0.0, 1.0);
      }
      
      drawTextColor(Point(260,350), "The most advanced generation of energy shield batteries!", 0.5, 0.5, 1.0, 1.0);
      drawTextColor(Point(240,300), "Your shields will now be able to hold an additional 50%,", 0.5, 0.5, 1.0, 1.0);
      drawTextColor(Point(240,250), "increasing your total shield capacity to 200%!", 0.5, 0.5, 1.0, 1.0);
      
      drawBigShip(Point(0,200), 0, false);
      drawWeapon(Point(0,200), 80, 0, 0.0, 1.0, 1.0);
      drawWeapon(Point(0,200), 40, 0, 0.0, 0.0, 1.0);
      drawWeapon(Point(0,200), 20, 0, 1.0, 0.2, 0.5);

   }
   
   // ***plasma range***
   if(selectorLocUpgradeScreen == 2)
   {
      drawShip(Point(-685, 285), 0);
      drawCockpit(Point(-685, 285), 4, 0);
      
      //description
      drawTextColor(Point(380, 450), "Plasma Cannon Barrel Augment", 1.0, 0.0, 1.0, 1.0);
      
      if(plasmaRangeUp == false)
      {
         drawText(Point(440, 400), "Requires");
         drawNumber(Point(510, 410), PLASMA_RANGE_COST);
         
         drawPill(Point(550, 404), 40, 10, 0, 0.0, 1.0, 0.0);
         drawText(Point(550, 404), "R");
      }
      else if (plasmaRangeUp == true)
      {
         drawTextColor(Point(450, 400), "Purchased!", 0.0, 1.0, 0.0, 1.0);
      }
      
      drawTextColor(Point(260,350), "By Augmenting the barrel on the plasma cannon we have", 0.5, 0.5, 1.0, 1.0);
      drawTextColor(Point(240,300), "been able to increase the range by 25%. You will be able", 0.5, 0.5, 1.0, 1.0);
      drawTextColor(Point(240,250), "to take down Aliens and Asteroids from afar.", 0.5, 0.5, 1.0, 1.0);
      
      //Draw Ship model
      drawBigShip(Point(0,200), 0, false);
      int numOfRect = 50;
      int width = 20;
      int hight = 250;
      for(int i = 0; i < numOfRect; i++) // filling in rect
      {
         drawRectColor(Point(0,300), width, hight, 0, 1.0, 0.0, 1.0);
         drawRectColor(Point(0,250), width-10, hight-10, 0, 1.0, 0.0, 1.0);
         width--;
         hight--;
      }

      drawWeapon(Point(0,200), 80, 0, 0.0, 0.0, 1.0);
      
      
   }
   
   // *** Plasma Heat sink ***
   if(selectorLocUpgradeScreen == 3)
   {
      drawShip(Point(-685, 235), 0);
      drawCockpit(Point(-685, 235), 4, 0);
      
      //description
      drawTextColor(Point(430, 450), "Plasma Heat Sink", 1.0, 0.0, 1.0, 1.0);
      
      if(plasmaHeatSink == false)
      {
         drawText(Point(440, 400), "Requires");
         drawNumber(Point(510, 410), PLASMA_HEAT_SINK_COST);
         
         drawPill(Point(550, 404), 40, 10, 0, 0.0, 1.0, 0.0);
         drawText(Point(550, 404), "R");
      }
      else if (plasmaHeatSink == true)
      {
         drawTextColor(Point(450, 400), "Purchased!", 0.0, 1.0, 0.0, 1.0);
      }
      
      drawTextColor(Point(260,350), "Using a brand new system for generating plasma bolts.", 0.5, 0.5, 1.0, 1.0);
      drawTextColor(Point(240,300), "Your Plasma Cannon will be able to shoot rapid fire, so long", 0.5, 0.5, 1.0, 1.0);
      drawTextColor(Point(240,250), "as you keep it from over heating.", 0.5, 0.5, 1.0, 1.0);
      
      //Draw Ship model
      drawBigShipPlasmaHeatSink(Point(0, 200), 0, false);
      drawBigShip(Point(0,200), 0, false);
      drawWeapon(Point(0,200), 80, 0, 0.0, 0.0, 1.0);
      
      

   }
   
   // SHOT GUN AMMO UPGRADE
   if(selectorLocUpgradeScreen == 4)
   {
      drawShip(Point(-685, 185), 0);
      drawCockpit(Point(-685, 185), 4, 0);
      
      //description
      
      drawTextColor(Point(390, 450), "Improved Ballistic S.G. Ammo", 1.0, 0.50, 0.0, 1.0);
      
      if(sgAmmoUp == false)
      {
         drawText(Point(440, 400), "Requires");
         drawNumber(Point(510, 410), SG_AMMO_COST);
         
         drawPill(Point(550, 404), 40, 10, 0, 0.0, 1.0, 0.0);
         drawText(Point(550, 404), "R");
      }
      else if (sgAmmoUp == true)
      {
         drawTextColor(Point(450, 400), "Purchased!", 0.0, 1.0, 0.0, 1.0);
      }
      
      drawTextColor(Point(260,350), "Reworking of the Ballistic Shot Gun Ammo packs have", 0.5, 0.5, 1.0, 1.0);
      drawTextColor(Point(240,300), "allowed us to increase its recharge capacity by 100%, thus", 0.5, 0.5, 1.0, 1.0);
      drawTextColor(Point(240,250), "giving you 40 shots per pickup instead of 20.", 0.5, 0.5, 1.0, 1.0);
      
      //Draw Shot gun pill
      
      //pill outter
      int numOfRect = 10;
      int width = 320;
      int hight = 80;
      for(int i = 0; i < numOfRect; i++) // filling in rect
      {
         drawPill(Point(0,200), width, hight, 0, 1.0, 0.50, 0.0);

         width--;
         hight--;
      }
      
      //pill middel
      int numOfRect1 = 10;
      int width1 = 160;
      int hight1 = 40;
      for(int i = 0; i < numOfRect1; i++) // filling in rect
      {
         drawPill(Point(0,200), width1, hight1, 0, 1.0, 1.0, 0.0);
         
         width1--;
         hight1--;
      }
      
      //pill inner
      int numOfRect2 = 10;
      int width2 = 80;
      int hight2 = 20;
      for(int i = 0; i < numOfRect2; i++) // filling in rect
      {
         drawPill(Point(0,200), width2, hight2, 0, 0.0, 1.0, 1.0);
         
         width2--;
         hight2--;
      }

      
      drawPill(Point(0,200), 320, 80, 0, 1.0, 0.50, 0.0);
      drawTitle(Point(20,230), "SG", 1.0, 1.0, 1.0, 1.0);
   }
   
   // Shot GUN UPGRADE
   if(selectorLocUpgradeScreen == 5)
   {
      drawShip(Point(-685, 135), 0);
      drawCockpit(Point(-685, 135), 4, 0);
      
      //description
      drawTextColor(Point(430, 450), "Ballistic Overload", 1.0, 0.50, 0.0, 1.0);
      
      if(shotGunUp == false)
      {
         drawText(Point(440, 400), "Requires");
         drawNumber(Point(510, 410), SHOT_GUN_UPGRADE);
         
         drawPill(Point(550, 404), 40, 10, 0, 0.0, 1.0, 0.0);
         drawText(Point(550, 404), "R");
      }
      else if (shotGunUp == true)
      {
         drawTextColor(Point(450, 400), "Purchased!", 0.0, 1.0, 0.0, 1.0);
      }
      
      drawTextColor(Point(260,350), "An addtion to the onboard Ballistic Shot Gun has allowed", 0.5, 0.5, 1.0, 1.0);
      drawTextColor(Point(240,300), "us to make it pack more of a puch. It now will shoot a 30%", 0.5, 0.5, 1.0, 1.0);
      drawTextColor(Point(240,250), "wider spread while launching triple the amout of projectiles.", 0.5, 0.5, 1.0, 1.0);
      
      drawTextColor(Point(460, 200), "CAUTION!", 1.0, 0.0, 0.0, 1.0);
      drawLine(Point(460, 196), Point(560, 196), 1.0, 0.0, 0.0);
      drawText(Point(370,170), "The energy requirements for this weapon exceed");
      drawText(Point(370,150), "that of the standard Ballistic SG. Therefor, it will");
      drawText(Point(370,130), "consume 3 ammo for each shot, instead of 1.");
      

      
      
      //Draw Ship model
      drawBigShipShotGunUp(Point(0, 200), 0, false);
      drawBigShip(Point(0,200), 0, false);
      drawWeapon(Point(0,200), 80, 0, 0.0, 0.0, 1.0);
   }
   
   // laser shot upgrade
   if(selectorLocUpgradeScreen == 6)
   {
      drawShip(Point(-685, 85), 0);
      drawCockpit(Point(-685, 85), 4, 0);
      
      //description
      drawTextColor(Point(420, 450), "Hyper Fusion Laser", 1.0, 0.0, 0.0, 1.0);
      
      if(laserShotUp == false)
      {
         drawText(Point(440, 400), "Requires");
         drawNumber(Point(510, 410), LASER_SHOT_COST);
         
         drawPill(Point(550, 404), 40, 10, 0, 0.0, 1.0, 0.0);
         drawText(Point(550, 404), "R");
      }
      else if (laserShotUp == true)
      {
         drawTextColor(Point(450, 400), "Purchased!", 0.0, 1.0, 0.0, 1.0);
      }
      
      drawTextColor(Point(260,350), "Improved fusion technology hsa let us extend the range of", 0.5, 0.5, 1.0, 1.0);
      drawTextColor(Point(240,300), "the Fusion Laser by 100%, allowing the laser to travel a much", 0.5, 0.5, 1.0, 1.0);
      drawTextColor(Point(240,250), "further distance than ever before.", 0.5, 0.5, 1.0, 1.0);
      
   
      //Draw Ship model
      drawBigShip(Point(0,200), 0, false);
      drawBigShipLaserShotUp(Point(0,200), 0, false);

      drawWeapon(Point(0,200), 80, 0, 0.0, 0.0, 1.0);
      
   }
   
   //laser damage upgrade
   if(selectorLocUpgradeScreen == 7)
   {
      drawShip(Point(-685, 35), 0);
      drawCockpit(Point(-685, 35), 4, 0);
      
      //description
      drawTextColor(Point(390, 450), "Focused Fusion Capacitors", 1.0, 0.0, 0.0, 1.0);
      
      if(laserDamageUp == false)
      {
         drawText(Point(440, 400), "Requires");
         drawNumber(Point(500, 410), LASER_DAMAGE_UP_COST);
         
         drawPill(Point(550, 404), 40, 10, 0, 0.0, 1.0, 0.0);
         drawText(Point(550, 404), "R");
      }
      else if (laserDamageUp == true)
      {
         drawTextColor(Point(450, 400), "Purchased!", 0.0, 1.0, 0.0, 1.0);
      }
      
      drawTextColor(Point(260,350), "Stolen Alien reseach tech has given us insight at how to", 0.5, 0.5, 1.0, 1.0);
      drawTextColor(Point(240,300), "increase the power output of the Fusion Laser dramatically.", 0.5, 0.5, 1.0, 1.0);
      drawTextColor(Point(240,250), "No Alien ships should be able to withstand more than 1 hit.", 0.5, 0.5, 1.0, 1.0);
      
      drawTextColor(Point(460, 200), "CAUTION!", 1.0, 0.0, 0.0, 1.0);
      drawLine(Point(460, 196), Point(560, 196), 1.0, 0.0, 0.0);
      drawText(Point(370,170), "The energy requirements for this weapon exceed");
      drawText(Point(370,150), "that of the standard Fusion Laser. Therefor, it");
      drawText(Point(370,130), "will consume 10 ammo for each shot, instead of 1.");
      

      
      //Draw Ship model
      drawBigShip(Point(0,200), 0, false);
      drawBigShipLaserDamage(Point(0, 200), 0, false);
      
      drawWeapon(Point(0,200), 80, 0, 0.0, 0.0, 1.0);
      

   }
   
   //deep scanner
   if(selectorLocUpgradeScreen == 8)
   {
      drawShip(Point(-685, -15), 0);
      drawCockpit(Point(-685, -15), 4, 0);
      
      //description
      
      drawTextColor(Point(430, 450), "Deep Scanner", 1.0, 1.0, 0.0, 1.0);
      
      if(deepScanUp == false)
      {
         drawText(Point(440, 400), "Requires");
         drawNumber(Point(510, 410), DEEP_SCAN_COST);
         
         drawPill(Point(550, 404), 40, 10, 0, 0.0, 1.0, 0.0);
         drawText(Point(550, 404), "R");
      }
      else if (deepScanUp == true)
      {
         drawTextColor(Point(450, 400), "Purchased!", 0.0, 1.0, 0.0, 1.0);
      }
      
      drawTextColor(Point(260,350), "Advancements to your ships onboard scanners will let you", 0.5, 0.5, 1.0, 1.0);
      drawTextColor(Point(240,300), "detect more power-ups in your vicinity. This advancement", 0.5, 0.5, 1.0, 1.0);
      drawTextColor(Point(240,250), "enables you to find 33% more power-ups from asteroids.", 0.5, 0.5, 1.0, 1.0);
      
      //Draw Ship model
      drawBigShip(Point(0,200), 0, false);
      drawWeapon(Point(0,200), 80, 0, 0.0, 0.0, 1.0);
      int numOfRect = 10;
      int width = 150;
      int hight = 5;
      for(int i = 0; i < numOfRect; i++) // filling in rect
      {
         drawPill(Point(0,200), width, hight, spin, 1.0, 1.0, 0.0);
         
         width--;
         hight--;
      }

      

   }
   
   
   
   //pickup range up
   if(selectorLocUpgradeScreen == 9)
   {
      drawShip(Point(-685, -65), 0);
      drawCockpit(Point(-685, -65), 4, 0);
      
      //description
      
      drawTextColor(Point(410, 450), "Recalibrated Harmonix", 1.0, 1.0, 0.0, 1.0);
      
      if(pickupRangeUp == false)
      {
         drawText(Point(440, 400), "Requires");
         drawNumber(Point(510, 410), PICKUP_RANGE_COST);
         
         drawPill(Point(550, 404), 40, 10, 0, 0.0, 1.0, 0.0);
         drawText(Point(550, 404), "R");
      }
      else if (pickupRangeUp == true)
      {
         drawTextColor(Point(450, 400), "Purchased!", 0.0, 1.0, 0.0, 1.0);
      }
      //30% greater distance
      drawTextColor(Point(260,350), "Readjusting the freqency of your ships shields weill enable", 0.5, 0.5, 1.0, 1.0);
      drawTextColor(Point(240,300), "you to collect power-ups like ammo, research and shields", 0.5, 0.5, 1.0, 1.0);
      drawTextColor(Point(240,250), "from further away instead of having to fly into them.", 0.5, 0.5, 1.0, 1.0);
      
      //Draw Ship model
      drawBigShip(Point(0,200), 0, false);
      drawWeapon(Point(0,200), 80, 0, 0.0, 0.0, 1.0);
      
      drawSheilds(Point(0,200), 200, 0.0, 1.0, 1.0, 1.0);
      
      
   }

   

   if(selectorLocUpgradeScreen == 10)
   {
      drawShip(Point(-685, -165), 0);
      drawCockpit(Point(-685, -165), 4, 0);
      
      
      //show progress
      drawTextColor(Point(450,450), "PROGRESS", 0.0, 1.0, 0.0, 1.0);
      drawLine(Point(450, 446), Point(565, 446), 0.0, 1.0, 0.0);
      
      drawTextColorTiny(Point(400, 425), "Sectors Cleared         :", 0.0, 1.0, 0.0, 1.0);
      drawNumber(Point(580, 435), getCurrentSector);
      
      drawTextColorTiny(Point(400, 400), "Asteroids Destroyed   :", 0.50, 0.50, 1.0, 1.0);
      drawNumber(Point(580, 410), getAsteroidsKilled);
      
      drawTextColorTiny(Point(400, 375), "Aliens Killed              :", 0.0, 1.0, 1.0, 1.0);
      drawNumber(Point(580, 385), getAliensKilled);
      
      drawTextColorTiny(Point(400, 350), "Deaths                      :", 1.0, 0.0, 0.0, 1.0);
      drawNumber(Point(580, 360), restarts);


      
      
      
      //description
      
      drawTitle(Point(-95,200), "Get Read For the", 0.0, 1.0, 0.0, 1.0);
      drawTitle(Point(-70, 150), "Next Sector!", 0.0, 1.0, 0.0, 1.0);
      
      drawText(Point(-60, 0), "Press 'Space' or");
      drawText(Point(-65, -30), "'Enter' to Continue");
      
      
      
      
      // flashing text
      int flash = random(-1, 2); // makes the text flash
      drawTitle(Point(-660, -175), "Proceed to Next Sector...", 0.0, flash, 0.0, 0.0);
   }

   //options
   if(selectorLocUpgradeScreen == 11)
   {
      drawShip(Point(-685, -215), 0);
      drawCockpit(Point(-685, -215), 4, 0);
   }

   
   
   
   
   //activeation
   if(ui.isSpace() || ui.isEnter())
   {
      switch(selectorLocUpgradeScreen)
      {
         case 0:
            if(researchPoints >= SHIELD1_COST && shieldBat1 == false)
            {
               shieldBat1 = true;
               researchPoints -= SHIELD1_COST;
            }
            break;
         case 1:
            if (researchPoints >= SHIELD2_COST && shieldBat1 == true && shieldBat2 == false)
            {
               shieldBat2 = true;
               researchPoints  -= SHIELD2_COST;
            }
            break;
         case 2:
            if(researchPoints >= PLASMA_RANGE_COST && plasmaRangeUp == false)
            {
               plasmaRangeUp = true;
               researchPoints -= PLASMA_RANGE_COST;
            }
            break;
         case 3:
            if(researchPoints >= PLASMA_HEAT_SINK_COST && plasmaHeatSink == false)
            {
               plasmaHeatSink = true;
               researchPoints -= PLASMA_HEAT_SINK_COST;
            }
            break;
            
         case 4:
            if(researchPoints >= SG_AMMO_COST && sgAmmoUp == false)
            {
               sgAmmoUp = true;
               researchPoints -= SG_AMMO_COST;
            }
            break;
         case 5:
            if(researchPoints >= SHOT_GUN_UPGRADE && shotGunUp == false)
            {
               shotGunUp = true;
               researchPoints -= SHOT_GUN_UPGRADE;
            }
            break;
         case 6:
            if(researchPoints >= LASER_SHOT_COST && laserShotUp == false)
            {
               laserShotUp = true;
               researchPoints -= LASER_SHOT_COST;
            }
            break;
         case 7:
            if(researchPoints >= LASER_DAMAGE_UP_COST && laserDamageUp == false)
            {
               laserDamageUp = true;
               researchPoints -= LASER_DAMAGE_UP_COST;
            }
            break;

         case 8:
            if(researchPoints >= DEEP_SCAN_COST && deepScanUp == false)
            {
               deepScanUp = true;
               researchPoints -= DEEP_SCAN_COST;
            }
            break;
            
         case 9:
            if(researchPoints >= PICKUP_RANGE_COST && pickupRangeUp == false)
            {
               pickupRangeUp = true;
               researchPoints -= PICKUP_RANGE_COST;
            }
            break;

            
            
         case 10:
            showUpgradeScreen = false;
            resumeGame = true;
            break;
            
         case 11:
            showUpgradeScreen = false;
            theSwitch = 3;
            break;
            
      }
   }
   
   //movement
   if(ui.isNavDown() && selectorLocUpgradeScreen < 11)
   {
      selectorLocUpgradeScreen++;
   }
   if(ui.isNavUp() && selectorLocUpgradeScreen > 0)
   {
      selectorLocUpgradeScreen--;
   }

   

   
   
   
   
}

