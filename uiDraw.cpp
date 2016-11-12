 /***********************************************************************
 * Source File:
 *    User Interface Draw : put pixels on the screen
 * Author:
 *    Br. Helfrich
 * Summary:
 *    This is the code necessary to draw on the screen. We have a collection
 *    of procedural functions here because each draw function does not
 *    retain state. In other words, they are verbs (functions), not nouns
 *    (variables) or a mixture (objects)
 ************************************************************************/

#include <string>     // need you ask?
#include <sstream>    // convert an integer into text
#include <cassert>    // I feel the need... the need for asserts
#include <time.h>     // for clock


#ifdef __APPLE__
#include <openGL/gl.h>    // Main OpenGL library
#include <GLUT/glut.h>    // Second OpenGL library
#endif // __APPLE__

#ifdef __linux__
#include <GL/gl.h>        // Main OpenGL library
#include <GL/glut.h>      // Second OpenGL library
#endif // __linux__

#ifdef _WIN32
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>         // OpenGL library we copied 
#define _USE_MATH_DEFINES
#include <math.h>
#endif // _WIN32

#include "point.h"
#include "uiDraw.h"

using namespace std;

#define deg2rad(value) ((M_PI / 180) * (value))

/*********************************************
 * NUMBER OUTLINES
 * We are drawing the text for score and things
 * like that by hand to make it look "old school."
 * These are how we render each individual charactger.
 * Note how -1 indicates "done".  These are paired
 * coordinates where the even are the x and the odd
 * are the y and every 2 pairs represents a point
 ********************************************/
const char NUMBER_OUTLINES[10][20] =
{
  {0, 0,  7, 0,   7, 0,  7,10,   7,10,  0,10,   0,10,  0, 0,  -1,-1, -1,-1},//0
  {7, 0,  7,10,  -1,-1, -1,-1,  -1,-1, -1,-1,  -1,-1, -1,-1,  -1,-1, -1,-1},//1
  {0, 0,  7, 0,   7, 0,  7, 5,   7, 5,  0, 5,   0, 5,  0,10,   0,10,  7,10},//2
  {0, 0,  7, 0,   7, 0,  7,10,   7,10,  0,10,   4, 5,  7, 5,  -1,-1, -1,-1},//3
  {0, 0,  0, 5,   0, 5,  7, 5,   7, 0,  7,10,  -1,-1, -1,-1,  -1,-1, -1,-1},//4
  {7, 0,  0, 0,   0, 0,  0, 5,   0, 5,  7, 5,   7, 5,  7,10,   7,10,  0,10},//5
  {7, 0,  0, 0,   0, 0,  0,10,   0,10,  7,10,   7,10,  7, 5,   7, 5,  0, 5},//6
  {0, 0,  7, 0,   7, 0,  7,10,  -1,-1, -1,-1,  -1,-1, -1,-1,  -1,-1, -1,-1},//7
  {0, 0,  7, 0,   0, 5,  7, 5,   0,10,  7,10,   0, 0,  0,10,   7, 0,  7,10},//8
  {0, 0,  7, 0,   7, 0,  7,10,   0, 0,  0, 5,   0, 5,  7, 5,  -1,-1, -1,-1} //9
};

/************************************************************************
 * DRAW DIGIT
 * Draw a single digit in the old school line drawing style.  The
 * size of the glyph is 8x11 or x+(0..7), y+(0..10)
 *   INPUT  topLeft   The top left corner of the character
 *          digit     The digit we are rendering: '0' .. '9'
 *************************************************************************/
void drawDigit(const Point & topLeft, char digit)
{
   // we better be only drawing digits
   assert(isdigit(digit));
   if (!isdigit(digit))
      return;

   // compute the row as specified by the digit
   int r = digit - '0';
   assert(r >= 0 && r <= 9);

   // go through each segment.
   for (int c = 0; c < 20 && NUMBER_OUTLINES[r][c] != -1; c += 4)
   {
      assert(NUMBER_OUTLINES[r][c    ] != -1 &&
             NUMBER_OUTLINES[r][c + 1] != -1 &&
             NUMBER_OUTLINES[r][c + 2] != -1 &&
             NUMBER_OUTLINES[r][c + 3] != -1);

      //Draw a line based off of the num structure for each number
      Point start;
      start.setX(topLeft.getX() + NUMBER_OUTLINES[r][c]);
      start.setY(topLeft.getY() - NUMBER_OUTLINES[r][c + 1]);
      Point end;
      end.setX(topLeft.getX() + NUMBER_OUTLINES[r][c + 2]);
      end.setY(topLeft.getY() - NUMBER_OUTLINES[r][c + 3]);
         
      drawLine(start, end, 1.0, 1.0, 1.0);
   }
}

/*************************************************************************
 * DRAW NUMBER
 * Display an integer on the screen using the 7-segment method
 *   INPUT  topLeft   The top left corner of the character
 *          digit     The digit we are rendering: '0' .. '9'
 *************************************************************************/
void drawNumber(const Point & topLeft, long number)
{
   // our cursor, if you will. It will advance as we output digits
   Point point = topLeft;
   
   // is this negative
   bool isNegative = (number < 0);
   number *= (isNegative ? -1 : 1);
   
   // render the number as text
   ostringstream sout;
   sout << number;
   string text = sout.str();

   // handle the negative
   if (isNegative)
   {
      glBegin(GL_LINES);
      glVertex2f(point.getX() + 2, point.getY() - 10); // 1, -5
      glVertex2f(point.getX() + 10, point.getY() - 10); // 5, -5
      glEnd();
      point.addX(11);
   }
   
   // walk through the text one digit at a time
   for (const char *p = text.c_str(); *p; p++)
   {
      assert(isdigit(*p));
      drawDigit(point, *p);
      point.addX(11); // spacing between the numbersr
   }
}


/*************************************************************************
 * DRAW TEXT
 * Draw text using a simple bitmap font
 *   INPUT  topLeft   The top left corner of the text
 *          text      The text to be displayed
 ************************************************************************/
void drawText(const Point & topLeft, const char * text)
{
   void *pFont = GLUT_BITMAP_HELVETICA_12;  // also try _18

   // prepare to draw the text from the top-left corner
   glRasterPos2f(topLeft.getX(), topLeft.getY());

   // loop through the text
   for (const char *p = text; *p; p++)
      glutBitmapCharacter(pFont, *p);
}

/*************************************************************************
 * DRAW TEXT COLOR (SAM TRENT WORKING!!!)
 * Draws text in color!
 *    Just be sure to enter in the values for red, green and blue
 *
 ************************************************************************/
void drawTextColor(const Point & topLeft, const char * text, float red,
                   float green, float blue, float trans)
{
   // Get ready...for color
   //glBegin(GL_LINES);
   
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
 //  glBegin(GL_LINE_LOOP);
  
   
   
   glPushAttrib(GL_CURRENT_BIT);
   glColor4f(red, green, blue, trans);
   void *pFont = GLUT_BITMAP_HELVETICA_18;  // also try _18
   
   // prepare to draw the text from the top-left corner
   glRasterPos2f(topLeft.getX(), topLeft.getY());
   
   // loop through the text
   for (const char *p = text; *p; p++)
      glutBitmapCharacter(pFont, *p);
   
   glPopAttrib(); // This sets the colour back to its original value
   
   //   // Complete drawing color
      glColor3f(1.0 /* red % */, 1.0 /* green % */, 1.0 /* blue % */);
      glEnd();
}


/*************************************************************************
 * DRAW TEXT COLOR TINY
 * Draws text in color!
 *    Just be sure to enter in the values for red, green and blue
 *
 ************************************************************************/
void drawTextColorTiny(const Point & topLeft, const char * text, float red,
                   float green, float blue, float trans)
{
   // Get ready...for color
   //glBegin(GL_LINES);
   
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   //  glBegin(GL_LINE_LOOP);
   
   
   
   glPushAttrib(GL_CURRENT_BIT);
   glColor4f(red, green, blue, trans);
   void *pFont = GLUT_BITMAP_HELVETICA_12;  // also try _18
   
   // prepare to draw the text from the top-left corner
   glRasterPos2f(topLeft.getX(), topLeft.getY());
   
   // loop through the text
   for (const char *p = text; *p; p++)
      glutBitmapCharacter(pFont, *p);
   
   glPopAttrib(); // This sets the colour back to its original value
   
   //   // Complete drawing color
   glColor3f(1.0 /* red % */, 1.0 /* green % */, 1.0 /* blue % */);
   glEnd();
}

/*************************************************************************
 * DRAW TITLE
 * Draws text in color!
 *    Just be sure to enter in the values for red, green and blue
 *
 ************************************************************************/
void drawTitle(const Point & topLeft, const char * text, float red,
                   float green, float blue, float trans)
{
   // Get ready...for color
  // glBegin(GL_LINES);
   
   //trying to make transparacy with title
   //float retran = 1.0;
//   if (retran == 1.0)
//   {
//      for (float i = 0.0; i < 1.0 ; i += 0.1 )
//      {
//         retran -= 0.0001;
//         std::cout << "retran A: " << retran << std::endl;
//         
//         
//      }
//   }
//   if (retran >= 0.0)
//   {
//      for (float i = 0.0; i < 1.0; i += 0.1)
//      {
//         retran += 0.0001;
//         std::cout << "retran B: " << retran << std::endl;
//      }
//   }
   
   
   
   glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
   glDisable(GL_DEPTH_TEST);
   // glBegin(GL_POINTS);
    //glBegin(GL_LINE_LOOP);
   glColor4f(red, green, blue, trans);
 

   
   
   
   
   glPushAttrib(GL_CURRENT_BIT);
   void *pFont = GLUT_BITMAP_TIMES_ROMAN_24;  // also try _18
   
   // prepare to draw the text from the top-left corner
   glRasterPos2f(topLeft.getX(), topLeft.getY());
   
   // loop through the text
   for (const char *p = text; *p; p++)
      glutBitmapCharacter(pFont, *p);
   
   glPopAttrib(); // This sets the colour back to its original value
   
   //   // Complete drawing color
   glColor3f(1.0 /* red % */, 1.0 /* green % */, 1.0 /* blue % */);
   //   glEnd();
}


/************************************************************************
 * DRAW POLYGON
 * Draw a POLYGON from a given location (center) of a given size (radius).
 *  INPUT   center   Center of the polygon
 *          radius   Size of the polygon
 *          points   How many points will we draw it.  Larger the number,
 *                   the more line segments we will use
 *          rotation True circles are rotation independent.  However, if you
 *                   are drawing a 3-sided polygon (triangle), this matters!
 *************************************************************************/
void drawPolygon(const Point & center, int radius, int points, int rotation)
{
   // begin drawing
   glBegin(GL_LINE_LOOP);

   //loop around a circle the given number of times drawing a line from
   //one point to the next
   for (double i = 0; i < 2 * M_PI; i += (2 * M_PI) / points)
   {
      Point temp(false /*check*/);
      temp.setX(center.getX() + (radius * cos(i)));
      temp.setY(center.getY() + (radius * sin(i)));
      rotate(temp, center, rotation);
      glVertex2f(temp.getX(), temp.getY());
   }

   // complete drawing
   glEnd();

}


/************************************************************************
 * ROTATE
 * Rotate a given point (point) around a given origin (center) by a given
 * number of degrees (angle).
 *    INPUT  point    The point to be moved
 *           center   The center point we will rotate around
 *           rotation Rotation in degrees
 *    OUTPUT point    The new position
 *************************************************************************/
void rotate(Point & point, const Point & origin, int rotation)
{
   // because sine and cosine are expensive, we want to call them only once
   double cosA = cos(deg2rad(rotation));
   double sinA = sin(deg2rad(rotation));

   // remember our original point
   Point tmp(false /*check*/);
   tmp.setX(point.getX() - origin.getX());
   tmp.setY(point.getY() - origin.getY());

   // find the new values
   point.setX(static_cast<int> (tmp.getX() * cosA -
                                tmp.getY() * sinA) +
              origin.getX());
   point.setY(static_cast<int> (tmp.getX() * sinA +
                                tmp.getY() * cosA) +
              origin.getY());
}

/************************************************************************
 * DRAW LINE
 * Draw a line on the screen from the beginning to the end.
 *   INPUT  begin     The position of the beginning of the line
 *          end       The position of the end of the line
 *************************************************************************/
void drawLine(const Point & begin, const Point & end,
              float red, float green, float blue)
{
   // Get ready...
   glBegin(GL_LINES);
   glColor3f(red, green, blue);

   // Draw the actual line
   glVertex2f(begin.getX(), begin.getY());
   glVertex2f(  end.getX(),   end.getY());

   // Complete drawing
   glColor3f(1.0 /* red % */, 1.0 /* green % */, 1.0 /* blue % */);
   glEnd();
}



/***********************************************************************
 * DRAW Lander
 * Draw a moon-lander spaceship on the screen at a given point
 ***********************************************************************/
void drawLander(const Point & point)
{
   // ultra simple point
   struct PT
   {
      int x;
      int y;
   } points[] = 
   {
      {-6, 0}, {-10,0}, {-8, 0}, {-8, 3},  // left foot
      {-5, 4}, {-5, 7}, {-8, 3}, {-5, 4},  // left leg
      {-1, 4}, {-3, 2}, { 3, 2}, { 1, 4}, {-1, 4}, // bottom
      { 5, 4}, { 5, 7}, {-5, 7}, {-3, 7},  // engine square
      {-6,10}, {-6,13}, {-3,16}, { 3,16},   // left of habitat
      { 6,13}, { 6,10}, { 3, 7}, { 5, 7},   // right of habitat
      { 5, 4}, { 8, 3}, { 5, 7}, { 5, 4},  // right leg
      { 8, 3}, { 8, 0}, {10, 0}, { 6, 0}   // right foot
   };

   // draw it
   glBegin(GL_LINE_STRIP);
   for (int i = 0; i < sizeof(points) / sizeof(points[0]); i++)
        glVertex2f(point.getX() + points[i].x,
                   point.getY() + points[i].y);

   // complete drawing
   glEnd();
   
   
}


/***********************************************************************
 * DRAW Lander Flame
 * Draw the flames coming out of a moonlander for thrust
 ***********************************************************************/
void drawLanderFlames(const Point & point, 
                      bool bottom,
                      bool left,
                      bool right)
{
   // simple point
   struct PT
   {
      int x;
      int y;
   };

   int iFlame = random(0, 3);  // so the flame flickers
   
   // draw it
   glBegin(GL_LINE_LOOP);
   glColor3f(1.0 /* red % */, 0.0 /* green % */, 0.0 /* blue % */);
   
   // bottom thrust
   if (bottom)
   {
      PT points[3][3] =
      {
         { {-5,  -6}, { 0, -1}, { 3, -10} },
         { {-3,  -6}, {-1, -2}, { 0, -15} },
         { { 2, -12}, { 1,  0}, { 6,  -4} }
      };
      
      glVertex2f(point.getX() - 2, point.getY() + 2);
      for (int i = 0; i < 3; i++)
         glVertex2f(point.getX() + points[iFlame][i].x,
                   point.getY() + points[iFlame][i].y);
      glVertex2f(point.getX() + 2, point.getY() + 2);
   }

   // right thrust
   if (right)
   {
      PT points[3][3] =
      {
         { {10, 14}, { 8, 12}, {12, 12} },
         { {12, 10}, { 8, 10}, {10,  8} },
         { {14, 11}, {14, 11}, {14, 11} }
      };
      
      glVertex2f(point.getX() + 6, point.getY() + 12);
      for (int i = 0; i < 3; i++)
         glVertex2f(point.getX() + points[iFlame][i].x,
                    point.getY() + points[iFlame][i].y);
      glVertex2f(point.getX() + 6, point.getY() + 10);
   }

   // left thrust
   if (left)
   {
      PT points[3][3] =
      {
         { {-10, 14}, { -8, 12}, {-12, 12} },
         { {-12, 10}, { -8, 10}, {-10,  8} },
         { {-14, 11}, {-14, 11}, {-14, 11} }
      };
      
      glVertex2f(point.getX() - 6, point.getY() + 12);
      for (int i = 0; i < 3; i++)
         glVertex2f(point.getX() + points[iFlame][i].x,
                    point.getY() + points[iFlame][i].y);
      glVertex2f(point.getX() - 6, point.getY() + 10);
   }

   glColor3f(1.0 /* red % */, 1.0 /* green % */, 1.0 /* blue % */);
   glEnd();
}


/******************************************************************
 * RANDOM
 * This function generates a random number.  
 *
 *    INPUT:   min, max : The number of values (min <= num < max)
 *    OUTPUT   <return> : Return the integer
 ****************************************************************/
int random(int min, int max)
{
   assert(min <= max);
   int num = (rand() % (max - min)) + min;
   assert(min <= num && num <= max);

   return num;
}

/******************************************************************
 * RANDOM
 * This function generates a random number.  
 *
 *    INPUT:   min, max : The number of values (min <= num < max)
 *    OUTPUT   <return> : Return the double
 ****************************************************************/
double random(double min, double max)
{
   assert(min <= max);
   double num = min + ((double)rand() / (double)RAND_MAX * (max - min));
   
   assert(min <= num && num <= max);

   return num;
}


/************************************************************************
 * DRAW RECTANGLE
 * Draw a rectangle on the screen centered on a given point (center) of
 * a given size (width, height), and at a given orientation (rotation)
 *  INPUT  center    Center of the rectangle
 *         width     Horizontal size
 *         height    Vertical size
 *         rotation  Orientation
 *************************************************************************/
void drawRect(const Point & center, int width, int height, int rotation)
{
   Point tl(false /*check*/); // top left
   Point tr(false /*check*/); // top right 
   Point bl(false /*check*/); // bottom left
   Point br(false /*check*/); // bottom right

   //Top Left point
   tl.setX(center.getX() - (width  / 2));
   tl.setY(center.getY() + (height / 2));

   //Top right point
   tr.setX(center.getX() + (width  / 2));
   tr.setY(center.getY() + (height / 2));

   //Bottom left point
   bl.setX(center.getX() - (width  / 2));
   bl.setY(center.getY() - (height / 2));

   //Bottom right point
   br.setX(center.getX() + (width  / 2));
   br.setY(center.getY() - (height / 2));

   //Rotate all points the given degrees
   rotate(tl, center, rotation);
   rotate(tr, center, rotation);
   rotate(bl, center, rotation);
   rotate(br, center, rotation);

   //Finally draw the rectangle
   glBegin(GL_LINE_STRIP);
   glVertex2f(tl.getX(), tl.getY());
   glVertex2f(tr.getX(), tr.getY());
   glVertex2f(br.getX(), br.getY());
   glVertex2f(bl.getX(), bl.getY());
   glVertex2f(tl.getX(), tl.getY());
   glEnd();
}

/************************************************************************
 * DRAW LASER
 * Draw a rectangle THATS A LASER on the screen centered on a given point (center) of
 * a given size (width, height), and at a given orientation (rotation)
 *  INPUT  center    Center of the rectangle
 *         width     Horizontal size
 *         height    Vertical size
 *         rotation  Orientation
 *************************************************************************/
void drawLaser(const Point & center, int width, int height, int rotation, float red, float green, float blue)
{
   glBegin(GL_LINE_LOOP);
   glColor3f(red /* red % */, green /* green % */, blue /* blue % */);

   Point tl(false /*check*/); // top left
   Point tr(false /*check*/); // top right
   Point bl(false /*check*/); // bottom left
   Point br(false /*check*/); // bottom right
   
   //Top Left point
   tl.setX(center.getX() - (width  / 2));
   tl.setY(center.getY() + (height / 2));
   
   //Top right point
   tr.setX(center.getX() + (width  / 2));
   tr.setY(center.getY() + (height / 2));
   
   //Bottom left point
   bl.setX(center.getX() - (width  / 2));
   bl.setY(center.getY() - (height / 2));
   
   //Bottom right point
   br.setX(center.getX() + (width  / 2));
   br.setY(center.getY() - (height / 2));
   
   //Rotate all points the given degrees
   rotate(tl, center, rotation);
   rotate(tr, center, rotation);
   rotate(bl, center, rotation);
   rotate(br, center, rotation);
   
   //Finally draw the rectangle
   glBegin(GL_LINE_STRIP);
   glVertex2f(tl.getX(), tl.getY());
   glVertex2f(tr.getX(), tr.getY());
   glVertex2f(br.getX(), br.getY());
   glVertex2f(bl.getX(), bl.getY());
   glVertex2f(tl.getX(), tl.getY());
   glColor3f(1.0 /* red % */, 1.0 /* green % */, 1.0 /* blue % */);
   glEnd();
}

/************************************************************************
 * DRAW RectColor
 * Draw a rectangle THATS A LASER on the screen centered on a given point (center) of
 * a given size (width, height), and at a given orientation (rotation)
 *  INPUT  center    Center of the rectangle
 *         width     Horizontal size
 *         height    Vertical size
 *         rotation  Orientation
 *************************************************************************/
void drawRectColor(const Point & center, int width, int height, int rotation, float red, float green, float blue)
{
   glBegin(GL_LINE_LOOP);
   glColor3f(red /* red % */, green /* green % */, blue /* blue % */);
   
   Point tl(false /*check*/); // top left
   Point tr(false /*check*/); // top right
   Point bl(false /*check*/); // bottom left
   Point br(false /*check*/); // bottom right
   
   //Top Left point
   tl.setX(center.getX() - (width  / 2));
   tl.setY(center.getY() + (height / 2));
   
   //Top right point
   tr.setX(center.getX() + (width  / 2));
   tr.setY(center.getY() + (height / 2));
   
   //Bottom left point
   bl.setX(center.getX() - (width  / 2));
   bl.setY(center.getY() - (height / 2));
   
   //Bottom right point
   br.setX(center.getX() + (width  / 2));
   br.setY(center.getY() - (height / 2));
   
   //Rotate all points the given degrees
   rotate(tl, center, rotation);
   rotate(tr, center, rotation);
   rotate(bl, center, rotation);
   rotate(br, center, rotation);
   
   //Finally draw the rectangle
   glBegin(GL_LINE_STRIP);
   glVertex2f(tl.getX(), tl.getY());
   glVertex2f(tr.getX(), tr.getY());
   glVertex2f(br.getX(), br.getY());
   glVertex2f(bl.getX(), bl.getY());
   glVertex2f(tl.getX(), tl.getY());
   glColor3f(1.0 /* red % */, 1.0 /* green % */, 1.0 /* blue % */);
   glEnd();
}



/************************************************************************
 * DRAW SHIP DEATH
 * Draw a rectangle on the screen centered on a given point (center) of
 * a given size (width, height), and at a given orientation (rotation)
 *  INPUT  center    Center of the rectangle
 *         width     Horizontal size
 *         height    Vertical size
 *         rotation  Orientation
 *************************************************************************/
void drawShipDeath(const Point & center, int width, int height, int rotation)
{
   
   
   glEnable(GL_BLEND);
   // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
   glDisable(GL_DEPTH_TEST);
   // glBegin(GL_POINTS);
   // glBegin(GL_LINE_LOOP);
   
   
   
   glPushAttrib(GL_CURRENT_BIT);
   glColor4f(1.0, 0.0, 0.0, 1.0);
   
   Point tl(false /*check*/); // top left
   Point tr(false /*check*/); // top right
   Point bl(false /*check*/); // bottom left
   Point br(false /*check*/); // bottom right
   
   //Top Left point
   tl.setX(center.getX() - (width  / 2));
   tl.setY(center.getY() + (height / 2));
   
   //Top right point
   tr.setX(center.getX() + (width  / 2));
   tr.setY(center.getY() + (height / 2));
   
   //Bottom left point
   bl.setX(center.getX() - (width  / 2));
   bl.setY(center.getY() - (height / 2));
   
   //Bottom right point
   br.setX(center.getX() + (width  / 2));
   br.setY(center.getY() - (height / 2));
   
   //Rotate all points the given degrees
   rotate(tl, center, rotation);
   rotate(tr, center, rotation);
   rotate(bl, center, rotation);
   rotate(br, center, rotation);
   
   //Finally draw the rectangle
   glBegin(GL_LINE_STRIP);
   glVertex2f(tl.getX(), tl.getY());
   glVertex2f(tr.getX(), tr.getY());
   glVertex2f(br.getX(), br.getY());
   glVertex2f(bl.getX(), bl.getY());
   glVertex2f(tl.getX(), tl.getY());
   
   glColor3f(1.0 /* red % */, 1.0 /* green % */, 1.0 /* blue % */);
   glEnd();
}

/************************************************************************
 * DRAW  Alien SHIP DEATH
 * Draw a rectangle on the screen centered on a given point (center) of
 * a given size (width, height), and at a given orientation (rotation)
 *  INPUT  center    Center of the rectangle
 *         width     Horizontal size
 *         height    Vertical size
 *         rotation  Orientation
 *************************************************************************/
void drawAlienShipDeath(const Point & center, int width, int height, int rotation)
{
   
   
   glEnable(GL_BLEND);
   // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
   glDisable(GL_DEPTH_TEST);
   // glBegin(GL_POINTS);
   // glBegin(GL_LINE_LOOP);
   
   
   
   glPushAttrib(GL_CURRENT_BIT);
   glColor4f(1.0, 0.0, 0.0, 1.0);
   
   Point tl(false /*check*/); // top left
   Point tr(false /*check*/); // top right
   Point bl(false /*check*/); // bottom left
   Point br(false /*check*/); // bottom right
   
   //Top Left point
   tl.setX(center.getX() - (width  / 2));
   tl.setY(center.getY() + (height / 2));
   
   //Top right point
   tr.setX(center.getX() + (width  / 2));
   tr.setY(center.getY() + (height / 2));
   
   //Bottom left point
   bl.setX(center.getX() - (width  / 2));
   bl.setY(center.getY() - (height / 2));
   
   //Bottom right point
   br.setX(center.getX() + (width  / 2));
   br.setY(center.getY() - (height / 2));
   
   //Rotate all points the given degrees
   rotate(tl, center, rotation);
   rotate(tr, center, rotation);
   rotate(bl, center, rotation);
   rotate(br, center, rotation);
   
   //Finally draw the rectangle
   glBegin(GL_LINE_STRIP);
   glVertex2f(tl.getX(), tl.getY());
   glVertex2f(tr.getX(), tr.getY());
   glVertex2f(br.getX(), br.getY());
   glVertex2f(bl.getX(), bl.getY());
   glVertex2f(tl.getX(), tl.getY());
   
   glColor3f(1.0 /* red % */, 1.0 /* green % */, 1.0 /* blue % */);
   glEnd();
}


/************************************************************************
 * DRAW  Warp field
 * Draw a rectangle on the screen centered on a given point (center) of
 * a given size (width, height), and at a given orientation (rotation)
 *  INPUT  center    Center of the rectangle
 *         width     Horizontal size
 *         height    Vertical size
 *         rotation  Orientation
 *************************************************************************/
void drawWarpField(const Point & center, int width, int height, int rotation)
{
   
   
   glEnable(GL_BLEND);
   // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
   glDisable(GL_DEPTH_TEST);
   // glBegin(GL_POINTS);
   // glBegin(GL_LINE_LOOP);
   
   
   
   glPushAttrib(GL_CURRENT_BIT);
   glColor4f(0.0, 1.0, 1.0, 1.0);
   
   Point tl(false /*check*/); // top left
   Point tr(false /*check*/); // top right
   Point bl(false /*check*/); // bottom left
   Point br(false /*check*/); // bottom right
   
   //Top Left point
   tl.setX(center.getX() - (width  / 2));
   tl.setY(center.getY() + (height / 2));
   
   //Top right point
   tr.setX(center.getX() + (width  / 2));
   tr.setY(center.getY() + (height / 2));
   
   //Bottom left point
   bl.setX(center.getX() - (width  / 2));
   bl.setY(center.getY() - (height / 2));
   
   //Bottom right point
   br.setX(center.getX() + (width  / 2));
   br.setY(center.getY() - (height / 2));
   
   //Rotate all points the given degrees
   rotate(tl, center, rotation);
   rotate(tr, center, rotation);
   rotate(bl, center, rotation);
   rotate(br, center, rotation);
   
   //Finally draw the rectangle
   glBegin(GL_LINE_STRIP);
   glVertex2f(tl.getX(), tl.getY());
   glVertex2f(tr.getX(), tr.getY());
   glVertex2f(br.getX(), br.getY());
   glVertex2f(bl.getX(), bl.getY());
   glVertex2f(tl.getX(), tl.getY());
   
   glColor3f(1.0 /* red % */, 1.0 /* green % */, 1.0 /* blue % */);
   glEnd();
}





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
              float red, float green, float blue)
{
  
  
   glEnable(GL_BLEND);
   glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
   glDisable(GL_DEPTH_TEST);
   glPushAttrib(GL_CURRENT_BIT);
   
   
   glColor4f(red, green, blue, 1.0);
   
   
   Point tl(false /*check*/); // top left
   Point tr(false /*check*/); // top right
   Point bl(false /*check*/); // bottom left
   Point br(false /*check*/); // bottom right
   
   //Top Left point
   tl.setX(center.getX() - (width  / 2));
   tl.setY(center.getY() + (height / 2));
   
   //Top right point
   tr.setX(center.getX() + (width  / 2));
   tr.setY(center.getY() + (height / 2));
   
   //Bottom left point
   bl.setX(center.getX() - (width  / 2));
   bl.setY(center.getY() - (height / 2));
   
   //Bottom right point
   br.setX(center.getX() + (width  / 2));
   br.setY(center.getY() - (height / 2));
   
   //Rotate all points the given degrees
   rotate(tl, center, rotation);
   rotate(tr, center, rotation);
   rotate(bl, center, rotation);
   rotate(br, center, rotation);
   
   //Finally draw the rectangle
   glBegin(GL_LINE_STRIP);
   glVertex2f(tl.getX(), tl.getY());
   glVertex2f(tr.getX(), tr.getY());
   glVertex2f(br.getX(), br.getY());
   glVertex2f(bl.getX(), bl.getY());
   glVertex2f(tl.getX(), tl.getY());
   glColor3f(1.0 /* red % */, 1.0 /* green % */, 1.0 /* blue % */);

   glEnd();
}



/************************************************************************
 * DRAW CIRCLE
 * Draw a circle from a given location (center) of a given size (radius).
 *  INPUT   center   Center of the circle
 *          radius   Size of the circle
 *************************************************************************/
void drawCircle(const Point & center, int radius)
{
   assert(radius > 1.0);
   const double increment = 1.0 / (double)radius;

   // begin drawing
   glBegin(GL_LINE_LOOP);

   // go around the circle
   for (double radians = 0; radians < M_PI * 2.0; radians += increment)
      glVertex2f(center.getX() + (radius * cos(radians)),
                 center.getY() + (radius * sin(radians)));
   
   // complete drawing
   glEnd();   
}
/************************************************************************
 * DRAW SHIELDS
 * Draw a circle from a given location (center) of a given size (radius).
 *  INPUT   center   Center of the circle
 *          radius   Size of the circle
 *************************************************************************/
void drawSheilds(const Point & center, int radius, float red, float green, float blue, float trans)
{
   assert(radius > 1.0);
   const double increment = 1.0 / (double)radius;
   
   // begin drawing
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glBegin(GL_LINE_LOOP);
 
   glColor4f(red, green, blue, trans);
   
   // go around the circle
   for (double radians = 0; radians < M_PI * 2.0; radians += increment)
      glVertex2f(center.getX() + (radius * cos(radians)),
                 center.getY() + (radius * sin(radians)));
   
   // complete drawing
   glColor3f(1.0 /* red % */, 1.0 /* green % */, 1.0 /* blue % */);
   glEnd();
}

/************************************************************************
 * DRAW ALIEN SHIELDS
 * Draw a circle from a given location (center) of a given size (radius).
 *  INPUT   center   Center of the circle
 *          radius   Size of the circle
 *************************************************************************/
void drawAlienSheilds(const Point & center, int radius, float red, float green, float blue, float trans)
{
   assert(radius > 1.0);
   const double increment = 1.0 / (double)radius;
   
   // begin drawing
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glBegin(GL_LINE_LOOP);
   
   glColor4f(red, green, blue, trans);
   
   // go around the circle
   for (double radians = 0; radians < M_PI * 2.0; radians += increment)
      glVertex2f(center.getX() + (radius * cos(radians)),
                 center.getY() + (radius * sin(radians)));
   
   // complete drawing
   glColor3f(1.0 /* red % */, 1.0 /* green % */, 1.0 /* blue % */);
   glEnd();
}

/************************************************************************
 * DRAW DOT
 * Draw a single point on the screen, 2 pixels by 2 pixels
 *  INPUT point   The position of the dow
 *************************************************************************/
void drawDot(const Point & point)
{
   // Get ready, get set...
   glBegin(GL_POINTS);

   // Go...
   glVertex2f(point.getX(),     point.getY()    );
   glVertex2f(point.getX() + 1, point.getY()    );
   glVertex2f(point.getX() + 1, point.getY() + 1);
   glVertex2f(point.getX(),     point.getY() + 1);

   // Done!  OK, that was a bit too dramatic
   glEnd();
}

/************************************************************************
 * DRAW DebrisBits
 * Draw a single point on the screen, 2 pixels by 2 pixels
 *  INPUT point   The position of the dow
 *************************************************************************/
void drawDebrisBits(const Point & point)
{
   // Get ready, get set...
   glBegin(GL_POINTS);
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glBegin(GL_LINE_LOOP);
   
   glColor4f(1.0, 0.0, 0.0, 1.0);

   
   // Go...
   glVertex2f(point.getX(),     point.getY()    );
   glVertex2f(point.getX() + 1, point.getY()    );
   glVertex2f(point.getX() + 1, point.getY() + 1);
   glVertex2f(point.getX(),     point.getY() + 1);
   
   // Done!  OK, that was a bit too dramatic
    glColor3f(1.0 /* red % */, 1.0 /* green % */, 1.0 /* blue % */);
   glEnd();
}

/************************************************************************
 * DRAW ALIENDebrisBits
 * Draw a single point on the screen, 2 pixels by 2 pixels
 *  INPUT point   The position of the dow
 *************************************************************************/
void drawAlienDebrisBits(const Point & point)
{
   // Get ready, get set...
   glBegin(GL_POINTS);
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glBegin(GL_LINE_LOOP);
   
   glColor4f(0.0, 1.0, 1.0, 1.0);
   
   
   // Go...
   glVertex2f(point.getX(),     point.getY()    );
   glVertex2f(point.getX() + 1, point.getY()    );
   glVertex2f(point.getX() + 1, point.getY() + 1);
   glVertex2f(point.getX(),     point.getY() + 1);
   
   // Done!  OK, that was a bit too dramatic
   glColor3f(1.0 /* red % */, 1.0 /* green % */, 1.0 /* blue % */);
   glEnd();
}


/************************************************************************
 * DRAW star
 * Draw a single point on the screen, 2 pixels by 2 pixels
 *  INPUT point   The position of the dow
 *************************************************************************/
void drawStar(const Point & point, float trans)
{
   // Get ready, get set...
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glBegin(GL_POINTS);
   glBegin(GL_LINE_LOOP);
   glColor4f(1.0 /* red % */, 1.0 /* green % */, 1.0 /* blue % */, trans);

   
   // Go...
   glVertex2f(point.getX(),     point.getY()    );
   glVertex2f(point.getX() + 1, point.getY()    );
   glVertex2f(point.getX() + 1, point.getY() + 1);
   glVertex2f(point.getX(),     point.getY() + 1);
   
   // Done!  OK, that was a bit too dramatic
   glColor3f(1.0, 1.0, 1.0); // reset to white

   glEnd();
}


/************************************************************************
 * DRAW Tough Bird
 * Draw a tough bird on the screen
 *  INPUT point   The position of the sacred
 *        radius  The size of the bird
 *        hits    How many its remaining to kill the bird 
 *************************************************************************/
void drawToughBird(const Point & center, float radius, int hits)
{
   assert(radius > 1.0);
   const double increment = M_PI / 6.0;
   
   // begin drawing
   glBegin(GL_TRIANGLES);   

   // three points: center, pt1, pt2
   Point pt1(false /*check*/);
   pt1.setX(center.getX() + (radius * cos(0.0)));
   pt1.setY(center.getY() + (radius * sin(0.0)));   
   Point pt2(pt1);

   // go around the circle
   for (double radians = increment;
        radians <= M_PI * 2.0 + .5;
        radians += increment)
   {
      pt2.setX(center.getX() + (radius * cos(radians)));
      pt2.setY(center.getY() + (radius * sin(radians)));

      glVertex2f(center.getX(), center.getY());
      glVertex2f(pt1.getX(),    pt1.getY()   );
      glVertex2f(pt2.getX(),    pt2.getY()   );
      
      pt1 = pt2;
   }
      
   // complete drawing
   glEnd();   

   // draw the score in the center
   if (hits > 0 && hits < 10)
   {
      glColor3f(0.0 /* red % */, 0.0 /* green % */, 0.0 /* blue % */);
      glRasterPos2f(center.getX() - 4, center.getY() - 3);
      glutBitmapCharacter(GLUT_BITMAP_8_BY_13, (char)(hits + '0'));
      glColor3f(1.0, 1.0, 1.0); // reset to white
   }
}

/************************************************************************
 * DRAW SAUCER
 * Draw a tough bird on the screen
 *  INPUT point   The position of the sacred
 *        radius  The size of the bird
 *        hits    How many its remaining to kill the bird
 *************************************************************************/
void drawSaucer(const Point & center, float radius, int hits)
{
   assert(radius > 1.0);
   const double increment = M_PI / 6.0;
   
   // begin drawing
   glBegin(GL_TRIANGLES);
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glBegin(GL_POINTS);
   glBegin(GL_LINE_LOOP);
   glColor4f(0.0 /* red % */, 1.0 /* green % */, 0.0 /* blue % */, 1.0);
   
   // three points: center, pt1, pt2
   Point pt1(false /*check*/);
   pt1.setX(center.getX() + (radius * cos(0.0)));
   pt1.setY(center.getY() + (radius * sin(0.0)));
   Point pt2(pt1);
   
   // go around the circle
   for (double radians = increment;
        radians <= M_PI * 2.0 + .5;
        radians += increment)
   {
      pt2.setX(center.getX() + (radius * cos(radians)));
      pt2.setY(center.getY() + (radius * sin(radians)));
      
      glVertex2f(center.getX(), center.getY());
      glVertex2f(pt1.getX(),    pt1.getY()   );
      glVertex2f(pt2.getX(),    pt2.getY()   );
      
      pt1 = pt2;
   }
   
   // complete drawing
   glEnd();
   
   // draw the score in the center
   if (hits > 0 && hits < 10)
   {
      glColor3f(0.0 /* red % */, 0.0 /* green % */, 0.0 /* blue % */);
      glRasterPos2f(center.getX() - 4, center.getY() - 3);
      glutBitmapCharacter(GLUT_BITMAP_8_BY_13, (char)(hits + '0'));
      glColor3f(1.0, 1.0, 1.0); // reset to white
   }
}



/************************************************************************
 * DRAW Sacred Bird
 * Draw a sacred bird on the screen
 *  INPUT point   The position of the sacred
 *        radius  The size of the bird
 *************************************************************************/
void drawSacredBird(const Point & center, float radius)
{
   // handle auto-rotation
   static float rotation = 0.0;   
   rotation += 5.0;

   
   // begin drawing
   glBegin(GL_LINE_LOOP);
   glColor3f(1.0 /* red % */, 0.0 /* green % */, 0.0 /* blue % */);

   
   //loop around a circle the given number of times drawing a line from
   //one point to the next
   for (int i = 0; i < 5; i++)
   {
      Point temp(false /*check*/);
      float radian = (float)i * (M_PI * 2.0) * 0.4;
      temp.setX(center.getX() + (radius * cos(radian)));
      temp.setY(center.getY() + (radius * sin(radian)));
      rotate(temp, center, rotation);
      glVertex2f(temp.getX(), temp.getY());
   }
   
   // complete drawing
   glColor3f(1.0, 1.0, 1.0); // reset to white
   glEnd();   
}

/**********************************************************************
 * DRAW SMALL ASTEROID
 **********************************************************************/
void drawSmallAsteroid( const Point & center, int rotation)
{
   // ultra simple point
   struct PT
   {
      int x;
      int y;
   } points[] = 
   {
      {-5, 9},  {4, 8},   {8, 4},   
      {8, -5},  {-2, -8}, {-2, -3}, 
      {-8, -4}, {-8, 4},  {-5, 10}
   };
   
   glBegin(GL_LINE_STRIP);
   for (int i = 0; i < sizeof(points)/sizeof(PT); i++)
   {
      Point pt(center.getX() + points[i].x, 
               center.getY() + points[i].y);
      rotate(pt, center, rotation);
      glVertex2f(pt.getX(), pt.getY());
   }
   glEnd();
}

/**********************************************************************
 * DRAW ALIEN ASTEROID!
 **********************************************************************/
void drawAlienAsteroid( const Point & center, int rotation)
{
   // begin drawing
   glBegin(GL_LINE_LOOP);
   glColor3f(0.0 /* red % */, 1.0 /* green % */, 0.0 /* blue % */);

   // ultra simple point
   struct PT
   {
      int x;
      int y;
   } points[] =
   {
      {-5, 9},  {4, 8},   {8, 4},
      {8, -5},  {-2, -8}, {-2, -3},
      {-8, -4}, {-8, 4},  {-5, 10}
   };
   
   glBegin(GL_LINE_STRIP);
   for (int i = 0; i < sizeof(points)/sizeof(PT); i++)
   {
      Point pt(center.getX() + points[i].x,
               center.getY() + points[i].y);
      rotate(pt, center, rotation);
      glVertex2f(pt.getX(), pt.getY());
   }
   glColor3f(1.0, 1.0, 1.0); // reset to white
   glEnd();
}


/**********************************************************************
 * DRAW MEDIUM ASTEROID
 **********************************************************************/
void drawMediumAsteroid( const Point & center, int rotation)
{
   // ultra simple point
   struct PT
   {
      int x;
      int y;
   } points[] = 
   {
      {2, 8},    {8, 15},    {12, 8}, 
      {6, 2},    {12, -6},   {2, -15},
      {-6, -15}, {-14, -10}, {-15, 0},
      {-4, 15},  {2, 8}
   };
   
   glBegin(GL_LINE_STRIP);
   for (int i = 0; i < sizeof(points)/sizeof(PT); i++)
   {
      Point pt(center.getX() + points[i].x, 
               center.getY() + points[i].y);
      rotate(pt, center, rotation);
      glVertex2f(pt.getX(), pt.getY());
   }
   glEnd();
}

/**********************************************************************
 * DRAW LARGE ASTEROID
 **********************************************************************/
void drawLargeAsteroid( const Point & center, int rotation)
{
   // ultra simple point
   struct PT
   {
      int x;
      int y;
   } points[] = 
   {
      {0, 12},    {8, 20}, {16, 14},
      {10, 12},   {20, 0}, {0, -20},
      {-18, -10}, {-20, -2}, {-20, 14},
      {-10, 20},  {0, 12}
   };
   
   //use this for your alien ship!
//   
//   int radius = 15 ;
//   
//   assert(radius > 1.0);
//   const double increment = M_PI / 6.0;
//   
   // begin drawing
   
   
   
//   glBegin(GL_TRIANGLES);
//   glEnable(GL_BLEND);
//   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//  // glBegin(GL_POINTS);
//   glBegin(GL_LINE_LOOP);
//   glColor3f(1.0 /* red % */, 0.0 /* green % */, 0.0 /* blue % */);
//   
//   // three points: center, pt1, pt2
//   Point pt1(false /*check*/);
//   pt1.setX(center.getX() + (radius * cos(0.0)));
//   pt1.setY(center.getY() + (radius * sin(0.0)));
//   Point pt2(pt1);
//   
//   // go around the circle
//   for (double radians = increment;
//        radians <= M_PI * 2.0 + .5;
//        radians += increment)
//   {
//      pt2.setX(center.getX() + (radius * cos(radians)));
//      pt2.setY(center.getY() + (radius * sin(radians)));
//      
//      glVertex2f(center.getX(), center.getY());
//      glVertex2f(pt1.getX(),    pt1.getY()   );
//      glVertex2f(pt2.getX(),    pt2.getY()   );
//      
//      pt1 = pt2;
//   }
   

   
   
   
   
   
   glBegin(GL_LINE_STRIP);
   for (int i = 0; i < sizeof(points)/sizeof(PT); i++)
   {  
      Point pt(center.getX() + points[i].x, 
               center.getY() + points[i].y);
      rotate(pt, center, rotation);
      glVertex2f(pt.getX(), pt.getY());
   }
   
   glColor3f(1.0 /* red % */, 1.0 /* green % */, 1.0 /* blue % */);

   glEnd();
}

/**********************************************************************
 * DRAW AlienShipPart
 **********************************************************************/
void drawAlienShipPart( const Point & center, int rotation, float red, float green, float blue)
{
   // ultra simple point
   struct PT
   {
      int x;
      int y;
   }
 //  points[] =
//   {
//      {0, 12},    {8, 20}, {16, 14},
//      {10, 12},   {20, 0}, {0, -20},
//      {-18, -10}, {-20, -2}, {-20, 14},
//      {-10, 20},  {0, 12}
//   };
   
   points[] =
   {
      {0, 24},    {16, 40}, {32, 28},
      {20, 24},   {40, 0}, {0, -40},
      {-36, -20}, {-40, -4}, {-40, 28},
      {-40, 80},  {0, 48}
   };

   
   //use this for your alien ship!
   
      int radius = 15 ;
   
      assert(radius > 1.0);
      const double increment = M_PI / 6.0;
   
   // begin drawing
   
   
   
      glBegin(GL_TRIANGLES);
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glBegin(GL_POINTS);
      glBegin(GL_LINE_LOOP);
      glColor3f(red /* red % */, green /* green % */, blue /* blue % */);
   
      // three points: center, pt1, pt2
//      Point pt1(false /*check*/);
//      pt1.setX(center.getX() + (radius * cos(0.0)));
//      pt1.setY(center.getY() + (radius * sin(0.0)));
//      Point pt2(pt1);
   
      // go around the circle
      for (double radians = increment;
           radians <= M_PI * 2.0 + .5;
           radians += increment)
//      {
//         pt2.setX(center.getX() + (radius * cos(radians)));
//         pt2.setY(center.getY() + (radius * sin(radians)));
//   
//         glVertex2f(center.getX(), center.getY());
//         glVertex2f(pt1.getX(),    pt1.getY()   );
//         glVertex2f(pt2.getX(),    pt2.getY()   );
//   
//         pt1 = pt2;
//      }
//   
   
   
   
   
   
   
   glBegin(GL_LINE_STRIP);
   for (int i = 0; i < sizeof(points)/sizeof(PT); i++)
   {
      Point pt(center.getX() + points[i].x,
               center.getY() + points[i].y);
      rotate(pt, center, rotation);
      glVertex2f(pt.getX(), pt.getY());
   }
   
    glColor3f(1.0 /* red % */, 1.0 /* green % */, 1.0 /* blue % */);
   
   glEnd();
}

/**********************************************************************
 * DRAW Alien ship part medium soild
 **********************************************************************/
void drawAlienShipPartSolid( const Point & center, int rotation, float red, float green, float blue)
{
   // ultra simple point
   struct PT
   {
      int x;
      int y;
   } points[] =
   {
      {2, 8},    {8, 15},    {12, 8},
      {6, 2},    {12, -6},   {2, -15},
      {-6, -15}, {-14, -10}, {-15, 0},
      {-4, 15},  {2, 8}
   };
   
   glBegin(GL_TRIANGLES);
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glBegin(GL_POINTS);
   glBegin(GL_LINE_LOOP);
   glColor3f(red /* red % */, green /* green % */, blue /* blue % */);
   

   
   glBegin(GL_LINE_STRIP);
   for (int i = 0; i < sizeof(points)/sizeof(PT); i++)
   {
      Point pt(center.getX() + points[i].x,
               center.getY() + points[i].y);
      rotate(pt, center, rotation);
      glVertex2f(pt.getX(), pt.getY());
   }
   glColor3f(1.0 /* red % */, 1.0 /* green % */, 1.0 /* blue % */);

   glEnd();
}


/**********************************************************************
 * DRAW AlienShipPartSide
 **********************************************************************/
void drawAlienShipPartSide( const Point & center, int rotation, float red, float green, float blue)
{
   // ultra simple point
   struct PT
   {
      int x;
      int y;
   }
   //small
//     points[] =
//      {
//         {0, 12},    {8, 20}, {16, 14},
//         {10, -24},   {20, -10}, {-10, -20},
//         {-18, -10}, {-20, -2}, {-20, 14},
//         {-10, 20},  {0, 12}
//      };
   //custom
//   points[] =
//   {
//    //  {0, 0},    {20, 20}, {40, 40},
////      {15, 15},   {30, 0}, {0, -30},
//      {-21, -15}, {-30, -4}, {-30, 21}, // top
//      {21, 15}, {30, -4}, {30, 21},     // bottom
//     // {-18, -10}, {8, -8}, {-10, 7}     //left side
//    //  {-10, 20},  {0, 12},
//     // {20, 30}, {30, 40}
   //};
   //big
//   points[] =
//   {
//      {0, 24},    {16, 40}, {32, 28},
//   //   {20, 24},   {40, 0}, {0, -40},
//      {-36, -20}, {-40, -4}, {-40, 28},
//      {-40, 80},  {0, 48}
//   };
   
   // medium
   points[] =
   {
     // {4, 16},    {16, 30},    {24, 16},
     // {24, 8},    {48, -24},   {8, -60},
      {-36, -20}, {-40, -4}, {-40, 28},
     // {36, 20}, {40, 4}, {40, 28},
    //  {-40, 80},  {0, 48},
      {12, 30}, {28, 20}, {30, 0},
      {-12, -30}, {-28, -20}, {-30, 0},
      {-8, 30},  {4, 16}
   };
   
   
   //use this for your alien ship!
   
   int radius = 15 ;
   
   assert(radius > 1.0);
   const double increment = M_PI / 6.0;
   
   // begin drawing
   
   
   
   glBegin(GL_TRIANGLES);
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glBegin(GL_POINTS);
   glBegin(GL_LINE_LOOP);
   glColor3f(red /* red % */, green /* green % */, blue /* blue % */);
   
   // three points: center, pt1, pt2
   //      Point pt1(false /*check*/);
   //      pt1.setX(center.getX() + (radius * cos(0.0)));
   //      pt1.setY(center.getY() + (radius * sin(0.0)));
   //      Point pt2(pt1);
   
   // go around the circle
   for (double radians = increment;
        radians <= M_PI * 2.0 + .5;
        radians += increment)
      //      {
      //         pt2.setX(center.getX() + (radius * cos(radians)));
      //         pt2.setY(center.getY() + (radius * sin(radians)));
      //
      //         glVertex2f(center.getX(), center.getY());
      //         glVertex2f(pt1.getX(),    pt1.getY()   );
      //         glVertex2f(pt2.getX(),    pt2.getY()   );
      //
      //         pt1 = pt2;
      //      }
      //
      
      
      
      
      
      
      glBegin(GL_LINE_STRIP);
   for (int i = 0; i < sizeof(points)/sizeof(PT); i++)
   {
      Point pt(center.getX() + points[i].x,
               center.getY() + points[i].y);
      rotate(pt, center, rotation);
      glVertex2f(pt.getX(), pt.getY());
   }
   
   glColor3f(1.0 /* red % */, 1.0 /* green % */, 1.0 /* blue % */);
   
   glEnd();
}

/**********************************************************************
 * DRAW MEDIUM Alien ship Part
 **********************************************************************/
void drawMediumAlienShipPart( const Point & center, int rotation, float red, float green, float blue)
{
   // ultra simple point
   struct PT
   {
      int x;
      int y;
   } points[] =
   {
      {2, 8},    {8, 15},    {12, 8},
      {6, 2},    {12, -6},   {2, -15},
      {-6, -15}, {-14, -10}, {-15, 0},
      {-4, 15},  {2, 8}
   };
   
   int radius = 15 ;
   
   assert(radius > 1.0);
   const double increment = M_PI / 6.0;
   
   // begin drawing
   
   
   
  // glBegin(GL_TRIANGLES);
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glBegin(GL_POINTS);
 //  glBegin(GL_LINE_LOOP);
   glColor3f(red /* red % */, green /* green % */, blue /* blue % */);

   
   glBegin(GL_LINE_STRIP);
   for (int i = 0; i < sizeof(points)/sizeof(PT); i++)
   {
      Point pt(center.getX() + points[i].x,
               center.getY() + points[i].y);
      rotate(pt, center, rotation);
      glVertex2f(pt.getX(), pt.getY());
   }
      glColor3f(1.0 /* red % */, 1.0 /* green % */, 1.0 /* blue % */);
   glEnd();
}

/**********************************************************************
 * DRAW barrel
 **********************************************************************/
void drawBarrel( const Point & center, int rotation, float red, float green, float blue)
{
   // ultra simple point
   struct PT
   {
      int x;
      int y;
   } points[] =
   {
      // l.wing     top    l.engine  r.engine   top     r.wing
    //  {0, 12}, {12, -12}, {4, -6}, {-4, -6}, {-12, -12}, {0, 12}
      {0, 18}, {3, -3}, {1, -2}, {-1, -2}, {-3, -3}, {0, 18}
   };
   
   int radius = 15 ;
   
   assert(radius > 1.0);
   const double increment = M_PI / 6.0;
   
   // begin drawing
   
   
   
   glBegin(GL_TRIANGLES);
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glBegin(GL_POINTS);
   glBegin(GL_LINE_LOOP);
   glColor3f(red /* red % */, green /* green % */, blue /* blue % */);
   
   
   glBegin(GL_LINE_STRIP);
   for (int i = 0; i < sizeof(points)/sizeof(PT); i++)
   {
      Point pt(center.getX() + points[i].x,
               center.getY() + points[i].y);
      rotate(pt, center, rotation);
      glVertex2f(pt.getX(), pt.getY());
   }
   glColor3f(1.0 /* red % */, 1.0 /* green % */, 1.0 /* blue % */);
   glEnd();
}



/**********************************************************************
 * DRAW SMALL Alien Ship Part
 **********************************************************************/
void drawSmallAienShipPart( const Point & center, int rotation, float red, float green, float blue)
{
   // ultra simple point
   struct PT
   {
      int x;
      int y;
   } points[] =
   {
      {-5, 9},  {4, 8},   {8, 4},
      {8, -5},  {-2, -8}, {-2, -3},
      {-8, -4}, {-8, 4},  {-5, 10}
   };
   
   //use this for your alien ship!
   
   int radius = 15 ;
   
   assert(radius > 1.0);
   const double increment = M_PI / 6.0;
   
   // begin drawing
   
   glBegin(GL_TRIANGLES);
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glBegin(GL_POINTS);
   glBegin(GL_LINE_LOOP);
   glColor3f(red /* red % */, green /* green % */, blue /* blue % */);
   
   glBegin(GL_LINE_STRIP);
   for (int i = 0; i < sizeof(points)/sizeof(PT); i++)
   {
      Point pt(center.getX() + points[i].x,
               center.getY() + points[i].y);
      rotate(pt, center, rotation);
      glVertex2f(pt.getX(), pt.getY());
   }
      glColor3f(1.0 /* red % */, 1.0 /* green % */, 1.0 /* blue % */);
   glEnd();
}
/**********************************************************************
 * DRAW green strip
 * a little added detail to the alien ship
 **********************************************************************/
void drawGreenStrip( const Point & center, int rotation, float red, float green, float blue)
{
   // ultra simple point
   struct PT
   {
      int x;
      int y;
   } points[] =
   {
      {25, 16},   {25, 5}, {15, -20},
   };
     //{20, 24},   {40, 0}, {0, -40},
   //use this for your alien ship!
   
   int radius = 15 ;
   
   assert(radius > 1.0);
   const double increment = M_PI / 6.0;
   
   // begin drawing
   
   
   
   glBegin(GL_TRIANGLES);
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glBegin(GL_POINTS);
   glBegin(GL_LINE_LOOP);
   glColor3f(red /* red % */, green /* green % */, blue /* blue % */);
   
   glBegin(GL_LINE_STRIP);
   for (int i = 0; i < sizeof(points)/sizeof(PT); i++)
   {
      Point pt(center.getX() + points[i].x,
               center.getY() + points[i].y);
      rotate(pt, center, rotation);
      glVertex2f(pt.getX(), pt.getY());
   }
   glColor3f(1.0 /* red % */, 1.0 /* green % */, 1.0 /* blue % */);
   glEnd();
}


/**********************************************************************
 * DRAW  Alien big ring
 **********************************************************************/
void drawSmallAienShipBigRing( const Point & center, int rotation, float red, float green, float blue)
{
   // ultra simple point
   struct PT
   {
      int x;
      int y;
   }
   
//   points[] =
//   {
//      {0, 24},    {16, 40}, {32, 28},
//      {20, 24},   {40, 0}, {0, -40},
//      {-36, -20}, {-40, -4}, {-40, 28},
//     // {-40, 80},  {0, 48}
//   };
   
   points[] =
   {
      {0, 24},  {32, 28},
      {20, 24},  {0, -40},
      {-36, -20}, {-40, 28},
      // {-40, 80},  {0, 48}
   };
   
   //use this for your alien ship!
   int radius = 15 ;
   
   assert(radius > 1.0);
   const double increment = M_PI / 6.0;
   
   // begin drawing
   
   
   
  // glBegin(GL_TRIANGLES);
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glBegin(GL_POINTS);
   //  glBegin(GL_LINE_LOOP);
   glColor3f(red /* red % */, green /* green % */, blue /* blue % */);
   
   glBegin(GL_LINE_STRIP);
   for (int i = 0; i < sizeof(points)/sizeof(PT); i++)
   {
      Point pt(center.getX() + points[i].x,
               center.getY() + points[i].y);
      rotate(pt, center, rotation);
      glVertex2f(pt.getX(), pt.getY());
   }
   glColor3f(1.0 /* red % */, 1.0 /* green % */, 1.0 /* blue % */);
   glEnd();
}

/************************************************************************
 * DRAW Alien bomber wing
 * Draw a spaceship on the screen
 *  INPUT point   The position of the ship
 *        angle   Which direction it is ponted
 *************************************************************************/
void drawAlienBomberWing(const Point & center, int rotation, bool thrust)
{
   // ultra simple point
   struct PT
   {
      int x;
      int y;
   };
   
   glBegin(GL_TRIANGLES);
   glColor3f(0.0 /* red % */, 1.0 /* green % */, 1.0 /* blue % */);
   
   // draw the ship
   //   const PT pointsShip[] =
   //   { // top   r.wing   r.engine l.engine  l.wing    top
   //      {0, 6}, {6, -6}, {2, -3}, {-2, -3}, {-6, -6}, {0, 6}
   //   };
   
   
   const PT pointsShip[] =
   { // top   r.wing   r.engine l.engine  l.wing    top
      {0, 12}, {12, -12}, {4, -6}, {-4, -6}, {-12, -12}, {0, 12}
   };
   
   
   glBegin(GL_LINE_STRIP);
   for (int i = 0; i < sizeof(pointsShip)/sizeof(PT); i++)
   {
      Point pt(center.getX() + pointsShip[i].x,
               center.getY() + pointsShip[i].y);
      rotate(pt, center, rotation);
      glVertex2f(pt.getX(), pt.getY());
   }
   glEnd();
   
   // draw the flame if necessary
   if (thrust)
   {
      const PT pointsFlame[3][5] =
      {
         { {-2, -3}, {-2, -13}, { 0, -6}, { 2, -13}, {2, -3} },
         { {-2, -3}, {-4,  -9}, {-1, -7}, { 1, -14}, {2, -3} },
         { {-2, -3}, {-1, -14}, { 1, -7}, { 4,  -9}, {2, -3} }
      };
      
      glBegin(GL_LINE_STRIP);
      glColor3f(1.0 /* red % */, 1.0 /* green % */, 0.0 /* blue % */);
      int iFlame = random(0, 3);
      for (int i = 0; i < 5; i++)
      {
         Point pt(center.getX() + pointsFlame[iFlame][i].x,
                  center.getY() + pointsFlame[iFlame][i].y);
         rotate(pt, center, rotation);
         glVertex2f(pt.getX(), pt.getY());
      }
      glColor3f(1.0, 1.0, 1.0); // reset to white
      glEnd();
   }
}



/************************************************************************       
 * DRAW Ship                                                                    
 * Draw a spaceship on the screen                                               
 *  INPUT point   The position of the ship                                      
 *        angle   Which direction it is ponted                                  
 *************************************************************************/
void drawShip(const Point & center, int rotation, bool thrust)
{
   // ultra simple point
   struct PT
   {
      int x;
      int y;
   };
   
   glBegin(GL_TRIANGLES);
   glColor3f(1.0 /* red % */, 0.0 /* green % */, 0.0 /* blue % */);
   
   // draw the ship                                                 
//   const PT pointsShip[] = 
//   { // top   r.wing   r.engine l.engine  l.wing    top
//      {0, 6}, {6, -6}, {2, -3}, {-2, -3}, {-6, -6}, {0, 6}  
//   };
   
   
   const PT pointsShip[] =
   { // top   r.wing   r.engine l.engine  l.wing    top
      {0, 12}, {12, -12}, {4, -6}, {-4, -6}, {-12, -12}, {0, 12}
   };

   
   glBegin(GL_LINE_STRIP);
   for (int i = 0; i < sizeof(pointsShip)/sizeof(PT); i++)
   {
      Point pt(center.getX() + pointsShip[i].x, 
               center.getY() + pointsShip[i].y);
      rotate(pt, center, rotation);
      glVertex2f(pt.getX(), pt.getY());
   }
   glEnd();

   // draw the flame if necessary
   if (thrust)
   {
      const PT pointsFlame[3][5] =
      {
         { {-2, -3}, {-2, -13}, { 0, -6}, { 2, -13}, {2, -3} },
         { {-2, -3}, {-4,  -9}, {-1, -7}, { 1, -14}, {2, -3} },
         { {-2, -3}, {-1, -14}, { 1, -7}, { 4,  -9}, {2, -3} }
      };
      
      glBegin(GL_LINE_STRIP);
      glColor3f(1.0 /* red % */, 1.0 /* green % */, 0.0 /* blue % */);
      int iFlame = random(0, 3);
      for (int i = 0; i < 5; i++)
      {
         Point pt(center.getX() + pointsFlame[iFlame][i].x, 
                  center.getY() + pointsFlame[iFlame][i].y);
         rotate(pt, center, rotation);
         glVertex2f(pt.getX(), pt.getY());
      }
      glColor3f(1.0, 1.0, 1.0); // reset to white                                  
      glEnd();
   }
}

/************************************************************************
 * DRAW Ship laser damaga up
 * Draw a spaceship on the screen
 *  INPUT point   The position of the ship
 *        angle   Which direction it is ponted
 *************************************************************************/
void drawShipLaserDamageUp(const Point & center, int rotation, bool thrust)
{
   // ultra simple point
   struct PT
   {
      int x;
      int y;
   };
   
   glBegin(GL_TRIANGLES);
   glColor3f(0.0 /* red % */, 1.0 /* green % */, 0.0 /* blue % */);
   
   // draw the ship
   //   const PT pointsShip[] =
   //   { // top   r.wing   r.engine l.engine  l.wing    top
   //      {0, 6}, {6, -6}, {2, -3}, {-2, -3}, {-6, -6}, {0, 6}
   //   };
   
   
   const PT pointsShip[] =
   { // top   r.wing   r.engine l.engine  l.wing    top
      {0, 3}, {10, -10}, {4, -5}, {-4, -5}, {-10, -10}, {0, 3}
      
    //{0, 12}, {12, -12}, {4, -6}, {-4, -6}, {-12, -12}, {0, 12}

      
      
    //  {0, 48}, {120, -120}, {64, -80}, {-64, -80}, {-120, -120}, {0, 48}
      
      //{0, 192}, {192, -192}, {64, -96}, {-64, -96}, {-192, -192}, {0, 192}
   };
   
   
   glBegin(GL_LINE_STRIP);
   for (int i = 0; i < sizeof(pointsShip)/sizeof(PT); i++)
   {
      Point pt(center.getX() + pointsShip[i].x,
               center.getY() + pointsShip[i].y);
      rotate(pt, center, rotation);
      glVertex2f(pt.getX(), pt.getY());
   }
   glEnd();
   
   // draw the flame if necessary
   if (thrust)
   {
      const PT pointsFlame[3][5] =
      {
         { {-2, -3}, {-2, -13}, { 0, -6}, { 2, -13}, {2, -3} },
         { {-2, -3}, {-4,  -9}, {-1, -7}, { 1, -14}, {2, -3} },
         { {-2, -3}, {-1, -14}, { 1, -7}, { 4,  -9}, {2, -3} }
      };
      
      glBegin(GL_LINE_STRIP);
      glColor3f(1.0 /* red % */, 1.0 /* green % */, 0.0 /* blue % */);
      int iFlame = random(0, 3);
      for (int i = 0; i < 5; i++)
      {
         Point pt(center.getX() + pointsFlame[iFlame][i].x,
                  center.getY() + pointsFlame[iFlame][i].y);
         rotate(pt, center, rotation);
         glVertex2f(pt.getX(), pt.getY());
      }
      glColor3f(1.0, 1.0, 1.0); // reset to white
      glEnd();
   }
}



/************************************************************************
 * DRAW Ship
 * Draw a spaceship on the screen
 *  INPUT point   The position of the ship
 *        angle   Which direction it is ponted
 *************************************************************************/
void drawShipLaserShotUp(const Point & center, int rotation, bool thrust)
{
   // ultra simple point
   struct PT
   {
      int x;
      int y;
   };
   
   glBegin(GL_TRIANGLES);
   glColor3f(1.0 /* red % */, 0.0 /* green % */, 0.0 /* blue % */);
   
   // draw the ship
   //   const PT pointsShip[] =
   //   { // top   r.wing   r.engine l.engine  l.wing    top
   //      {0, 6}, {6, -6}, {2, -3}, {-2, -3}, {-6, -6}, {0, 6}
   //   };
   
   
   const PT pointsShip[] =
   { //   top   r.wing   r.engine l.engine  l.wing    top
      //{-20, -19}, {-4, 4}, {-2, 6}, {2, 6}, {4, 4}, {20, -19}
      {-14, -5}, {-4, 4}, {-1, 2}, {1, 2}, {4, 4}, {14, -5}
       //{0, 12}, {12, -12}, {4, -6}, {-4, -6}, {-12, -12}, {0, 12}
    //{-200, -96}, {-48, 48}, {-16, 24}, {16, 24}, {48, 48}, {200, -96}

   };
   
   
   glBegin(GL_LINE_STRIP);
   for (int i = 0; i < sizeof(pointsShip)/sizeof(PT); i++)
   {
      Point pt(center.getX() + pointsShip[i].x,
               center.getY() + pointsShip[i].y);
      rotate(pt, center, rotation);
      glVertex2f(pt.getX(), pt.getY());
   }
   glEnd();
   
   // draw the flame if necessary
   if (thrust)
   {
      const PT pointsFlame[3][5] =
      {
         { {-2, -3}, {-2, -13}, { 0, -6}, { 2, -13}, {2, -3} },
         { {-2, -3}, {-4,  -9}, {-1, -7}, { 1, -14}, {2, -3} },
         { {-2, -3}, {-1, -14}, { 1, -7}, { 4,  -9}, {2, -3} }
      };
      
      glBegin(GL_LINE_STRIP);
      glColor3f(1.0 /* red % */, 1.0 /* green % */, 0.0 /* blue % */);
      int iFlame = random(0, 3);
      for (int i = 0; i < 5; i++)
      {
         Point pt(center.getX() + pointsFlame[iFlame][i].x,
                  center.getY() + pointsFlame[iFlame][i].y);
         rotate(pt, center, rotation);
         glVertex2f(pt.getX(), pt.getY());
      }
      glColor3f(1.0, 1.0, 1.0); // reset to white
      glEnd();
   }
}



/************************************************************************
 * DRAW Shot gun Up
 * Draw a spaceship on the screen
 *  INPUT point   The position of the ship
 *        angle   Which direction it is ponted
 *************************************************************************/
void drawShotGunUp(const Point & center, int rotation, bool thrust)
{
   // ultra simple point
   struct PT
   {
      int x;
      int y;
   };
   
   glBegin(GL_TRIANGLES);
   glColor3f(1.0 /* red % */, 0.50 /* green % */, 0.0 /* blue % */);
   
   // draw the ship
   //   const PT pointsShip[] =
   //   { // top   r.wing   r.engine l.engine  l.wing    top
   //      {0, 6}, {6, -6}, {2, -3}, {-2, -3}, {-6, -6}, {0, 6}
   //   };
   
   
   const PT pointsShip[] =
   { // top   r.wing   r.engine l.engine  l.wing    top
      {5, 8}, {12, -12}, {4, -6}, {-4, -6}, {-12, -12}, {-5, 8}
   };
   
   
   glBegin(GL_LINE_STRIP);
   for (int i = 0; i < sizeof(pointsShip)/sizeof(PT); i++)
   {
      Point pt(center.getX() + pointsShip[i].x,
               center.getY() + pointsShip[i].y);
      rotate(pt, center, rotation);
      glVertex2f(pt.getX(), pt.getY());
   }
   glEnd();
   
   // draw the flame if necessary
   if (thrust)
   {
      const PT pointsFlame[3][5] =
      {
         { {-2, -3}, {-2, -13}, { 0, -6}, { 2, -13}, {2, -3} },
         { {-2, -3}, {-4,  -9}, {-1, -7}, { 1, -14}, {2, -3} },
         { {-2, -3}, {-1, -14}, { 1, -7}, { 4,  -9}, {2, -3} }
      };
      
      glBegin(GL_LINE_STRIP);
      glColor3f(1.0 /* red % */, 1.0 /* green % */, 0.0 /* blue % */);
      int iFlame = random(0, 3);
      for (int i = 0; i < 5; i++)
      {
         Point pt(center.getX() + pointsFlame[iFlame][i].x,
                  center.getY() + pointsFlame[iFlame][i].y);
         rotate(pt, center, rotation);
         glVertex2f(pt.getX(), pt.getY());
      }
      glColor3f(1.0, 1.0, 1.0); // reset to white
      glEnd();
   }
}



/************************************************************************
 * DRAW Ship
 * Draw a spaceship on the screen
 *  INPUT point   The position of the ship
 *        angle   Which direction it is ponted
 *************************************************************************/
void drawShipPlasmaHeatSink(const Point & center, int rotation, bool thrust)
{
   // ultra simple point
   struct PT
   {
      int x;
      int y;
   };
   
   glBegin(GL_TRIANGLES);
   glColor3f(1.0 /* red % */, 0.0 /* green % */, 1.0 /* blue % */);
   
   // draw the ship
   //   const PT pointsShip[] =
   //   { // top   r.wing   r.engine l.engine  l.wing    top
   //      {0, 6}, {6, -6}, {2, -3}, {-2, -3}, {-6, -6}, {0, 6}
   //   };
   
   
   const PT pointsShip[] =
   { // top   r.wing   r.engine l.engine  l.wing    top
      {1, 5}, {15, -15}, {7, -9}, {-7, -9}, {-15, -15}, {-1, 5}
   };//+3
   
   
   glBegin(GL_LINE_STRIP);
   for (int i = 0; i < sizeof(pointsShip)/sizeof(PT); i++)
   {
      Point pt(center.getX() + pointsShip[i].x,
               center.getY() + pointsShip[i].y);
      rotate(pt, center, rotation);
      glVertex2f(pt.getX(), pt.getY());
   }
   glEnd();
   
   // draw the flame if necessary
   if (thrust)
   {
      const PT pointsFlame[3][5] =
      {
         { {-2, -3}, {-2, -13}, { 0, -6}, { 2, -13}, {2, -3} },
         { {-2, -3}, {-4,  -9}, {-1, -7}, { 1, -14}, {2, -3} },
         { {-2, -3}, {-1, -14}, { 1, -7}, { 4,  -9}, {2, -3} }
      };
      
      glBegin(GL_LINE_STRIP);
      glColor3f(1.0 /* red % */, 1.0 /* green % */, 0.0 /* blue % */);
      int iFlame = random(0, 3);
      for (int i = 0; i < 5; i++)
      {
         Point pt(center.getX() + pointsFlame[iFlame][i].x,
                  center.getY() + pointsFlame[iFlame][i].y);
         rotate(pt, center, rotation);
         glVertex2f(pt.getX(), pt.getY());
      }
      glColor3f(1.0, 1.0, 1.0); // reset to white
      glEnd();
   }
}



/************************************************************************
 * DRAW Big Ship
 * Draw a spaceship on the screen
 *  INPUT point   The position of the ship
 *        angle   Which direction it is ponted
 *************************************************************************/
void drawBigShip(const Point & center, int rotation, bool thrust)
{
   // ultra simple point
   struct PT
   {
      int x;
      int y;
   };
   
   glBegin(GL_TRIANGLES);
   glColor3f(1.0 /* red % */, 0.0 /* green % */, 0.0 /* blue % */);
   
   // draw the ship
   //   const PT pointsShip[] =
   //   { // top   r.wing   r.engine l.engine  l.wing    top
   //      {0, 6}, {6, -6}, {2, -3}, {-2, -3}, {-6, -6}, {0, 6}
   //   };
   
   
   const PT pointsShip[] =
   { // top   r.wing   r.engine l.engine  l.wing    top
      {0, 192}, {192, -192}, {64, -96}, {-64, -96}, {-192, -192}, {0, 192}
   };
   
   
   glBegin(GL_LINE_STRIP);
   for (int i = 0; i < sizeof(pointsShip)/sizeof(PT); i++)
   {
      Point pt(center.getX() + pointsShip[i].x,
               center.getY() + pointsShip[i].y);
      rotate(pt, center, rotation);
      glVertex2f(pt.getX(), pt.getY());
   }
   glEnd();
   
   // draw the flame if necessary
   if (thrust)
   {
      const PT pointsFlame[3][5] =
      {
         { {-2, -3}, {-2, -13}, { 0, -6}, { 2, -13}, {2, -3} },
         { {-2, -3}, {-4,  -9}, {-1, -7}, { 1, -14}, {2, -3} },
         { {-2, -3}, {-1, -14}, { 1, -7}, { 4,  -9}, {2, -3} }
      };
      
      glBegin(GL_LINE_STRIP);
      glColor3f(1.0 /* red % */, 1.0 /* green % */, 0.0 /* blue % */);
      int iFlame = random(0, 3);
      for (int i = 0; i < 5; i++)
      {
         Point pt(center.getX() + pointsFlame[iFlame][i].x,
                  center.getY() + pointsFlame[iFlame][i].y);
         rotate(pt, center, rotation);
         glVertex2f(pt.getX(), pt.getY());
      }
      glColor3f(1.0, 1.0, 1.0); // reset to white
      glEnd();
   }
}

/************************************************************************
 * DRAW Big Ship laser damage
 * Draw a spaceship on the screen
 *  INPUT point   The position of the ship
 *        angle   Which direction it is ponted
 *************************************************************************/
void drawBigShipLaserDamage(const Point & center, int rotation, bool thrust)
{
   // ultra simple point
   struct PT
   {
      int x;
      int y;
   };
   
   glBegin(GL_TRIANGLES);
   glColor3f(0.0 /* red % */, 1.0 /* green % */, 0.0 /* blue % */);
   
   // draw the ship
   //   const PT pointsShip[] =
   //   { // top   r.wing   r.engine l.engine  l.wing    top
   //      {0, 6}, {6, -6}, {2, -3}, {-2, -3}, {-6, -6}, {0, 6}
   //   };
   
   
   const PT pointsShip[] =
   { // top      r.wing     r.engine    l.engine     l.wing        top
      {0, 48}, {120, -120}, {64, -80}, {-64, -80}, {-120, -120}, {0, 48}

    //{0, 192}, {192, -192}, {64, -96}, {-64, -96}, {-192, -192}, {0, 192}

   };
   
   
   glBegin(GL_LINE_STRIP);
   for (int i = 0; i < sizeof(pointsShip)/sizeof(PT); i++)
   {
      Point pt(center.getX() + pointsShip[i].x,
               center.getY() + pointsShip[i].y);
      rotate(pt, center, rotation);
      glVertex2f(pt.getX(), pt.getY());
   }
   glEnd();
   
   // draw the flame if necessary
   if (thrust)
   {
      const PT pointsFlame[3][5] =
      {
         { {-2, -3}, {-2, -13}, { 0, -6}, { 2, -13}, {2, -3} },
         { {-2, -3}, {-4,  -9}, {-1, -7}, { 1, -14}, {2, -3} },
         { {-2, -3}, {-1, -14}, { 1, -7}, { 4,  -9}, {2, -3} }
      };
      
      glBegin(GL_LINE_STRIP);
      glColor3f(1.0 /* red % */, 1.0 /* green % */, 0.0 /* blue % */);
      int iFlame = random(0, 3);
      for (int i = 0; i < 5; i++)
      {
         Point pt(center.getX() + pointsFlame[iFlame][i].x,
                  center.getY() + pointsFlame[iFlame][i].y);
         rotate(pt, center, rotation);
         glVertex2f(pt.getX(), pt.getY());
      }
      glColor3f(1.0, 1.0, 1.0); // reset to white
      glEnd();
   }
}



/************************************************************************
 * DRAW Big Ship plasma heat sink
 * Draw a spaceship on the screen
 *  INPUT point   The position of the ship
 *        angle   Which direction it is ponted
 *************************************************************************/
void drawBigShipPlasmaHeatSink(const Point & center, int rotation, bool thrust)
{
   // ultra simple point
   struct PT
   {
      int x;
      int y;
   };
   
   glBegin(GL_TRIANGLES);
   glColor3f(1.0 /* red % */, 0.0 /* green % */, 1.0 /* blue % */);
   
   // draw the ship
   //   const PT pointsShip[] =
   //   { // top   r.wing   r.engine l.engine  l.wing    top
   //      {0, 6}, {6, -6}, {2, -3}, {-2, -3}, {-6, -6}, {0, 6}
   //   };
   
   
   const PT pointsShip[] =
   { //    top       r.wing    r.engine    l.engine      l.wing       top
      //{0, 192}, {192, -192}, {64, -96}, {-64, -96}, {-192, -192}, {0, 192}
      {20, 128}, {224, -224}, {96, -128}, {-96, -128}, {-224, -224}, {-20, 128}
      // +32
   };
   
   
   glBegin(GL_LINE_STRIP);
   for (int i = 0; i < sizeof(pointsShip)/sizeof(PT); i++)
   {
      Point pt(center.getX() + pointsShip[i].x,
               center.getY() + pointsShip[i].y);
      rotate(pt, center, rotation);
      glVertex2f(pt.getX(), pt.getY());
   }
   glEnd();
   
   // draw the flame if necessary
   if (thrust)
   {
      const PT pointsFlame[3][5] =
      {
         { {-2, -3}, {-2, -13}, { 0, -6}, { 2, -13}, {2, -3} },
         { {-2, -3}, {-4,  -9}, {-1, -7}, { 1, -14}, {2, -3} },
         { {-2, -3}, {-1, -14}, { 1, -7}, { 4,  -9}, {2, -3} }
      };
      
      glBegin(GL_LINE_STRIP);
      glColor3f(1.0 /* red % */, 1.0 /* green % */, 0.0 /* blue % */);
      int iFlame = random(0, 3);
      for (int i = 0; i < 5; i++)
      {
         Point pt(center.getX() + pointsFlame[iFlame][i].x,
                  center.getY() + pointsFlame[iFlame][i].y);
         rotate(pt, center, rotation);
         glVertex2f(pt.getX(), pt.getY());
      }
      glColor3f(1.0, 1.0, 1.0); // reset to white
      glEnd();
   }
}

/************************************************************************
 * DRAW Big Ship plasma heat sink
 * Draw a spaceship on the screen
 *  INPUT point   The position of the ship
 *        angle   Which direction it is ponted
 *************************************************************************/
void drawBigShipShotGunUp(const Point & center, int rotation, bool thrust)
{
   // ultra simple point
   struct PT
   {
      int x;
      int y;
   };
   
   glBegin(GL_TRIANGLES);
   glColor3f(1.0 /* red % */, 0.50 /* green % */, 0.0 /* blue % */);
   
   // draw the ship
   //   const PT pointsShip[] =
   //   { // top   r.wing   r.engine l.engine  l.wing    top
   //      {0, 6}, {6, -6}, {2, -3}, {-2, -3}, {-6, -6}, {0, 6}
   //   };
   
   
   const PT pointsShip[] =
   { //    top       r.wing    r.engine    l.engine      l.wing       top
      //{0, 192}, {192, -192}, {64, -96}, {-64, -96}, {-192, -192}, {0, 192}
    //  {20, 128}, {224, -224}, {96, -128}, {-96, -128}, {-224, -224}, {-20, 128}
      // +32
      
      {30, 180}, {192, -192}, {64, -96}, {-64, -96}, {-192, -192}, {-30, 180}
   };
   
   
   glBegin(GL_LINE_STRIP);
   for (int i = 0; i < sizeof(pointsShip)/sizeof(PT); i++)
   {
      Point pt(center.getX() + pointsShip[i].x,
               center.getY() + pointsShip[i].y);
      rotate(pt, center, rotation);
      glVertex2f(pt.getX(), pt.getY());
   }
   glEnd();
   
   // draw the flame if necessary
   if (thrust)
   {
      const PT pointsFlame[3][5] =
      {
         { {-2, -3}, {-2, -13}, { 0, -6}, { 2, -13}, {2, -3} },
         { {-2, -3}, {-4,  -9}, {-1, -7}, { 1, -14}, {2, -3} },
         { {-2, -3}, {-1, -14}, { 1, -7}, { 4,  -9}, {2, -3} }
      };
      
      glBegin(GL_LINE_STRIP);
      glColor3f(1.0 /* red % */, 1.0 /* green % */, 0.0 /* blue % */);
      int iFlame = random(0, 3);
      for (int i = 0; i < 5; i++)
      {
         Point pt(center.getX() + pointsFlame[iFlame][i].x,
                  center.getY() + pointsFlame[iFlame][i].y);
         rotate(pt, center, rotation);
         glVertex2f(pt.getX(), pt.getY());
      }
      glColor3f(1.0, 1.0, 1.0); // reset to white
      glEnd();
   }
}


/************************************************************************
 * DRAW Big Ship laser shot up
 * Draw a spaceship on the screen
 *  INPUT point   The position of the ship
 *        angle   Which direction it is ponted
 *************************************************************************/
void drawBigShipLaserShotUp(const Point & center, int rotation, bool thrust)
{
   // ultra simple point
   struct PT
   {
      int x;
      int y;
   };
   
   glBegin(GL_TRIANGLES);
   glColor3f(1.0 /* red % */, 0.0 /* green % */, 0.0 /* blue % */);
   
   // draw the ship
   //   const PT pointsShip[] =
   //   { // top   r.wing   r.engine l.engine  l.wing    top
   //      {0, 6}, {6, -6}, {2, -3}, {-2, -3}, {-6, -6}, {0, 6}
   //   };
   
   
   const PT pointsShip[] =
   { //  top       r.wing    r.engine    l.engine      l.wing       top
      {-200, -96}, {-48, 48}, {-16, 24}, {16, 24}, {48, 48}, {200, -96}

      //{{-190, -192}, {-48, 48}, {-16, 24}, {16, 24}, {48, 48}, {190, -192}
   };
   
   
   glBegin(GL_LINE_STRIP);
   for (int i = 0; i < sizeof(pointsShip)/sizeof(PT); i++)
   {
      Point pt(center.getX() + pointsShip[i].x,
               center.getY() + pointsShip[i].y);
      rotate(pt, center, rotation);
      glVertex2f(pt.getX(), pt.getY());
   }
   glEnd();
   
   // draw the flame if necessary
   if (thrust)
   {
      const PT pointsFlame[3][5] =
      {
         { {-2, -3}, {-2, -13}, { 0, -6}, { 2, -13}, {2, -3} },
         { {-2, -3}, {-4,  -9}, {-1, -7}, { 1, -14}, {2, -3} },
         { {-2, -3}, {-1, -14}, { 1, -7}, { 4,  -9}, {2, -3} }
      };
      
      glBegin(GL_LINE_STRIP);
      glColor3f(1.0 /* red % */, 1.0 /* green % */, 0.0 /* blue % */);
      int iFlame = random(0, 3);
      for (int i = 0; i < 5; i++)
      {
         Point pt(center.getX() + pointsFlame[iFlame][i].x,
                  center.getY() + pointsFlame[iFlame][i].y);
         rotate(pt, center, rotation);
         glVertex2f(pt.getX(), pt.getY());
      }
      glColor3f(1.0, 1.0, 1.0); // reset to white
      glEnd();
   }
}





/************************************************************************
 * DRAW EXPLOTION (sam trent)
 * Draw a EXPLOTION
 *
 * Draws a small red circle when a bullet comes into contact with an
 *   asteroid
 *************************************************************************/
void drawExplotion(const Point & center, float radius, int hits)
{
   assert(radius > 1.0);
   const double increment = M_PI / 6.0;
   
   // begin drawing
   glBegin(GL_TRIANGLES);
   glColor3f(1.0 /* red % */, 0.0 /* green % */, 0.0 /* blue % */);
   
   // three points: center, pt1, pt2
   Point pt1(false /*check*/);
   pt1.setX(center.getX() + (radius * cos(0.0)));
   pt1.setY(center.getY() + (radius * sin(0.0)));
   Point pt2(pt1);
   
   // go around the circle
   for (double radians = increment;
        radians <= M_PI * 2.0 + .5;
        radians += increment)
   {
      pt2.setX(center.getX() + (radius * cos(radians)));
      pt2.setY(center.getY() + (radius * sin(radians)));
      
      glVertex2f(center.getX(), center.getY());
      glVertex2f(pt1.getX(),    pt1.getY()   );
      glVertex2f(pt2.getX(),    pt2.getY()   );
      
      pt1 = pt2;
   }
   
   // complete drawing
   glEnd();
   
   // draw the shields in the center
   if (hits > 0 && hits < 10)
   {
      glColor3f(0.0 /* red % */, 0.0 /* green % */, 0.0 /* blue % */);
      glRasterPos2f(center.getX() - 4, center.getY() - 3);
      glutBitmapCharacter(GLUT_BITMAP_8_BY_13, (char)(hits + '0'));
      glColor3f(1.0, 1.0, 1.0); // reset to white
   }
   glColor3f(1.0, 1.0, 1.0); // reset to white
}

/************************************************************************
 * DRAW star
 * Draw a single point on the screen, 2 pixels by 2 pixels
 *  INPUT point   The position of the dow
 *************************************************************************/
void drawShipBullet(const Point & point, float red, float gre, float blue,
                    float trans)
{
   // Get ready, get set...
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glBegin(GL_POINTS);
   glBegin(GL_LINE_LOOP);
   glColor4f(red /* red % */, gre /* green % */, blue /* blue % */, trans);
   
   
   // Go...
   glVertex2f(point.getX(),     point.getY()    );
   glVertex2f(point.getX() + 1, point.getY()    );
   glVertex2f(point.getX() + 1, point.getY() + 1);
   glVertex2f(point.getX(),     point.getY() + 1);
   
   // Done!  OK, that was a bit too dramatic
   glColor3f(1.0, 1.0, 1.0); // reset to white
   
   glEnd();
}

/************************************************************************
 * DRAW COCKPIT
 * Draw a FILLED IN CIRCLE
 *  INPUT point   The position of the sacred
 *        radius  The size of the bird
 *        hits    How many its remaining to kill the bird
 *************************************************************************/
void drawCockpit(const Point & center, float radius, int hits)
{
   assert(radius > 1.0);
   const double increment = M_PI / 6.0;
   
   // begin drawing
   glBegin(GL_TRIANGLES);
   glColor3f(0.0 /* red % */, 0.0 /* green % */, 1.0 /* blue % */);
   
   // three points: center, pt1, pt2
   Point pt1(false /*check*/);
   pt1.setX(center.getX() + (radius * cos(0.0)));
   pt1.setY(center.getY() + (radius * sin(0.0)));
   Point pt2(pt1);
   
   // go around the circle
   for (double radians = increment;
        radians <= M_PI * 2.0 + .5;
        radians += increment)
   {
      pt2.setX(center.getX() + (radius * cos(radians)));
      pt2.setY(center.getY() + (radius * sin(radians)));
      
      glVertex2f(center.getX(), center.getY());
      glVertex2f(pt1.getX(),    pt1.getY()   );
      glVertex2f(pt2.getX(),    pt2.getY()   );
      
      pt1 = pt2;
   }
   
   // complete drawing
   glEnd();
   
   // draw the shields in the center
   if (hits > 0 && hits < 10)
   {
      glColor3f(0.0 /* red % */, 0.0 /* green % */, 0.0 /* blue % */);
      glRasterPos2f(center.getX() - 4, center.getY() - 3);
      glutBitmapCharacter(GLUT_BITMAP_8_BY_13, (char)(hits + '0'));
      glColor3f(1.0, 1.0, 1.0); // reset to white
   }
   glColor3f(1.0, 1.0, 1.0); // reset to white
}

/************************************************************************
 * DRAW WEAPON
 * Draw a FILLED IN CIRCLE
 *  INPUT point   The position of the sacred
 *        radius  The size of the bird
 *        hits    How many its remaining to kill the bird
 *************************************************************************/
void drawWeapon(const Point & center, float radius, int hits, float red, float green, float blue)
{
   assert(radius >= 1.0);
   const double increment = M_PI / 6.0;
   
   // begin drawing
   glBegin(GL_TRIANGLES);
   glColor3f(red /* red % */, green /* green % */, blue /*  % */);
   
   // three points: center, pt1, pt2
   Point pt1(false /*check*/);
   pt1.setX(center.getX() + (radius * cos(0.0)));
   pt1.setY(center.getY() + (radius * sin(0.0)));
   Point pt2(pt1);
   
   // go around the circle
   for (double radians = increment;
        radians <= M_PI * 2.0 + .5;
        radians += increment)
   {
      pt2.setX(center.getX() + (radius * cos(radians)));
      pt2.setY(center.getY() + (radius * sin(radians)));
      
      glVertex2f(center.getX(), center.getY());
      glVertex2f(pt1.getX(),    pt1.getY()   );
      glVertex2f(pt2.getX(),    pt2.getY()   );
      
      pt1 = pt2;
   }
   
   // complete drawing
   glEnd();
   
   // draw the shields in the center
   if (hits > 0 && hits < 10)
   {
      glColor3f(0.0 /* red % */, 0.0 /* green % */, 0.0 /* blue % */);
      glRasterPos2f(center.getX() - 4, center.getY() - 3);
      glutBitmapCharacter(GLUT_BITMAP_8_BY_13, (char)(hits + '0'));
      glColor3f(1.0, 1.0, 1.0); // reset to white
   }
   glColor3f(1.0, 1.0, 1.0); // reset to white
}


/************************************************************************
 * DRAW Alive bullet
 * Draw a FILLED IN CIRCLE
 *  INPUT point   The position of the sacred
 *        radius  The size of the bird
 *        hits    How many its remaining to kill the bird
 *************************************************************************/
void drawAlienBullet(const Point & center, float radius, int hits)
{
   assert(radius > 1.0);
   const double increment = M_PI / 6.0;
   
   // begin drawing
   glBegin(GL_TRIANGLES);
   glColor3f(0.0 /* red % */, 1.0 /* green % */, 0.0 /* blue % */);
   
   // three points: center, pt1, pt2
   Point pt1(false /*check*/);
   pt1.setX(center.getX() + (radius * cos(0.0)));
   pt1.setY(center.getY() + (radius * sin(0.0)));
   Point pt2(pt1);
   
   // go around the circle
   for (double radians = increment;
        radians <= M_PI * 2.0 + .5;
        radians += increment)
   {
      pt2.setX(center.getX() + (radius * cos(radians)));
      pt2.setY(center.getY() + (radius * sin(radians)));
      
      glVertex2f(center.getX(), center.getY());
      glVertex2f(pt1.getX(),    pt1.getY()   );
      glVertex2f(pt2.getX(),    pt2.getY()   );
      
      pt1 = pt2;
   }
   
   // complete drawing
   glEnd();
   
   // draw the shields in the center
   if (hits > 0 && hits < 10)
   {
      glColor3f(0.0 /* red % */, 0.0 /* green % */, 0.0 /* blue % */);
      glRasterPos2f(center.getX() - 4, center.getY() - 3);
      glutBitmapCharacter(GLUT_BITMAP_8_BY_13, (char)(hits + '0'));
      glColor3f(1.0, 1.0, 1.0); // reset to white
   }
   glColor3f(1.0, 1.0, 1.0); // reset to white
}




