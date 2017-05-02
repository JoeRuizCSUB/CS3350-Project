/* Jonathan Roman
 *
 * This is the code I have contributed to 
 * my groups project.
 */
#include<iostream>
#include <GL/glx.h>
#include <cmath>
#include "typedefine.h"
#include <unistd.h>
#include <ctime>
#include <string.h>

void astronautCollision(Game *g, int &health)
{
    Asteroid *a = g->ahead;
    // Attempt to detect collision between asteroid and
    // astronaut
    Flt d2, d3, dist2;
    Flt d4, d5, dist4;
    while(a){
	d2 = g->astronaut.pos[0] - a->pos[0];
	d3 = g->astronaut.pos[1] - a->pos[1];
	dist2 = (d2*d2 + d3*d3);

	d4 = g->astronaut.pos[0] - a->pos[0];
	d5 = (g->astronaut.pos[1]+20) - a->pos[1];
	dist4 = (d4*d4 + d5*d5);

	if (dist2 < (a->radius)*(a->radius)+900 || dist4 < (a->radius)*(a->radius+4)+900) {
	    //std::cout << "asteroid hit." << std::endl;
	    //this asteroid is hit.

	    // Reducing health when hitting an asteroid.
	    if (health >= 0){
		health = health - 120;
		// So that health display does not show 
		// a negative number.
		if (health < 0) {
		    health = 0;
		}
	    }
	    if (a->radius > MINIMUM_ASTEROID_SIZE) {
		//break it into pieces.
		Asteroid *ta = a;
		buildAsteroidFragment(ta, a); 
		int r = rand()%10+5;
		for (int k=0; k<r; k++) {
		    //get the next asteroid position in the array
		    Asteroid *ta = new Asteroid;
		    buildAsteroidFragment(ta, a); 
		    //add to front of asteroid linked list
		    ta->next = g->ahead;
		    if (g->ahead != NULL)
			g->ahead->prev = ta; 
		    g->ahead = ta; 
		    g->small_asteroids++;
		}
	    } else {
		a->color[0] = 1.0;
		a->color[1] = 0.1;
		a->color[2] = 0.1;
		//asteroid is too small to break up
		//delete the asteroid and bullet
		Asteroid *savea = a->next;
		deleteAsteroid(g, a); 
		a = savea;
		g->small_asteroids--;
	    }

	}
	if (a == NULL)
	    break;
	a = a->next;
    }   
    // End attempt...
}

// Used to display how much fuel is left and decreases as it
// is used..
void fuelbar(int x,  Rect r, float &fuel)
{
    float fuelView = fuel;// - 150;
    // glDisable(GL_TEXTURE_2D);
    glColor3f(0.01,0.23,0.25);
    float cx = x;
    //int cy = y;
    glBegin(GL_QUADS);

    glVertex2i(cx-255,80);
    glVertex2i(cx+55,80);
    glVertex2i(cx+55,50);
    glVertex2i(cx-255,50);
    glEnd();
    glEnable(GL_TEXTURE_2D);
    glColor3f(0.0,1.0,0.0);
    if (fuel <= 50) {
	glColor3f(1.0,0.0,0.0);
    }

    // Setting up location of box
    glBegin(GL_QUADS);
    glVertex2i(cx-250,75);
    glVertex2i(cx+fuelView-250,75);
    glVertex2i(cx+fuelView-250,55);
    glVertex2i(cx-250,55);
    glEnd();
    r.bot = 60;
    r.left = cx - 100;
    r.center = 1;
    //glEnable(GL_TEXTURE_2D);		If you want to see amount
    ggprint8b(&r,20,0x00ffffff, "Fuel");//%d", (int)fuel);
    glEnable(GL_TEXTURE_2D);

}

// Used to determin if astronaut has fuel to use
int fuelRemains(float fuel)
{
    if (fuel > 0)
	return 1;
    else
	return 0;
}
// Reduces the fuel the astronaut has left
float reduceFuel(float fuel)
{
    fuel = fuel - .1;
    //So that fuel does not go negative..
    if (fuel < 0) {
	fuel = 0;
    }
    return fuel;
}
// Used to determin if atronaut has bullets
// to shoot
int remainingAmo(int bulletsRemain)
{
    if (bulletsRemain > 0) {
	return 1;
    }
    else {
	return 0;
    }
}
// Reduces number of bullets remaining
int reduceAmo(int bulletsRemain)
{
    return --bulletsRemain;
}

int getHealthPack(Game *g, HealthBox *healthbox, int &health)
{
    // Attempt to detect collision between health pach and
    // astronaut
    Flt d2, d3, dist2;
    d2 = g->astronaut.pos[0] - healthbox->pos[0];
    d3 = g->astronaut.pos[1] - healthbox->pos[1];
    dist2 = (d2*d2 + d3*d3);
    if (dist2 < healthbox->radius*healthbox->radius && health < 100) {
	// You can come into the radius of the healthbox
	health += 50;
	return 1;
    }
    return 0;
}
int getFuelPack(Game *g, FuelBox *fuelbox, float &fuel)
{
    // Attempt to detect collision between fuel pack and
    // astronaut
    Flt d2, d3, dist2;
    d2 = g->astronaut.pos[0] - fuelbox->pos[0];
    d3 = g->astronaut.pos[1] - fuelbox->pos[1];
    dist2 = (d2*d2 + d3*d3);
    if (dist2 < fuelbox->radius*fuelbox->radius && fuel < 100) {
	// You can come into the radius of the fuel pack
	fuel += 100;
	return 1;
    }
    return 0;
}

int getAmoPack(Game *g, AmoBox *amobox, int &bulletsRemain)
{
    // Attempt to detect collision between amo pack and
    // astronaut
    Flt d2, d3, dist2;
    d2 = g->astronaut.pos[0] - amobox->pos[0];
    d3 = g->astronaut.pos[1] - amobox->pos[1];
    dist2 = (d2*d2 + d3*d3);
    if (dist2 < amobox->radius*amobox->radius && bulletsRemain < 20) {
	// You can come into the radius of the amo pack
	bulletsRemain += 15;
	return 1;
    }
    return 0;
}

// Initialize the values of the FuelBox struct
void buildFuelBox(FuelBox *f)
{
    f->radius = 45;
    f->angle = 0.0f;
    f->pos[0] = 100;//(Flt)(rand() % xres);
    f->pos[1] = 100;//(Flt)(rand() % xres);
    f->pos[2] = 0.0f;
    f->angle = 0.0;

    f->rotate = rnd() * 4.0 - 2.0;

    f->vel[0] = (Flt)(rnd()/2);
    f->vel[1] = (Flt)(rnd()/2);    

}

// Displays Fuel.ppm image
void DrawFuelBox(GLuint fuelSilhouette, FuelBox *f)
{

    glPushMatrix();
    glTranslatef(f->pos[0], f->pos[1], f->pos[2]);
    glRotatef(f->angle+10, 0.0f, 0.0f, 1.0f);

    // Texture
    glBindTexture(GL_TEXTURE_2D, fuelSilhouette);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glBegin(GL_QUADS);
    // Center is 0 so going + and - lets us draw around
    // the center.
    glTexCoord2f(0.0f, 1.0f); glVertex2i(-15,-25);
    glTexCoord2f(0.0f, 0.0f); glVertex2i(-15,25);
    glTexCoord2f(1.0f, 0.0f); glVertex2i(15,25);
    glTexCoord2f(1.0f, 1.0f); glVertex2i(15,-25);
    glEnd();
    glPopMatrix();
    glBegin(GL_POINTS);
    // glVertex2f(a->pos[0], a->pos[1]);
    glEnd();

}

// Initialize the values of the AmoBox struct
void buildAmoBox(AmoBox *a)
{
    a->radius = 35;
    a->angle = 0.0f;
    a->pos[0] = (Flt)(rand() % xres);
    a->pos[1] = (Flt)(rand() % xres);
    a->pos[2] = 0.0f;
    a->angle = 0.0;

    a->rotate = rnd() * 5.0 - 2.0;
    a->vel[0] = (Flt)(rnd());
    a->vel[1] = (Flt)(rnd());    
}

// Display AmoPack.ppm image
void DrawAmoBox(GLuint ammoSilhouette, AmoBox *a){

    glPushMatrix();
    glTranslatef(a->pos[0], a->pos[1], a->pos[2]);
    glRotatef(a->angle+10, 0.0f, 0.0f, 1.0f);

    // Texture
    //glBindTexture(GL_TEXTURE_2D, amoBoxTexture);
    glBindTexture(GL_TEXTURE_2D, ammoSilhouette);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glBegin(GL_QUADS);
    // Center is 0 so going + and - lets us draw around
    // the center.
    glTexCoord2f(0.0f, 1.0f); glVertex2i(-15,-20);
    glTexCoord2f(0.0f, 0.0f); glVertex2i(-15,20);
    glTexCoord2f(1.0f, 0.0f); glVertex2i(15,20);
    glTexCoord2f(1.0f, 1.0f); glVertex2i(15,-20);
    glEnd();
    glPopMatrix();
    glBegin(GL_POINTS);
    // glVertex2f(a->pos[0], a->pos[1]);
    glEnd();
}





void buildAlien(Alien *a)
{
    a->radius = 15;
    a->angle = 0.0f;
    a->pos[0] = (Flt)(rand() % xres);
    a->pos[1] = (Flt)(rand() % yres);
    a->pos[2] = 0.0f;
    a->angle = 0.0;

    a->rotate = rnd() * 10.0 - 2.0;
    a->vel[0] = (Flt)(rnd()/5);
    a->vel[1] = (Flt)(rnd()/5);
    a->dead = 0;

}
void DrawAlien(GLuint AlienTexture, Alien *a)
{

    glPushMatrix();
    glTranslatef(a->pos[0], a->pos[1], a->pos[2]);
    glRotatef(a->angle+10, 0.0f, 0.0f, 1.0f);

    // Texture
    glBindTexture(GL_TEXTURE_2D, AlienTexture);
    glBegin(GL_QUADS);
    // Center is 0 so going + and - lets us draw around
    // the center.
    glTexCoord2f(0.0f, 1.0f); glVertex2i(-15,-20);
    glTexCoord2f(0.0f, 0.0f); glVertex2i(-15,20);
    glTexCoord2f(1.0f, 0.0f); glVertex2i(15,20);
    glTexCoord2f(1.0f, 1.0f); glVertex2i(15,-20);
    glEnd();
    glPopMatrix();
    glBegin(GL_POINTS);
    // glVertex2f(a->pos[0], a->pos[1]);
    glEnd();

}

// The physics applied so that the alien comes in your
// direction and attacks you. 
void AlienFollows(Game *g, Alien *alien)
{

    Flt d1 = abs(g->astronaut.pos[0] - alien->pos[0]+1);
    Flt d2 = abs(g->astronaut.pos[1] - alien->pos[1]-1);

    Flt dist1 = sqrt(d1*d1 + d2*d2);

    Flt d3 = abs(g->astronaut.pos[0] - alien->pos[0]+1);
    Flt d4 = abs(g->astronaut.pos[1] - alien->pos[1]+1);

    Flt dist2 = sqrt(d3*d3 + d4*d4);

    Flt d5 = abs(g->astronaut.pos[0] - alien->pos[0]-1);
    Flt d6 = abs(g->astronaut.pos[1] - alien->pos[1]+1);

    Flt dist3 = sqrt(d5*d5 + d6*d6);

    Flt d7 = abs(g->astronaut.pos[0] - alien->pos[0]-1);
    Flt d8 = abs(g->astronaut.pos[1] - alien->pos[1]-1);

    Flt dist4 = sqrt(d7*d7 + d8*d8);

    if (dist1 < dist2 && dist1 < dist3 && dist1 < dist4) {

	alien->pos[0]-= .5;
	alien->pos[1]+= .5;

    }
    else if (dist2 < dist1 && dist2 < dist3 && dist2 < dist4) {
	alien->pos[0]-= .5;
	alien->pos[1]-= .5;

    }
    else if (dist3 < dist2 && dist3 < dist1 && dist3 < dist4) {
	alien->pos[0]+= .5;
	alien->pos[1]-= .5;

    }
    else if (dist4 < dist2 && dist4 < dist3 && dist4 < dist1) {
	alien->pos[0]+= .5;
	alien->pos[1]+= .5;

    }
}
int AlienHits(Game *g, Alien *alien, int &health)
{
    // Attempt to detect collision between amo pack and
    // astronaut
    Flt d2, d3, dist2;
    d2 = g->astronaut.pos[0] - alien->pos[0];
    d3 = g->astronaut.pos[1] - alien->pos[1];
    dist2 = (d2*d2 + d3*d3) - 20;
    if (dist2 < alien->radius*alien->radius) {
	health -= 20;
	alien->pos[0] = (Flt)(rand() % xres);
	alien->pos[1] = (Flt)(rand() % yres);
	return 0;
    }
    return 1;
}
int ShotAlien(Game *g, Alien *alien, int &score)
{

    //is there a bullet within its radius?
    int i=0;

    Flt d0, d1, dist;
    while (i < g->nbullets) {
	Bullet *b = &g->barr[i];
	d0 = b->pos[0] - alien->pos[0];
	d1 = b->pos[1] - alien->pos[1];
	dist = (d0*d0 + d1*d1) - 40;
	if (dist < (alien->radius*alien->radius)) {
	    score += 5;
	    //delete the bullet...
	    memcpy(&g->barr[i], &g->barr[g->nbullets-1], sizeof(Bullet));
	    g->nbullets--;
	    return 1;
	}
	i++;
    }
    return 0;
}

