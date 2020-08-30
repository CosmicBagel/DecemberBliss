#include <stdio.h>

//learning goals
//figure out a nice way to reroute STD out such that we can catch everything being sent out of the app (shouldn't be too hard)
// same for cerr stream
//figure out a nice way to reroute STD in such that we can add to it,
//bounus  = not have the program lock up while waiting for input (will probably be really hard)
int main(){
    printf("router");
    return 0;
}