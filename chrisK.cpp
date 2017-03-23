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
#include "typedefine.h"



void healthbar(int x, int y, Rect r, int &health) 
{
    int healthView = health;
    // glDisable(GL_TEXTURE_2D);
    glColor3f(0.0,0.0,0.0);
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
    if (healthView <= 20) {
	glColor3f(1.0,0.0,0.0);
    }

    glBegin(GL_QUADS);
    glVertex2i(cx-150,cy+10);
    glVertex2i(cx+healthView-150,cy+10);
    glVertex2i(cx+healthView-150,cy-10);
    glVertex2i(cx-150,cy-10);
    glEnd(); 
    r.bot = cy -5;
    r.left = cx ;
    r.center = 1;
    //glEnable(GL_TEXTURE_2D);		If you want to see amount
    ggprint8b(&r,20,0x00ffffff, "Health");// %d", health);
    glEnable(GL_TEXTURE_2D);

}

void buildHealthBox(HealthBox *h) {
    h->radius = 100;
    h->angle = 0.0f;
    h->pos[0] = (Flt)(rand() % xres);
    h->pos[1] = (Flt)(rand() % xres);
    h->pos[2] = 0.0f;
    h->angle = 0.0;

    h->rotate = rnd() * 10.0 - 2.0;
    h->vel[0] = (Flt)(rnd()/2);
    h->vel[1] = (Flt)(rnd()/2);

}


// Function used to draw health box and update the position
// so that it "floats" in space.


void DrawHealthBox(GLuint healthBoxTexture, HealthBox *h) {

    glPushMatrix();
    glTranslatef(h->pos[0], h->pos[1], h->pos[2]);
    glRotatef(h->angle/5, 0.0f, 0.0f, 1.0f);


    // Texture

    glBindTexture(GL_TEXTURE_2D, healthBoxTexture);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex2i(100,0);
    glTexCoord2f(0.0f, 0.0f); glVertex2i(100,yres/30);
    glTexCoord2f(1.0f, 0.0f); glVertex2i(xres/10,yres/30);
    glTexCoord2f(1.0f, 1.0f); glVertex2i(xres/10,0);
    glEnd();
    glPopMatrix();
    glBegin(GL_POINTS);

    // glVertex2f(a->pos[0], a->pos[1]);

    glEnd();
}
