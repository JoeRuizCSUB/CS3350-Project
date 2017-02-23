// Author:	Joe Ruiz
// Spring 2017
// CS 3350 Software Engineering
// My contributions to the game "Engage Hyperdrive" aka Space Escape
//
//
// Gravity Formula from stackoverflow
/*float g = 9.8f;
float gravity(Vec p1_pos, Vec p2_pos, float p1_mass, float p2_mass)
{
   float distance = (p2_pos - p1_pos); 
   return g * p1_mass * p2_mass / (distance * distance);
}
*/

#include <GL/glx.h>
void pauseGame(int xsize, int ysize, Rect *pausebox)
{
    glClear(GL_COLOR_BUFFER_BIT);
    pausebox->bot = ysize / 2;
    pausebox->left = xsize / 2;
    pausebox->center = 1;
    ggprint8b(&pausebox, 20, 0x00ff0000, "GAME PAUSED...");
    return;
}
