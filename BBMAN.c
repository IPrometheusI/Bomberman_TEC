

//@Author: Maickol Fernandez

#include "SDL.h"   /* Se incluye la libreria SDL */
#include <stdio.h>
#include <unistd.h>




int main() {

    SDL_Surface *screen;

    /* Initialize the SDL library */
    if( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
        fprintf(stderr,
                "Couldn't initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }

    /* Clean up on exit */
    atexit(SDL_Quit);
    
    /*
     * Initialize the display in a 1280x720 8-bit palettized mode,
     * requesting a software surface
     */
    screen = SDL_SetVideoMode(1280,720 , 8, SDL_SWSURFACE);

    sleep(2); // format is Sleep(x); where x is # of seconds.
    
    if ( screen == NULL ) {
        fprintf(stderr, "Couldn't set 640x480x8 video mode: %s\n",
                        SDL_GetError());
        exit(1);}
}
