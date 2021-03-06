// Author:	Joe Ruiz
// Spring 2017
// CS 3350 Software Engineering
// My contributions to the game "Engage Hyperdrive" aka Space Escape
//
/* Summary of Functions 
   The functions pauseGame, deadGame, and strandedGame are used as menus
   to give the player a chance to resume the game, restart the game from
   the beginning, return to main menu, or exit the game. 

   The restartLevel function resets all pertinent variables to zero, 
   deletes all asteroids in the linked list, and reinitializes the game.

   The init and initBigAsteroid functions are called at the start of the game or
   when the game is restarted. initBigAsteroid is called whenever more asteroids
   are added to the game.

   The asteroidsRemainingBox counts the number of asteroids from the Game 
   structure as big_asteriods and small_asteroids.

   The windowBorderCollision function prevents the astronaut/player from 
   wrapping from the windows from right to left and top to bottom.

   The moreAsteroids function checks to see if the conditions are met to add
   more asteroids to the playing field.
   */

#include <GL/glx.h>
#include "fonts.h"
#include "typedefine.h"
#include "math.h"
#include <ctime>
#include "string.h"

extern Alien alienEnemy;
extern HealthBox healthbox;
extern FuelBox fuelbox;
extern AmoBox amobox;
extern int keys[65536];
extern bool GameStartMenu;

void pauseGame(int x, int y, Rect pausebox)
{
    glColor3f(0.01, 0.23, 0.25);
    float cx = x/2;
    int cy = y/2;
    glBegin(GL_QUADS);
    glVertex2i(cx - 155, cy + 80);
    glVertex2i(cx + 155, cy + 80);
    glVertex2i(cx + 155, cy - 80);
    glVertex2i(cx - 155, cy - 80);
    glEnd();

    glColor3f(0.0, 0.0, 0.0); 
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
    glVertex2i(cx - 150, cy + 75);
    glVertex2i(cx + 150, cy + 75);
    glVertex2i(cx + 150, cy - 75);
    glVertex2i(cx - 150, cy - 75);
    glEnd();

    glEnable(GL_TEXTURE_2D);
    pausebox.bot = cy + 30;
    pausebox.left = cx - 110;
    pausebox.center = 0;
    ggprint16(&pausebox, 25, 0x00ffffff, "GAME PAUSED...");
    ggprint12(&pausebox, 20, 0x00ffffff, "Press P to Resume");
    ggprint12(&pausebox, 20, 0x00ffffff, "Press R to Restart Game");
    ggprint12(&pausebox, 20, 0x00ffffff, "Press M to Return to Menu");
    ggprint12(&pausebox, 20, 0x00ffffff, "Press E to Exit Game");
    glEnable(GL_TEXTURE_2D);

}

void restartLevel(int &health, float &fuel, int &bulletsRemain, int &score, 
	int &levelnum, int &background, Game *g)
{
    health = 300;
    fuel = 300;
    bulletsRemain = 30;
    score = 0;
    levelnum = 1;
    background = 1;
    g->nbullets = 0;
    g->ahead = NULL;
    g->big_asteroids = 0;
    g->small_asteroids = 0;
    g->astronaut.pos[0] = 10.0;
    g->astronaut.pos[1] = 20.0;
    g->astronaut.vel[0] = 0.0;
    g->astronaut.vel[1] = 0.0;
    g->astronaut.angle = 0.0;

}

void deadGame(int x, int y, Rect pausebox)
{
    glColor3f(0.01, 0.23, 0.25);
    float cx = x/2;
    int cy = y/2;
    glBegin(GL_QUADS);
    glVertex2i(cx - 155, cy + 85);
    glVertex2i(cx + 155, cy + 85);
    glVertex2i(cx + 155, cy - 85);
    glVertex2i(cx - 155, cy - 85);
    glEnd();
    glEnable(GL_TEXTURE_2D);

    glColor3f(0.0, 0.0, 0.0); 
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
    glVertex2i(cx - 150, cy + 80);
    glVertex2i(cx + 150, cy + 80);
    glVertex2i(cx + 150, cy - 80);
    glVertex2i(cx - 150, cy - 80);
    glEnd();

    pausebox.bot = cy + 30;
    pausebox.left = cx - 110;
    pausebox.center = 0;
    ggprint16(&pausebox, 25, 0x00ffffff, "YOU ARE DEAD!");
    ggprint16(&pausebox, 25, 0x00ffffff, "You hit too many objects!");
    ggprint12(&pausebox, 20, 0x00ffffff, "Press R to Restart Game");
    ggprint12(&pausebox, 20, 0x00ffffff, "Press M to Return to Menu");
    ggprint12(&pausebox, 20, 0x00ffffff, "Press E to Exit Game");
    ggprint12(&pausebox, 20, 0x00ffffff, "Press V to See & Enter to ScoreBoard");
    glEnable(GL_TEXTURE_2D);

}

void strandedGame(int x, int y, Rect pausebox)
{
    glColor3f(0.01, 0.23, 0.25);
    float cx = x/2;
    int cy = y/2;
    glBegin(GL_QUADS);
    glVertex2i(cx - 150, cy + 80);
    glVertex2i(cx + 150, cy + 80);
    glVertex2i(cx + 150, cy - 80);
    glVertex2i(cx - 150, cy - 80);
    glEnd();
    glEnable(GL_TEXTURE_2D);

    glColor3f(0.0, 0.0, 0.0); 
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
    glVertex2i(cx - 155, cy + 85);
    glVertex2i(cx + 155, cy + 85);
    glVertex2i(cx + 155, cy - 85);
    glVertex2i(cx - 155, cy - 85);
    glEnd();

    pausebox.bot = cy + 30;
    pausebox.left = cx - 110;
    pausebox.center = 0;
    ggprint16(&pausebox, 25, 0x00ffffff, "YOU ARE STRANDED!");
    ggprint16(&pausebox, 25, 0x00ffffff, "You are out of fuel!");
    ggprint12(&pausebox, 20, 0x00ffffff, "Press R to Restart Game");
    ggprint12(&pausebox, 20, 0x00ffffff, "Press M to Return to Menu");
    ggprint12(&pausebox, 20, 0x00ffffff, "Press E to Exit Game");
    ggprint12(&pausebox, 20, 0x00ffffff, "Press V to See & Enter to ScoreBoard");
    glEnable(GL_TEXTURE_2D);

}

void initBigAsteroid(Game *g, bool GameStartMenu)
{
    Asteroid *a = new Asteroid;
    a->nverts = 8;
    if (GameStartMenu){
	a->radius = 20.0;
    }
    else{
	a->radius = 80.0;
    }
    Flt r2 = a->radius / 2.0;
    Flt angle = 0.0f;
    Flt inc = (PI * 2.0) / (Flt)a->nverts;
    for (int i=0; i<a->nverts; i++) {
	a->vert[i][0] = sin(angle) * (r2 + rnd() * a->radius);
	a->vert[i][1] = cos(angle) * (r2 + rnd() * a->radius);
	angle += inc;
    }
    if (GameStartMenu){
	a->pos[0] = (Flt)(rand() % xres);
    }
    else {
	a->pos[0] = xres;
    }
    a->pos[1] = (Flt)(rand() % yres);
    a->pos[2] = 0.0f;
    a->angle = 0.0;
    a->rotate = rnd() * 2.0 - 2.0;
    a->vel[0] = -(Flt)(rnd());
    a->vel[1] = (Flt)(rnd());
    a->next = g->ahead;
    if (g->ahead != NULL)
	g->ahead->prev = a;
    g->ahead = a;
    g->big_asteroids++;
}

void init(Game *g, int i, bool GameStartMenu)
{
    //build 3 big asteroids...
    for (int j=0; j<i; j++) 
	initBigAsteroid(g, GameStartMenu);
    buildAlien(&alienEnemy);
    buildHealthBox(&healthbox);
    buildFuelBox(&fuelbox);
    buildAmoBox(&amobox);
    clock_gettime(CLOCK_REALTIME, &g->bulletTimer);
    memset(keys, 0, 65536);
}

void asteroidsRemainingBox(Rect r, Game *g)
{
    r.bot = yres - 20;
    r.left = 10;
    r.center = 0;
    ggprint8b(&r, 16, 0x00ff0000, "CS3350 - Space Escape");
    ggprint8b(&r, 16, 0x00ffff00, "Big Asteroids Remaining: %i",
	    g->big_asteroids);
    ggprint8b(&r, 16, 0x00ffff00, "Small Asteroids Remaining: %i",
	    g->small_asteroids);
}

void windowBorderCollision(Game *g)
{
    if (g->astronaut.pos[0] < 0.0) {
	g->astronaut.pos[0] = 0.0;
    }
    if (g->astronaut.pos[0] > (float)xres) {
	g->astronaut.pos[0] = (float)xres;
    }
    if (g->astronaut.pos[1] < 0.0) {
	g->astronaut.pos[1] = 0.0;
    }
    if (g->astronaut.pos[1] > (float)yres) {
	g->astronaut.pos[1] = (float)yres;
    }
}

void moreAsteroids(Game *g, int levelnum) 
{
    if (levelnum == 1){
	if (g->small_asteroids < 10 && g->big_asteroids < 1)
	    initBigAsteroid(g, GameStartMenu);
    }
    if (levelnum == 2){
	if ((g->small_asteroids < 10 && g->big_asteroids < 2) ||
		(g->big_asteroids < 1))
	    initBigAsteroid(g, GameStartMenu);
    }
    if (levelnum == 3){
	if ((g->small_asteroids < 8 && g->big_asteroids < 3 ) ||
		(g->big_asteroids < 2))
	    initBigAsteroid(g, GameStartMenu);
    }
}
