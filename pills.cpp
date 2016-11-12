//
//  pills.cpp
//  asteroids pt2
//
//  Created by Sam Trent on 3/18/16.
//  Copyright © 2016 Sam Trent. All rights reserved.
//


#include <stdio.h>
#include "pills.h"


void ShieldPill::draw(bool isPaused, bool upSGAmmo)
{
   fade -= 0.00000000001;
   drawPill(point, 40, 10, 0, 0.0, 0.0, getFade());
   
   drawText(point, "S");
   if(isPaused == false)
   {
      timeToExpire(timeAlive--);
   }
}

void ShieldPill::timeToExpire(int timeAlive)
{
   if (timeAlive <= 0)
   {
      setAlive(false);
   }
}

void ShieldPill::fadePill(int timeAlive)
{
   if (timeAlive <= SHIELD_PILL_TIME - 10)
   {
      
      //fade  - 0.00000000001;
   }
}


// *** SHOT GUN PILL

void ShotGunPill::draw(bool isPaused, bool upSGAmmo)
{
   if (upSGAmmo == false)
   {
      fade -= 0.00000000001;
      drawPill(point, 40, 10, 0, 1.0, 0.50, 0.0);
      
      drawText(point, "SG");
      if(isPaused == false)
      {
         timeToExpire(timeAlive--);
      }
   }
   else if(upSGAmmo == true)
   {
      fade -= 0.00000000001;
      drawPill(point, 40, 10, 0, 1.0, 0.50, 0.0);
      drawPill(point, 20, 5, 0, 1.0, 1.0, 0.0);
      drawPill(point, 10, 2, 0, 0.0, 1.0, 1.0);
      
      drawText(point, "SG");
      if(isPaused == false)
      {
         timeToExpire(timeAlive--);
      }
      
   }
}

// *** LASER PILL ****
void LaserPill::draw(bool isPaused, bool upSGAmmo)
{
   fade -= 0.00000000001;
   drawPill(point, 40, 10, 0, 1.0, 0.0, 0.0);
   
   drawText(point, "L");
   if(isPaused == false)
   {
      timeToExpire(timeAlive--);
   }
}

// *** RESEARCH PILL ***
void ResearchPill::draw(bool isPaused, bool upSGAmmo)
{
   fade -= 0.00000000001;
   drawPill(point, 40, 10, 0, 0.0, 1.0, 0.0);
   
   drawText(point, "R");
   if(isPaused == false)
   {
      timeToExpire(timeAlive--);
   }

}

