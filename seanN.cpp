//Author: Sean Nickell
//Purpose of file: OpenAL and OpenGL work. 
//Date Modified: 3/2/17

#include <GL/glx.h>
#include </usr/include/AL/alut.h>
#include "ppm.h"

extern int xres, yres;

void getBackground(int background, GLuint* glTexture)
{
    	glBindTexture(GL_TEXTURE_2D, glTexture[background]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex2i(0,0);
	glTexCoord2f(0.0f, 0.0f); glVertex2i(0,yres);
	glTexCoord2f(1.0f, 0.0f); glVertex2i(xres,yres);
	glTexCoord2f(1.0f, 1.0f); glVertex2i(xres,0);
	glEnd();    
}

void getAudio(int num, ALuint* alSource)
{
    if (num == 0) {
	alSourceStop(alSource[0]);
	alSourcePlay(alSource[0]);
    }
    else {
	alSourceStopv(8, alSource);
	for (int i=1; i<8; i++) {
	    if (num == i) 
		alSourcePlay(alSource[i]);
	}	
    }
}

