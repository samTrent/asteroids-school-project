/***********************************************************************
 * Header File:
 *    User Interface Draw : put pixels on the screen
 * Author:
 *    Br. Helfrich
 * Summary:
 *    This is the code necessary to draw on the screen. We have a collection
 *    of procedural functions here because each draw function does not
 *    retain state. In other words, they are verbs (functions), not nouns
 *    (variables) or a mixture (objects)
 ************************************************************************/

#ifndef UI_DRAW_H
#define UI_DRAW_H

#include <string>     // To display text on the screen
#include <cmath>      // for M_PI, sin() and cos()
#include "point.h"    // Where things are drawn
using std::string;

/************************************************************************
 * DRAW DIGIT
 * Draw a single digit in the old school line drawing style.  The
 * size of the glyph is 8x11 or x+(0..7), y+(0..10)
 *************************************************************************/
void drawDigit(const Point & topLeft, char digit);

/*************************************************************************
 * DRAW NUMBER
 * Display an integer on the screen using the 7-segment method
 *************************************************************************/
void drawNumber(const Point & topLeft, long number);

/*************************************************************************
 * DRAW TEXT
 * Draw text using a simple bitmap font
 ************************************************************************/
void drawText(const Point & topLeft, const char * text);

/************************************************************************
 * ROTATE
 * Rotate a given point (point) around a given origin (center) by a given
 * number of degrees (angle).
 *************************************************************************/
void rotate(Point & point, const Point & origin, int rotation = 0);

/************************************************************************
 * DRAW RECTANGLE
 * Draw a rectangle on the screen centered on a given point (center) of
 * a given size (width, height), and at a given orientation (rotation)
 * measured in degrees (0 - 360)
 *************************************************************************/
void drawRect(const Point & center, int width, int height, int rotation);

/************************************************************************
 * DRAW CIRCLE
 * Draw a circle from a given location (center) of a given size (radius).
 *************************************************************************/
void drawCircle(const Point & center, int radius);

/************************************************************************
 * DRAW POLYGON
 * Draw a polygon from a given location (center) of a given size (radius).
 *************************************************************************/
void drawPolygon(const Point & center,
                 int radius = 20,
                 int points = 4,
                 int rotation = 0);

/************************************************************************
 * DRAW LINE
 * Draw a line on the screen from the beginning to the end.
 *************************************************************************/
void drawLine(const Point & begin, const Point & end,
             float red, float green, float blue);


/***********************************************************************
 * DRAW Lander
 * Draw a moon-lander spaceship on the screen at a given point
 ***********************************************************************/
void drawLander(const Point & point);

/***********************************************************************
 * DRAW Lander Flame
 * Draw the flames coming out of a moonlander for thrust
 ***********************************************************************/
void drawLanderFlames(const Point & point, 
                      bool bottom,
                      bool left,
                      bool right);

/************************************************************************
 * DRAW DOT
 * Draw a single point on the screen, 2 pixels by 2 pixels
 *************************************************************************/
void drawDot(const Point & point);

/************************************************************************
 * DRAW Sacred Bird
 * Draw the bird on the screen
 *************************************************************************/
void drawSacredBird(const Point & center, float radius);

/************************************************************************
 * DRAW Tough Bird
 * Draw a tough bird on the screen
 *************************************************************************/
void drawToughBird(const Point & center, float radius, int hits);

/************************************************************************      
 * DRAW Ship                                                                   
 * Draw the spaceship on the screen                                         
 *************************************************************************/
void drawShip(const Point & point, int rotation, bool thrust = false);

/**********************************************************************
 * DRAW * ASTEROID
 **********************************************************************/
void drawSmallAsteroid( const Point & point, int rotation);
void drawMediumAsteroid(const Point & point, int rotation);
void drawLargeAsteroid( const Point & point, int rotation);
/**********************************************************************
 * DRAW ALIEN ASTEROID!
 **********************************************************************/
void drawAlienAsteroid( const Point & center, int rotation);

/******************************************************************
 * RANDOM
 * This function generates a random number.  The user specifies
 * The parameters 
 *    INPUT:   min, max : The number of values (min <= num <= max)
 *    OUTPUT   <return> : Return the integer
 ****************************************************************/
int    random(int    min, int    max);
double random(double min, double max);

/**********************************************************************
 * makes an explotion (by Sam Trent)
 **********************************************************************/
void drawExplotion(const Point & center, float radius, int hits);

/************************************************************************
 * DRAW star
 * Draw a single point on the screen, 2 pixels by 2 pixels
 *  INPUT point   The position of the dow
 *************************************************************************/
void drawStar(const Point & point, float trans);

/************************************************************************
 * DRAW  Warp field
 * Draw a rectangle on the screen centered on a given point (center) of
 * a given size (width, height), and at a given orientation (rotation)
 *  INPUT  center    Center of the rectangle
 *         width     Horizontal size
 *         height    Vertical size
 *         rotation  Orientation
 *************************************************************************/
void drawWarpField(const Point & center, int width, int height, int rotation);

/************************************************************************
 * DRAW shipBullet
 * Draw a single point on the screen, 2 pixels by 2 pixels
 *  INPUT point   The position of the dow
 *************************************************************************/
void drawShipBullet(const Point & point, float red, float gre, float blue,
                    float trans);
/*************************************************************************
 * DRAW TEXT color
 * Draw text using a simple bitmap font
 ************************************************************************/
void drawTextColor(const Point & topLeft, const char * text, float red,
                   float green, float blue, float trans);


/************************************************************************
 * DRAW COCKPIT
 * Draw a FILLED IN CIRCLE
 *  INPUT point   The position of the sacred
 *        radius  The size of the bird
 *        hits    How many its remaining to kill the bird
 *************************************************************************/
void drawCockpit(const Point & center, float radius, int hits);

/*************************************************************************
* DRAW TITLE
* Draws text in color!
*    Just be sure to enter in the values for red, green and blue
*
************************************************************************/
void drawTitle(const Point & topLeft, const char * text, float red,
               float green, float blue, float trans);
/************************************************************************
 * DRAW SHIELDS
 * Draw a circle from a given location (center) of a given size (radius).
 *  INPUT   center   Center of the circle
 *          radius   Size of the circle
 *************************************************************************/
void drawSheilds(const Point & center, int radius, float red, float green, float blue, float trans);

/************************************************************************
 * DRAW ALIEN SHIELDS
 * Draw a circle from a given location (center) of a given size (radius).
 *  INPUT   center   Center of the circle
 *          radius   Size of the circle
 *************************************************************************/
void drawAlienSheilds(const Point & center, int radius, float red, float green, float blue, float trans);

/************************************************************************
 * DRAW SHIP DEATH
 * Draw a rectangle on the screen centered on a given point (center) of
 * a given size (width, height), and at a given orientation (rotation)
 *  INPUT  center    Center of the rectangle
 *         width     Horizontal size
 *         height    Vertical size
 *         rotation  Orientation
 *************************************************************************/
void drawShipDeath(const Point & center, int width, int height, int rotation);


/************************************************************************
 * DRAW DebrisBits
 * Draw a single point on the screen, 2 pixels by 2 pixels
 *  INPUT point   The position of the dow
 *************************************************************************/
void drawDebrisBits(const Point & point);

/************************************************************************
 * DRAW Pill
 * Draw a rectangle on the screen centered on a given point (center) of
 * a given size (width, height), and at a given orientation (rotation)
 *  INPUT  center    Center of the rectangle
 *         width     Horizontal size
 *         height    Vertical size
 *         rotation  Orientation
 *************************************************************************/
void drawPill(const Point & center, int width, int height, int rotation,
              float red, float green, float blue);

/************************************************************************
 * DRAW SAUCER
 * Draw a tough bird on the screen
 *  INPUT point   The position of the sacred
 *        radius  The size of the bird
 *        hits    How many its remaining to kill the bird
 *************************************************************************/
void drawSaucer(const Point & center, float radius, int hits);

/************************************************************************
 * DRAW  Alien SHIP DEATH
 * Draw a rectangle on the screen centered on a given point (center) of
 * a given size (width, height), and at a given orientation (rotation)
 *  INPUT  center    Center of the rectangle
 *         width     Horizontal size
 *         height    Vertical size
 *         rotation  Orientation
 *************************************************************************/
void drawAlienShipDeath(const Point & center, int width, int height, int rotation);

/************************************************************************
 * DRAW ALIENDebrisBits
 * Draw a single point on the screen, 2 pixels by 2 pixels
 *  INPUT point   The position of the dow
 *************************************************************************/
void drawAlienDebrisBits(const Point & point);

/************************************************************************
 * DRAW Alive bullet
 * Draw a FILLED IN CIRCLE
 *  INPUT point   The position of the sacred
 *        radius  The size of the bird
 *        hits    How many its remaining to kill the bird
 *************************************************************************/
void drawAlienBullet(const Point & center, float radius, int hits);

/*************************************************************************
 * DRAW TEXT COLOR TINY
 * Draws text in color!
 *    Just be sure to enter in the values for red, green and blue
 *
 ************************************************************************/
void drawTextColorTiny(const Point & topLeft, const char * text, float red,
                       float green, float blue, float trans);

/************************************************************************
 * DRAW WEAPON
 * Draw a FILLED IN CIRCLE
 *  INPUT point   The position of the sacred
 *        radius  The size of the bird
 *        hits    How many its remaining to kill the bird
 *************************************************************************/
void drawWeapon(const Point & center, float radius, int hits, float red, float green, float blue);

/**********************************************************************
 * DRAW AlienShipPart
 **********************************************************************/
void drawAlienShipPart( const Point & center, int rotation, float red, float green, float blue);

/**********************************************************************
 * DRAW Alien ship part medium soild
 **********************************************************************/
void drawAlienShipPartSolid( const Point & center, int rotation, float red, float green, float blue);

/**********************************************************************
 * DRAW SMALL Alien Ship Part
 **********************************************************************/
void drawSmallAienShipPart( const Point & center, int rotation, float red, float green, float blue);

/**********************************************************************
 * DRAW MEDIUM Alien ship Part
 **********************************************************************/
void drawMediumAlienShipPart( const Point & center, int rotation, float red, float green, float blue);

/**********************************************************************
 * DRAW AlienShipPartSide
 **********************************************************************/
void drawAlienShipPartSide( const Point & center, int rotation, float red, float green, float blue);

/**********************************************************************
 * DRAW SMALL Alien big ring
 **********************************************************************/
void drawSmallAienShipBigRing( const Point & center, int rotation, float red, float green, float blue);
/**********************************************************************
 * DRAW green strip
 **********************************************************************/
void drawGreenStrip( const Point & center, int rotation, float red, float green, float blue);

/************************************************************************
 * DRAW LASER
 * Draw a rectangle THATS A LASER on the screen centered on a given point (center) of
 * a given size (width, height), and at a given orientation (rotation)
 *  INPUT  center    Center of the rectangle
 *         width     Horizontal size
 *         height    Vertical size
 *         rotation  Orientation
 *************************************************************************/
void drawLaser(const Point & center, int width, int height, int rotation, float red, float green, float blue);

/************************************************************************
 * DRAW Big Ship
 * Draw a spaceship on the screen
 *  INPUT point   The position of the ship
 *        angle   Which direction it is ponted
 *************************************************************************/
void drawBigShip(const Point & center, int rotation, bool thrust);

/************************************************************************
 * DRAW RectColor
 * Draw a rectangle THATS A LASER on the screen centered on a given point (center) of
 * a given size (width, height), and at a given orientation (rotation)
 *  INPUT  center    Center of the rectangle
 *         width     Horizontal size
 *         height    Vertical size
 *         rotation  Orientation
 *************************************************************************/
void drawRectColor(const Point & center, int width, int height, int rotation, float red, float green, float blue);

/**********************************************************************
 * DRAW barrel
 **********************************************************************/
void drawBarrel( const Point & center, int rotation, float red, float green, float blue);

/************************************************************************
* DRAW Big Ship plasma heat sink
* Draw a spaceship on the screen
*  INPUT point   The position of the ship
*        angle   Which direction it is ponted
*************************************************************************/
void drawBigShipPlasmaHeatSink(const Point & center, int rotation, bool thrust);
/************************************************************************
 * DRAW Ship
 * Draw a spaceship on the screen
 *  INPUT point   The position of the ship
 *        angle   Which direction it is ponted
 *************************************************************************/
void drawShipPlasmaHeatSink(const Point & center, int rotation, bool thrust);

/************************************************************************
 * DRAW Big Ship plasma heat sink
 * Draw a spaceship on the screen
 *  INPUT point   The position of the ship
 *        angle   Which direction it is ponted
 *************************************************************************/
void drawBigShipShotGunUp(const Point & center, int rotation, bool thrust);

/************************************************************************
 * DRAW Shot gun Up
 * Draw a spaceship on the screen
 *  INPUT point   The position of the ship
 *        angle   Which direction it is ponted
 *************************************************************************/
void drawShotGunUp(const Point & center, int rotation, bool thrust);

/************************************************************************
 * DRAW Big Ship laser shot up
 * Draw a spaceship on the screen
 *  INPUT point   The position of the ship
 *        angle   Which direction it is ponted
 *************************************************************************/
void drawBigShipLaserShotUp(const Point & center, int rotation, bool thrust);
/************************************************************************
 * DRAW Ship
 * Draw a spaceship on the screen
 *  INPUT point   The position of the ship
 *        angle   Which direction it is ponted
 *************************************************************************/
void drawShipLaserShotUp(const Point & center, int rotation, bool thrust);
/************************************************************************
 * DRAW Big Ship laser damage
 * Draw a spaceship on the screen
 *  INPUT point   The position of the ship
 *        angle   Which direction it is ponted
 *************************************************************************/
void drawBigShipLaserDamage(const Point & center, int rotation, bool thrust);

/************************************************************************
 * DRAW Ship laser damaga up
 * Draw a spaceship on the screen
 *  INPUT point   The position of the ship
 *        angle   Which direction it is ponted
 *************************************************************************/
void drawShipLaserDamageUp(const Point & center, int rotation, bool thrust);

/************************************************************************
 * DRAW Alien bomber wing
 * Draw a spaceship on the screen
 *  INPUT point   The position of the ship
 *        angle   Which direction it is ponted
 *************************************************************************/
void drawAlienBomberWing(const Point & center, int rotation, bool thrust);



#endif // UI_DRAW_H
