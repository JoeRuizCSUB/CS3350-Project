int SeanKeypress(int &counter)
{
    if (counter <5) 
	counter++;
    else
       counter = 1;

    return(counter);
}
