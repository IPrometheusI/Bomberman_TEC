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
#define PLAYER_START_X 90
#define PLAYER_START_Y 130

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


// Inicialización de valores para x, y, w (ancho), y h (alto)
float valores_destructibles[16][4] = {
    {233, 125, 1.5 * BLOCK_SIZE, 1.5 * BLOCK_SIZE},
    {233, 278 + 50, 1.5 * BLOCK_SIZE, 1.5 * BLOCK_SIZE},
    {78, 278, 1.5 * BLOCK_SIZE, 1.5 * BLOCK_SIZE},
    {155, 433, 1.5 * BLOCK_SIZE, 1.5 * BLOCK_SIZE},
    {310, 278, 1.5 * BLOCK_SIZE, 1.5 * BLOCK_SIZE},
    {387, 433, 1.5 * BLOCK_SIZE, 1.5 * BLOCK_SIZE},
    {387, 125, 1.5 * BLOCK_SIZE, 1.5 * BLOCK_SIZE},
    {387, 588, 1.5 * BLOCK_SIZE, 1.5 * BLOCK_SIZE},
    {310, 588, 1.5 * BLOCK_SIZE, 1.5 * BLOCK_SIZE},
    {852, 588, 1.5 * BLOCK_SIZE, 1.5 * BLOCK_SIZE},
    {852, 433, 1.5 * BLOCK_SIZE, 1.5 * BLOCK_SIZE},
    {543, 278, 1.5 * BLOCK_SIZE, 1.5 * BLOCK_SIZE},
    {698, 125, 1.5 * BLOCK_SIZE, 1.5 * BLOCK_SIZE},
    {930, 278, 1.5 * BLOCK_SIZE, 1.5 * BLOCK_SIZE},
    {1007, 201, 1.5 * BLOCK_SIZE, 1.5 * BLOCK_SIZE},
    {1162, 278, 1.5 * BLOCK_SIZE, 1.5 * BLOCK_SIZE}
};


float map_elements_values[25][4] = {
    {1240, 50, 1 * BLOCK_SIZE, 50 * BLOCK_SIZE},
    {1, 50, 1.5 * BLOCK_SIZE, 50 * BLOCK_SIZE},
    {1, 665, 50 * BLOCK_SIZE, 1.5 * BLOCK_SIZE},
    {1, 50, 50 * BLOCK_SIZE, 1.5 * BLOCK_SIZE},
    {155, 200, 1.5*BLOCK_SIZE, 1.5*BLOCK_SIZE},
    {310, 200, 1.5*BLOCK_SIZE, 1.5*BLOCK_SIZE},
    {465, 200, 1.5*BLOCK_SIZE, 1.5*BLOCK_SIZE},
    {620, 200, 1.5*BLOCK_SIZE, 1.5*BLOCK_SIZE},
    {775, 200, 1.5*BLOCK_SIZE, 1.5*BLOCK_SIZE},
    {930, 200, 1.5*BLOCK_SIZE, 1.5*BLOCK_SIZE},
    {1085, 200, 1.5*BLOCK_SIZE, 1.5*BLOCK_SIZE},
    {155, 355, 1.5*BLOCK_SIZE, 1.5*BLOCK_SIZE},
    {310, 355, 1.5*BLOCK_SIZE, 1.5*BLOCK_SIZE},
    {465, 355, 1.5*BLOCK_SIZE, 1.5*BLOCK_SIZE},
    {620, 355, 1.5*BLOCK_SIZE, 1.5*BLOCK_SIZE},
    {775, 355, 1.5*BLOCK_SIZE, 1.5*BLOCK_SIZE},
    {930, 355, 1.5*BLOCK_SIZE, 1.5*BLOCK_SIZE},
    {1085, 355, 1.5*BLOCK_SIZE, 1.5*BLOCK_SIZE},
    {155, 510, 1.5*BLOCK_SIZE, 1.5*BLOCK_SIZE},
    {310, 510, 1.5*BLOCK_SIZE, 1.5*BLOCK_SIZE},
    {465, 510, 1.5*BLOCK_SIZE, 1.5*BLOCK_SIZE},
    {620, 510, 1.5*BLOCK_SIZE, 1.5*BLOCK_SIZE},
    {775, 510, 1.5*BLOCK_SIZE, 1.5*BLOCK_SIZE},
    {930, 510, 1.5*BLOCK_SIZE, 1.5*BLOCK_SIZE},
    {1085, 510, 1.5*BLOCK_SIZE, 1.5*BLOCK_SIZE},
};


int NUM_DESTRUCTIBLES = 16;
int NUM_MAP_ELEMENTS = 25;

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


struct game_element_t lista_destructibles[16];
struct game_element_t map_elements[25];


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
static void init_game(
    game_element_t *player, 
    game_element_t map_elements[], int num_map_elements, 
    game_element_t lista_destructibles[], int num_lista_destructibles, 
    game_element_t *bomb_object, 
    game_element_t *explosion_object
) {
	// Here the function is receiving the pointer to the player object
	// it modifies the player object directly
	player->x = PLAYER_START_X;
	player->y = PLAYER_START_Y;
	player->w = 1.3*BLOCK_SIZE;
	player->h = 1.3*BLOCK_SIZE;
	
	
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

	
	for (int i = 0; i < 25; i++) { // Ajustado a 25 para coincidir con la cantidad de valores definidos
	    map_elements[i].x = map_elements_values[i][0];
	    map_elements[i].y = map_elements_values[i][1];
	    map_elements[i].w = map_elements_values[i][2];
	    map_elements[i].h = map_elements_values[i][3];
	}
	
	
	// Inicialización de los valores para cada elemento
	for (int i = 0; i < 16; i++) {
	    lista_destructibles[i].x = valores_destructibles[i][0];
	    lista_destructibles[i].y = valores_destructibles[i][1];
	    lista_destructibles[i].w = valores_destructibles[i][2];
	    lista_destructibles[i].h = valores_destructibles[i][3];
	}

		
	
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

void move_player(int d, game_element_t *player, game_element_t lista_destructibles[], int ld_size, game_element_t map_elements[], int me_size) {
    player->x += (d == LEFT) ? -MOVEMENT_DELTA : (d == RIGHT) ? MOVEMENT_DELTA : 0;
    player->y += (d == UP) ? -MOVEMENT_DELTA : (d == DOWN) ? MOVEMENT_DELTA : 0;

    // Combinar los arreglos para simplificar la comprobación de colisiones
    game_element_t* all_elements[ld_size + me_size];
    for (int i = 0; i < ld_size; i++) {
        all_elements[i] = &lista_destructibles[i];
    }
    for (int i = 0; i < me_size; i++) {
        all_elements[ld_size + i] = &map_elements[i];
    }

    // Comprobar colisión con todos los elementos
    for (int i = 0; i < ld_size + me_size; i++) {
        if (check_collision(*player, *all_elements[i]) == TRUE) {
            // Revertir movimiento
            player->x -= (d == LEFT) ? -MOVEMENT_DELTA : (d == RIGHT) ? MOVEMENT_DELTA : 0;
            player->y -= (d == UP) ? -MOVEMENT_DELTA : (d == DOWN) ? MOVEMENT_DELTA : 0;
            break; // Suponiendo que solo necesitas detectar la primera colisión
        }
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
static void draw_game_element_des(game_element_t destructibles[]) {

	SDL_Rect src;
	SDL_Rect dest;
	int i;

	for (i = 0; i < 2; i++) {
	
		src.x = destructibles->x;
		src.y = destructibles->y;
		src.w = destructibles->w;
		src.h = destructibles->h;
		
		dest.x = 0;
		dest.y = 0;
		dest.w = destructibles->w;
		dest.h = destructibles->h;
	
		SDL_BlitSurface(Block, &dest, screen, &src);
	
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




void destroy_block(game_element_t *map_des_block, game_element_t *explosion_object){
	
			
	for (int i=0;i<16;i++){
		printf("Elemento %d",i);
		printf("\n");
		printf("Posicion x:%d",lista_destructibles[i].x);
		printf("\n");
		printf("Posicion y:%d",lista_destructibles[i].y);
		printf("\n");
		printf("Width:%d",lista_destructibles[i].w);
		printf("\n");
		printf("Height:%d",lista_destructibles[i].h);
		printf("\n");

		if (check_collision(*explosion_object, lista_destructibles[i]) == TRUE ){
			printf("*********Detecto colision***********\n");
			printf("\n");

			lista_destructibles[i].x = -4000;
			lista_destructibles[i].y = -4000;
			lista_destructibles[i].w = 0*BLOCK_SIZE;
			lista_destructibles[i].h = 0*BLOCK_SIZE;
			
			break;		
		}
		
		else{
		
		printf("--------No detecto colision------------\n");
		printf("\n");

		}
	}
}

//void destroy_block(game_element_t *map_des_block, game_element_t *explosion_object);

void time_bomb_countdown(game_element_t *obj1, game_element_t *obj2, game_element_t destructible[]){


if (bomb_placed && SDL_GetTicks() - bomb_timer > 2000) { // 5000 milisegundos = 5 segundos
    // Ejecuta tu instrucción especial aquí, después de 5 segundos

    	bomb_placed = 0; // Resetea la condición para permitir colocar otra bomba
    	printf("################################################################\n");
    	printf("                          Chequeo 1                   \n");
    	printf("################################################################\n");
    	obj2->x = obj1->x;
    	obj2->y = obj1->y;
    	destroy_block(destructible, obj2);
    	draw_explosion(obj2);
    	
    	
    	printf("################################################################\n");
    	printf("                          Chequeo 2                   \n");
    	printf("################################################################\n");
    	obj2->x = obj1->x;
    	obj2->y = obj1->y+77;
    	destroy_block(destructible, obj2);
    	draw_explosion(obj2);
    	
    	
    	
    	printf("################################################################\n");
    	printf("                          Chequeo 3                   \n");
    	printf("################################################################\n");
    	obj2->x = obj1->x;
    	obj2->y = obj1->y-77;
    	destroy_block(destructible, obj2);
    	draw_explosion(obj2);
    	
    	
  	printf("################################################################\n");
    	printf("                          Chequeo 4                   \n");
    	printf("################################################################\n");
    	obj2->x = obj1->x+77;
    	obj2->y = obj1->y;
    	destroy_block(destructible, obj2);
      	draw_explosion(obj2);
    	
    	
    	printf("################################################################\n");
    	printf("                          Chequeo 5                   \n");
    	printf("################################################################\n");
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


	SDL_BlitSurface(skin, &src, screen, &dest);
	

}

// Main function

int main (int argc, char *args[]) {
	int direccion;
	// Define the player and the maps
	game_element_t player;
	game_element_t bomb_object;
	game_element_t obj1;
	//SDL_Renderer *renderer;
	// For the project the elements of the map should be created
	// dinamically (using malloc) and using linked lists.
	
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

	
init_game(&player, map_elements, NUM_MAP_ELEMENTS, lista_destructibles, NUM_DESTRUCTIBLES, &bomb_object, &explosion_object);
	
	
	
	
	// The & means "Address of"
	//render loop
	while(quit == FALSE) {
	
	
	//render_block(renderer, &map_des_block);
	
		//check for new events every frame
		SDL_PumpEvents();

		const Uint8 *keystate = SDL_GetKeyboardState(NULL);
		

		
		if (keystate[SDL_SCANCODE_ESCAPE]) {
		
			quit = TRUE;
		}
		
		if (keystate[SDL_SCANCODE_DOWN]) {
			
			move_player(DOWN, &player, lista_destructibles, NUM_DESTRUCTIBLES, map_elements, NUM_MAP_ELEMENTS);

			
			direccion = DOWN;
		}

		if (keystate[SDL_SCANCODE_UP]) {
			
			move_player(UP, &player, lista_destructibles, NUM_DESTRUCTIBLES, map_elements, NUM_MAP_ELEMENTS);

			
			direccion = UP;
		}
		
		if (keystate[SDL_SCANCODE_LEFT]) {
			
			move_player(LEFT, &player, lista_destructibles, NUM_DESTRUCTIBLES, map_elements, NUM_MAP_ELEMENTS);

			
			direccion = LEFT;
		}

		if (keystate[SDL_SCANCODE_RIGHT]) {
			
move_player(RIGHT, &player, lista_destructibles, NUM_DESTRUCTIBLES, map_elements, NUM_MAP_ELEMENTS);

			
			direccion = RIGHT;
			
		}
		//destroy_block(&explosion_object, lista_destructibles);
		//draw background
		//SDL_SetRenderDrawColor(renderer, 255,255,255,255);
		SDL_RenderClear(renderer);
		//render_block(renderer, &map_des_block);
		SDL_FillRect(screen, NULL, BLUE);
		
		//Renderizar la pantalla
		
		//SDL_RenderPresent(renderer);
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
			

			// We draw the map element that is going to be static
			draw_skin(&player);
			for (int i = 0; i < 25; i++) { // Hay 25 elementos en total, desde map_element hasta map_element24
   				 if (i < 4) {
        // Los primeros 4 elementos usan draw_game_elementLimites
       				 draw_game_elementLimites(&map_elements[i]);}
       				 
			    	 else {
				// Los elementos restantes usan draw_game_element
				draw_game_element(&map_elements[i]);
			    }
			}
			
			
			
			for(int i=0;i<16;i++){
			draw_game_element_des(&lista_destructibles[i]);
			}
						
					
			time_bomb_countdown(&bomb_object,&explosion_object,lista_destructibles);
							
							
							
			
									
			
			//draw_game_element(&bomb_object);
			draw_bomb(&bomb_object);


			//draw the score
			draw_game_element_0_score();
	
			//draw the score
			draw_game_element_1_score();
			//draw a bomb
			if (keystate[SDL_SCANCODE_B]) {
				//printf("%d",map_des_block.x);
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

