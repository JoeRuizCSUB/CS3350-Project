// Author:	Joe Ruiz
// Spring 2017
// CS 3350 Software Engineering
// My contributions to the game "Engage Hyperdrive"
//
//
// Gravity Formula from stackoverflow
float g = 9.8f;
float gravity(Vec p1_pos, Vec p2_pos, float p1_mass, float p2_mass)
{
   float distance = (p2_pos - p1_pos); 
   return g * p1_mass * p2_mass / (distance * distance);
}
