// Author:	Joe Ruiz
// Spring 2017
// CS 3350 Software Engineering
// My contributions to the game "Engage Hyperdrive" aka Space Escape
//
#include <GL/glx.h>
#include "fonts.h"
#include "typedefine.h"
#include "math.h"

void pauseGame(int x, int y, Rect pausebox)
{

    float cx = x/2;
    int cy = y/2;
    glColor3f(0.01, 0.23, 0.25);
    glBegin(GL_QUADS);
    glVertex2i(cx - 155, cy + 80);
    glVertex2i(cx + 155, cy + 80);
    glVertex2i(cx + 155, cy - 80);
    glVertex2i(cx - 155, cy - 80);
    glEnd();
    glEnable(GL_TEXTURE_2D);

    glColor3f(0.0, 0.1, 0.0);
    glVertex2i(cx - 150, cy + 75);
    glVertex2i(cx + 150, cy + 75);
    glVertex2i(cx + 150, cy - 75);
    glVertex2i(cx - 150, cy - 75);
    glEnd();
   
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

void restartLevel(int &health, float &fuel, int &bulletsRemain, int &score)
{
    health = 300;
    fuel = 300;
    bulletsRemain = 30;
    score = 0;

}

void deadGame(int x, int y, Rect pausebox)
{
    glColor3f(0.0, 0.0, 0.0);

    float cx = x/2;
    int cy = y/2;
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
    ggprint16(&pausebox, 25, 0x00ffffff, "YOU ARE DEAD!");
    ggprint16(&pausebox, 25, 0x00ffffff, "You hit too many objects!");
    ggprint12(&pausebox, 20, 0x00ffffff, "Press R to Restart Game");
    ggprint12(&pausebox, 20, 0x00ffffff, "Press M to Return to Menu");
    ggprint12(&pausebox, 20, 0x00ffffff, "Press E to Exit Game");
    glEnable(GL_TEXTURE_2D);

}

void strandedGame(int x, int y, Rect pausebox)
{
    glColor3f(0.0, 0.0, 0.0);

    float cx = x/2;
    int cy = y/2;
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
    ggprint16(&pausebox, 25, 0x00ffffff, "YOU ARE STRANDED!");
    ggprint16(&pausebox, 25, 0x00ffffff, "You are out of fuel!");
    ggprint12(&pausebox, 20, 0x00ffffff, "Press R to Restart Game");
    ggprint12(&pausebox, 20, 0x00ffffff, "Press M to Return to Menu");
    ggprint12(&pausebox, 20, 0x00ffffff, "Press E to Exit Game");
    glEnable(GL_TEXTURE_2D);

}

void initBigAsteroid(Game *g)
{
    Asteroid *a = new Asteroid;
    a->nverts = 4;
    a->radius = 80.0;
    Flt r2 = a->radius / 2.0;
    Flt angle = 0.0f;
    Flt inc = (PI * 2.0) / (Flt)a->nverts;
    for (int i=0; i<a->nverts; i++) {
	a->vert[i][0] = sin(angle) * (r2 + rnd() * a->radius);
	a->vert[i][1] = cos(angle) * (r2 + rnd() * a->radius);
	angle += inc;
    }
    a->pos[0] = xres;
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
