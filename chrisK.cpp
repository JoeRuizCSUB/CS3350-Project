//Author: Chris Kelly
//Purpose: SPACE Escape personal file 
//Date - 3-2-17
//CS 3350 

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <ctime>
#include <cmath>
#include <X11/Xlib.h>
//#include <X11/Xutil.h>
//#include <GL/gl.h>
//#include <GL/glu.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "log.h"
#include "fonts.h"
#include "ppm.h"


	

void healthbar(int x, int y, Rect r) 
{
   // glDisable(GL_TEXTURE_2D);
    glColor3f(1.0,0.0,0.0);
    int cx = x;
    int cy = y;
    glBegin(GL_QUADS);
    		glVertex2i(cx-155,cy+15);
    		glVertex2i(cx+155,cy+15);
    		glVertex2i(cx+155,cy-15);
    		glVertex2i(cx-155,cy-15);
   glEnd(); 
   glEnable(GL_TEXTURE_2D);
   glColor3f(0.0,1.0,0.0);
   glBegin(GL_QUADS);
    		glVertex2i(cx-150,cy+10);
    		glVertex2i(cx+150,cy+10);
    		glVertex2i(cx+150,cy-10);
    		glVertex2i(cx-150,cy-10);
   glEnd(); 
   r.bot = cy -5;
   r.left = cx ;
   r.center = 1;
  //glEnable(GL_TEXTURE_2D);
   ggprint8b(&r,20,0x00ffffff, "Health");
   glEnable(GL_TEXTURE_2D);





}
