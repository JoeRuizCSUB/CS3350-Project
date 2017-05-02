//3350 Spring 2017
//
//program: /steroids.cpp
//author:  Gordon Griesel
//date:    2014
//mod spring 2015: added constructors
//
//This program is a game starting point for 335
//
// Possible requirements:
// ----------------------
// welcome screen *
// menu
// multiple simultaneous key-press
// show exhaust for thrusting
// move the asteroids
// collision detection for bullet on asteroid
// collision detection for asteroid on astronaut
// control of bullet launch point
// life span for each bullet
// cleanup the bullets that miss a target
// split asteroids into pieces when blasted
// random generation of new asteroids
// score keeping
// levels of difficulty
// sound
// use of textures
// 
//
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <ctime>
#include <cmath>
#include <X11/Xlib.h>
//#include <X11/Xutil.h>
//#include <GL/gl.h>
//#include <GL/glu.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include </usr/include/AL/alut.h>
#include "log.h"
#include "fonts.h"
#include "ppm.h"
////////////////////////////////////////////////////////////
// Jonathan Added
// Contains all of the structures
// most declarations
#include "typedefine.h"
////////////////////////////////////////////////////////////

bool GameStartMenu = true;      //
//X Windows variables
Display *dpy;   //
Window win;     //
GLXContext glc; //

/*********************************************************/ 
// Global Variables
// added by sean
// **Textures**

Ppmimage *Level1=NULL;
GLuint Level1Texture;

Ppmimage *Level2=NULL;
GLuint Level2Texture;

Ppmimage *Level3=NULL;
GLuint Level3Texture;

GLuint healthSilhouette;
GLuint ammoSilhouette;
GLuint fuelSilhouette;

int background = 1;
// **levels**
int levelnum = 1;
bool backstoryOn = false;
// **Sounds**
ALuint alBuffer[9];
ALuint alSource[9];
int sound = 1;
// Global Variables end
/********************************************************/

GLuint silhouetteTexture;
GLuint silhouette_astronautpicTexturepic;

Ppmimage *healthBox = NULL;
GLuint healthBoxTexture;

Ppmimage *fuelBox = NULL;
GLuint fuelBoxTexture;

Ppmimage *amoBox = NULL;
GLuint amoBoxTexture;

Ppmimage *Asteroidpic = NULL;
GLuint AsteroidTexturepic;

Ppmimage *astronautpic = NULL;
GLuint astronautpicTexturepic;

Ppmimage *alien= NULL;
GLuint alienTexturepic;


Ppmimage *bulletImage;
GLuint bulletTexture;
Sprite bullet_sprite;


int xres=1250, yres=900;
int keys[65536];

// Joe's Global Variables
Rect pbox;
int pause_game = 1;
int level = 1;
int restart = 0;
int dead = 0;
Game game;
// end of Joe

//Chris's Global Variables
Ppmimage *StartUpMenu = NULL;
GLuint StartUpMenuTexture;
int menu = 1;
int score = 50;
//end of Chris
// Jonathan's Global Variables
// Used to keep track of how much health, fuel 
// and bullets remain
int health = 300;
float fuel = 300;
int bulletsRemain = 30;
HealthBox healthbox;
FuelBox fuelbox;
AmoBox amobox;
Alien alienEnemy;
int gotHealth = 0;
int gotFuel = 0;
int gotAmo = 0;
int alienActive = 0;
// end of Jonathan

int main(void)
{
    logOpen();
    initXWindows();
    init_opengl();	
    init_openal(alBuffer, alSource);
    init(&game, 7, GameStartMenu);
    srand(time(NULL));
    clock_gettime(CLOCK_REALTIME, &timePause);
    clock_gettime(CLOCK_REALTIME, &timeStart);

    int done=0;
    while (!done) {
	while (XPending(dpy)) {
	    XEvent e;
	    XNextEvent(dpy, &e);
	    check_resize(&e);
	    done = check_keys(&e, &game);
	}
	clock_gettime(CLOCK_REALTIME, &timeCurrent);
	timeSpan = timeDiff(&timeStart, &timeCurrent);
	timeCopy(&timeStart, &timeCurrent);
	physicsCountdown += timeSpan;
	while (physicsCountdown >= physicsRate) {
	    physics(&game);
	    physicsCountdown -= physicsRate;
	}
	render(&game);
	glXSwapBuffers(dpy, win);
    }
    cleanupXWindows();
    cleanup_fonts();
    cleanup_openAl(alBuffer, alSource);
    logClose();
    return 0;
}

void cleanupXWindows(void)
{
    XDestroyWindow(dpy, win);
    XCloseDisplay(dpy);
}

void set_title(void)
{
    //Set the window title bar.
    XMapWindow(dpy, win);
    XStoreName(dpy, win, "CS3350 Project - Space Escape");
}

void setup_screen_res(const int w, const int h)
{
    xres = w;
    yres = h;
}

void initXWindows(void)
{
    GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
    //GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, None };
    XSetWindowAttributes swa;
    setup_screen_res(xres, yres);
    dpy = XOpenDisplay(NULL);
    if (dpy == NULL) {
	std::cout << "\n\tcannot connect to X server" << std::endl;
	exit(EXIT_FAILURE);
    }
    Window root = DefaultRootWindow(dpy);
    XVisualInfo *vi = glXChooseVisual(dpy, 0, att);
    if (vi == NULL) {
	std::cout << "\n\tno appropriate visual found\n" << std::endl;
	exit(EXIT_FAILURE);
    } 
    Colormap cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
    swa.colormap = cmap;
    swa.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask |
	PointerMotionMask | MotionNotify | ButtonPress | ButtonRelease |
	StructureNotifyMask | SubstructureNotifyMask;
    win = XCreateWindow(dpy, root, 0, 0, xres, yres, 0,
	    vi->depth, InputOutput, vi->visual,
	    CWColormap | CWEventMask, &swa);
    set_title();
    glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
    glXMakeCurrent(dpy, win, glc);
    show_mouse_cursor(0);
}

void reshape_window(int width, int height)
{
    //window has been resized.
    setup_screen_res(width, height);
    //
    glViewport(0, 0, (GLint)width, (GLint)height);
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();
    glOrtho(0, xres, 0, yres, -1, 1);
    set_title();
}

void init_opengl(void)
{
    //OpenGL initialization
    glViewport(0, 0, xres, yres);
    //Initialize matrices
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();
    //This sets 2D mode (no perspective)
    glOrtho(0, xres, 0, yres, -1, 1);
    //
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_FOG);
    glDisable(GL_CULL_FACE);
    //
    //Clear the screen to black
    glClearColor(0.0, 0.0, 0.0, 1.0);
    //Do this to allow fonts
    glEnable(GL_TEXTURE_2D);
    initialize_fonts();


    // Created by Sean
    system("convert ./Images/Level1.jpg ./Images/Level1.ppm");
    Level1 = ppm6GetImage("./Images/Level1.ppm");
    glGenTextures(1, &Level1Texture);

    glBindTexture(GL_TEXTURE_2D, Level1Texture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3,

	    Level1->width, Level1->height,
	    0, GL_RGB, GL_UNSIGNED_BYTE, Level1->data);

    system("convert ./Images/Level2.jpg ./Images/Level2.ppm");
    Level2 = ppm6GetImage("./Images/Level2.ppm");
    glGenTextures(1, &Level2Texture);

    glBindTexture(GL_TEXTURE_2D, Level2Texture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3,
	    Level2->width, Level2->height,
	    0, GL_RGB, GL_UNSIGNED_BYTE, Level2->data);

    system("convert ./Images/Level3.jpg ./Images/Level3.ppm");
    Level3 = ppm6GetImage("./Images/Level3.ppm");
    glGenTextures(1, &Level3Texture);

    glBindTexture(GL_TEXTURE_2D, Level3Texture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3,
	    Level3->width, Level3->height,
	    0, GL_RGB, GL_UNSIGNED_BYTE, Level3->data);
    //End of Seans	

    //Chris's Code
    system("convert ./Images/StartUpMenu.jpg ./Images/StartUpMenu.ppm");
    StartUpMenu = ppm6GetImage("./Images/StartUpMenu.ppm");
    glGenTextures(1, &StartUpMenuTexture);

    glBindTexture(GL_TEXTURE_2D, StartUpMenuTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3,
	    StartUpMenu->width, StartUpMenu->height,
	    0, GL_RGB, GL_UNSIGNED_BYTE, StartUpMenu->data);


    system("convert ./Images/Health.jpg ./Images/Health.ppm");
    healthBox= ppm6GetImage("./Images/Health.ppm");
    glGenTextures(1, &healthBoxTexture);
    glBindTexture(GL_TEXTURE_2D, healthBoxTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3,
	    healthBox->width, healthBox->height,
	    0, GL_RGB, GL_UNSIGNED_BYTE, healthBox->data);

    system("convert ./Images/Fuel.jpg ./Images/Fuel.ppm");
    fuelBox= ppm6GetImage("./Images/Fuel.ppm");
    glGenTextures(1, &fuelBoxTexture);
    glBindTexture(GL_TEXTURE_2D, fuelBoxTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3,
	    fuelBox->width, fuelBox->height,
	    0, GL_RGB, GL_UNSIGNED_BYTE, fuelBox->data);

    system("convert ./Images/AmoPack.jpg ./Images/AmoPack.ppm");
    amoBox= ppm6GetImage("./Images/AmoPack.ppm");
    glGenTextures(1, &amoBoxTexture);
    glBindTexture(GL_TEXTURE_2D, amoBoxTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3,
	    amoBox->width, amoBox->height,
	    0, GL_RGB, GL_UNSIGNED_BYTE, amoBox->data);


    //system("convert ./Images/Asteroid.jpg ./Images/Asteroid.ppm");
    Asteroidpic= ppm6GetImage("./Images/Asteroid.ppm");
    glGenTextures(1, &AsteroidTexturepic);
    glBindTexture(GL_TEXTURE_2D, AsteroidTexturepic);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3,
	    Asteroidpic->width, Asteroidpic->height,
	    0, GL_RGB, GL_UNSIGNED_BYTE, Asteroidpic->data);


    alien= ppm6GetImage("./Images/Alien2.ppm");
    glGenTextures(1, &alienTexturepic);
    glBindTexture(GL_TEXTURE_2D, alienTexturepic);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3,
	    alien->width, alien->height,
	    0, GL_RGB, GL_UNSIGNED_BYTE, alien->data);




    //system("convert ./Images/Astronaut.jpg ./Images/Astronaut.ppm");
    astronautpic= ppm6GetImage("./Images/Astronaut.ppm");
    glGenTextures(1, &astronautpicTexturepic);
    glBindTexture(GL_TEXTURE_2D, astronautpicTexturepic);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3,
	    astronautpic->width, astronautpic->height,
	    0, GL_RGB, GL_UNSIGNED_BYTE, astronautpic->data);

    glGenTextures(1, &bulletTexture);
    bulletImage = ppm6GetImage("./Images/bullet.ppm");
    int w3 = bulletImage->width;
    int h3 = bulletImage->height;
    glBindTexture(GL_TEXTURE_2D, bulletTexture);
    //
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, w3, h3, 0, GL_RGB, GL_UNSIGNED_BYTE, bulletImage->data);







    int w = Asteroidpic->width;
    int h = Asteroidpic->height;

    glGenTextures(1, &silhouetteTexture);
    glBindTexture(GL_TEXTURE_2D, silhouetteTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    unsigned char *silhouetteData = buildAlphaData(Asteroidpic);
    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA, w, h, 0,
	    GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData);


    glGenTextures(1, &silhouette_astronautpicTexturepic);
    glBindTexture(GL_TEXTURE_2D, silhouette_astronautpicTexturepic);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    unsigned char *silhouetteData2 = buildAlphaData(astronautpic);
    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA, w, h, 0,
	    GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData2);

    glGenTextures(1, &ammoSilhouette);
    glBindTexture(GL_TEXTURE_2D, ammoSilhouette);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    unsigned char *silhouetteData3 = buildAlphaData(amoBox);
    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA, w, h, 0,
	    GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData3);

    glGenTextures(1, &healthSilhouette);
    glBindTexture(GL_TEXTURE_2D, healthSilhouette);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    unsigned char *silhouetteData4 = buildAlphaData(healthBox);
    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA, w, h, 0,
	    GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData4);

    glGenTextures(1, &fuelSilhouette);
    glBindTexture(GL_TEXTURE_2D, fuelSilhouette);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    unsigned char *silhouetteData5 = buildAlphaData(fuelBox);
    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA, w, h, 0,
	    GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData5);

    free(silhouetteData);
    free(silhouetteData2);
    free(silhouetteData3);
    free(silhouetteData4);
    free(silhouetteData5);
    //ck end

}

void check_resize(XEvent *e)
{
    //The ConfigureNotify is sent by the
    //server if the window is resized.
    if (e->type != ConfigureNotify)
	return;
    XConfigureEvent xce = e->xconfigure;
    if (xce.width != xres || xce.height != yres) {
	//Window size did change.
	reshape_window(xce.width, xce.height);
    }
}

void normalize(Vec v)
{
    Flt len = v[0]*v[0] + v[1]*v[1];
    if (len == 0.0f) {
	v[0] = 1.0;
	v[1] = 0.0;
	return;
    }
    len = 1.0f / sqrt(len);
    v[0] *= len;
    v[1] *= len;
}

void set_mouse_position(int x, int y)
{
    XWarpPointer(dpy, None, win, 0, 0, 0, 0, x, y);
}

void show_mouse_cursor(const int onoff)
{
    if (onoff) {
	//this removes our own blank cursor.
	XUndefineCursor(dpy, win);
	return;
    }
    //vars to make blank cursor
    Pixmap blank;
    XColor dummy;
    char data[1] = {0};
    Cursor cursor;
    //make a blank cursor
    blank = XCreateBitmapFromData (dpy, win, data, 1, 1);
    if (blank == None)
	std::cout << "error: out of memory." << std::endl;
    cursor = XCreatePixmapCursor(dpy, blank, blank, &dummy, &dummy, 0, 0);
    XFreePixmap(dpy, blank);
    //this makes you the cursor. then set it using this function
    XDefineCursor(dpy, win, cursor);
    //after you do not need the cursor anymore use this function.
    //it will undo the last change done by XDefineCursor
    //(thus do only use ONCE XDefineCursor and then XUndefineCursor):
}


int check_keys(XEvent *e, Game *g)
{
    //keyboard input?
    static int shift=0;
    int key = XLookupKeysym(&e->xkey, 0);
    //Log("key: %i\n", key);
    if (e->type == KeyRelease) {
	keys[key]=0;
	if (key == XK_Shift_L || key == XK_Shift_R)
	    shift=0;
	return 0;
    }
    if (e->type == KeyPress) {
	//std::cout << "press" << std::endl;
	keys[key]=1;
	if (key == XK_Shift_L || key == XK_Shift_R) {
	    shift=1;
	    return 0;
	}
    } else {
	return 0;
    }
    if (shift){}
    switch (key) {

	case XK_Return:	
	    if (backstoryOn) { 
		backstoryOn = false;
		pause_game = false;
		getAudio(3, alSource);
	    }
	    break;
	case XK_Escape:
	    return 1;
	case XK_e:
	    if (pause_game)
		return 1;
	case XK_r:
	    if (pause_game || dead || fuel==0) {
		if(levelnum != 1)
		    getAudio(3, alSource);

		restartLevel(health, fuel, bulletsRemain, score, levelnum,
			background, g);
		dead = 0;
		alienEnemy.dead = 0;
		init(&game, 3, GameStartMenu);
		pause_game = 0;
	    }
	    break;
	case XK_s:
	    // Conditional statement so that
	    // game does not start before pressing 's'
	    if (GameStartMenu == true){
		backstoryOn = true;
		GameStartMenu = false;
		pause_game = false;
		getAudio(2, alSource);
	    }
	    break;
	case XK_p:
	    // Added by Joe
	    // Do not allow to be paused before the game
	    // starts.
	    if (GameStartMenu == false && !dead) {
		pause_game ^= 1;
	    }
	    // end of Joe
	    break;
	case XK_Down:
	    break;
	case XK_equal:
	    break;
	case XK_minus:
	    break;
    }
    return 0;
}

void deleteAsteroid(Game *g, Asteroid *node)
{
    //remove a node from doubly-linked list
    if (node->prev == NULL) {
	if (node->next == NULL) {
	    //only 1 item in list.
	    g->ahead = NULL;
	} else {
	    //at beginning of list.
	    node->next->prev = NULL;
	    g->ahead = node->next;
	}
    } else {
	if (node->next == NULL) {
	    //at end of list.
	    node->prev->next = NULL;
	} else {
	    //in middle of list.
	    node->prev->next = node->next;
	    node->next->prev = node->prev;
	}
    }
    delete node;
    node = NULL;
}

void buildAsteroidFragment(Asteroid *ta, Asteroid *a)
{
    //build ta from a
    ta->nverts = 8;
    ta->radius = a->radius / 2.0;
    Flt r2 = ta->radius / 2.0;
    Flt angle = 0.0f;
    Flt inc = (PI * 2.0) / (Flt)ta->nverts;
    for (int i=0; i<ta->nverts; i++) {
	ta->vert[i][0] = sin(angle) * (r2 * ta->radius);
	ta->vert[i][1] = cos(angle) * (r2 * ta->radius);
	angle += inc;
    }
    ta->pos[0] = a->pos[0] + rnd()*10.0-5.0;
    ta->pos[1] = a->pos[1] + rnd()*10.0-5.0;
    ta->pos[2] = 0.0f;
    ta->angle = 0.0;
    ta->rotate = a->rotate + (rnd() * 4.0 - 2.0);
    ta->vel[0] = a->vel[0] + (rnd()*2.0-1.0);
    ta->vel[1] = a->vel[1] + (rnd()*2.0-1.0);
    //std::cout << "frag" << std::endl;
}

void physics(Game *g)
{
    if (pause_game)
	return;
    Flt d0,d1,dist;
    //Update astronaut position
    g->astronaut.pos[0] += g->astronaut.vel[0];
    g->astronaut.pos[1] += g->astronaut.vel[1];

    //Joe added slow astronaut, cannot stay at full speed forever
    g->astronaut.vel[0] *= 0.9971;
    g->astronaut.vel[1] *= 0.9971;

    //Check for collision with window edges
    //Joe added window edge borders for astronaut only
    windowBorderCollision(g);
    //
    //Update bullet positions
    struct timespec bt;
    clock_gettime(CLOCK_REALTIME, &bt);
    int i=0;
    while (i < g->nbullets) {
	Bullet *b = &g->barr[i];

	// Jonathan Changed From Original...
	// Bullets are now removed as soon as they hit any
	// edge of the screen instead of timing how long the
	// bullet has been alive. (Was previously used for
	// wrap around of bullet)
	if ((b->pos[0] < 0.0) || (b->pos[0] > (float)xres) ||
		(b->pos[1] < 0.0) ||  (b->pos[1] > (float)yres) ) {

	    memcpy(&g->barr[i], &g->barr[g->nbullets-1], sizeof(Bullet));
	    g->nbullets--;
	    //do not increment i.
	    continue;
	}

	//move the bullet
	b->pos[0] += b->vel[0];
	b->pos[1] += b->vel[1];
	//Check for collision with window edges
	////////////////////////////////////////////////////////////////////////////////////
	// Jonathan commented out so that bullets would 
	// end at window edges. No wrap around is wanted for the
	// game...
	if (b->pos[0] < 0.0) {
	    //b->pos[0] += (float)xres;
	}
	else if (b->pos[0] > (float)xres) {
	    //b->pos[0] -= (float)xres;
	}
	else if (b->pos[1] < 0.0) {
	    //b->pos[1] += (float)yres;
	}
	else if (b->pos[1] > (float)yres) {
	    //b->pos[1] -= (float)yres;
	}
	////////////////////////////////////////////////////////////////////////////////////
	i++;
    }
    //
    //Update asteroid positions

    if (levelnum == 1){
	if (g->small_asteroids < 10 && g->big_asteroids < 1)
	    initBigAsteroid(g, GameStartMenu);
    }
    if (levelnum == 2){
	if ((g->small_asteroids < 10 && g->big_asteroids < 2) ||
		(g->big_asteroids < 2))
	    initBigAsteroid(g, GameStartMenu);
    }
    if (levelnum == 3){
	if ((g->small_asteroids < 8 && g->big_asteroids < 3 ) || 
		(g->big_asteroids < 3))
	    initBigAsteroid(g, GameStartMenu);
    }
    Asteroid *a= g->ahead;
    while (a) {
	a->pos[0] += a->vel[0];
	a->pos[1] += a->vel[1];
	//Check for collision with window edges
	if (a->pos[0] < -100.0) {
	    a->pos[0] += (float)xres+200;
	}
	else if (a->pos[0] > (float)xres+100) {
	    a->pos[0] -= (float)xres+200;
	}
	else if (a->pos[1] < -100.0) {
	    a->pos[1] += (float)yres+200;
	}
	else if (a->pos[1] > (float)yres+100) {
	    a->pos[1] -= (float)yres+200;
	}
	a->angle += a->rotate;
	a = a->next;
    }


    // Health image
    healthbox.pos[0] += healthbox.vel[0];
    healthbox.pos[1] += healthbox.vel[1];
    //Check for collision with window edges
    if (healthbox.pos[0] < -100.0) {
	healthbox.pos[0] += (float)xres+200;
    }
    else if (healthbox.pos[0] > (float)xres+100) {
	healthbox.pos[0] -= (float)xres+200;
    }
    else if (healthbox.pos[1] < -100.0) {
	healthbox.pos[1] += (float)yres+200;
    }
    else if (healthbox.pos[1] > (float)yres+100) {
	healthbox.pos[1] -= (float)yres+200;
    }
    healthbox.angle += healthbox.rotate;

    // Fuel image
    fuelbox.pos[0] += fuelbox.vel[0];
    fuelbox.pos[1] += fuelbox.vel[1];
    //Check for collision with window edges
    if (fuelbox.pos[0] < -100.0) {
	fuelbox.pos[0] += (float)xres+200;
    }
    else if (fuelbox.pos[0] > (float)xres+100) {
	fuelbox.pos[0] -= (float)xres+200;
    }
    else if (fuelbox.pos[1] < -100.0) {
	fuelbox.pos[1] += (float)yres+200;
    }
    else if (fuelbox.pos[1] > (float)yres+100) {
	fuelbox.pos[1] -= (float)yres+200;
    }
    fuelbox.angle += fuelbox.rotate;

    // Amo pack image
    amobox.pos[0] += amobox.vel[0];
    amobox.pos[1] += amobox.vel[1];
    //Check for collision with window edges
    if (amobox.pos[0] < -100.0) {
	amobox.pos[0] += (float)xres+200;
    }
    else if (amobox.pos[0] > (float)xres+100) {
	amobox.pos[0] -= (float)xres+200;
    }
    else if (amobox.pos[1] < -100.0) {
	amobox.pos[1] += (float)yres+200;
    }
    else if (amobox.pos[1] > (float)yres+100) {
	amobox.pos[1] -= (float)yres+200;
    }
    amobox.angle += amobox.rotate;


    if (alienActive){
	AlienFollows(g, &alienEnemy);
    }

    ////////////////////////////////////////////////////////////////////////
    // Jonathan Added
    // Added
    // Attempt to detect collision between asteroid and
    // astronaut. Was successful, code was moved into
    // jonathanR.cpp. 
    astronautCollision(g, health);
    if (!gotHealth){
	gotHealth = getHealthPack(g, &healthbox, health);
    }

    if (!gotFuel){
	gotFuel = getFuelPack(g, &fuelbox, fuel);
    }
    if (!gotAmo){
	gotAmo = getAmoPack(g, &amobox, bulletsRemain);
    }
    if (alienActive == 1){
	alienActive = AlienHits(g, &alienEnemy, health);
    }


    // Currently exits the game if all health is lost. Change
    // it so that the game starts over...
    if (health == 0){
	// Make sure to pause the game and expect an input.
	// 'dead' is used to ensure that 'p' in the pause_game menu
	// is no longer allowed to be pressed since health is now 0 
	// which means you either restart or exit.
	pause_game = true;
	dead = 1;
	if (pause_game) {
	    deadGame(xres, yres, pbox);
	    //int restart = checkPauseKeys(key, restart);
	    //if (restart)
	    //return 1;
	}

	//exit(0);
    }
    // End attempt...
    ////////////////////////////////////////////////////////////////////////


    //Asteroid collision with bullets?
    //If collision detected:
    //     1. delete the bullet
    //     2. break the asteroid into pieces
    //        if asteroid small, delete it
    a = g->ahead;
    while (a) {
	//is there a bullet within its radius?
	int i=0;
	while (i < g->nbullets) {
	    Bullet *b = &g->barr[i];
	    d0 = b->pos[0] - (a->pos[0]);
	    d1 = b->pos[1] - (a->pos[1]);
	    dist = (d0*d0 + d1*d1) - 250;
	    if (dist < (a->radius*a->radius)) {
		//std::cout << "asteroid hit." << std::endl;
		//this asteroid is hit.
		score = Score(score);
		getAudio(6, alSource);
		if (a->radius > MINIMUM_ASTEROID_SIZE) {
		    //break it into pieces.
		    g->big_asteroids--;
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
		    //asteroid is too small to break up
		    //delete the asteroid and bullet
		    Asteroid *savea = a->next;
		    deleteAsteroid(g, a);
		    a = savea;
		    g->small_asteroids--;
		}
		//delete the bullet...
		memcpy(&g->barr[i], &g->barr[g->nbullets-1], sizeof(Bullet));
		g->nbullets--;
		if (a == NULL)
		    break;
		//continue;
	    }
	    i++;
	}
	if (a == NULL)
	    break;
	a = a->next;
    }
    if (alienActive){
	alienEnemy.dead = ShotAlien(g, &alienEnemy, score);
    }
    //---------------------------------------------------
    //check keys pressed now
    if (keys[XK_Left]) {
	g->astronaut.angle += 4.0;
	if (g->astronaut.angle >= 360.0f)
	    g->astronaut.angle -= 360.0f;
    }
    if (keys[XK_Right]) {
	g->astronaut.angle -= 4.0;
	if (g->astronaut.angle < 0.0f)
	    g->astronaut.angle += 360.0f;
    }
    // Jonathan
    // Added second conditional statement to stop
    // thrust from occuring if no fuel remains
    if (keys[XK_Up] && fuelRemains(fuel)) {
	//apply thrust
	//convert astronaut angle to radians
	Flt rad = ((g->astronaut.angle+90.0) / 360.0f) * PI * 2.0;
	//convert angle to a vector
	Flt xdir = cos(rad);
	Flt ydir = sin(rad);

	//Joe slowed astronaut down
	g->astronaut.vel[0] += xdir*0.01f;
	g->astronaut.vel[1] += ydir*0.01f;
	Flt speed = sqrt(g->astronaut.vel[0]*g->astronaut.vel[0]+
		g->astronaut.vel[1]*g->astronaut.vel[1]);
	if (speed > 10.0f) {
	    speed = 10.0f;
	    normalize(g->astronaut.vel);
	    g->astronaut.vel[0] *= speed;
	    g->astronaut.vel[1] *= speed;
	}
    }
    if (keys[XK_space]) {
	//a little time between each bullet
	struct timespec bt;
	clock_gettime(CLOCK_REALTIME, &bt);
	double ts = timeDiff(&g->bulletTimer, &bt);
	if (ts > 0.1) {
	    timeCopy(&g->bulletTimer, &bt);

	    // Jonathan
	    // Added additional conditional statement so that astronaut
	    // does not have unlimited amount of bullets.
	    if ( (g->nbullets < MAX_BULLETS) && remainingAmo(bulletsRemain)) {
		getAudio(0, alSource);
		bulletsRemain = reduceAmo(bulletsRemain);//bulletsRemain - 1;
		//shoot a bullet...
		//Bullet *b = new Bullet;
		Bullet *b = &g->barr[g->nbullets];
		timeCopy(&b->time, &bt);
		b->pos[0] = g->astronaut.pos[0];
		b->pos[1] = g->astronaut.pos[1]+4;
		b->vel[0] = g->astronaut.vel[0];
		b->vel[1] = g->astronaut.vel[1];
		//convert astronaut angle to radians
		Flt rad = (-(g->astronaut.angle-89.0) / 360.0f) * PI * 2.0;
		//convert angle to a vector
		Flt xdir = sin(rad);
		Flt ydir = cos(rad);
		b->pos[0] += xdir*20.0f;
		b->pos[1] += ydir*20.0f;
		b->vel[0] += xdir*6.0f + rnd()*0.1;
		b->vel[1] += ydir*6.0f + rnd()*0.1;
		b->color[0] = 1.0f;
		b->color[1] = 1.0f;
		b->color[2] = 1.0f;
		g->nbullets++;
	    }
	}
    }
}

void render(Game *g)
{
    Rect r;
    glClear(GL_COLOR_BUFFER_BIT);	
    if(GameStartMenu == true) {
	glBindTexture(GL_TEXTURE_2D, StartUpMenuTexture);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex2i(0,0);
	glTexCoord2f(0.0f, 0.0f); glVertex2i(0,yres);
	glTexCoord2f(1.0f, 0.0f); glVertex2i(xres,yres);
	glTexCoord2f(1.0f, 1.0f); glVertex2i(xres,0);
	glEnd();


	{
	    Asteroid *a = g->ahead;
	    while (a) {
		//Log("draw asteroid...\n");
		//glColor3fv(a->color);
		glPushMatrix();
		glTranslatef(a->pos[0], a->pos[1], a->pos[2]);
		glRotatef(a->angle, 0.0f, 0.0f, 1.0f);

		//DrawAsteroids(AsteroidTexturepic, a);
		glBindTexture(GL_TEXTURE_2D, silhouetteTexture);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.0f);
		//glColor4ub(255,255,255,255);
		glBegin(GL_QUADS);

		glTexCoord2f(0.0f, 1.0f); glVertex2i(-a->radius-10,-a->radius-10);
		glTexCoord2f(0.0f, 0.0f); glVertex2i(-a->radius-10,a->radius+10);
		glTexCoord2f(1.0f, 0.0f); glVertex2i(a->radius+10,a->radius+10);
		glTexCoord2f(1.0f, 1.0f); glVertex2i(a->radius+10,-a->radius-10);

		glEnd();
		glPopMatrix();	

		glPopMatrix();
		//glBegin(GL_POINTS);
		//glVertex2f(a->pos[0], a->pos[1]);
		//glEnd();
		//	a = a->next;
		//  }
		//while (a) {
		a->pos[0] += a->vel[0];
		a->pos[1] += a->vel[1];
		//Check for collision with window edges
		if (a->pos[0] < -100.0) {
		    a->pos[0] += (float)xres+200;
		}
		else if (a->pos[0] > (float)xres+100) {
		    a->pos[0] -= (float)xres+200;
		}
		else if (a->pos[1] < -100.0) {
		    a->pos[1] += (float)yres+200;
		}
		else if (a->pos[1] > (float)yres+100) {
		    a->pos[1] -= (float)yres+200;
		}
		a->angle += a->rotate;
		a = a->next;
	}

	}


    }
    else if (backstoryOn) {
	pause_game = 1;
	backstory(r);
	restartLevel(health, fuel, bulletsRemain, score, levelnum,
		background, g);
	dead = 0;
	alienEnemy.dead = 0;
	init(&game, 3, GameStartMenu);
	pause_game = 0;

    }
    else if (!backstoryOn && !GameStartMenu) { 
	changeBackground(background, Level1Texture, Level2Texture,  
		Level3Texture);


	asteroidsRemainingBox(r, g);	
	if (score >= 300 && levelnum ==1){
	    levelnum = 2;
	    nextLevel(health, fuel, bulletsRemain, g, GameStartMenu);
	    getAudio(4, alSource);		
	    background = 2;
	    changeBackground(background, Level1Texture, Level2Texture, Level3Texture); 
	}
	if (score >= 750 && levelnum ==2){
	    levelnum = 3;
	    nextLevel(health, fuel, bulletsRemain, g, GameStartMenu);
	    getAudio(5, alSource);
	    background = 3;
	    changeBackground(background, Level1Texture, Level2Texture, Level3Texture);
	}

	showLevel(r, levelnum);
	// Display and decrease health when colliding with asteroid
	// Chris added
	//
	menubar(((xres/2)),r, score);
	healthbar(((xres/2)+200),r, health);

	// Display and decrease fuel when pressing up key
	// Jonathan added
	fuelbar(((xres/2)-190),r, fuel);
	//pause game created by Joe
	if (pause_game && dead==0)
	    pauseGame(xres, yres, pbox);
	if (dead) 
	    deadGame(xres, yres, pbox);
	if (fuel == 0 && g->astronaut.vel[0] < 0.08 && g->astronaut.vel[1] 
		< 0.08) {
	    strandedGame(xres, yres, pbox);	    
	    pause_game = 1;
	}
	if (!pause_game) {
	    //--------------------------------------------------------
	    // Jonathan
	    if (keys[XK_Up] && fuelRemains(fuel)) {
		int i;
		//draw thrust
		// Jonathan Added
		// Reduce fuel remaining
		if (fuelRemains(fuel)) {
		    fuel = reduceFuel(fuel);
		}
		Flt rad = (((g->astronaut.angle+90.0) / 360.0f) * PI * 2.0);
		//convert angle to a vector
		Flt xdir = cos(rad);
		Flt ydir = sin(rad);
		Flt xs,ys,xe,ye,r;
		glBegin(GL_LINES);
		for (i=0; i<8; i++) {
		    xs = (-xdir * 11.0f + rnd() * 8.0 - 2.0);
		    ys = (-ydir * 11.0f + rnd() * 8.0 - 2.0);
		    r = rnd()+30.0;
		    xe = (-xdir * r + rnd() * 18.0 - 9.0);
		    ye = (-ydir * r + rnd() * 18.0 - 9.0);
		    glColor3f(rnd()*.3+.7, rnd()*.3+.7, 0);
		    glVertex2f(g->astronaut.pos[0]+xs,g->astronaut.pos[1]+ys);
		    glVertex2f(g->astronaut.pos[0]+xe,g->astronaut.pos[1]+ye);
		    glColor3f(1.0f, 1.0f, 1.0f);
		}
		glEnd();
	    }

	}

	//Draw the bullets
	for (int i=0; i<g->nbullets; i++) {
	    Bullet *b = &g->barr[i]; 
	    //Log("draw bullet...\n");
	    glBegin(GL_POINTS);
	    glVertex2f(b->pos[0],      b->pos[1]);
	    glVertex2f(b->pos[0]-1.0f, b->pos[1]);
	    glVertex2f(b->pos[0]+1.0f, b->pos[1]);
	    glVertex2f(b->pos[0],      b->pos[1]-1.0f);
	    glVertex2f(b->pos[0],      b->pos[1]+1.0f);
	    glVertex2f(b->pos[0]-1.0f, b->pos[1]-1.0f);
	    glVertex2f(b->pos[0]-1.0f, b->pos[1]+1.0f);
	    glVertex2f(b->pos[0]+1.0f, b->pos[1]-1.0f);
	    glVertex2f(b->pos[0]+1.0f, b->pos[1]+1.0f);
	    glEnd();
	}
	glPushMatrix();
	glTranslatef(g->astronaut.pos[0], g->astronaut.pos[1], g->astronaut.pos[2]);
	//float angle = atan2(astronaut.dir[1], astronaut.dir[0]);
	glRotatef(g->astronaut.angle, 0.0f, 0.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, silhouette_astronautpicTexturepic);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex2i(-20,-20);
	glTexCoord2f(0.0f, 0.0f); glVertex2i(-20,20);
	glTexCoord2f(1.0f, 0.0f); glVertex2i(20,20);
	glTexCoord2f(1.0f, 1.0f); glVertex2i(20,-20);
	glEnd();
	glPopMatrix();


	//-------------------------------------------------------------------------

	if (score > 50 && alienEnemy.dead == 0){
	    DrawAlien(alienTexturepic, &alienEnemy);
	    alienActive = 1;
	}
	else{
	    alienActive = 0;
	}

	if (levelnum > 1 && score > 0 && alienEnemy.dead == 0){
	    DrawAlien(alienTexturepic, &alienEnemy);
	    alienActive = 1;
	}

	bulletdisplay(bulletsRemain,bullet_sprite);
	//Draw the asteroids
	{
	    Asteroid *a = g->ahead;
	    while (a) {
		//Log("draw asteroid...\n");
		//glColor3fv(a->color);
		glPushMatrix();
		glTranslatef(a->pos[0], a->pos[1], a->pos[2]);
		glRotatef(a->angle, 0.0f, 0.0f, 1.0f);

		//DrawAsteroids(AsteroidTexturepic, a);
		glBindTexture(GL_TEXTURE_2D, silhouetteTexture);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.0f);
		//glColor4ub(255,255,255,255);
		glBegin(GL_QUADS);

		glTexCoord2f(0.0f, 1.0f); glVertex2i(-a->radius-10,-a->radius-10);
		glTexCoord2f(0.0f, 0.0f); glVertex2i(-a->radius-10,a->radius+10);
		glTexCoord2f(1.0f, 0.0f); glVertex2i(a->radius+10,a->radius+10);
		glTexCoord2f(1.0f, 1.0f); glVertex2i(a->radius+10,-a->radius-10);

		glEnd();
		glPopMatrix();	

		glPopMatrix();
		//glBegin(GL_POINTS);
		//glVertex2f(a->pos[0], a->pos[1]);
		//glEnd();
		a = a->next;
	    }
	    if (fuel < 100) {
		DrawFuelBox(fuelSilhouette, &fuelbox);
		gotFuel = 0;
	    }
	    if (health < 100) {
		DrawHealthBox(healthSilhouette, &healthbox);
		gotHealth = 0;
	    }
	    if (bulletsRemain < 20){				 
		DrawAmoBox(ammoSilhouette, &amobox);
		gotAmo = 0;
	    }

	}
    }
}
