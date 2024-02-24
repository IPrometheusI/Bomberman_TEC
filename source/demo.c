//Using libs SDL, glibc
#include <SDL.h>	//SDL version 2.0
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>


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
#define DROP 4


// Block size that will be used for drawing on the screen
#define BLOCK_SIZE 50


// Coordinates of the starting player
#define PLAYER_START_X 85
#define PLAYER_START_Y 125

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
#define BLUE 0xFF7D0000
#define GREEN 0x00FF00FF // Verde con opacidad completa
#define YELLOW 0xFFFF00FF // Amarillo con opacidad completa
#define ORANGE 0xFF7D0000


// Timing delays
#define INPUT_DELAY_MS 500

// Time to render a frame in ms = 1000/60
#define FRAME_TIME_MS 16

// Movement diferential
#define MOVEMENT_DELTA 5

Uint32 bomb_timer = 0; // Almacena el momento en que se coloca la bomba
int bomb_placed = 0; // Indica si la bomba ha sido colocada (0 = no, 1 = sí)


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
static SDL_Surface *Block;
static SDL_Surface *Block_des;
static SDL_Surface *bomb_image;
static SDL_Surface *fire;
static SDL_Surface *skin;
static SDL_Surface *monster;



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
static void init_game(game_element_t *player, game_element_t *map_element, game_element_t *map_element1,game_element_t *map_element2, game_element_t *map_element3, game_element_t *map_element4, game_element_t *map_element5,game_element_t *map_element6, game_element_t *map_element7,  game_element_t *map_element8, game_element_t *map_element9,game_element_t *map_element10, game_element_t *map_element11, game_element_t *map_element12, game_element_t *map_element13,game_element_t *map_element14, game_element_t *map_element15, game_element_t *map_element16,  game_element_t *map_element17, game_element_t *map_element18,game_element_t *map_element19, game_element_t *map_element20, game_element_t *map_element21, game_element_t *map_element22,game_element_t *map_element23, game_element_t *map_element24, game_element_t *map_des_block, game_element_t *map_des_block1, game_element_t *map_des_block2,game_element_t *map_des_block3,game_element_t *map_des_block4,game_element_t *map_des_block5,game_element_t *map_des_block6,game_element_t *map_des_block7,game_element_t *map_des_block8,game_element_t *map_des_block9,game_element_t *map_des_block10,game_element_t *map_des_block11,game_element_t *map_des_block12,game_element_t *map_des_block13,game_element_t *map_des_block14,game_element_t *map_des_block15, game_element_t *bomb_object, game_element_t *explosion_object) {
		
	// Here the function is receiving the pointer to the player object
	// it modifies the player object directly
	player->x = PLAYER_START_X;
	player->y = PLAYER_START_Y;
	player->w = 1.5*BLOCK_SIZE;
	player->h = 1.5*BLOCK_SIZE;
	
	
	//Objeto Bomba
	bomb_object->x = PLAYER_START_X+2000;
	bomb_object->y = PLAYER_START_Y+2000;
	bomb_object->w = BLOCK_SIZE;
	bomb_object->h = BLOCK_SIZE;
	
	//Explosion
	explosion_object->x = PLAYER_START_X+2000;
	explosion_object->y = PLAYER_START_Y+2000;
	explosion_object->w = BLOCK_SIZE;
	explosion_object->h = BLOCK_SIZE;

// aca inician los bloques de las orillas
	map_element->x = 1240;    //screen->w/64;
	map_element->y = 50;    //screen->h/10;
	map_element->w = 1*BLOCK_SIZE;
	map_element->h = 50*BLOCK_SIZE;
	
	map_element1->x = 1;  //screen->w/168;
	map_element1->y = 50;   //screen->h/168;
	map_element1->w = 1.5*BLOCK_SIZE;
	map_element1->h = 50*BLOCK_SIZE;
	
	map_element2->x = 1;
	map_element2->y = 665;
	map_element2->w = 50*BLOCK_SIZE;
	map_element2->h = 1.5*BLOCK_SIZE;
	
	map_element3->x = 1;
	map_element3->y = 50;
	map_element3->w = 50*BLOCK_SIZE;
	map_element3->h = 1.5*BLOCK_SIZE;
	
	// aca inician los bloques cuadrados de izquierda a derecha primer fila
	map_element4->x = 155;
	map_element4->y = 200;
	map_element4->w = 1.5*BLOCK_SIZE;
	map_element4->h = 1.5*BLOCK_SIZE;
	
	map_element5->x = 310;
	map_element5->y = 200;
	map_element5->w = 1.5*BLOCK_SIZE;
	map_element5->h = 1.5*BLOCK_SIZE;
	
	map_element6->x = 465;
	map_element6->y = 200;
	map_element6->w = 1.5*BLOCK_SIZE;
	map_element6->h = 1.5*BLOCK_SIZE;
	
	map_element7->x = 620;
	map_element7->y = 200;
	map_element7->w = 1.5*BLOCK_SIZE;
	map_element7->h = 1.5*BLOCK_SIZE;
	
	map_element8->x = 775;
	map_element8->y = 200;
	map_element8->w = 1.5*BLOCK_SIZE;
	map_element8->h = 1.5*BLOCK_SIZE;
	
	map_element9->x = 930;
	map_element9->y = 200;
	map_element9->w = 1.5*BLOCK_SIZE;
	map_element9->h = 1.5*BLOCK_SIZE;
	
	map_element10->x = 1085;
	map_element10->y = 200;
	map_element10->w = 1.5*BLOCK_SIZE;
	map_element10->h = 1.5*BLOCK_SIZE;
	
	// aca inician los bloques cuadrados de izquierda a derecha segunda fila
	
	map_element11->x = 155;
	map_element11->y = 355;
	map_element11->w = 1.5*BLOCK_SIZE;
	map_element11->h = 1.5*BLOCK_SIZE;
	
	map_element12->x = 310;
	map_element12->y = 355;
	map_element12->w = 1.5*BLOCK_SIZE;
	map_element12->h = 1.5*BLOCK_SIZE;
	
	map_element13->x = 465;
	map_element13->y = 355;
	map_element13->w = 1.5*BLOCK_SIZE;
	map_element13->h = 1.5*BLOCK_SIZE;
	
	map_element14->x = 620;
	map_element14->y = 355;
	map_element14->w = 1.5*BLOCK_SIZE;
	map_element14->h = 1.5*BLOCK_SIZE;
	
	map_element15->x = 775;
	map_element15->y = 355;
	map_element15->w = 1.5*BLOCK_SIZE;
	map_element15->h = 1.5*BLOCK_SIZE;
	
	map_element16->x = 930;
	map_element16->y = 355;
	map_element16->w = 1.5*BLOCK_SIZE;
	map_element16->h = 1.5*BLOCK_SIZE;
	
	map_element17->x = 1085;
	map_element17->y = 355;
	map_element17->w = 1.5*BLOCK_SIZE;
	map_element17->h = 1.5*BLOCK_SIZE;
	
	// aca inician los bloques cuadrados de izquierda a derecha tercera fila
	
	map_element18->x = 155;
	map_element18->y = 510;
	map_element18->w = 1.5*BLOCK_SIZE;
	map_element18->h = 1.5*BLOCK_SIZE;
	
	map_element19->x = 310;
	map_element19->y = 510;
	map_element19->w = 1.5*BLOCK_SIZE;
	map_element19->h = 1.5*BLOCK_SIZE;
	
	map_element20->x = 465;
	map_element20->y = 510;
	map_element20->w = 1.5*BLOCK_SIZE;
	map_element20->h = 1.5*BLOCK_SIZE;
	
	map_element21->x = 620;
	map_element21->y = 510;
	map_element21->w = 1.5*BLOCK_SIZE;
	map_element21->h = 1.5*BLOCK_SIZE;
	
	map_element22->x = 775;
	map_element22->y = 510;
	map_element22->w = 1.5*BLOCK_SIZE;
	map_element22->h = 1.5*BLOCK_SIZE;
	
	map_element23->x = 930;
	map_element23->y = 510;
	map_element23->w = 1.5*BLOCK_SIZE;
	map_element23->h = 1.5*BLOCK_SIZE;
	
	map_element24->x = 1085;
	map_element24->y = 510;
	map_element24->w = 1.5*BLOCK_SIZE;
	map_element24->h = 1.5*BLOCK_SIZE;
	
	//Bloques destructibles
	map_des_block->x = 233;
	map_des_block->y = 125;
	map_des_block->w = 1.5*BLOCK_SIZE;
	map_des_block->h = 1.5*BLOCK_SIZE;
	
	map_des_block1->x = 233;
	map_des_block1->y = 278+50;
	map_des_block1->w = 1.5*BLOCK_SIZE;
	map_des_block1->h = 1.5*BLOCK_SIZE;
	
	map_des_block2->x = 78;
	map_des_block2->y = 278;
	map_des_block2->w = 1.5*BLOCK_SIZE;
	map_des_block2->h = 1.5*BLOCK_SIZE;
	
	map_des_block3->x = 155;
	map_des_block3->y = 433;
	map_des_block3->w = 1.5*BLOCK_SIZE;
	map_des_block3->h = 1.5*BLOCK_SIZE;
	
	map_des_block4->x = 310;
	map_des_block4->y = 278;
	map_des_block4->w = 1.5*BLOCK_SIZE;
	map_des_block4->h = 1.5*BLOCK_SIZE;
	
	
	map_des_block5->x = 387;
	map_des_block5->y = 433;
	map_des_block5->w = 1.5*BLOCK_SIZE;
	map_des_block5->h = 1.5*BLOCK_SIZE;
	
	map_des_block6->x = 387;
	map_des_block6->y = 125;
	map_des_block6->w = 1.5*BLOCK_SIZE;
	map_des_block6->h = 1.5*BLOCK_SIZE;
	
	map_des_block7->x = 387;
	map_des_block7->y = 588;
	map_des_block7->w = 1.5*BLOCK_SIZE;
	map_des_block7->h = 1.5*BLOCK_SIZE;
	
	map_des_block8->x = 310;
	map_des_block8->y = 588;
	map_des_block8->w = 1.5*BLOCK_SIZE;
	map_des_block8->h = 1.5*BLOCK_SIZE;
	
	map_des_block9->x = 852;
	map_des_block9->y = 588;
	map_des_block9->w = 1.5*BLOCK_SIZE;
	map_des_block9->h = 1.5*BLOCK_SIZE;
	
	map_des_block10->x = 852;
	map_des_block10->y = 433;
	map_des_block10->w = 1.5*BLOCK_SIZE;
	map_des_block10->h = 1.5*BLOCK_SIZE;
	
	map_des_block11->x = 543;
	map_des_block11->y = 278;
	map_des_block11->w = 1.5*BLOCK_SIZE;
	map_des_block11->h = 1.5*BLOCK_SIZE;
	
	map_des_block12->x = 698;
	map_des_block12->y = 125;
	map_des_block12->w = 1.5*BLOCK_SIZE;
	map_des_block12->h = 1.5*BLOCK_SIZE;
	
	map_des_block13->x = 930;
	map_des_block13->y = 278;
	map_des_block13->w = 1.5*BLOCK_SIZE;
	map_des_block13->h = 1.5*BLOCK_SIZE;
	
	map_des_block14->x = 1007;
	map_des_block14->y = 201;
	map_des_block14->w = 1.5*BLOCK_SIZE;
	map_des_block14->h = 1.5*BLOCK_SIZE;
	
	map_des_block15->x = 1162;
	map_des_block15->y = 278;
	map_des_block15->w = 1.5*BLOCK_SIZE;
	map_des_block15->h = 1.5*BLOCK_SIZE;
	
	
	
	
	
	
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

void move_player(int d, game_element_t *player, game_element_t *map_element, game_element_t *map_element1,game_element_t *map_element2, game_element_t *map_element3, game_element_t *map_element4, game_element_t *map_element5,game_element_t *map_element6, game_element_t *map_element7,  game_element_t *map_element8, game_element_t *map_element9,game_element_t *map_element10, game_element_t *map_element11, game_element_t *map_element12, game_element_t *map_element13,game_element_t *map_element14, game_element_t *map_element15, game_element_t *map_element16,  game_element_t *map_element17, game_element_t *map_element18,game_element_t *map_element19, game_element_t *map_element20, game_element_t *map_element21, game_element_t *map_element22,game_element_t *map_element23, game_element_t *map_element24, game_element_t *map_des_block, game_element_t *map_des_block1,game_element_t *map_des_block2,game_element_t *map_des_block3,game_element_t *map_des_block4,game_element_t *map_des_block5,game_element_t *map_des_block6,game_element_t *map_des_block7,game_element_t *map_des_block8,game_element_t *map_des_block9,game_element_t *map_des_block10,game_element_t *map_des_block11, game_element_t *map_des_block12,game_element_t *map_des_block13,game_element_t *map_des_block14,game_element_t *map_des_block15){
	//
	if (d == LEFT) {
		player->x -= MOVEMENT_DELTA;
		if (check_collision(*player, *map_element) == TRUE  || check_collision(*player, *map_element1) == TRUE || check_collision(*player, *map_element2) == TRUE || 
		check_collision(*player, *map_element3) == TRUE || check_collision(*player, *map_element4) == TRUE || check_collision(*player, *map_element5) == TRUE || 
		check_collision(*player, *map_element6) == TRUE || check_collision(*player, *map_element7) == TRUE || check_collision(*player, *map_element8) == TRUE || 
		check_collision(*player, *map_element9) == TRUE || check_collision(*player, *map_element10) == TRUE || check_collision(*player, *map_element11) == TRUE || 
		check_collision(*player, *map_element12) == TRUE || check_collision(*player, *map_element13) == TRUE || check_collision(*player, *map_element14) == TRUE || 
		check_collision(*player, *map_element15) == TRUE || check_collision(*player, *map_element16) == TRUE || check_collision(*player, *map_element17) == TRUE || 
		check_collision(*player, *map_element17) == TRUE || check_collision(*player, *map_element18) == TRUE || check_collision(*player, *map_element19) == TRUE || 
		check_collision(*player, *map_element20) == TRUE || check_collision(*player, *map_element21) == TRUE || check_collision(*player, *map_element22) == TRUE || 
		check_collision(*player, *map_element23) == TRUE || check_collision(*player, *map_element24) == TRUE || check_collision(*player, *map_des_block) == TRUE || 
		check_collision(*player, *map_des_block1) == TRUE || check_collision(*player, *map_des_block2) == TRUE || check_collision(*player, *map_des_block3) == TRUE ||
		check_collision(*player, *map_des_block4) == TRUE || check_collision(*player, *map_des_block5) == TRUE || check_collision(*player, *map_des_block6) == TRUE || 
		check_collision(*player, *map_des_block7) == TRUE || check_collision(*player, *map_des_block8) == TRUE || check_collision(*player, *map_des_block9) == TRUE ||
		check_collision(*player, *map_des_block10) == TRUE || check_collision(*player, *map_des_block11) == TRUE || check_collision(*player, *map_des_block12) == TRUE ||
		check_collision(*player, *map_des_block13) == TRUE || check_collision(*player, *map_des_block14) == TRUE || check_collision(*player, *map_des_block15) == TRUE)
			player->x += MOVEMENT_DELTA;
	}

	if (d == RIGHT) {
		player->x += MOVEMENT_DELTA;
		if (check_collision(*player, *map_element) == TRUE  || check_collision(*player, *map_element1) == TRUE || check_collision(*player, *map_element2) == TRUE || 
		check_collision(*player, *map_element3) == TRUE || check_collision(*player, *map_element4) == TRUE || check_collision(*player, *map_element5) == TRUE || 
		check_collision(*player, *map_element6) == TRUE || check_collision(*player, *map_element7) == TRUE   || check_collision(*player, *map_element8) == TRUE || 
		check_collision(*player, *map_element9) == TRUE || check_collision(*player, *map_element10) == TRUE || check_collision(*player, *map_element11) == TRUE || 
		check_collision(*player, *map_element12) == TRUE || check_collision(*player, *map_element13) == TRUE || check_collision(*player, *map_element14) == TRUE || 
		check_collision(*player, *map_element15) == TRUE || check_collision(*player, *map_element16) == TRUE || check_collision(*player, *map_element17) == TRUE || 
		check_collision(*player, *map_element17) == TRUE   || check_collision(*player, *map_element18) == TRUE || check_collision(*player, *map_element19) == TRUE || 
		check_collision(*player, *map_element20) == TRUE || check_collision(*player, *map_element21) == TRUE || check_collision(*player, *map_element22) == TRUE || 
		check_collision(*player, *map_element23) == TRUE || check_collision(*player, *map_element24) == TRUE || check_collision(*player, *map_des_block) == TRUE || 
		check_collision(*player, *map_des_block1) == TRUE || check_collision(*player, *map_des_block2) == TRUE || check_collision(*player, *map_des_block3) == TRUE ||
		check_collision(*player, *map_des_block4) == TRUE || check_collision(*player, *map_des_block5) == TRUE || check_collision(*player, *map_des_block6) == TRUE || 
		check_collision(*player, *map_des_block7) == TRUE || check_collision(*player, *map_des_block8) == TRUE || check_collision(*player, *map_des_block9) == TRUE ||
		check_collision(*player, *map_des_block10) == TRUE || check_collision(*player, *map_des_block11) == TRUE || check_collision(*player, *map_des_block12) == TRUE ||
		check_collision(*player, *map_des_block13) == TRUE || check_collision(*player, *map_des_block14) == TRUE || check_collision(*player, *map_des_block15) == TRUE)
			
			player->x -= MOVEMENT_DELTA;
	}
		
	if (d == UP) {
		player->y -= MOVEMENT_DELTA;
		if (check_collision(*player, *map_element) == TRUE  || check_collision(*player, *map_element1) == TRUE || check_collision(*player, *map_element2) == TRUE || 
		check_collision(*player, *map_element3) == TRUE || check_collision(*player, *map_element4) == TRUE || check_collision(*player, *map_element5) == TRUE || 
		check_collision(*player, *map_element6) == TRUE || check_collision(*player, *map_element7) == TRUE   || check_collision(*player, *map_element8) == TRUE || 
		check_collision(*player, *map_element9) == TRUE || check_collision(*player, *map_element10) == TRUE || check_collision(*player, *map_element11) == TRUE || 
		check_collision(*player, *map_element12) == TRUE || check_collision(*player, *map_element13) == TRUE || check_collision(*player, *map_element14) == TRUE || 
		check_collision(*player, *map_element15) == TRUE || check_collision(*player, *map_element16) == TRUE || check_collision(*player, *map_element17) == TRUE || 
		check_collision(*player, *map_element17) == TRUE   || check_collision(*player, *map_element18) == TRUE || check_collision(*player, *map_element19) == TRUE || 
		check_collision(*player, *map_element20) == TRUE || check_collision(*player, *map_element21) == TRUE || check_collision(*player, *map_element22) == TRUE || 
		check_collision(*player, *map_element23) == TRUE || check_collision(*player, *map_element24) == TRUE || check_collision(*player, *map_des_block) == TRUE || 
		check_collision(*player, *map_des_block1) == TRUE || check_collision(*player, *map_des_block2) == TRUE || check_collision(*player, *map_des_block3) == TRUE ||
		check_collision(*player, *map_des_block4) == TRUE || check_collision(*player, *map_des_block5) == TRUE || check_collision(*player, *map_des_block6) == TRUE || 
		check_collision(*player, *map_des_block7) == TRUE || check_collision(*player, *map_des_block8) == TRUE || check_collision(*player, *map_des_block9) == TRUE ||
		check_collision(*player, *map_des_block10) == TRUE || check_collision(*player, *map_des_block11) == TRUE || check_collision(*player, *map_des_block12) == TRUE ||
		check_collision(*player, *map_des_block13) == TRUE || check_collision(*player, *map_des_block14) == TRUE || check_collision(*player, *map_des_block15) == TRUE)
			player->y += MOVEMENT_DELTA;
	}

	if (d == DOWN) {
		player->y += MOVEMENT_DELTA;
		if (check_collision(*player, *map_element) == TRUE  || check_collision(*player, *map_element1) == TRUE || check_collision(*player, *map_element2) == TRUE || 
		check_collision(*player, *map_element3) == TRUE || check_collision(*player, *map_element4) == TRUE || check_collision(*player, *map_element5) == TRUE || 
		check_collision(*player, *map_element6) == TRUE || check_collision(*player, *map_element7) == TRUE   || check_collision(*player, *map_element8) == TRUE || 
		check_collision(*player, *map_element9) == TRUE || check_collision(*player, *map_element10) == TRUE || check_collision(*player, *map_element11) == TRUE || 
		check_collision(*player, *map_element12) == TRUE || check_collision(*player, *map_element13) == TRUE || check_collision(*player, *map_element14) == TRUE || 
		check_collision(*player, *map_element15) == TRUE || check_collision(*player, *map_element16) == TRUE || check_collision(*player, *map_element17) == TRUE || 
		check_collision(*player, *map_element17) == TRUE   || check_collision(*player, *map_element18) == TRUE || check_collision(*player, *map_element19) == TRUE || 
		check_collision(*player, *map_element20) == TRUE || check_collision(*player, *map_element21) == TRUE || check_collision(*player, *map_element22) == TRUE || 
		check_collision(*player, *map_element23) == TRUE || check_collision(*player, *map_element24) == TRUE || check_collision(*player, *map_des_block) == TRUE || 
		check_collision(*player, *map_des_block1) == TRUE || check_collision(*player, *map_des_block2) == TRUE || check_collision(*player, *map_des_block3) == TRUE ||
		check_collision(*player, *map_des_block4) == TRUE || check_collision(*player, *map_des_block5) == TRUE || check_collision(*player, *map_des_block6) == TRUE || 
		check_collision(*player, *map_des_block7) == TRUE || check_collision(*player, *map_des_block8) == TRUE || check_collision(*player, *map_des_block9) == TRUE ||
		check_collision(*player, *map_des_block10) == TRUE || check_collision(*player, *map_des_block11) == TRUE || check_collision(*player, *map_des_block12) == TRUE ||
		check_collision(*player, *map_des_block13) == TRUE || check_collision(*player, *map_des_block14) == TRUE || check_collision(*player, *map_des_block15) == TRUE)
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
	SDL_Rect dest;
	int i;

	for (i = 0; i < 2; i++) {
	
		src.x = element->x;
		src.y = element->y;
		src.w = element->w;
		src.h = element->h;
		
		dest.x = 0;
		dest.y = 0;
		dest.w = element->w;
		dest.h = element->h;
	
		SDL_BlitSurface(Block_des, &dest, screen, &src);
	
	}
}
static void draw_game_elementLimites(game_element_t *element) {
	
	SDL_Rect src;
	int i;

	for (i = 0; i < 2; i++){
	
		src.x = element->x;
		src.y = element->y;
		src.w = element->w;
		src.h = element->h;
	
		int r = SDL_FillRect(screen, &src, 0xFF000000);
		
		if (r !=0){
		
			printf("fill rectangle failed in func draw_paddle()");
		}
	}
}
static void draw_game_element_des(game_element_t *element) {

	SDL_Rect src;
	SDL_Rect dest;
	int i;

	for (i = 0; i < 2; i++) {
	
		src.x = element->x;
		src.y = element->y;
		src.w = element->w;
		src.h = element->h;
		
		dest.x = 0;
		dest.y = 0;
		dest.w = element->w;
		dest.h = element->h;
	
		SDL_BlitSurface(Block, &dest, screen, &src);
	
	}
}
static void draw_skin(game_element_t *player){

	SDL_Rect src;
	SDL_Rect dest;
	
	src.x = 0;
	src.y = 0;
	src.w = 64;
	src.h = 64;

	dest.x = player -> x;
	dest.y = player -> y;
	dest.w = 64;
	dest.h = 64;
	//if(i== RIGHT){
	
	//}

	SDL_BlitSurface(skin, &src, screen, &dest);
	

}
static void draw_game_element_monster(game_element_t *monsters) {

	SDL_Rect src;
	SDL_Rect dest;

	int num_monsters = 5;

	for (int i = 0; i < num_monsters; i++) {
	
		src.x = monsters[i].x;
		src.y = monsters[i].y;
		src.w = monsters[i].w;
		src.h = monsters[i].h;
		
		dest.x = 0;
		dest.y = 0;
		dest.w = monsters[i].w;
		dest.h = monsters[i].h;
	
		SDL_BlitSurface(monster, &dest, screen, &src);
	
	}
}
	


static void draw_explosion(game_element_t *explosion_object){

	SDL_Rect src;
	SDL_Rect dest;

	src.x = 0;
	src.y = 0;
	src.w = 64;
	src.h = 64;

	dest.x = explosion_object->x;
	dest.y = explosion_object->y;
	dest.w = 64;
	dest.h = 64;
	
	SDL_BlitSurface(fire, &src, screen, &dest);

}
static void draw_bomb(game_element_t *bomb_object){



	SDL_Rect src;
	SDL_Rect dest;

	src.x = 0;
	src.y = 0;
	src.w = 64;
	src.h = 64;

	dest.x = bomb_object->x;
	dest.y = bomb_object->y;
	dest.w = 64;
	dest.h = 64;
	
	SDL_BlitSurface(bomb_image, &src, screen, &dest);

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


void destroy_block(game_element_t *map_des_block, game_element_t *explosion_object);

void time_bomb_countdown(game_element_t *obj1, game_element_t *obj2, game_element_t *destructible){


if (bomb_placed && SDL_GetTicks() - bomb_timer > 2000) { // 5000 milisegundos = 5 segundos
    // Ejecuta tu instrucción especial aquí, después de 5 segundos

    	bomb_placed = 0; // Resetea la condición para permitir colocar otra bomba
    	
    	obj2->x = obj1->x;
    	obj2->y = obj1->y;
    	destroy_block(destructible, obj2);
    	draw_explosion(obj2);
    	
    	obj2->x = obj1->x;
    	obj2->y = obj1->y+77;
    	destroy_block(destructible, obj2);
    	draw_explosion(obj2);
    	
    	obj2->x = obj1->x;
    	obj2->y = obj1->y-77;
    	destroy_block(destructible, obj2);
    	draw_explosion(obj2);
    	
    	obj2->x = obj1->x+77;
    	obj2->y = obj1->y;
    	destroy_block(destructible, obj2);
    	draw_explosion(obj2);
    	
    	obj2->x = obj1->x-77;
    	obj2->y = obj1->y;
    	destroy_block(destructible, obj2);
    	draw_explosion(obj2);
    	
    	
    	obj1->x = 2000;
	obj1->y = 2000; 
    		   
    
    }
    
else {

	return;
}

}


void destroy_block(game_element_t *map_des_block, game_element_t *explosion_object){
	
	//if (d == RIGHT){
	//explosion_object->x += MOVEMENT_DELTA;
	if (check_collision(*explosion_object, *map_des_block)  == FALSE ){
		//printf("calc solve error");
		
		map_des_block->x = -4000;
		map_des_block->y = -4000;
		map_des_block->w = 0;
		map_des_block->h = 0;
		
		}

		//explosion_object->x -= MOVEMENT_DELTA;
		
		//kills_counter = kills_counter + 1;
	
	//}
	
	//if (d == LEFT){
	//explosion_object->x -= MOVEMENT_DELTA;
	if (check_collision(*explosion_object, *map_des_block) == FALSE){
		//printf("calc solve error");
		
		map_des_block->x = -4000;
		map_des_block->y = -4000;
		map_des_block->w = 0;
		map_des_block->h = 0;
		}

		//explosion_object->x += MOVEMENT_DELTA;
		
		//kills_counter = kills_counter + 1;
	
	//}
	
	//if (d == UP){
	//explosion_object->y -= MOVEMENT_DELTA;
	if ( check_collision(*explosion_object, *map_des_block) == FALSE ){
		//printf("calc solve error");
		
		map_des_block->x = -4000;
		map_des_block->y = -4000;
		map_des_block->w = 0;
		map_des_block->h = 0;
	
		
		}

		//explosion_object->y += MOVEMENT_DELTA;
		
		//kills_counter = kills_counter + 1;
	
	//}
	
	//if (d == DOWN){
	//explosion_object->y += MOVEMENT_DELTA;
	if (check_collision(*explosion_object, *map_des_block) == FALSE ){
		//printf("calc solve error");

		map_des_block->x = -4000;
		map_des_block->y = -4000;
		map_des_block->w = 0;
		map_des_block->h = 0;
		}

		//explosion_object->y -= MOVEMENT_DELTA;
		
		//kills_counter = kills_counter + 1;
	
	//}
	
	else{
	
	return;}
	
	
	}


// Main function

int main (int argc, char *args[]) {

	int direccion;
	// crear los monstruos
	int num_monster = 5;
	
	int posiciones_monsters[][2]={
	{916,125},
	{1070,125},
	{839,510},
	{1070,587},
	{233,587},
	{618,587},
	{233,202},
	};
	
	game_element_t *monsters = (game_element_t*)malloc (sizeof(game_element_t));
	
	for(int i=0; i<num_monster; i++) {
	  
		monsters[i].x = posiciones_monsters[i][0];                
		monsters[i].y = posiciones_monsters[i][1];                  
		monsters[i].w = 1.5*BLOCK_SIZE;
		monsters[i].h = 1.5*BLOCK_SIZE;
		printf("se creo enemigo ");
		
	
	}
	
	//free(monster);
	//return 0;	
	
	// Define the player and the maps
	game_element_t player;
	game_element_t bomb_object;
	game_element_t obj1;
	
	//SDL_Renderer *renderer;
	// For the project the elements of the map should be created
	// dinamically (using malloc) and using linked lists.
	
	game_element_t map_element, map_element1,map_element2,map_element3,map_element4,map_element5,map_element6,map_element7,
	map_element8,map_element9,map_element10,map_element11,map_element12,map_element13,map_element14,map_element15,map_element16,
	map_element17,map_element18,map_element19,map_element20,map_element21,map_element22,map_element23,map_element24;
	
	game_element_t map_des_block,map_des_block1,map_des_block2,map_des_block3,map_des_block4,map_des_block5,map_des_block6,map_des_block7,
	map_des_block8,map_des_block9,map_des_block10,map_des_block11,map_des_block12,map_des_block13,map_des_block14,map_des_block15;
	
	
	
	game_element_t explosion_object;
	
	//SDL_Renderer *renderer;
	

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
	init_game(&player, &map_element,&map_element1, &map_element2,&map_element3, &map_element4, &map_element5, &map_element6, &map_element7, &map_element8,&map_element9, 		    &map_element10,&map_element11, &map_element12, &map_element13, &map_element14, &map_element15, &map_element16, &map_element17, &map_element18,&map_element19, &map_element20,&map_element21, &map_element22, &map_element23, &map_element24,&map_des_block, &map_des_block1, &map_des_block2, &map_des_block3, &map_des_block4, &map_des_block5, &map_des_block6, &map_des_block7, &map_des_block8,
	&map_des_block9, &map_des_block10, &map_des_block11, &map_des_block12, &map_des_block13, &map_des_block14, &map_des_block15, &bomb_object, &explosion_object); // The & means "Address of"
	//render loop
	while(quit == FALSE) {
	
	
	//render_block(renderer, &map_des_block);
	
		//check for new events every frame
		SDL_PumpEvents();

		const Uint8 *keystate = SDL_GetKeyboardState(NULL);
		

		
		if (keystate[SDL_SCANCODE_ESCAPE]) {
		
			quit = TRUE;
		}
		
		if (keystate[SDL_SCANCODE_S]) {
			
			move_player(DOWN, &player, &map_element, &map_element1, &map_element2, &map_element3,&map_element4, &map_element5, &map_element6, &map_element7, &map_element8, &map_element9, &map_element10, &map_element11,&map_element12, &map_element13, &map_element14, &map_element15, &map_element16, &map_element17, &map_element18, &map_element19, &map_element20, &map_element21,&map_element22, &map_element23, &map_element24,&map_des_block,&map_des_block1, &map_des_block2, &map_des_block3, &map_des_block4, &map_des_block5, &map_des_block6, &map_des_block7, &map_des_block8, &map_des_block9, &map_des_block10, &map_des_block11, &map_des_block12, &map_des_block13, &map_des_block14, &map_des_block15);
			
			direccion = DOWN;
		}

		if (keystate[SDL_SCANCODE_W]) {
			
			move_player(UP, &player, &map_element, &map_element1, &map_element2, &map_element3,&map_element4, &map_element5, &map_element6, &map_element7, &map_element8, &map_element9, &map_element10, &map_element11,&map_element12, &map_element13, &map_element14, &map_element15, &map_element16, &map_element17, &map_element18, &map_element19, &map_element20, &map_element21,&map_element22, &map_element23, &map_element24,&map_des_block,&map_des_block1, &map_des_block2, &map_des_block3, &map_des_block4, &map_des_block5, &map_des_block6, &map_des_block7, &map_des_block8, &map_des_block9, &map_des_block10, &map_des_block11, &map_des_block12, &map_des_block13, &map_des_block14, &map_des_block15);
			
			direccion = UP;
		}
		
		if (keystate[SDL_SCANCODE_A]) {
			
			move_player(LEFT, &player, &map_element, &map_element1, &map_element2, &map_element3,&map_element4, &map_element5, &map_element6, &map_element7, &map_element8, &map_element9, &map_element10, &map_element11,&map_element12, &map_element13, &map_element14, &map_element15, &map_element16, &map_element17, &map_element18, &map_element19, &map_element20, &map_element21,&map_element22, &map_element23, &map_element24,&map_des_block,&map_des_block1, &map_des_block2, &map_des_block3, &map_des_block4, &map_des_block5, &map_des_block6, &map_des_block7, &map_des_block8, &map_des_block9, &map_des_block10, &map_des_block11, &map_des_block12, &map_des_block13, &map_des_block14, &map_des_block15);
			
			direccion = LEFT;
		}

		if (keystate[SDL_SCANCODE_D]) {
			
			move_player(RIGHT, &player, &map_element, &map_element1, &map_element2, &map_element3,&map_element4, &map_element5, &map_element6, &map_element7, &map_element8, &map_element9, &map_element10, &map_element11,&map_element12, &map_element13, &map_element14, &map_element15, &map_element16, &map_element17, &map_element18, &map_element19, &map_element20, &map_element21,&map_element22, &map_element23, &map_element24,&map_des_block,&map_des_block1, &map_des_block2, &map_des_block3, &map_des_block4, &map_des_block5, &map_des_block6, &map_des_block7, &map_des_block8, &map_des_block9, &map_des_block10, &map_des_block11, &map_des_block12, &map_des_block13, &map_des_block14, &map_des_block15);
			
			direccion = RIGHT;
			
		}
		destroy_block(&explosion_object, &map_des_block);
		//draw background
		
		SDL_RenderClear(renderer);
		
		SDL_FillRect(screen, NULL, BLUE);
		
		//Renderizar la pantalla
		
		
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
		
			time_bomb_countdown(&bomb_object,&explosion_object,&map_des_block);
			
			
		
			//if either player wins, change to game over state
			if (FALSE) {	//Doing nothing for the moment
				
				state = GAME_OVER;	

			} 
		
			// Here we draw the player that we move across 
			

			// We draw the map element that is going to be static
			draw_skin(&player);
			draw_game_elementLimites(&map_element);
			draw_game_elementLimites(&map_element1);
			draw_game_elementLimites(&map_element2);
			draw_game_elementLimites(&map_element3);
			draw_game_element(&map_element4);
			draw_game_element(&map_element5);
			draw_game_element(&map_element6);
			draw_game_element(&map_element7);
			draw_game_element(&map_element8);
			draw_game_element(&map_element9);
			draw_game_element(&map_element10);
			draw_game_element(&map_element11);
			draw_game_element(&map_element12);
			draw_game_element(&map_element13);
			draw_game_element(&map_element14);
			draw_game_element(&map_element15);
			draw_game_element(&map_element16);
			draw_game_element(&map_element17);
			draw_game_element(&map_element18);
			draw_game_element(&map_element19);
			draw_game_element(&map_element20);
			draw_game_element(&map_element21);
			draw_game_element(&map_element22);
			draw_game_element(&map_element23);
			draw_game_element(&map_element24);
			draw_game_element_des(&map_des_block);
			draw_game_element_des(&map_des_block1);
			draw_game_element_des(&map_des_block2);
			draw_game_element_des(&map_des_block3);
			draw_game_element_des(&map_des_block4);
			draw_game_element_des(&map_des_block5);
			draw_game_element_des(&map_des_block6);
			draw_game_element_des(&map_des_block7);
			draw_game_element_des(&map_des_block8);
			draw_game_element_des(&map_des_block9);
			draw_game_element_des(&map_des_block10);
			draw_game_element_des(&map_des_block11);
			draw_game_element_des(&map_des_block12);
			draw_game_element_des(&map_des_block13);
			draw_game_element_des(&map_des_block14);
			draw_game_element_des(&map_des_block15);
			draw_bomb(&bomb_object);
			int num_monsters = 5;
			//for(int i = 0; i<num_monsters; i++){
			//draw_game_element_monster(&monsters[i]);
			//}
			draw_game_element_monster(monsters);


			//draw the score
			draw_game_element_0_score();
	
			//draw the score
			draw_game_element_1_score();
			//draw a bomb
			if (keystate[SDL_SCANCODE_B]) {
	
				bomb_object.x = player.x;
				bomb_object.y = player.y;
				
				draw_bomb(&bomb_object);
				
				// Coloca la bomba y guarda el momento actual
    				bomb_timer = SDL_GetTicks();
    				bomb_placed = 1; // Indica que la bomba ha sido colocada	

			}
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
	SDL_FreeSurface(Block_des);
	SDL_FreeSurface(Block);
	SDL_FreeSurface(fire);
	SDL_FreeSurface(skin);
	SDL_FreeSurface(bomb_image);
	SDL_FreeSurface(monster);

	

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
	Block = SDL_LoadBMP("Block.bmp");
	Block_des = SDL_LoadBMP("Block_des.bmp");
	bomb_image = SDL_LoadBMP("bomb.bmp");
	fire = SDL_LoadBMP("fire.bmp");
	skin = SDL_LoadBMP("skin.bmp");
	monster = SDL_LoadBMP("monster.bmp");
	

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
