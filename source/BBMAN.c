//Using libs SDL, glibc
#include <SDL.h>	//SDL version 2.0
#include <stdlib.h>
#include <stdio.h>


// Numbers bitmap, some routines and and SDL initialization taken from
// https://github.com/flightcrank/pong


// Please follow the coding guidelines described in:
// https://users.ece.cmu.edu/~eno/coding/CCodingStandard.html



/* Do not use magic numbers in the code, define the meaning of the number
   in this section and the use the define across the code. This increases
   readability accross the code
*/


// The constants used to define the size of the window
#define SCREEN_WIDTH 1280	//window height
#define SCREEN_HEIGHT 720	//window width

// The constants used for the movement of the block
#define LEFT 0
#define RIGHT 1
#define UP 2
#define DOWN 3


// Block size that will be used for drawing on the screen
#define BLOCK_SIZE 50


// Coordinates of the starting player
#define PLAYER_START_X 150
#define PLAYER_START_Y 150

// For the return of the functions
#define SUCCESS 0
#define FAILURE 1

// Defines of the game states of each screen
#define START_SCREEN 0
#define LEVEL_1 1
#define GAME_OVER 2

// Defines for states
#define FALSE 0
#define TRUE 1

// The colors used in the game
#define BLACK 0x000000ff
#define WHITE 0xffffffff

// Timing delays
#define INPUT_DELAY_MS 500

// Time to render a frame in ms = 1000/60
#define FRAME_TIME_MS 16

// Movement diferential
#define MOVEMENT_DELTA 5



//function prototypes
//initialise SDL
int init_SDL(int w, int h, int argc, char *args[]);


typedef struct player_t {

	int x; 
	int y;
	int w; 
	int h;

} player_t;

typedef struct game_element_t {
	int x;
	int y;
	int w;
	int h;

} game_element_t;

// This is one of the few cases where it makes sense to use magic numbers
// Avoid the use of global variables at maximum
int g_score[] = {0,0}; 
// Avoid the use of global variables, modify the code to avoid its use.
int g_width, g_height;		//used if fullscreen

SDL_Window* window = NULL;	//The window we'll be rendering to
SDL_Renderer *renderer;		//The renderer SDL will use to draw to the screen

//surfaces
static SDL_Surface *screen;
static SDL_Surface *title;
static SDL_Surface *numbermap;
static SDL_Surface *end;

//textures
SDL_Texture *screen_texture;

// Always make a header for each one of the use created functions

/* Function: init_game
 * ---------------------------------
 * This function initialises the position of the player in the screen
 * (the block that can be moved with the WASD keys), and the block that
 * is part of the map, which is a test for collisions. Please note that
 * in this functions we are passing pointers, not values.
 *
 * Arguments:
 *	player:	Pointer to the player object
 *	map_element: Pointer to the map element object.
 * 
 * Return:
 * 	void.
 */
static void init_game(game_element_t *player, game_element_t *map_element, game_element_t *map_element1,game_element_t *map_element2, game_element_t *map_element3) {
	// Here the function is receiving the pointer to the player object
	// it modifies the player object directly
	player->x = PLAYER_START_X;
	player->y = PLAYER_START_Y;
	player->w = BLOCK_SIZE;
	player->h = BLOCK_SIZE;
 
 	//Barrera de la derecha
	map_element->x = 1210;    //screen->w/64;
	map_element->y = 1;    //screen->h/10;
	map_element->w = 1.5*BLOCK_SIZE;
	map_element->h = 50*BLOCK_SIZE;
	
	//Barrera de la izquierda
	map_element1->x = 1;  //screen->w/168;
	map_element1->y = 1;   //screen->h/168;
	map_element1->w = 1.5*BLOCK_SIZE;
	map_element1->h = 50*BLOCK_SIZE;
	
	//Barrera superior
	map_element2->x = 640;
	map_element2->y = 1;
	map_element2->w = 50*BLOCK_SIZE;
	map_element2->h = 1.5*BLOCK_SIZE;
	
	//Barrera inferior
	map_element2->x = 640;
	map_element2->y = 600;
	map_element2->w = 50*BLOCK_SIZE;
	map_element2->h = 1.5*BLOCK_SIZE;
	
	//Inamovible
	map_element3->x = screen->w/2;
	map_element3->y = screen->h/1;
	map_element3->w = 1.5*BLOCK_SIZE;
	map_element3->h = 1.5*BLOCK_SIZE;
	

}



/* Function: check_collision
 * --------------------------
 * This function verifies if there is collision between two game
 * elements.
 *
 * Arguments:
 * 	a: game element to be checked
 *	b: game element to be checked
 *
 * Return:
 *	TRUE if there is collision 
 * 	FALSE if there isn't collision
 */
int check_collision(game_element_t a, game_element_t b){
	
	int left_a = a.x;
	int right_a = a.x + a.w;
	int top_a = a.y;
	int bottom_a = a.y + a.h;

	int left_b = b.x;
	int right_b = b.x + b.w;
	int top_b = b.y;
	int bottom_b = b.y + b.h;

	if (bottom_a <= top_b)
		return FALSE;

	if (top_a >= bottom_b)
		return FALSE;

	if (right_a <= left_b)
		return FALSE;

	if (left_a >= right_b)
		return FALSE;

	// If none of the conditions worked, return TRUE

	return TRUE;

}


/* Function: move_player
 * ---------------------
 * This function is in charge of relocating the player position
 * across the screen
 *
 * Arguments:
 *	d: direction on which the player is going to be moved.
 *
 * Return:
 *	void.
 */

void move_player(int d, game_element_t *player, game_element_t *map_element, game_element_t *map_element1,game_element_t *map_element2, game_element_t *map_element3){
	//
	if (d == LEFT) {
		player->x -= MOVEMENT_DELTA;
		if (check_collision(*player, *map_element) == TRUE  || check_collision(*player, *map_element1) == TRUE || check_collision(*player, *map_element2) == TRUE || check_collision(*player, *map_element3) == TRUE)
			player->x += MOVEMENT_DELTA;
	}

	if (d == RIGHT) {
		player->x += MOVEMENT_DELTA;
		if (check_collision(*player, *map_element) == TRUE  || check_collision(*player, *map_element1) == TRUE || check_collision(*player, *map_element2) == TRUE || check_collision(*player, *map_element3) == TRUE)
			player->x -= MOVEMENT_DELTA;
	}
		
	if (d == UP) {
		player->y -= MOVEMENT_DELTA;
		if (check_collision(*player, *map_element) == TRUE  || check_collision(*player, *map_element1) == TRUE || check_collision(*player, *map_element2) == TRUE || check_collision(*player, *map_element3) == TRUE)
			player->y += MOVEMENT_DELTA;
	}

	if (d == DOWN) {
		player->y += MOVEMENT_DELTA;
		if (check_collision(*player, *map_element) == TRUE  || check_collision(*player, *map_element1) == TRUE || check_collision(*player, *map_element2) == TRUE || check_collision(*player, *map_element3) == TRUE)
			player->y -= MOVEMENT_DELTA;
	}
}

/* Function: draw_game_over
 * ----------------------------
 * Currently this function is not being used but it will function
 * when the developed game requires a game over screen.
 *
 * Arguments: 
 *	none
 *
 * Return:
 *
 *	void.
 */
static void draw_game_over() { 

	SDL_Rect p1;
	SDL_Rect dest;

	p1.x = 0;
	p1.y = 0;
	p1.w = end->w;
	p1.h = 75;

	dest.x = (screen->w / 2) - (end->w / 2);
	dest.y = (screen->h / 2) - (75 / 2);
	dest.w = end->w;
	dest.h = 75;
	
	SDL_BlitSurface(end, &p1, screen, &dest);
	
}

/* Function: draw_menu
 * --------------------------------
 * This function is in charge of drawing the first screen that is
 * presented to the player.
 *
 * Arguments:
 *	none
 *
 * Return:
 *	void.
 */
static void draw_menu() {

	SDL_Rect src;
	SDL_Rect dest;

	src.x = 0;
	src.y = 0;
	src.w = title->w;
	src.h = title->h;

	dest.x = (screen->w / 2) - (src.w / 2);
	dest.y = (screen->h / 2) - (src.h / 2);
	dest.w = title->w;
	dest.h = title->h;

	SDL_BlitSurface(title, &src, screen, &dest);
}


/* Function: draw_game_element
 * -------------------------------
 * This functions draws a game element on the screen according to its
 * properties and coordinates. Please note that the element to be 
 * drawn is passed as an reference insted of a value.
 *
 * Arguments:
 *	element: Element to be drawn on the screen.
 *
 * Return:
 *	void.
 */
static void draw_game_element(game_element_t *element) {

	SDL_Rect src;
	int i;

	for (i = 0; i < 2; i++) {
	
		src.x = element->x;
		src.y = element->y;
		src.w = element->w;
		src.h = element->h;
	
		int r = SDL_FillRect(screen, &src, 0xffffffff);
		
		if (r !=0){
		
			printf("fill rectangle failed in func draw_paddle()");
		}
	}
}

// The header of this two functions are for the student to complete
// Try changing the coordinates to see the effect that has on the game
// Also, are here the magic numbers justified? Or should be declared
// as constants in the beggining of the file?
static void draw_game_element_0_score() {
	
	SDL_Rect src;
	SDL_Rect dest;

	src.x = 0;
	src.y = 0;
	src.w = 64;
	src.h = 64;

	dest.x = (screen->w / 2) - src.w - 12; //12 is just padding spacing
	dest.y = 0;
	dest.w = 64;
	dest.h = 64;

	if (g_score[0] > 0 && g_score[0] < 10) {
		
		src.x += src.w * g_score[0];
	}
	
	SDL_BlitSurface(numbermap, &src, screen, &dest);
}

static void draw_game_element_1_score() {
	
	SDL_Rect src;
	SDL_Rect dest;

	src.x = 0;
	src.y = 0;
	src.w = 64;
	src.h = 64;

	dest.x = (screen->w / 2) + 12;
	dest.y = 0;
	dest.w = 64;
	dest.h = 64;
	
	if (g_score[1] > 0 && g_score[1] < 10) {
		
		src.x += src.w * g_score[1];
	}

	SDL_BlitSurface(numbermap, &src, screen, &dest);
}

// Main function

int main (int argc, char *args[]) {

	// Define the player and the maps
	game_element_t player;

	// For the project the elements of the map should be created
	// dinamically (using malloc) and using linked lists.
	game_element_t map_element; 
	game_element_t map_element1;
	game_element_t map_element2; 
	game_element_t map_element3;

	//SDL Window setup
	if (init_SDL(SCREEN_WIDTH, SCREEN_HEIGHT, argc, args) == FAILURE) {
		
		return FAILURE;
	}
	
	SDL_GetWindowSize(window, &g_width, &g_height);
	
	int sleep = 0;
	int quit = FALSE;
	int state = START_SCREEN;
	Uint32 next_game_tick = SDL_GetTicks();
	
	// Initialize the ball position data. 
	init_game(&player, &map_element,&map_element1, &map_element2,&map_element3); // The & means "Address of"
	
	//render loop
	while(quit == FALSE) {
	
		//check for new events every frame
		SDL_PumpEvents();

		const Uint8 *keystate = SDL_GetKeyboardState(NULL);
		
		if (keystate[SDL_SCANCODE_ESCAPE]) {
		
			quit = TRUE;
		}
		
		if (keystate[SDL_SCANCODE_S]) {
			
			move_player(DOWN, &player, &map_element, &map_element1, &map_element2, &map_element3);
		}

		if (keystate[SDL_SCANCODE_W]) {
			
			move_player(UP, &player, &map_element, &map_element1, &map_element2, &map_element3);
		}
		
		if (keystate[SDL_SCANCODE_A]) {
			
			move_player(LEFT, &player, &map_element, &map_element1, &map_element2, &map_element3);
		}

		if (keystate[SDL_SCANCODE_D]) {
			
			move_player(RIGHT, &player, &map_element, &map_element1, &map_element2, &map_element3);
		}
		//draw background
		SDL_RenderClear(renderer);
		SDL_FillRect(screen, NULL, BLACK);
		
		//display main menu
		if (state == START_SCREEN ) {
		
			if (keystate[SDL_SCANCODE_SPACE]) {
				
				state = LEVEL_1;
			}
		
			//draw menu 
			draw_menu();
		
		//display gameover
		} else if (state == GAME_OVER) {
		
			if (keystate[SDL_SCANCODE_SPACE]) {
				state = START_SCREEN;
				// delay for a little bit so the space bar press doesnt get 
				// triggered twice
				// while the main menu is showing
            			SDL_Delay(INPUT_DELAY_MS);
			}

				draw_game_over();
				
		//display the game
		} else if (state == LEVEL_1) {
			
		
			//if either player wins, change to game over state
			if (FALSE) {	//Doing nothing for the moment
				
				state = GAME_OVER;	

			} 
		
			// Here we draw the player that we move across 
			draw_game_element(&player);

			// We draw the map element that is going to be static
			draw_game_element(&map_element);
			draw_game_element(&map_element1);
			draw_game_element(&map_element2);
			draw_game_element(&map_element3);


			//draw the score
			draw_game_element_0_score();
	
			//draw the score
			draw_game_element_1_score();
		}
	
		SDL_UpdateTexture(screen_texture, NULL, screen->pixels, 
						  screen->w * sizeof (Uint32));
		SDL_RenderCopy(renderer, screen_texture, NULL, NULL);

		//draw to the display
		SDL_RenderPresent(renderer);
				
		//time it takes to render  frame in milliseconds
		next_game_tick += FRAME_TIME_MS;
		sleep = next_game_tick - SDL_GetTicks();
	
		if( sleep >= 0 ) {
            				
			SDL_Delay(sleep);
		}
	}

	//free loaded images
	SDL_FreeSurface(screen);
	SDL_FreeSurface(title);
	SDL_FreeSurface(numbermap);
	SDL_FreeSurface(end);

	//free renderer and all textures used with it
	SDL_DestroyRenderer(renderer);
	
	//Destroy window 
	SDL_DestroyWindow(window);

	//Quit SDL subsystems 
	SDL_Quit(); 
	 
	return SUCCESS;
	
}

/* Function: init_SDL
 * ----------------------------
 * This function initialises the required environment for SDL
 * to work.
 *
 * Arguments:
 *	width: The width of the screen.
 * 	height: The height of the screen.
 *	argc: The same parameter from the main function.
 *	args: The same parameter from the main function.
 *
 * Return:
 *	SUCCESS if no problem reported.
 *	FAILURE if any initialization failed
 */
int init_SDL(int width, int height, int argc, char *args[]) {

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {

		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		
		return FAILURE;
	} 
	
	int i;
	
	for (i = 0; i < argc; i++) {
		
		//Create window	
		if(strcmp(args[i], "-f")) {
			
			SDL_CreateWindowAndRenderer(SCREEN_WIDTH, 
										SCREEN_HEIGHT, 
										SDL_WINDOW_SHOWN, 
										&window, 
										&renderer);
		
		} else {
		
			SDL_CreateWindowAndRenderer(SCREEN_WIDTH, 
										SCREEN_HEIGHT, 
										SDL_WINDOW_FULLSCREEN_DESKTOP, 
										&window, 
										&renderer);
		}
	}

	if (window == NULL) { 
		
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		
		return FAILURE;
	}

	// Create the screen surface where all the elements will be drawn
	screen = SDL_CreateRGBSurfaceWithFormat(0, width, height, 32, 
											SDL_PIXELFORMAT_RGBA32);
	
	if (screen == NULL) {
		
		printf("Could not create the screen surfce! SDL_Error: %s\n", 
			   SDL_GetError());

		return FAILURE;
	}

	// Create the screen texture to render the screen surface to the actual 
	// display
	screen_texture = SDL_CreateTextureFromSurface(renderer, screen);

	if (screen_texture == NULL) {
		
		printf("Could not create the screen_texture! SDL_Error: %s\n", 
				SDL_GetError());

		return FAILURE;
	}

	//Load the title image
	title = SDL_LoadBMP("title.bmp");

	if (title == NULL) {
		
		printf("Could not Load title image! SDL_Error: %s\n", SDL_GetError());

		return FAILURE;
	}
	
	//Load the numbermap image
	numbermap = SDL_LoadBMP("numbermap.bmp");

	if (numbermap == NULL) {
		
		printf("Could not Load numbermap image! SDL_Error: %s\n", 
				SDL_GetError());

		return FAILURE;
	}
	
	//Load the gameover image
	end = SDL_LoadBMP("gameover.bmp");

	if (end == NULL) {
		
		printf("Could not Load title image! SDL_Error: %s\n", SDL_GetError());

		return FAILURE;
	}
	
	// Set the title colourkey. 
	Uint32 colorkey = SDL_MapRGB(title->format, 255, 0, 255);
	SDL_SetColorKey(title, SDL_TRUE, colorkey);
	SDL_SetColorKey(numbermap, SDL_TRUE, colorkey);
	
	return SUCCESS;
}
