/* Jonathan Roman
 *
 * This is the code I have contributed to 
 * my groups project.
 */
#include<iostream>
#include <GL/glx.h>
#include <cmath>
using namespace std;
void drawFilledCircle(GLfloat x, GLfloat y, GLfloat radius){
    int i;
    int triangleAmount = 200; //# of triangles used to draw circle

    //GLfloat radius = 0.8f; //radius
    GLfloat twicePi = 2.0f * 3.125;

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y); // center of circle
    for(i = 0; i <= triangleAmount;i++) {
	// To ensure inside loop
	std::cout << "in loop\n";	
	glVertex2f(
		x + (radius * cos(i *  twicePi / triangleAmount)), 
		y + (radius * sin(i * twicePi / triangleAmount))
		);
    }
    // Statement used to ensure entering function when called
    // in asteroids.cpp
//    std::cout << "Inside Draw Circle Function\n";
    glEnd();
}

