// Basic Hello World.  From
// http://lazyfoo.net/SDL_tutorials/lesson01/index2.php
//#include "SDL/SDL.h"
#include "SDL2/SDL.h"
#include <stdio.h>

const int SCREEN_WIDTH  = 640;
const int SCREEN_HEIGHT = 480;

int main( int argc, char* args[] ){
    // define the images
    //SDL_Surface* screen = NULL;
    //SDL_Surface* hello = NULL;

    //SDL_Init( SDL_INIT_EVERYTHING );
    //SDL_Quit();
    
    // SDL2 version
    SDL_Window* window = NULL;

    SDL_Surface* screenSurface = NULL;

    printf( "%d\n", SDL_Init( SDL_INIT_VIDEO ) );

    return 0;
}
