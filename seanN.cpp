//Author: Sean Nickell
//Purpose of file: Keypresses, and soon to be rendering
//Date Modified: 3/2/17

#include <GL/glx.h>
#include </usr/include/AL/alut.h>
#include "ppm.h"

extern int xres, yres;

void changeBackground(int background, GLuint Level1Texture, GLuint Level2Texture, 
	GLuint Level3Texture, GLuint Level4Texture, GLuint Level5Texture)
{
    if (background ==1) {
	glBindTexture(GL_TEXTURE_2D, Level1Texture);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex2i(0,0);
	glTexCoord2f(0.0f, 0.0f); glVertex2i(0,yres);
	glTexCoord2f(1.0f, 0.0f); glVertex2i(xres,yres);
	glTexCoord2f(1.0f, 1.0f); glVertex2i(xres,0);
	glEnd();
    }

    if (background ==2) {
	glBindTexture(GL_TEXTURE_2D, Level2Texture);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex2i(0,0);
	glTexCoord2f(0.0f, 0.0f); glVertex2i(0,yres);
	glTexCoord2f(1.0f, 0.0f); glVertex2i(xres,yres);
	glTexCoord2f(1.0f, 1.0f); glVertex2i(xres,0);
	glEnd();
    }

    if (background ==3) {
	glBindTexture(GL_TEXTURE_2D, Level3Texture);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex2i(0,0);
	glTexCoord2f(0.0f, 0.0f); glVertex2i(0,yres);
	glTexCoord2f(1.0f, 0.0f); glVertex2i(xres,yres);
	glTexCoord2f(1.0f, 1.0f); glVertex2i(xres,0);
	glEnd();
    }

    if (background ==4) {
	glBindTexture(GL_TEXTURE_2D, Level4Texture);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex2i(0,0);
	glTexCoord2f(0.0f, 0.0f); glVertex2i(0,yres);
	glTexCoord2f(1.0f, 0.0f); glVertex2i(xres,yres);
	glTexCoord2f(1.0f, 1.0f); glVertex2i(xres,0);
	glEnd();
    }

    if (background ==5) {
	glBindTexture(GL_TEXTURE_2D, Level5Texture);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex2i(0,0);
	glTexCoord2f(0.0f, 0.0f); glVertex2i(0,yres);
	glTexCoord2f(1.0f, 0.0f); glVertex2i(xres,yres);
	glTexCoord2f(1.0f, 1.0f); glVertex2i(xres,0);
	glEnd();
    }
}

void getAudio(int num, ALuint* alSource)
{
    if (num == 0) {
	alSourceStop(alSource[0]);
	alSourcePlay(alSource[0]);
    }
    else if (num == 8) {
	alSourceStop(alSource[8]);
	alSourcePlay(alSource[8]);
    }

    else {
	alSourceStopv(8, alSource);
	for (int i=1; i<8; i++) {
	    if (num == i)
		alSourcePlay(alSource[i]);
	}
    }
}

//openal function for audio created by Sean
void init_openal(ALuint *alBuffer, ALuint *alSource)
{

    alutInit(0, NULL);

    alGetError();

    float vec[6] = {0.0f,0.0f,1.0f, 0.0f,1.0f,0.0f};
    alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
    alListenerfv(AL_ORIENTATION, vec);
    alListenerf(AL_GAIN, 1.0f);

    alBuffer[0] = alutCreateBufferFromFile("./Sounds/Bullet.wav");
    alBuffer[1] = alutCreateBufferFromFile("./Sounds/StartScreen.wav");
    alBuffer[2] = alutCreateBufferFromFile("./Sounds/Backstory.wav");
    alBuffer[3] = alutCreateBufferFromFile("./Sounds/Level1.wav");
    alBuffer[4] = alutCreateBufferFromFile("./Sounds/Level2.wav");                                               alBuffer[5] = alutCreateBufferFromFile("./Sounds/Level3.wav");
    alBuffer[6] = alutCreateBufferFromFile("./Sounds/Level4.wav");
    alBuffer[7] = alutCreateBufferFromFile("./Sounds/Level5.wav");
    alBuffer[8] = alutCreateBufferFromFile("./Sounds/Explosion.wav");

    alGenSources(9, alSource);
    alSourcei(alSource[0], AL_BUFFER, alBuffer[0]);
    alSourcei(alSource[1], AL_BUFFER, alBuffer[1]);
    alSourcei(alSource[2], AL_BUFFER, alBuffer[2]);
    alSourcei(alSource[3], AL_BUFFER, alBuffer[3]);
    alSourcei(alSource[4], AL_BUFFER, alBuffer[4]);
    alSourcei(alSource[5], AL_BUFFER, alBuffer[5]);
    alSourcei(alSource[6], AL_BUFFER, alBuffer[6]);
    alSourcei(alSource[7], AL_BUFFER, alBuffer[7]);
    alSourcei(alSource[8], AL_BUFFER, alBuffer[8]);

    alSourcef(alSource[0], AL_GAIN, 1.0f);
    alSourcef(alSource[0], AL_PITCH, 1.0f);
    alSourcei(alSource[0], AL_LOOPING, AL_FALSE);

    alSourcef(alSource[8], AL_GAIN, 1.0f);
    alSourcef(alSource[8], AL_PITCH, 1.0f);
    alSourcei(alSource[8], AL_LOOPING, AL_FALSE);


    for (int i=1; i<8; i++) {
	alSourcef(alSource[i], AL_GAIN, 1.0f);
	alSourcef(alSource[i], AL_PITCH, 1.0f);
	alSourcei(alSource[i], AL_LOOPING, AL_TRUE);
    }
    getAudio(1, alSource);	
}

