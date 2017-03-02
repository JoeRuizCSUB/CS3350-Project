//Author: Sean Nickell
//Purpose of file: Keypresses, and soon to be rendering
//Date Modified: 3/2/17

extern int xres, yres;

int SeanKeypress(int &counter)
{
    if (counter <5) 
	counter++;
    else
       counter = 1;

    return(counter);
}


