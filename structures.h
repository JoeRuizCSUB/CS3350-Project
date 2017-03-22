struct Astronaut {
    Vec dir;
    Vec pos;
    Vec vel;
    int mass;
    float angle;
    float color[3];
    Astronaut() {
    VecZero(dir);
    pos[0] = (Flt)(xres/2);
    pos[1] = (Flt)(yres/2);
    pos[2] = 0.0f;
    VecZero(vel);
    angle = 0.0;
    color[0] = 1.0;
    color[1] = 1.0;
    color[2] = 1.0;
    }
};

struct Bullet {
    Vec pos;
    Vec vel;
    float color[3];
    struct timespec time;
    Bullet() { }
};


struct Asteroid {
    Vec pos;
    Vec vel;
    int mass;
    int nverts;
    Flt radius;
    Vec vert[8];
    float angle;
    float rotate;
    float color[3];
    struct Asteroid *prev;
    struct Asteroid *next;
    Asteroid() {
    prev = NULL;
    next = NULL;
    }
};


// Created by Joe
struct Debris {
    Vec pos;
    Vec vel;
    int mass;
    Flt radius;
    Debris() { }
};

struct Game {
    Astronaut astronaut;
    Asteroid *ahead;
    Bullet *barr;
    int nasteroids;
    int nbullets;
    struct timespec bulletTimer;
    Game() {
    ahead = NULL;
    barr = new Bullet[MAX_BULLETS];
    nasteroids = 0;
    nbullets = 0;
    }
    ~Game() {
    delete [] barr;
    }
};
