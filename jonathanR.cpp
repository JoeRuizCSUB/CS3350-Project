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

void astronautCollision(Game *g){
    Asteroid *a = g->ahead;
    // Attempt to detect collision between asteroid and
    // astronaut
    Flt d2, d3, dist2;
    while(a){
	d2 = g->astronaut.pos[0] - a->pos[0];
	d3 = g->astronaut.pos[1] - a->pos[1];
	dist2 = (d2*d2 + d3*d3);
	if (dist2 < a->radius*a->radius){
	    //std::cout << "asteroid hit." << std::endl;
	    //this asteroid is hit.
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
		    g->nasteroids++;
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
		g->nasteroids--;
	    }

	}
	if (a == NULL)
	    break;
	a = a->next;
    }   
    // End attempt...
}
