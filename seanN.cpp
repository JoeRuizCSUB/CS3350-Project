//Author: Sean Nickell
//Purpose of file: audio/visual/level system
//also responsible for composing all the sound files from scratch
//Date Modified: 4/27/17
#include <GL/glx.h>
#include "typedefine.h"
#include </usr/include/AL/alut.h>
#include "ppm.h"
#include <X11/keysym.h>
#include <ctime>

extern int xres, yres;

//Changes Background after a level is beaten
void changeBackground(int background, GLuint Level1Texture, GLuint Level2Texture, 
	GLuint Level3Texture)
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
}

//Plays audio loops and single sounds
void getAudio(int num, ALuint* alSource)
{
    if (num == 0) {
	alSourceStop(alSource[0]);
	alSourcePlay(alSource[0]);
    }
    else if (num == 6) {
	alSourceStop(alSource[6]);
	alSourcePlay(alSource[6]);
    }

    else {
	alSourceStopv(6, alSource);
	for (int i=1; i<6; i++) {
	    if (num == i)
		alSourcePlay(alSource[i]);
	}
    }
}

//openal initialize function for audio created by Sean
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
    alBuffer[4] = alutCreateBufferFromFile("./Sounds/Level2.wav");
    alBuffer[5] = alutCreateBufferFromFile("./Sounds/Level3.wav");
    alBuffer[6] = alutCreateBufferFromFile("./Sounds/Explosion.wav");

    alGenSources(7, alSource);
    alSourcei(alSource[0], AL_BUFFER, alBuffer[0]);
    alSourcei(alSource[1], AL_BUFFER, alBuffer[1]);
    alSourcei(alSource[2], AL_BUFFER, alBuffer[2]);
    alSourcei(alSource[3], AL_BUFFER, alBuffer[3]);
    alSourcei(alSource[4], AL_BUFFER, alBuffer[4]);
    alSourcei(alSource[5], AL_BUFFER, alBuffer[5]);
    alSourcei(alSource[6], AL_BUFFER, alBuffer[6]);

    alSourcef(alSource[0], AL_GAIN, 1.0f);
    alSourcef(alSource[0], AL_PITCH, 1.0f);
    alSourcei(alSource[0], AL_LOOPING, AL_FALSE);

    alSourcef(alSource[6], AL_GAIN, 1.0f);
    alSourcef(alSource[6], AL_PITCH, 1.0f);
    alSourcei(alSource[6], AL_LOOPING, AL_FALSE);


    for (int i=1; i<6; i++) {
	alSourcef(alSource[i], AL_GAIN, 1.0f);
	alSourcef(alSource[i], AL_PITCH, 1.0f);
	alSourcei(alSource[i], AL_LOOPING, AL_TRUE);
    }
    getAudio(1, alSource);	
}

//Closes openAl and deletes the sounds and buffers
void cleanup_openAl(ALuint *alBuffer, ALuint *alSource)
{
	for(int i=0; i<6; i++) {
	alDeleteSources(7, &alSource[i]);
	alDeleteBuffers(7, &alBuffer[i]);
	}
 	ALCcontext *Context = alcGetCurrentContext();
        ALCdevice *Device = alcGetContextsDevice(Context);
        alcMakeContextCurrent(NULL);
        alcDestroyContext(Context);
        alcCloseDevice(Device);

}

//Shows level in top right corner
void showLevel(Rect r, int levelnum)
{
    r.bot = yres - 40;
    r.left = xres - 100;
    r.center = 0;
    ggprint16(&r, 16, 0x00ff0000, "Level %i", levelnum);
}

//Similar to restart, restets all the stats except score
void nextLevel(int &health, float &fuel, int &bulletsRemain, Game *g, bool GameStartMenu)
{
    health = 300;
    fuel = 300;
    bulletsRemain= 30;
    g->ahead = NULL;
    g->big_asteroids = 0;
    for(int i=0; i<4; i++) 
	initBigAsteroid(g, GameStartMenu);
    g->astronaut.pos[0] = 10.0;
    g->astronaut.pos[1] = 20.0;
    g->astronaut.vel[0] = 0.0;
    g->astronaut.vel[1] = 0.0;
    g->astronaut.angle = 0.0;
}

//Shows the backstory right after menu
void backstory(Rect r)
{   
    r.bot = yres - 40;
    r.left = xres - 200;
    r.center = 0;
    ggprint16(&r, 16, 0x00ffffff, "[Enter] to Continue");

    r.bot = yres/2;
    r.left = (xres/2) -180;
    r.center = 0;
    ggprint16(&r, 16, 0x00ffffff, "Mission control to ExoExplorer...");

    r.bot = yres/2 -20;
    r.left = (xres/2) -245;
    r.center = 0;
    ggprint16(&r, 16, 0x00ffffff, "We heard your spaceship has been destroyed...");
	
    r.bot = yres/2 -40;
    r.left = (xres/2) -380;
    r.center = 0;
    ggprint16(&r, 16, 0x00ffffff, "We can lock in on you and hurl you closer and closer to earth with our tractor beams...");

    r.bot = yres/2 -60;
    r.left = (xres/2) -340;
    r.center = 0;
    ggprint16(&r, 16, 0x00ffffff, "But you will first need to rid the area of enough asteroids, to ensure safety...");
    
    r.bot = yres/2 -80;
    r.left = (xres/2) -300;
    r.center = 0;
    ggprint16(&r, 16, 0x00ffffff, "One last thing, Aliens have been spotted in this area so be careful...");

    r.bot = yres/2 -140;
    r.left = (xres/2) -90;
    r.center = 0;
    ggprint16(&r, 16, 0x00ffffff, "Goodluck...");

}


