#include <SDL.h>	
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>




// The constants used to define the size of the window
#define SCREEN_WIDTH 1280	//window height
#define SCREEN_HEIGHT 720	//window width

// The constants used for the movement of the block
#define LEFT 0
#define RIGHT 1
#define UP 2
#define DOWN 3
#define CENTERED 5
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
int state = 0;
#define START_SCREEN 0
#define LEVEL_1 1
#define GAME_OVER 2
#define LEVEL_2 3
#define LEVEL_3 4
#define WIN 5

// Defines for states
#define FALSE 0
#define TRUE 1

// The colors used in the game
#define BLUE 0xFF7D0000

// Timing delays
#define INPUT_DELAY_MS 500

// Time to render a frame in ms = 1000/60
#define FRAME_TIME_MS 16


// Movement diferential
int MOVEMENT_DELTA = 2;

// esto espara que se muevan mas rapido con cada nivel los zorros
int monster_speed = 5;

Uint32 bomb_timer = 0; // Almacena el momento en que se coloca la bomba

Uint32 tiempoInicio = 0; // Almacena el momento en que el temporizador empezó

int bomb_placed = 0; // Indica si la bomba ha sido colocada (0 = no, 1 = sí)
int bomb_placed_1 = 0;
int bomb_placed_2 = 0;
int bomb_placed_3 = 0;

int number_bombs = 0; // Cantidad de bombas que se pueden lanzar a la vez

int add_range = 0;

// Spawn de monstruos
int num_monster = 3;

// Se inicializa contador de vidas
int contador_vidas = 3;
// contador de las muertes del monstruo
int contador_muerte;

//Coordenadas de bordes y bloques inamovibles
float map_elements_values[25][4] = {
//top right
    {1240, 50, 1 * BLOCK_SIZE, 50 * BLOCK_SIZE},
    
//top left    
    {1, 50, 1.5 * BLOCK_SIZE, 50 * BLOCK_SIZE},
    
//bottom    
    {1, 665, 50 * BLOCK_SIZE, 1.5 * BLOCK_SIZE},
    
//ceiling    
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

//Coordenadas posibles de bloques destructibles
int coordenadas_destructibles[][2] = {

    // Fila 1
    {233,125},{310, 125}, {387, 125}, {464, 125}, {541, 125}, {618, 125}, {695, 125}, {772, 125}, {853, 125}, {930, 125}, {1005, 125}, {1085, 125}, {1165, 125},

    // Fila 2
    {78, 202}, {232, 202}, {386, 202}, {540, 202}, {694, 202}, {853, 202}, {1005, 202}, {1160, 202},
    
    // Fila 3 
    {78, 279}, {155, 279}, {232, 279}, {309, 279}, {386, 279}, {463, 279}, {540, 279}, {617, 279}, {694, 279}, {771, 279}, {853, 279}, {930, 279}, {1005, 279}, {1085, 279}, {1165, 279},
    
    // Fila 4 
    {78, 356}, {232, 356}, {386, 356}, {540, 356}, {694, 356}, {853, 356}, {1005, 356}, {1165, 356},
    
    // Fila 5 
    {78, 433}, {155, 433}, {232, 433}, {309, 433}, {386, 433}, {463, 433}, {540, 433}, {617, 433}, {694, 433}, {771, 433}, {848, 433}, {930, 433}, {1005, 433}, {1085, 433}, {1165, 433},
    
    // Fila 6 
    {78, 510}, {232, 510}, {386, 510}, {540, 510}, {694, 510}, {853, 510}, {1005, 510}, {1165, 510},
    
    // Fila 7 
    {78, 587}, {155, 587}, {232, 587}, {309, 587}, {386, 587}, {463, 587}, {540, 587}, {617, 587}, {694, 587}, {771, 587}, {853, 587}, {930, 587}, {1005, 587}, {1085, 587}, {1165, 587}

};




//Variables del numero de elementos de un arreglo para argumentos
int NUM_DESTRUCTIBLES = 16;
int NUM_MAP_ELEMENTS = 25;

//function prototypes
//initialise SDL
int init_SDL(int w, int h, int argc, char *args[]);


bool fire_touched = false;

int puntaje = 0;

int temporizador;


//Se inicializan los powerups
int powerup_velocidad = 0;

int powerup_firepower = 0;

int powerup_addbomb = 0;



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


int g_score[] = {0,0,0}; 

//matriz centenas, decenas y unidades
int arreglo_temporizador[]={0,0,0};


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
static SDL_Surface *monster2;
static SDL_Surface *monster3;
static SDL_Surface *portal;
static SDL_Surface *grass;
static SDL_Surface *vida;
static SDL_Surface *score;
static SDL_Surface *clock_image;
static SDL_Surface *powerup_addbomb_image;
static SDL_Surface *powerup_speed_image;
static SDL_Surface *powerup_explosion_range_image;
static SDL_Surface *lava;
static SDL_Surface *win;
static SDL_Surface *agua;


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
    game_element_t *bomb_object1,
    game_element_t *bomb_object2,
    game_element_t *bomb_object3,
    game_element_t *explosion_object,
    game_element_t *explosion_object2,
    game_element_t *monsters,
    game_element_t *portal_object,
    game_element_t *powerup_addbomb_object,
    game_element_t *powerup_speed_object,
    game_element_t *powerup_explosion_range_object
    
    ) {
    
    
	
	player->x = PLAYER_START_X;
	player->y = PLAYER_START_Y;
	player->w = 1*BLOCK_SIZE;
	player->h = 1*BLOCK_SIZE;
	
	
	bomb_object->x = PLAYER_START_X+2000;
	bomb_object->y = PLAYER_START_Y+2000;
	bomb_object->w = BLOCK_SIZE;
	bomb_object->h = BLOCK_SIZE;
	
	bomb_object1->x = PLAYER_START_X+2000;
	bomb_object1->y = PLAYER_START_Y+2000;
	bomb_object1->w = BLOCK_SIZE;
	bomb_object1->h = BLOCK_SIZE;
	
	bomb_object2->x = PLAYER_START_X+2000;
	bomb_object2->y = PLAYER_START_Y+2000;
	bomb_object2->w = BLOCK_SIZE;
	bomb_object2->h = BLOCK_SIZE;
	
	bomb_object3->x = PLAYER_START_X+2000;
	bomb_object3->y = PLAYER_START_Y+2000;
	bomb_object3->w = BLOCK_SIZE;
	bomb_object3->h = BLOCK_SIZE;
	
	
	
	portal_object->w = BLOCK_SIZE;
	portal_object->h = BLOCK_SIZE;
	
	explosion_object->x = PLAYER_START_X+2000;
	explosion_object->y = PLAYER_START_Y+2000;
	explosion_object->w = 80;
	explosion_object->h = 80;
	
	explosion_object2->x = PLAYER_START_X+2000;
	explosion_object2->y = PLAYER_START_Y+2000;
	explosion_object2->w = 80;
	explosion_object2->h = 80;
	
	

	powerup_addbomb_object->w = 70;
	powerup_addbomb_object->h = 67;
	
	powerup_speed_object->w = 70;
	powerup_speed_object->h = 53;
	

	powerup_explosion_range_object->w = 70;
	powerup_explosion_range_object->h = 70;
	
	
	
	
	
	
	for (int i = 0; i < 26; i++) { 
	    map_elements[i].x = map_elements_values[i][0];
	    map_elements[i].y = map_elements_values[i][1];
	    map_elements[i].w = map_elements_values[i][2];
	    map_elements[i].h = map_elements_values[i][3];
	}
	
	
    	srand(time(NULL));
    	int num_coordenadas = sizeof(coordenadas_destructibles) / 	
							  sizeof(coordenadas_destructibles[0]);	

	bool coordenadas_usadas[num_coordenadas];
	memset(coordenadas_usadas, false, num_coordenadas * sizeof(bool));


	for (int i = 0; i < 16; i++) {
	
		int index_aleatorio = rand() % num_coordenadas; 
		if(i==0){

			portal_object->x=coordenadas_destructibles[index_aleatorio][0];
			portal_object->y=coordenadas_destructibles[index_aleatorio][1];

			lista_destructibles[i].x = coordenadas_destructibles[index_aleatorio][0];
			lista_destructibles[i].y = coordenadas_destructibles[index_aleatorio][1];
			lista_destructibles[i].w = 1.5 * BLOCK_SIZE; 
			lista_destructibles[i].h = 1.5 * BLOCK_SIZE; 
			}
		else if (i==1){
		
			powerup_addbomb_object->x = coordenadas_destructibles[index_aleatorio][0];
			powerup_addbomb_object->y = coordenadas_destructibles[index_aleatorio][1];
			
			lista_destructibles[i].x = coordenadas_destructibles[index_aleatorio][0];
			lista_destructibles[i].y = coordenadas_destructibles[index_aleatorio][1];
			lista_destructibles[i].w = 1.5 * BLOCK_SIZE;
			lista_destructibles[i].h = 1.5 * BLOCK_SIZE;
			
		}
		
		else if (i==2){
			powerup_speed_object->x = coordenadas_destructibles[index_aleatorio][0];
			powerup_speed_object->y = coordenadas_destructibles[index_aleatorio][1];
			
			lista_destructibles[i].x = coordenadas_destructibles[index_aleatorio][0];
			lista_destructibles[i].y = coordenadas_destructibles[index_aleatorio][1];
			lista_destructibles[i].w = 1.5 * BLOCK_SIZE;
			lista_destructibles[i].h = 1.5 * BLOCK_SIZE;
			
		
		
		
		}
		
		else if (i==3){
			powerup_explosion_range_object->x = coordenadas_destructibles[index_aleatorio][0];
			powerup_explosion_range_object->y = coordenadas_destructibles[index_aleatorio][1];
			
			lista_destructibles[i].x = coordenadas_destructibles[index_aleatorio][0];
			lista_destructibles[i].y = coordenadas_destructibles[index_aleatorio][1];
			lista_destructibles[i].w = 1.5 * BLOCK_SIZE;
			lista_destructibles[i].h = 1.5 * BLOCK_SIZE;
			
		
		}
			
			
		else{
		
			lista_destructibles[i].x = coordenadas_destructibles[index_aleatorio][0];
			lista_destructibles[i].y = coordenadas_destructibles[index_aleatorio][1];
			lista_destructibles[i].w = 1.5 * BLOCK_SIZE; 
			lista_destructibles[i].h = 1.5 * BLOCK_SIZE; 
	
		
		coordenadas_usadas[index_aleatorio] = true;}
		
    }
    

	for (int i = 0; i < num_monster; i++) {
	    int index_aleatorio;
	    do {
		index_aleatorio = rand() % num_coordenadas;
		
	    } 

	    while (coordenadas_usadas[index_aleatorio]); 
	    monsters[i].x = coordenadas_destructibles[index_aleatorio][0];
	    monsters[i].y = coordenadas_destructibles[index_aleatorio][1];
	   
	    monsters[i].w = 1.5 * BLOCK_SIZE;
	    monsters[i].h = 1.5 * BLOCK_SIZE;
	    printf("index_aleatorio agregado: %d",index_aleatorio);
		printf("\n");
	    coordenadas_usadas[index_aleatorio] = true;
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
void move_player(
	int d,
 	game_element_t *player,
 	game_element_t lista_destructibles[], 
 	int ld_size, 
 	game_element_t map_elements[], 
 	int me_size)
{
    
    player->x += (d == LEFT) ? -MOVEMENT_DELTA : (d == RIGHT) ? MOVEMENT_DELTA : 0;
    player->y += (d == UP) ? -MOVEMENT_DELTA : (d == DOWN) ? MOVEMENT_DELTA : 0;

    game_element_t* all_elements[ld_size + me_size];
    for (int i = 0; i < ld_size; i++) {
        all_elements[i] = &lista_destructibles[i];
    }
    for (int i = 0; i < me_size; i++) {
        all_elements[ld_size + i] = &map_elements[i];
    }

    for (int i = 0; i < ld_size + me_size; i++) {
        if (check_collision(*player, *all_elements[i]) == TRUE) {
            // Revertir movimiento
            player->x -= (d == LEFT) ? -MOVEMENT_DELTA : (d == RIGHT) ? MOVEMENT_DELTA : 0;
            player->y -= (d == UP) ? -MOVEMENT_DELTA : (d == DOWN) ? MOVEMENT_DELTA : 0;
            break; 
        }
    }
}
/* Function: move_monster
 * ---------------------
 * Esta funcion se encarga de reubicar al monstruo en distintan posiciones
 del mapa 
 *
 * Arguments:
 *	d: Es la direccion random donde se va mover el monstruo
 *	lista_destructibles, map element: elementos donde chocaran los monstruos
 *	bom_objects: Bombas donde detectan colision los monstruos
 *
 *
 * Return:
 *	void.
 */ 
 
void move_monsters(
	game_element_t *map_element, 
	game_element_t lista_destructibles[], 
	game_element_t *monsters, 
	game_element_t *bomb_object,
	game_element_t *bomb_object1,
	game_element_t *bomb_object2,
	game_element_t *bomb_object3) {

    srand(time(0)); 

   
    for(int i = 0; i<num_monster; i++){
     int d = rand() % 4; 
     int collision;
	switch(d) {
        case LEFT:
            monsters[i].x -= monster_speed;
            for(int j = 0; j < 26; j++) {

                collision = check_collision(monsters[i], map_element[j]);
                if(TRUE == collision) {
                    monsters[i].x += monster_speed;
                    break;
                }
            }
            for(int j = 0; j < 15; j++) {
                collision = check_collision(monsters[i], lista_destructibles[j]);
                if(TRUE == collision) {
                    monsters[i].x += monster_speed;
                    break;
                }
            }       
                    collision = check_collision(monsters[i], *bomb_object);
                    if (TRUE == collision) {
                        monsters[i].x += monster_speed;
                        break;
                    
                }
                    
                    collision = check_collision(monsters[i], *bomb_object1);
                    if (TRUE == collision) {
                        monsters[i].x += monster_speed;
                        break;
                    
                }

                collision = check_collision(monsters[i], *bomb_object2);
                    if (TRUE == collision) {
                        monsters[i].x += monster_speed;
                        break;
                    
                }
                    
                    collision = check_collision(monsters[i], *bomb_object3);
                    if (TRUE == collision) {
                        monsters[i].x += monster_speed;
                        break;
                    
                }
               	    
            break;
///////////////////////////////////////////////////////////////////
        case RIGHT:
            monsters[i].x += monster_speed;
            for(int j = 0; j < 26; j++) {
                collision = check_collision(monsters[i], map_element[j]);
                if(TRUE == collision) {
                    monsters[i].x -= monster_speed;
                    break;
                }
            }
            for(int j = 0; j < 15; j++) {
                collision = check_collision(monsters[i], lista_destructibles[j]);
                if(TRUE == collision) {
                    monsters[i].x -= monster_speed;
                    break;
                }
            }

                   collision = check_collision(monsters[i], *bomb_object);
                    if (TRUE == collision) {
                        monsters[i].x -= monster_speed;
                        break;
                    
                }
                    
                    collision = check_collision(monsters[i], *bomb_object1);
                    if (TRUE == collision) {
                        monsters[i].x -= monster_speed;
                        break;
                    
                }

                collision = check_collision(monsters[i], *bomb_object2);
                    if (TRUE == collision) {
                        monsters[i].x -= monster_speed;
                        break;
                    
                }
                    
                    collision = check_collision(monsters[i], *bomb_object3);
                    if (TRUE == collision) {
                        monsters[i].x -= monster_speed;
                        break;
                    
                }
            break;
///////////////////////////////////////////////////////////////////////////
        case UP:
            monsters[i].y -= monster_speed;
            for(int j = 0; j < 26; j++) {
                collision = check_collision(monsters[i], map_element[j]);
                if(TRUE == collision) {
                    monsters[i].y += monster_speed;
                    break;
                }
            }
            for(int j = 0; j < 15; j++) {
                collision = check_collision(monsters[i], lista_destructibles[j]);
                if(TRUE == collision) {
                    monsters[i].y += monster_speed;
                    break;
                }
            }

                collision = check_collision(monsters[i], *bomb_object);
                    if (TRUE == collision) {
                        monsters[i].y += monster_speed;
                        break;
                    
                }
                    
                    collision = check_collision(monsters[i], *bomb_object1);
                    if (TRUE == collision) {
                        monsters[i].y += monster_speed;
                        break;
                    
                }

                collision = check_collision(monsters[i], *bomb_object2);
                    if (TRUE == collision) {
                        monsters[i].y += monster_speed;
                        break;
                    
                }
                    
                    collision = check_collision(monsters[i], *bomb_object3);
                    if (TRUE == collision) {
                        monsters[i].y += monster_speed;
                        break;
                    
                }
            break;
//////////////////////////////////////////////////////////////////
        case DOWN:
            monsters[i].y += monster_speed;
            for(int j = 0; j < 26; j++) {
                collision = check_collision(monsters[i], map_element[j]);
                if(TRUE == collision) {
                    monsters[i].y -= monster_speed;
                    break;
                }
                else{}
            }

            for(int j = 0; j < 15; j++) {
                collision = check_collision(monsters[i], lista_destructibles[j]);
                if(TRUE == collision) {
                    monsters[i].y -= monster_speed;
                    break;

                }

                else{}
            }

                collision = check_collision(monsters[i], *bomb_object);
                    if (TRUE == collision) {
                        monsters[i].y -= monster_speed;
                        break;
                    
                }
                    
                    collision = check_collision(monsters[i], *bomb_object1);
                    if (TRUE == collision) {
                        monsters[i].y -= monster_speed;
                        break;
                    
                }

                collision = check_collision(monsters[i], *bomb_object2);
                    if (TRUE == collision) {
                        monsters[i].y -= monster_speed;
                        break;
                    
                }
                    
                    collision = check_collision(monsters[i], *bomb_object3);
                    if (TRUE == collision) {
                        monsters[i].y -= monster_speed;
                        break;
                    
                }
            break;
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

	SDL_Rect src;
	SDL_Rect dest;

	src.x = 0;
	src.y = 0;
	src.w = end->w;
	src.h = end->h;

	dest.x = (screen->w / 2) - (src.w / 2);
	dest.y = (screen->h / 2) - (src.h / 2);
	dest.w = end->w;
	dest.h = end->h;

	SDL_BlitSurface(end, &src, screen, &dest);
}
	
/* Function: draw_win
 * ----------------------------
 * Esta funcion es para dibujar la pantalla de ganador, en cuanto se completen todos los niveles.
 *
 * Arguments: 
 *	none
 *
 * Return:
 *
 *	void.
 */
static void draw_win() {

	SDL_Rect src;
	SDL_Rect dest;

	src.x = 0;
	src.y = 0;
	src.w = win->w;
	src.h = win->h;

	dest.x = (screen->w / 2) - (src.w / 2);
	dest.y = (screen->h / 2) - (src.h / 2);
	dest.w = win->w;
	dest.h = win->h;

	SDL_BlitSurface(win, &src, screen, &dest);
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
/* Function: draw_game_elementLImites
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
/* Function: draw_game_element
 * -------------------------------
 * This functions draws a game element on the screen according to its
 * properties and coordinates. Please note that the element to be 
 * drawn is passed as an reference insted of a value.
 *
 * Arguments:
 *	destructiubles: Element to be drawn on the screen.
 *
 * Return:
 *	void.
 */
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


/* Function: draw_explosion
 * -------------------------------
 * Esta funcion dibuja las explosiones en pantalla de acuerdo con
 * las propiedades que se le asignan.
 *
 *
 * Arguments:
 *	explosion_object: Elemento que sera dibujado en pantalla
 *
 * Return:
 *	void.
 */
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

/* Function: draw_explosion
 * -------------------------------
 * Esta funcion dibuja las vidas en la pantalla, segun las caracteristicas dadas.
 *Note que conforme pierde vidas, se van dibujando menos corazones en pantalla.
 *Esto debido a que se envian los corazones a posiciones fuera de la pantalla.
 * Arguments:
 *	dest: Elemento que sera dibujado en pantalla.
 *
 * Return:
 *	void.
 */
static void draw_vida(int contador_vidas){

	SDL_Rect src;
	SDL_Rect dest;
	SDL_Rect dest2;
	SDL_Rect dest3;
	
	
	src.x = 0;
	src.y = 0;
	src.w = 64;
	src.h = 64;
	
	if(contador_vidas == 3){
		dest.x = 1000;
		dest.y = 0;
		dest.w = 64;
		dest.h = 64;
		
		dest2.x = 1000+77;
		dest2.y = 0;
		dest2.w = 64;
		dest2.h = 64;
		
		dest3.x = 1000+2*77;
		dest3.y = 0;
		dest3.w = 64;
		dest3.h = 64;
		
		SDL_BlitSurface(vida, &src, screen, &dest);
		SDL_BlitSurface(vida, &src, screen, &dest2);
		SDL_BlitSurface(vida, &src, screen, &dest3);
	
	}
	else if (contador_vidas == 2){
		dest.x = 1000;
		dest.y = 0;
		dest.w = 64;
		dest.h = 64;
		
		dest2.x = 1000+77;
		dest2.y = 0;
		dest2.w = 64;
		dest2.h = 64;
		
		dest3.x = 1000+2*77+1000;
		dest3.y = 0;
		dest3.w = 64;
		dest3.h = 64;
		
		SDL_BlitSurface(vida, &src, screen, &dest);
		SDL_BlitSurface(vida, &src, screen, &dest2);
		SDL_BlitSurface(vida, &src, screen, &dest3);
	}
	
	else if (contador_vidas == 1) {
		dest.x = 1000;
		dest.y = 0;
		dest.w = 64;
		dest.h = 64;
		
		dest2.x = 1000+77+1000;
		dest2.y = 0;
		dest2.w = 64;
		dest2.h = 64;
		
		dest3.x = 1000+2*77+1000;
		dest3.y = 0;
		dest3.w = 64;
		dest3.h = 64;
		
		SDL_BlitSurface(vida, &src, screen, &dest);
		SDL_BlitSurface(vida, &src, screen, &dest2);
		SDL_BlitSurface(vida, &src, screen, &dest3);
		
	}
	
	else {
		dest.x = 1000+1000;
		dest.y = 0;
		dest.w = 64;
		dest.h = 64;
		
		dest2.x = 1000+77+1000;
		dest2.y = 0;
		dest2.w = 64;
		dest2.h = 64;
		
		dest3.x = 1000+2*77+1000;
		dest3.y = 0;
		dest3.w = 64;
		dest3.h = 64;
		
		SDL_BlitSurface(vida, &src, screen, &dest);
		SDL_BlitSurface(vida, &src, screen, &dest2);
		SDL_BlitSurface(vida, &src, screen, &dest3);
	
	}
	
	

}
/* Function: destroy_block
 * -------------------------------
 *Esta funcion detecta la colision entre dos objetos, en este caso entre
 *los bloques destructibles y la explosion de fuego.
 *Note que la forma de operar es si detecta colision, envia los bloques fuera de pantalla.
 *
 * Arguments:
 *	lista_destructibles: Elemento que sera destruido.
 *      explosion_object: 
 *
 * Return:
 *	void.
 */

void destroy_block(game_element_t *map_des_block, game_element_t *explosion_object){
	
			
	for (int i=0;i<16;i++){
		

		if (check_collision(*explosion_object, lista_destructibles[i]) == TRUE ){
			

			lista_destructibles[i].x = -4000;
			lista_destructibles[i].y = -4000;
			lista_destructibles[i].w = 0*BLOCK_SIZE;
			lista_destructibles[i].h = 0*BLOCK_SIZE;
			
			break;		
		}
		
		else{
		

		}
	}
}

/* Function: kill_monsters
 * -------------------------------
 *Esta funcion envia a los monstruos fuera de la pantalla, si se detecta la collision con el poder de fuego.
 *Note que ademas si hay colision, el puntaje incrementa su valor y ademas si mata un monstruo, se restara 
 *uno al contador de muertes, para asi poder abrir el portal.
 *
 * Arguments:
 *	Monsters: Elemento que sera destruido.
 *	explosion_object: Poder de fuego que mata mosntruos.
 * Return:
 *	void.
 */
void kill_monsters(game_element_t *explosion_object, game_element_t *monsters){
	
	
	for (int i=0; i<num_monster; i++){
	if (check_collision(*explosion_object, monsters[i]) == TRUE){
			puntaje+=50;
			monsters[i].x = -4000;
			monsters[i].y = -4000;
			monsters[i].w = 0*BLOCK_SIZE;
			monsters[i].h = 0*BLOCK_SIZE;
			contador_muerte--;	
			break;		
		}
		
		else{		
		}
	}
}
/* Function: kill_player
 * -------------------------------
 *Esta funcion detecta colision del player, con el poder de fuego y los monstruos,
 *en caso de detectar colision, se resta una vida al contador de vidas.
 *Note que si detecta cualquier tipo de colision, el player reaparecera en la posicion inicial.
 *
 *
 * Arguments:
 *	Monsters: Enemigo que intentara matar al player.
 *	explosion_object: Poder de fuego peligro para el player.
 *	player: Jugador que intentara no colisionar con los anteriores argumentos.
 * Return:
 *	void.
 */
 
void kill_player(game_element_t *explosion_object, game_element_t *player, game_element_t *monsters, int state) {

    if (temporizador < 145 && state == LEVEL_1 ){ //Establece 5 segundos inmortal.

	    if (check_collision(*explosion_object, *player) == TRUE ) {
	        if (!fire_touched) {
	         
	            printf("entre");
	            player->x = PLAYER_START_X;
	            player->y = PLAYER_START_Y;
	            player->w = 1 * BLOCK_SIZE;
	            player->h = 1 * BLOCK_SIZE;
	            
	            contador_vidas--;
	            
	            fire_touched = true; 
	        }
	    } else {

	        for (int i = 0; i < num_monster; ++i) {
	            if (check_collision(monsters[i], *player)) {
	              

	                player->x = PLAYER_START_X;
	                player->y = PLAYER_START_Y;
	                player->w = 1 * BLOCK_SIZE;
	                player->h = 1 * BLOCK_SIZE;
	                
		      		contador_vidas--;

	            }
	            else{
	            
	            }
	        }
	        fire_touched = false; 
	    }
	}

	else if (temporizador < 145 && state == LEVEL_2){

		if (check_collision(*explosion_object, *player) == TRUE) {
	        if (!fire_touched) {
	         
	            
	            player->x = PLAYER_START_X;
	            player->y = PLAYER_START_Y;
	            player->w = 1 * BLOCK_SIZE;
	            player->h = 1 * BLOCK_SIZE;
	            
	            contador_vidas--;
	            
	            fire_touched = true; 
	        }
	    } else {

	        for (int i = 0; i < num_monster; ++i) {
	            if (check_collision(monsters[i], *player)) {
	              

	                player->x = PLAYER_START_X;
	                player->y = PLAYER_START_Y;
	                player->w = 1 * BLOCK_SIZE;
	                player->h = 1 * BLOCK_SIZE;
	                
		      		contador_vidas--;

	            }
	            else{
	            
	            }
	        }
	        fire_touched = false; 
	    }
	}


	else if (temporizador < 145 && state == LEVEL_3){

		if (check_collision(*explosion_object, *player) == TRUE) {
	        if (!fire_touched) {
	         
	            
	            player->x = PLAYER_START_X;
	            player->y = PLAYER_START_Y;
	            player->w = 1 * BLOCK_SIZE;
	            player->h = 1 * BLOCK_SIZE;
	            
	            contador_vidas--;
	            
	            fire_touched = true; 
	        }
	    } else {

	        for (int i = 0; i < num_monster; ++i) {
	            if (check_collision(monsters[i], *player)) {
	              

	                player->x = PLAYER_START_X;
	                player->y = PLAYER_START_Y;
	                player->w = 1 * BLOCK_SIZE;
	                player->h = 1 * BLOCK_SIZE;
	                
		      	contador_vidas--;

	            }
	            else{
	            
	            }
	        }
	        fire_touched = false; 
	    }
	}



	else if (temporizador < 145 && state == 4){

		if (check_collision(*explosion_object, *player) == TRUE) {
	        if (!fire_touched) {
	         
	            
	            player->x = PLAYER_START_X;
	            player->y = PLAYER_START_Y;
	            player->w = 1 * BLOCK_SIZE;
	            player->h = 1 * BLOCK_SIZE;
	            
	            contador_vidas--;
	            
	            fire_touched = true; 
	        }
	    } 



    else{
    	return;
    }
  



}
}

/* Function: time_bomb_countdown
 * -------------------------------
 * Esta funcion regula el tiempo de colocacion de bombas, ademas controla la colocacion de la bomba 2 y 3.
 *Tambien aumenta el poder de fuego hacia varias direcciones en caso de obtener powerups.
 *Nota: cuando se toma una segunda bomba el boton utilizado no es el original. Botones asignados para la cantidad de bombas
 * 1,2,3 y estas activan las bombas 1,2 y 3 sucesivamente. Recalcar que la bomba 2 nos se pondra hasta tomar el powerup de bomba, igual la bomba3.
 *
 * Arguments:
 *
 *	bomb_objects: son las bombas a colocar.
 *	explosion_object,explosion_object2: Poderes de fuego utilizados.
 *	destructible: Lista de elementos destructribles
 *	monsters: monstruos que intetaran matar al player
*	add_range: rango obtenido por colisionar con el powerup de fuego.
 *	bomb_placed: indica el estado de la bomba: colocada o no.
 * Return:
 *	void.
 */

void time_bomb_countdown(
	game_element_t *bomb_object,
	game_element_t *bomb_object_1,
	game_element_t *bomb_object2,
	game_element_t *bomb_object3,
	game_element_t *explosion_object,
	game_element_t *explosion_object2,
	game_element_t destructible[],
	game_element_t *monsters,
	game_element_t *player,
	game_element_t map_elements[]){


if (bomb_placed && SDL_GetTicks() - bomb_timer > 2000) { 

    	bomb_placed = 0; 
    	if (add_range == 0){
	    	explosion_object->x = bomb_object->x;
	    	explosion_object->y = bomb_object->y;
	    	destroy_block(destructible, explosion_object);
			kill_player(explosion_object, player, monsters, 4);
			kill_monsters(explosion_object, monsters);	
		    draw_explosion(explosion_object);



	    	
    		explosion_object->x = bomb_object->x;
			explosion_object->y = bomb_object->y+77;
			destroy_block(destructible, explosion_object);
			kill_player(explosion_object, player, monsters, 4);
			kill_monsters(explosion_object, monsters);
			draw_explosion(explosion_object);

			explosion_object->x = bomb_object->x;
			explosion_object->y = bomb_object->y-77;
			destroy_block(destructible, explosion_object);
			kill_player(explosion_object, player, monsters, 4);
			kill_monsters(explosion_object, monsters);
			draw_explosion(explosion_object);

			explosion_object->x = bomb_object->x+77;
	    	explosion_object->y = bomb_object->y;
	    	destroy_block(destructible, explosion_object);
			kill_player(explosion_object, player, monsters, 4);
			kill_monsters(explosion_object, monsters);
			draw_explosion(explosion_object);
	    	
	    	explosion_object->x = bomb_object->x-77;
	    	explosion_object->y = bomb_object->y;
	    	destroy_block(destructible, explosion_object);
			kill_player(explosion_object, player, monsters, 4);
			kill_monsters(explosion_object, monsters);
			draw_explosion(explosion_object);


	    	}

		



  	
  	else if (add_range == 1){
	  		explosion_object->x = bomb_object->x;
	    	explosion_object->y = bomb_object->y;
	    	destroy_block(destructible, explosion_object);
			kill_player(explosion_object, player, monsters, 4);
			kill_monsters(explosion_object, monsters);
	    	draw_explosion(explosion_object);
	    	
	    	
	    	explosion_object->x = bomb_object->x;
	    	explosion_object->y = bomb_object->y+77;
	    	
	    	explosion_object2->x = bomb_object->x;
	    	explosion_object2->y = bomb_object->y+77+77;

	    	destroy_block(destructible, explosion_object);
	    	destroy_block(destructible, explosion_object2);
	    	
			kill_player(explosion_object, player, monsters, 4);
			kill_monsters(explosion_object, monsters);
			kill_monsters(explosion_object2, monsters);

			kill_player(explosion_object2, player, monsters, 4);
			
	    	draw_explosion(explosion_object);
	    	draw_explosion(explosion_object2);
	    	
	    	
	    	explosion_object->x = bomb_object->x;
	    	explosion_object->y = bomb_object->y-77;
	    	destroy_block(destructible, explosion_object);
			kill_player(explosion_object, player, monsters, 4);
			kill_monsters(explosion_object, monsters);
		    draw_explosion(explosion_object);
	    	
	    	
	    	explosion_object->x = bomb_object->x+77;
	    	explosion_object->y = bomb_object->y;

	    	
	    	explosion_object2->x = bomb_object->x+77+77;
	    	explosion_object2->y = bomb_object->y;
	    	
	    	destroy_block(destructible, explosion_object);
	    	destroy_block(destructible, explosion_object2);
		
			kill_player(explosion_object, player, monsters, 4);
			kill_monsters(explosion_object, monsters);
			kill_monsters(explosion_object2, monsters);
			kill_player(explosion_object2, player, monsters, 4);
		      	
	      	draw_explosion(explosion_object);
	      	draw_explosion(explosion_object2);
	    	
	    	
	    	explosion_object->x = bomb_object->x-77;
	    	explosion_object->y = bomb_object->y;
	    	destroy_block(destructible, explosion_object);
			kill_player(explosion_object, player, monsters, 4);
			kill_monsters(explosion_object, monsters);


	  		draw_explosion(explosion_object);
  	  	 	
  	}
  	
  	else if (add_range == 2){
	  		explosion_object->x = bomb_object->x;
	    	explosion_object->y = bomb_object->y;
	    	destroy_block(destructible, explosion_object);
			kill_player(explosion_object, player, monsters, 4);
			kill_monsters(explosion_object, monsters);
	    	draw_explosion(explosion_object);
	    	
	    	
	    	explosion_object->x = bomb_object->x;
	    	explosion_object->y = bomb_object->y+77;
	    	
	    	explosion_object2->x = bomb_object->x;
	    	explosion_object2->y = bomb_object->y+77+77;

	    	destroy_block(destructible, explosion_object);
	    	destroy_block(destructible, explosion_object2);
	    	
			kill_player(explosion_object, player, monsters, 4);
			kill_monsters(explosion_object, monsters);
			kill_monsters(explosion_object2, monsters);

			kill_player(explosion_object2, player, monsters, 4);
		
	    	draw_explosion(explosion_object);
	    	draw_explosion(explosion_object2);
	    	
	    	
	    	explosion_object->x = bomb_object->x;
	    	explosion_object->y = bomb_object->y-77;
	    	
	    	explosion_object2->x = bomb_object->x;
	    	explosion_object2->y = bomb_object->y-77-77;
	    	
	    	
	    	
	    	destroy_block(destructible, explosion_object);
	    	destroy_block(destructible, explosion_object2);
		
		
			kill_player(explosion_object, player, monsters, 4);
			kill_monsters(explosion_object, monsters);
			kill_monsters(explosion_object2, monsters);

			kill_player(explosion_object2, player, monsters, 4);
	    	
	    	
	    	draw_explosion(explosion_object);
	    	draw_explosion(explosion_object2);
	    	
	    	
	    	explosion_object->x = bomb_object->x+77;
	    	explosion_object->y = bomb_object->y;

	    	
	    	explosion_object2->x = bomb_object->x+77+77;
	    	explosion_object2->y = bomb_object->y;
	    	
	    	destroy_block(destructible, explosion_object);
	    	destroy_block(destructible, explosion_object2);
		
			kill_player(explosion_object, player, monsters, 4);
			kill_monsters(explosion_object, monsters);
			kill_monsters(explosion_object2, monsters);

			kill_player(explosion_object2, player, monsters, 4);
	      	
	      	draw_explosion(explosion_object);
	      	draw_explosion(explosion_object2);
	    	
	    	
	    	explosion_object->x = bomb_object->x-77;
	    	explosion_object->y = bomb_object->y;
	    	destroy_block(destructible, explosion_object);
			kill_player(explosion_object, player, monsters, 4);
			kill_monsters(explosion_object, monsters);
		  	draw_explosion(explosion_object);
  	  	 	
  	}
  	
  	else if (add_range == 3){
	  	explosion_object->x = bomb_object->x;
	    	explosion_object->y = bomb_object->y;
	    	destroy_block(destructible, explosion_object);
			kill_player(explosion_object, player, monsters, 4);
			kill_monsters(explosion_object, monsters);
	    	draw_explosion(explosion_object);
	    	
	    	
	    	explosion_object->x = bomb_object->x;
	    	explosion_object->y = bomb_object->y+77;
	    	
	    	explosion_object2->x = bomb_object->x;
	    	explosion_object2->y = bomb_object->y+77+77;

	    	destroy_block(destructible, explosion_object);
	    	destroy_block(destructible, explosion_object2);
	    	
			kill_player(explosion_object, player, monsters, 4);
			kill_monsters(explosion_object, monsters);
			kill_monsters(explosion_object2, monsters);

			kill_player(explosion_object2, player, monsters, 4);
		
	    	draw_explosion(explosion_object);
	    	draw_explosion(explosion_object2);
	    	
	    	
	    	explosion_object->x = bomb_object->x;
	    	explosion_object->y = bomb_object->y-77;
	    	
	    	explosion_object2->x = bomb_object->x;
	    	explosion_object2->y = bomb_object->y-77-77;
	    	
	    	
	    	
	    	destroy_block(destructible, explosion_object);
	    	destroy_block(destructible, explosion_object2);
		
		
			kill_player(explosion_object, player, monsters, 4);
			kill_monsters(explosion_object, monsters);
			kill_monsters(explosion_object2, monsters);

			kill_player(explosion_object2, player, monsters, 4);
	    	
	    	
	    	draw_explosion(explosion_object);
	    	draw_explosion(explosion_object2);
	    	
	    	
	    	explosion_object->x = bomb_object->x+77;
	    	explosion_object->y = bomb_object->y;

	    	
	    	explosion_object2->x = bomb_object->x+77+77;
	    	explosion_object2->y = bomb_object->y;
	    	
	    	destroy_block(destructible, explosion_object);
	    	destroy_block(destructible, explosion_object2);
		
			kill_player(explosion_object, player, monsters, 4);
			kill_monsters(explosion_object, monsters);
			kill_monsters(explosion_object2, monsters);

			kill_player(explosion_object2, player, monsters, 4);
	      	
	      	draw_explosion(explosion_object);
	      	draw_explosion(explosion_object2);
	    	
	    	
	    	explosion_object->x = bomb_object->x-77;
	    	explosion_object->y = bomb_object->y;
	    	
	    	explosion_object2->x = bomb_object->x-77-77;
	    	explosion_object2->y = bomb_object->y;
	    	
	    	
	    	destroy_block(destructible, explosion_object);
	    	destroy_block(destructible, explosion_object2);
	    	
	    	
			kill_player(explosion_object, player, monsters, 4);
			kill_monsters(explosion_object, monsters);
			kill_monsters(explosion_object2, monsters);

			kill_player(explosion_object2, player, monsters, 4);
	  	
	  	
	  	draw_explosion(explosion_object);
	  	draw_explosion(explosion_object2);
  	  	 	
  	}
  	
  	
  	
  	
    	explosion_object->x = 2000;
    	explosion_object2->x = 2000;
    	
    	bomb_object->x = 2000;
		bomb_object->y = 2000; 
    		   
    
    }
    
    
  // ······································Tecla2·····································
  
    
   else if (bomb_placed_1 && SDL_GetTicks() - bomb_timer > 2000) { 


    	bomb_placed_1 = 0; 
    	
    	if (add_range == 0){

	    	explosion_object->x = bomb_object_1->x;
	    	explosion_object->y = bomb_object_1->y;
	    	destroy_block(destructible, explosion_object);
		kill_player(explosion_object, player, monsters, 4);
		kill_monsters(explosion_object, monsters);
	    	draw_explosion(explosion_object);
	    	
	    	
	    	
	    	explosion_object->x = bomb_object_1->x;
	    	explosion_object->y = bomb_object_1->y+77;
	    	destroy_block(destructible, explosion_object);
		kill_player(explosion_object, player, monsters, 4);
		kill_monsters(explosion_object, monsters);
	    	draw_explosion(explosion_object);
	    	
	    	
	    	
	    	
	    	explosion_object->x = bomb_object_1->x;
	    	explosion_object->y = bomb_object_1->y-77;
	    	destroy_block(destructible, explosion_object);
		kill_player(explosion_object, player, monsters, 4);
		kill_monsters(explosion_object, monsters);
	    	draw_explosion(explosion_object);
	    	
	    	
	  	
	    	explosion_object->x = bomb_object_1->x+77;
	    	explosion_object->y = bomb_object_1->y;
	    	destroy_block(destructible, explosion_object);
		kill_player(explosion_object, player, monsters, 4);
		kill_monsters(explosion_object, monsters);
	      	draw_explosion(explosion_object);
	    	
	    	
	    	
	    	explosion_object->x = bomb_object_1->x-77;
	    	explosion_object->y = bomb_object_1->y;
	    	destroy_block(destructible, explosion_object);
		kill_player(explosion_object, player, monsters, 4);
		kill_monsters(explosion_object, monsters);
	  	draw_explosion(explosion_object);}
  	
  	else if (add_range == 1){
  	

	  		explosion_object->x = bomb_object_1->x;
	    	explosion_object->y = bomb_object_1->y;	
	    	destroy_block(destructible, explosion_object);
			kill_player(explosion_object, player, monsters, 4);
			kill_monsters(explosion_object, monsters);
	    	draw_explosion(explosion_object);
	    	
	    	
	    	explosion_object->x = bomb_object_1->x;
	    	explosion_object->y = bomb_object_1->y+77;
	    	
	    	explosion_object2->x = bomb_object_1->x;
	    	explosion_object2->y = bomb_object_1->y+77+77;
	    	
	    	destroy_block(destructible, explosion_object);
	    	destroy_block(destructible, explosion_object2);
	    	
		kill_player(explosion_object, player, monsters, 4);
		kill_monsters(explosion_object, monsters);
		kill_monsters(explosion_object2, monsters);
		
		kill_player(explosion_object2, player, monsters, 4);
		
	    	draw_explosion(explosion_object);
	    	draw_explosion(explosion_object2);

	    	
	    	
	    	
	    	explosion_object->x = bomb_object_1->x;
	    	explosion_object->y = bomb_object_1->y-77;
	    	destroy_block(destructible, explosion_object);
		kill_player(explosion_object, player, monsters, 4);
		kill_monsters(explosion_object, monsters);
	    	draw_explosion(explosion_object);
	    	
	  	
	    	explosion_object->x = bomb_object_1->x+77;
	    	explosion_object->y = bomb_object_1->y;
	    	
	    	explosion_object2->x = bomb_object_1->x+77+77;
	    	explosion_object2->y = bomb_object_1->y;
	    
	    	destroy_block(destructible, explosion_object);
	    	destroy_block(destructible, explosion_object2);
	    	
	    	
		kill_player(explosion_object, player, monsters, 4);
		kill_monsters(explosion_object, monsters);
		kill_monsters(explosion_object2, monsters);

		kill_player(explosion_object2, player, monsters, 4);
	      	
	      	draw_explosion(explosion_object);
	      	draw_explosion(explosion_object2);
	    	
	    	
	    	explosion_object->x = bomb_object_1->x-77;
	    	explosion_object->y = bomb_object_1->y;
	    	destroy_block(destructible, explosion_object);
		kill_player(explosion_object, player, monsters, 4);
		kill_monsters(explosion_object, monsters);
	  	draw_explosion(explosion_object);
  	
  	
  	}
  	
  	
    
	
	else if (add_range == 2){
  	

	  	explosion_object->x = bomb_object_1->x;
	    	explosion_object->y = bomb_object_1->y;	
	    	destroy_block(destructible, explosion_object);
			kill_player(explosion_object, player, monsters, 4);
			kill_monsters(explosion_object, monsters);
	    	draw_explosion(explosion_object);
	    	
	    	
	    	explosion_object->x = bomb_object_1->x;
	    	explosion_object->y = bomb_object_1->y+77;
	    	
	    	explosion_object2->x = bomb_object_1->x;
	    	explosion_object2->y = bomb_object_1->y+77+77;
	    	
	    	destroy_block(destructible, explosion_object);
	    	destroy_block(destructible, explosion_object2);
	    	
			kill_player(explosion_object, player, monsters, 4);
			kill_monsters(explosion_object, monsters);
			kill_monsters(explosion_object2, monsters);

			kill_player(explosion_object2, player, monsters, 4);
			
	    	draw_explosion(explosion_object);
	    	draw_explosion(explosion_object2);

	    	
	    	
	    	
	    	explosion_object->x = bomb_object_1->x;
	    	explosion_object->y = bomb_object_1->y-77;
	    	
	    	explosion_object->x = bomb_object_1->x;
	    	explosion_object2->y = bomb_object_1->y-77-77;
	    	
	    	destroy_block(destructible, explosion_object);
	    	destroy_block(destructible, explosion_object2);
	    	
		kill_player(explosion_object, player, monsters, 4);
		kill_monsters(explosion_object, monsters);
		kill_monsters(explosion_object2, monsters);

		kill_player(explosion_object2, player, monsters, 4);
	    	
	    	draw_explosion(explosion_object);
	    	draw_explosion(explosion_object2);
	    	
	  	
	    	explosion_object->x = bomb_object_1->x+77;
	    	explosion_object->y = bomb_object_1->y;
	    	
	    	explosion_object2->x = bomb_object_1->x+77+77;
	    	explosion_object2->y = bomb_object_1->y;
	    
	    	destroy_block(destructible, explosion_object);
	    	destroy_block(destructible, explosion_object2);
	    	
	    	
		kill_player(explosion_object, player, monsters, 4);
		kill_monsters(explosion_object, monsters);
		kill_monsters(explosion_object2, monsters);

		kill_player(explosion_object2, player, monsters, 4);
	      	
	      	draw_explosion(explosion_object);
	      	draw_explosion(explosion_object2);
	    	
	    	
	    	explosion_object->x = bomb_object_1->x-77;
	    	explosion_object->y = bomb_object_1->y;
	    	destroy_block(destructible, explosion_object);
		kill_player(explosion_object, player, monsters, 4);
		kill_monsters(explosion_object, monsters);
	  	draw_explosion(explosion_object);
  	
  	}
  	
	else if (add_range == 3){
	
	
	  		explosion_object->x = bomb_object_1->x;
	    	explosion_object->y = bomb_object_1->y;	
	    	destroy_block(destructible, explosion_object);
			kill_player(explosion_object, player, monsters, 4);
			kill_monsters(explosion_object, monsters);
	    	draw_explosion(explosion_object);
	    	
	    	
	    	explosion_object->x = bomb_object_1->x;
	    	explosion_object->y = bomb_object_1->y+77;
	    	
	    	explosion_object2->x = bomb_object_1->x;
	    	explosion_object2->y = bomb_object_1->y+77+77;
	    	
	    	destroy_block(destructible, explosion_object);
	    	destroy_block(destructible, explosion_object2);
	    	
		kill_player(explosion_object, player, monsters, 4);
		kill_monsters(explosion_object, monsters);
		kill_monsters(explosion_object2, monsters);

		kill_player(explosion_object2, player, monsters, 4);
		
	    	draw_explosion(explosion_object);
	    	draw_explosion(explosion_object2);

	    	
	    	
	    	
	    	explosion_object->x = bomb_object_1->x;
	    	explosion_object->y = bomb_object_1->y-77;
	    	
	    	explosion_object->x = bomb_object_1->x;
	    	explosion_object2->y = bomb_object_1->y-77-77;
	    	
	    	destroy_block(destructible, explosion_object);
	    	destroy_block(destructible, explosion_object2);
	    	
		kill_player(explosion_object, player, monsters, 4);
		kill_monsters(explosion_object, monsters);
		kill_monsters(explosion_object2, monsters);

		kill_player(explosion_object2, player, monsters, 4);
	    	
	    	draw_explosion(explosion_object);
	    	draw_explosion(explosion_object2);
	    	
	  	
	    	explosion_object->x = bomb_object_1->x+77;
	    	explosion_object->y = bomb_object_1->y;
	    	
	    	explosion_object2->x = bomb_object_1->x+77+77;
	    	explosion_object2->y = bomb_object_1->y;
	    
	    	destroy_block(destructible, explosion_object);
	    	destroy_block(destructible, explosion_object2);
	    	
	    	
		kill_player(explosion_object, player, monsters, 4);
		kill_monsters(explosion_object, monsters);
		kill_monsters(explosion_object2, monsters);

		kill_player(explosion_object2, player, monsters, 4);
	      	
	      	draw_explosion(explosion_object);
	      	draw_explosion(explosion_object2);
	    	
	    	
	    	explosion_object->x = bomb_object_1->x-77;
	    	explosion_object->y = bomb_object_1->y;
	    	
	    	explosion_object2->x = bomb_object_1->x-77-77;
	    	explosion_object2->y = bomb_object_1->y;
	    	
	    	
	    	destroy_block(destructible, explosion_object);
	    	destroy_block(destructible, explosion_object2);

		kill_player(explosion_object, player, monsters, 4);
		kill_monsters(explosion_object, monsters);
		kill_monsters(explosion_object2, monsters);

		kill_player(explosion_object2, player, monsters, 4);
		
	  	draw_explosion(explosion_object);
	  	draw_explosion(explosion_object2);
	
	}
    	
    	explosion_object->x = 2000;
    	explosion_object2->x = 2000;
    	
    	bomb_object_1->x = 2000;
		bomb_object_1->y = 2000; 	   
    
   } 
    
    
  // ······································Tecla3·····································
  
    else if (bomb_placed_2 && SDL_GetTicks() - bomb_timer > 2000) { 


    	bomb_placed_2 = 0; 
    	
    	if(add_range == 0){
    	
	    	explosion_object->x = bomb_object2->x;
	    	explosion_object->y = bomb_object2->y;
	    	destroy_block(destructible, explosion_object);
		kill_player(explosion_object, player, monsters, 4);
		kill_monsters(explosion_object, monsters);
	    	
	    	draw_explosion(explosion_object);
	    	
	    	
	    	
	    	explosion_object->x = bomb_object2->x;
	    	explosion_object->y = bomb_object2->y+77;
	    	destroy_block(destructible, explosion_object);
		kill_player(explosion_object, player, monsters, 4);
		kill_monsters(explosion_object, monsters);
	    	draw_explosion(explosion_object);
	    	
	    	
	    	
	    	
	    	explosion_object->x = bomb_object2->x;
	    	explosion_object->y = bomb_object2->y-77;
	    	destroy_block(destructible, explosion_object);
		kill_player(explosion_object, player, monsters, 4);
		kill_monsters(explosion_object, monsters);
	    	draw_explosion(explosion_object);
	    	
	    	
	  	
	    	explosion_object->x = bomb_object2->x+77;
	    	explosion_object->y = bomb_object2->y;
	    	destroy_block(destructible, explosion_object);
		kill_player(explosion_object, player, monsters, 4);
		kill_monsters(explosion_object, monsters);
	      	draw_explosion(explosion_object);
	    	
	    	
	    	
	    	explosion_object->x = bomb_object2->x-77;
	    	explosion_object->y = bomb_object2->y;
	    	destroy_block(destructible, explosion_object);
		kill_player(explosion_object, player, monsters, 4);
		kill_monsters(explosion_object, monsters);
	  	draw_explosion(explosion_object); }
	  
	else if(add_range == 1){
	
		explosion_object->x = bomb_object2->x;
	    	explosion_object->y = bomb_object2->y;
	    	destroy_block(destructible, explosion_object);
		kill_player(explosion_object, player, monsters, 4);
		kill_monsters(explosion_object, monsters);    	
	    	draw_explosion(explosion_object);
	    	
	    	
		    	explosion_object->x = bomb_object2->x;
	    	explosion_object->y = bomb_object2->y+77;
	    	
	    	explosion_object2->x = bomb_object2->x;
	    	explosion_object2->y = bomb_object2->y+77+77;
	    	
	    	
	    	destroy_block(destructible, explosion_object);
	    	destroy_block(destructible, explosion_object2);
	    		    	
		kill_player(explosion_object, player, monsters, 4);
		kill_monsters(explosion_object, monsters);
		kill_monsters(explosion_object2, monsters);

		kill_player(explosion_object2, player, monsters, 4);
				
	    	draw_explosion(explosion_object);
	    	draw_explosion(explosion_object2);
	    	
	    	
    	
	    	explosion_object->x = bomb_object2->x;
	    	explosion_object->y = bomb_object2->y-77;
	    	destroy_block(destructible, explosion_object);
		kill_player(explosion_object, player, monsters, 4);
		kill_monsters(explosion_object, monsters);
	    	draw_explosion(explosion_object);
	    	
	    	
	  	
	    	explosion_object->x = bomb_object2->x+77;
	    	explosion_object->y = bomb_object2->y;
		explosion_object2->x = bomb_object2->x+77+77;
	    	explosion_object2->y = bomb_object2->y;
	    
	    	
	    	destroy_block(destructible, explosion_object);
	    	destroy_block(destructible, explosion_object2);
		
		
		kill_player(explosion_object, player, monsters, 4);
		kill_monsters(explosion_object, monsters);
		kill_monsters(explosion_object2, monsters);

		kill_player(explosion_object2, player, monsters, 4);
	      	
	      		      	
	      	draw_explosion(explosion_object);
	      	draw_explosion(explosion_object2);
	    	
	    	
	    	
	    	explosion_object->x = bomb_object2->x-77;
	    	explosion_object->y = bomb_object2->y;
	    	destroy_block(destructible, explosion_object);
		kill_player(explosion_object, player, monsters, 4);
		kill_monsters(explosion_object, monsters);
	  	draw_explosion(explosion_object);
	  	
	  	}
	
	
	
	else if(add_range == 2){
	
	
		explosion_object->x = bomb_object2->x;
	    	explosion_object->y = bomb_object2->y;
	    	destroy_block(destructible, explosion_object);
		kill_player(explosion_object, player, monsters, 4);
		kill_monsters(explosion_object, monsters);    	
	    	draw_explosion(explosion_object);
	    	
	    	
		    	explosion_object->x = bomb_object2->x;
	    	explosion_object->y = bomb_object2->y+77;
	    	
	    	explosion_object2->x = bomb_object2->x;
	    	explosion_object2->y = bomb_object2->y+77+77;
	    	
	    	
	    	destroy_block(destructible, explosion_object);
	    	destroy_block(destructible, explosion_object2);
	    		    	
		kill_player(explosion_object, player, monsters, 4);
		kill_monsters(explosion_object, monsters);
		kill_monsters(explosion_object2, monsters);

		kill_player(explosion_object2, player, monsters, 4);
				
	    	draw_explosion(explosion_object);
	    	draw_explosion(explosion_object2);
	    	
	    	
    	
	    	explosion_object->x = bomb_object2->x;
	    	explosion_object->y = bomb_object2->y-77;

		explosion_object2->x = bomb_object2->x;
	    	explosion_object2->y = bomb_object2->y-77-77;	    	
	    	
	    	destroy_block(destructible, explosion_object);
	    	destroy_block(destructible, explosion_object2);
	    	
		kill_player(explosion_object, player, monsters, 4);
		kill_monsters(explosion_object, monsters);
		kill_monsters(explosion_object2, monsters);

		kill_player(explosion_object2, player, monsters, 4);
	    	
	    	draw_explosion(explosion_object);
	    	draw_explosion(explosion_object2);
	    	
	    	
	  	
	    	explosion_object->x = bomb_object2->x+77;
	    	explosion_object->y = bomb_object2->y;
			explosion_object2->x = bomb_object2->x+77+77;
	    	explosion_object2->y = bomb_object2->y;
	    
	    	
	    	destroy_block(destructible, explosion_object);
	    	destroy_block(destructible, explosion_object2);
		
		
		kill_player(explosion_object, player, monsters, 4);
		kill_monsters(explosion_object, monsters);
		kill_monsters(explosion_object2, monsters);

		kill_player(explosion_object2, player, monsters, 4);
	      	
	      		      	
	      	draw_explosion(explosion_object);
	      	draw_explosion(explosion_object2);
	    	
	    	
	    	
	    	explosion_object->x = bomb_object2->x-77;
	    	explosion_object->y = bomb_object2->y;
	    	destroy_block(destructible, explosion_object);
		kill_player(explosion_object, player, monsters, 4);
		kill_monsters(explosion_object, monsters);
	  	draw_explosion(explosion_object);	
	}

	else if(add_range == 3){
	
	
		explosion_object->x = bomb_object2->x;
	    	explosion_object->y = bomb_object2->y;
	    	destroy_block(destructible, explosion_object);
		kill_player(explosion_object, player, monsters, 4);
		kill_monsters(explosion_object, monsters);    	
	    	draw_explosion(explosion_object);
	    	
	    	
		    	explosion_object->x = bomb_object2->x;
	    	explosion_object->y = bomb_object2->y+77;
	    	
	    	explosion_object2->x = bomb_object2->x;
	    	explosion_object2->y = bomb_object2->y+77+77;
	    	
	    	
	    	destroy_block(destructible, explosion_object);
	    	destroy_block(destructible, explosion_object2);
	    		    	
		kill_player(explosion_object, player, monsters, 4);
		kill_monsters(explosion_object, monsters);
		kill_monsters(explosion_object2, monsters);
	
		kill_player(explosion_object2, player, monsters, 4);
				
	    	draw_explosion(explosion_object);
	    	draw_explosion(explosion_object2);
	    	
	    	
    	
	    	explosion_object->x = bomb_object2->x;
	    	explosion_object->y = bomb_object2->y-77;

			explosion_object2->x = bomb_object2->x;
	    	explosion_object2->y = bomb_object2->y-77-77;	    	
	    	
	    	destroy_block(destructible, explosion_object);
	    	destroy_block(destructible, explosion_object2);
	    	
		kill_player(explosion_object, player, monsters, 4);
		kill_monsters(explosion_object, monsters);
		kill_monsters(explosion_object2, monsters);

		kill_player(explosion_object2, player, monsters, 4);
	    	
	    	draw_explosion(explosion_object);
	    	draw_explosion(explosion_object2);
	    	
	    	
	  	
	    	explosion_object->x = bomb_object2->x+77;
	    	explosion_object->y = bomb_object2->y;
			explosion_object2->x = bomb_object2->x+77+77;
	    	explosion_object2->y = bomb_object2->y;
	    
	    	
	    	destroy_block(destructible, explosion_object);
	    	destroy_block(destructible, explosion_object2);
		
		
		kill_player(explosion_object, player, monsters, 4);
		kill_monsters(explosion_object, monsters);
		kill_monsters(explosion_object2, monsters);
	
		kill_player(explosion_object2, player, monsters, 4);
	      	
	      		      	
	      	draw_explosion(explosion_object);
	      	draw_explosion(explosion_object2);
	    	
	    	
	    	
	    	explosion_object->x = bomb_object2->x-77;
	    	explosion_object->y = bomb_object2->y;
	    	
	    	explosion_object2->x = bomb_object2->x-77-77;
	    	explosion_object2->y = bomb_object2->y;
	    	
	    	
	    	destroy_block(destructible, explosion_object);
	    	destroy_block(destructible, explosion_object2);

		kill_player(explosion_object, player, monsters, 4);
		kill_monsters(explosion_object, monsters);
		kill_monsters(explosion_object2, monsters);

		kill_player(explosion_object2, player, monsters, 4);
		
	  	draw_explosion(explosion_object);
	  	draw_explosion(explosion_object2);
		
	
	
	}
  	
  	
    	explosion_object->x = 2000;
    	explosion_object2->x = 2000;
    	
    	bomb_object2->x = 2000;
		bomb_object2->y = 2000; 
    		   
    
    }
//---------------------------------------Tecla 4---------------------------------------------------------

    
    else if (bomb_placed_3 && SDL_GetTicks() - bomb_timer > 2000) { 


	bomb_placed_3 = 0; 
    	
    	if(add_range == 0){
    	
	    	explosion_object->x = bomb_object3->x;
	    	explosion_object->y = bomb_object3->y;
	    	destroy_block(destructible, explosion_object);
		kill_player(explosion_object, player, monsters, 4);
		kill_monsters(explosion_object, monsters);
	    	
	    	draw_explosion(explosion_object);
	    	
	    	
	    	
	    	explosion_object->x = bomb_object3->x;
	    	explosion_object->y = bomb_object3->y+77;
	    	destroy_block(destructible, explosion_object);
		kill_player(explosion_object, player, monsters, 4);
		kill_monsters(explosion_object, monsters);
	    	draw_explosion(explosion_object);
	    	
	    	
	    	
	    	
	    	explosion_object->x = bomb_object3->x;
	    	explosion_object->y = bomb_object3->y-77;
	    	destroy_block(destructible, explosion_object);
		kill_player(explosion_object, player, monsters, 4);
		kill_monsters(explosion_object, monsters);
	    	draw_explosion(explosion_object);
	    	
	    	
	  	
	    	explosion_object->x = bomb_object3->x+77;
	    	explosion_object->y = bomb_object3->y;
	    	destroy_block(destructible, explosion_object);
		kill_player(explosion_object, player, monsters, 4);
		kill_monsters(explosion_object, monsters);
	      	draw_explosion(explosion_object);
	    	
	    	
	    	
	    	explosion_object->x = bomb_object3->x-77;
	    	explosion_object->y = bomb_object3->y;
	    	destroy_block(destructible, explosion_object);
		kill_player(explosion_object, player, monsters, 4);
		kill_monsters(explosion_object, monsters);
	  	draw_explosion(explosion_object); }
	  
	else if(add_range == 1){
	
		explosion_object->x = bomb_object3->x;
	    	explosion_object->y = bomb_object3->y;
	    	destroy_block(destructible, explosion_object);
		kill_player(explosion_object, player, monsters, 4);
		kill_monsters(explosion_object, monsters);    	
	    	draw_explosion(explosion_object);
	    	
	    	
	    	explosion_object->x = bomb_object3->x;
	    	explosion_object->y = bomb_object3->y+77;
	    	
	    	explosion_object2->x = bomb_object3->x;
	    	explosion_object2->y = bomb_object3->y+77+77;
	    	
	    	
	    	destroy_block(destructible, explosion_object);
	    	destroy_block(destructible, explosion_object2);
	    		    	
		kill_player(explosion_object, player, monsters, 4);
		kill_monsters(explosion_object, monsters);
		kill_monsters(explosion_object2, monsters);

		kill_player(explosion_object2, player, monsters, 4);
				
	    	draw_explosion(explosion_object);
	    	draw_explosion(explosion_object2);
	    	
	    	
    	
	    	explosion_object->x = bomb_object3->x;
	    	explosion_object->y = bomb_object3->y-77;
	    	destroy_block(destructible, explosion_object);
		kill_player(explosion_object, player, monsters, 4);
		kill_monsters(explosion_object, monsters);
	    	draw_explosion(explosion_object);
	    	
	    	
	  	
	    	explosion_object->x = bomb_object3->x+77;
	    	explosion_object->y = bomb_object3->y;
			explosion_object2->x = bomb_object3->x+77+77;
	    	explosion_object2->y = bomb_object3->y;
	    
	    	
	    	destroy_block(destructible, explosion_object);
	    	destroy_block(destructible, explosion_object2);
		
		
		kill_player(explosion_object, player, monsters, 4);
		kill_monsters(explosion_object, monsters);
		kill_monsters(explosion_object2, monsters);

		kill_player(explosion_object2, player, monsters, 4);
	      	
	      		      	
	      	draw_explosion(explosion_object);
	      	draw_explosion(explosion_object2);
	    	
	    	
	    	
	    	explosion_object->x = bomb_object3->x-77;
	    	explosion_object->y = bomb_object3->y;
	    	destroy_block(destructible, explosion_object);
		kill_player(explosion_object, player, monsters, 4);
		kill_monsters(explosion_object, monsters);
	  	draw_explosion(explosion_object);
	  	
	  	}
	
	
	
	else if(add_range == 2){
	
	
		explosion_object->x = bomb_object3->x;
	    	explosion_object->y = bomb_object3->y;
	    	destroy_block(destructible, explosion_object);
		kill_player(explosion_object, player, monsters, 4);
		kill_monsters(explosion_object, monsters);    	
	    	draw_explosion(explosion_object);
	    	
	    	
		    	explosion_object->x = bomb_object3->x;
	    	explosion_object->y = bomb_object3->y+77;
	    	
	    	explosion_object2->x = bomb_object3->x;
	    	explosion_object2->y = bomb_object3->y+77+77;
	    	
	    	
	    	destroy_block(destructible, explosion_object);
	    	destroy_block(destructible, explosion_object2);
	    		    	
		kill_player(explosion_object, player, monsters, 4);
		kill_monsters(explosion_object, monsters);
		kill_monsters(explosion_object2, monsters);

		kill_player(explosion_object2, player, monsters, 4);
				
	    	draw_explosion(explosion_object);
	    	draw_explosion(explosion_object2);
	    	
	    	
    	
	    	explosion_object->x = bomb_object3->x;
	    	explosion_object->y = bomb_object3->y-77;

		explosion_object2->x = bomb_object3->x;
	    	explosion_object2->y = bomb_object3->y-77-77;	    	
	    	
	    	destroy_block(destructible, explosion_object);
	    	destroy_block(destructible, explosion_object2);
	    	
		kill_player(explosion_object, player, monsters, 4);
		kill_monsters(explosion_object, monsters);
		kill_monsters(explosion_object2, monsters);

		kill_player(explosion_object2, player, monsters, 4);
	    	
	    	draw_explosion(explosion_object);
	    	draw_explosion(explosion_object2);
	    	
	    	
	  	
	    	explosion_object->x = bomb_object3->x+77;
	    	explosion_object->y = bomb_object3->y;
			explosion_object2->x = bomb_object3->x+77+77;
	    	explosion_object2->y = bomb_object3->y;
	    
	    	
	    	destroy_block(destructible, explosion_object);
	    	destroy_block(destructible, explosion_object2);
		
		
		kill_player(explosion_object, player, monsters, 4);
		kill_monsters(explosion_object, monsters);
		kill_monsters(explosion_object2, monsters);

		kill_player(explosion_object2, player, monsters, 4);
	      	
	      		      	
	      	draw_explosion(explosion_object);
	      	draw_explosion(explosion_object2);
	    	
	    	
	    	
	    	explosion_object->x = bomb_object3->x-77;
	    	explosion_object->y = bomb_object3->y;
	    	destroy_block(destructible, explosion_object);
		kill_player(explosion_object, player, monsters, 4);
		kill_monsters(explosion_object, monsters);
	  	draw_explosion(explosion_object);	
	}

	else if(add_range == 3){
	
	
		explosion_object->x = bomb_object3->x;
	    	explosion_object->y = bomb_object3->y;
	    	destroy_block(destructible, explosion_object);
		kill_player(explosion_object, player, monsters, 4);
		kill_monsters(explosion_object, monsters);    	
	    	draw_explosion(explosion_object);
	    	
	    	
		    	explosion_object->x = bomb_object3->x;
	    	explosion_object->y = bomb_object3->y+77;
	    	
	    	explosion_object2->x = bomb_object3->x;
	    	explosion_object2->y = bomb_object3->y+77+77;
	    	
	    	
	    	destroy_block(destructible, explosion_object);
	    	destroy_block(destructible, explosion_object2);
	    		    	
		kill_player(explosion_object, player, monsters, 4);
		kill_monsters(explosion_object, monsters);
		kill_monsters(explosion_object2, monsters);
	
		kill_player(explosion_object2, player, monsters, 4);
				
	    	draw_explosion(explosion_object);
	    	draw_explosion(explosion_object2);
	    	
	    	
    	
	    	explosion_object->x = bomb_object3->x;
	    	explosion_object->y = bomb_object3->y-77;

			explosion_object2->x = bomb_object3->x;
	    	explosion_object2->y = bomb_object3->y-77-77;	    	
	    	
	    	destroy_block(destructible, explosion_object);
	    	destroy_block(destructible, explosion_object2);
	    	
		kill_player(explosion_object, player, monsters, 4);
		kill_monsters(explosion_object, monsters);
		kill_monsters(explosion_object2, monsters);

		kill_player(explosion_object2, player, monsters, 4);
	    	
	    	draw_explosion(explosion_object);
	    	draw_explosion(explosion_object2);
	    	
	    	
	  	
	    	explosion_object->x = bomb_object3->x+77;
	    	explosion_object->y = bomb_object3->y;
			explosion_object2->x = bomb_object3->x+77+77;
	    	explosion_object2->y = bomb_object3->y;
	    
	    	
	    	destroy_block(destructible, explosion_object);
	    	destroy_block(destructible, explosion_object2);
		
		
		kill_player(explosion_object, player, monsters, 4);
		kill_monsters(explosion_object, monsters);
		kill_monsters(explosion_object2, monsters);
	
		kill_player(explosion_object2, player, monsters, 4);
	      	
	      		      	
	      	draw_explosion(explosion_object);
	      	draw_explosion(explosion_object2);
	    	
	    	
	    	
	    	explosion_object->x = bomb_object3->x-77;
	    	explosion_object->y = bomb_object3->y;
	    	
	    	explosion_object2->x = bomb_object3->x-77-77;
	    	explosion_object2->y = bomb_object3->y;
	    	
	    	
	    	destroy_block(destructible, explosion_object);
	    	destroy_block(destructible, explosion_object2);

		kill_player(explosion_object, player, monsters, 4);
		kill_monsters(explosion_object, monsters);
		kill_monsters(explosion_object2, monsters);

		kill_player(explosion_object2, player, monsters, 4);
		
	  	draw_explosion(explosion_object);
	  	draw_explosion(explosion_object2);
		
	
	
	}
  	
    	explosion_object->x = 2000;
    	explosion_object2->x = 2000;

    	
    	bomb_object3->x = 2000;
		bomb_object3->y = 2000; 
    		   
    
    }
    
    
    

    
else {

	return;
}

}

/* Function: draw_bomb
 * -------------------------------
 *Esta funcion dibuja en pantalla las cantidades de bombas obtenidas en el juego, esto segun la posicion del player.
 *
 * Arguments:
 *	bomb_object: Bomba que se dibujara en patalla.
 * Return:
 *	void.
 */

static void draw_bomb(game_element_t *bomb_object, int event ){

	

	SDL_Rect src;
	SDL_Rect dest;
	SDL_Rect dest1;
	SDL_Rect dest2;
	SDL_Rect dest3;
	

	src.x = 0;
	src.y = 0;
	src.w = 64;
	src.h = 64;
	
	if (event == 0){
	dest.x = bomb_object->x;
	dest.y = bomb_object->y;
	dest.w = 64;
	dest.h = 64;

	SDL_BlitSurface(bomb_image, &src, screen, &dest);}
	
	if (event == 1){
	dest1.x = bomb_object->x;
	dest1.y = bomb_object->y;
	dest1.w = 64;
	dest1.h = 64;

	SDL_BlitSurface(bomb_image, &src, screen, &dest1);}
	
	if (event == 2){
	dest2.x = bomb_object->x;
	dest2.y = bomb_object->y;
	dest2.w = 64;
	dest2.h = 64;

	SDL_BlitSurface(bomb_image, &src, screen, &dest2);}
	
	if (event == 3){
	dest3.x = bomb_object->x;
	dest3.y = bomb_object->y;
	dest3.w = 64;
	dest3.h = 64;

	SDL_BlitSurface(bomb_image, &src, screen, &dest3);}
	
	
	
}
	
	
				
	
/* Function: draw_skin
 * -------------------------------
 *Esta funcion dibuja en pantalla una skin sobre el player.
 *
 * Arguments:
 *	player: Puntero del objeto player.
 * Return:
 *	void.
 */


static void draw_skin(int d, game_element_t *player){
	
	
	SDL_Rect src;
	SDL_Rect dest;

	if(d==0){
	src.x = 48;
	src.y = 192;
	src.w = 48;
	src.h = 64;
	
	dest.x = player -> x;
	dest.y = player -> y;
	dest.w = 64;
	dest.h = 64;}
	
	if(d==1){
	src.x = 48;
	src.y = 64;
	src.w = 48;
	src.h = 64;
	
	dest.x = player -> x;
	dest.y = player -> y;
	dest.w = 64;
	dest.h = 64;}
	
	if(d==2){
	src.x = 48;
	src.y = 0;
	src.w = 48;
	src.h = 64;
	
	dest.x = player -> x;
	dest.y = player -> y;
	dest.w = 64;
	dest.h = 64;}
	
	if(d==3){
	src.x = 48;
	src.y = 128;
	src.w = 48;
	src.h = 64;
	
	dest.x = player -> x;
	dest.y = player -> y;
	dest.w = 64;
	dest.h = 64;}
	
	
	//48x64

	SDL_BlitSurface(skin, &src, screen, &dest);
	

}
/* Function: draw_portal
 * -------------------------------
 *Esta funcion dibuja un portal detras de un bloque destruictible.
 *
 * Arguments:
 *	portal_object: Encargado de avanzar al siguiente nivel.
 * Return:
 *	void.
 */

static void draw_portal(game_element_t *portal_object){
	
	SDL_Rect src;
	SDL_Rect dest;
	
	src.x = 0;
	src.y = 0;
	src.w = 64;
	src.h = 64;

	dest.x = portal_object -> x+25;
	dest.y = portal_object -> y;
	dest.w = 64;
	dest.h = 64;


	SDL_BlitSurface(portal, &src, screen, &dest);
	

}
/* Function: draw_grass
 * -------------------------------
 *Esta funcion dibuja un fondo de pantalla para el nivel 1.
 *
 * Arguments:
 *	none
 * Return:
 *	void.
 */
static void draw_grass(){
	
	SDL_Rect src;
	SDL_Rect dest;
	
	src.x = 0;
	src.y = 0;
	src.w = 1280;
	src.h = 640;

	dest.x = 78;
	dest.y = 125;
	dest.w = 1280;
	dest.h = 640;


	SDL_BlitSurface(grass, &src, screen, &dest);
	

}
/* Function: draw_lava
 * -------------------------------
 *Esta funcion dibuja un fondo de pantalla para el nivel 2.
 *
 * Arguments:
 *	none
 * Return:
 *	void.
 */
static void draw_lava(){
	
	SDL_Rect src;
	SDL_Rect dest;
	
	src.x = 0;
	src.y = 0;
	src.w = 1280;
	src.h = 640;

	dest.x = 78;
	dest.y = 125;
	dest.w = 1280;
	dest.h = 640;


	SDL_BlitSurface(lava, &src, screen, &dest);
	

}
/* Function: draw_agua
 * -------------------------------
 *Esta funcion dibuja un fondo de pantalla para el nivel 3.
 *
 * Arguments:
 *	none
 * Return:
 *	void.
 */
static void draw_agua() {

	SDL_Rect src;
	SDL_Rect dest;

	src.x = 0;
	src.y = 0;
	src.w = agua->w;
	src.h = agua->h;

	dest.x = (screen->w / 2) - (src.w / 2);
	dest.y = (screen->h / 2) - (src.h / 2);
	dest.w = agua->w;
	dest.h = agua->h;

	SDL_BlitSurface(agua, &src, screen, &dest);
}



/* Function: draw_game_element_monster
 * -------------------------------
  *Esta funcion dibuja los monstruos en el nivel 1.
 *
 * Arguments:
 *	monsters: Lista de monstruos.
 * Return:
 *	void.
 */
static void draw_game_element_monster(game_element_t *monsters) {

	SDL_Rect src;
	SDL_Rect dest;
	
	for (int i = 0; i < num_monster; i++) {
	
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
/* Function: draw_game_element_monster2
 * -------------------------------
 *Esta funcion dibuja los monstruos en el nivel 3.
 *
 * Arguments:
 *	monsters: Lista de monstruos.
 * Return:
 *	void.
 */
static void draw_game_element_monster2(game_element_t *monsters) {

	SDL_Rect src;
	SDL_Rect dest;
	
	for (int i = 0; i < num_monster; i++) {
	
		src.x = monsters[i].x;
		src.y = monsters[i].y;
		src.w = monsters[i].w;
		src.h = monsters[i].h;
		
		dest.x = 0;
		dest.y = 0;
		dest.w = monsters[i].w;
		dest.h = monsters[i].h;
	
		SDL_BlitSurface(monster3, &dest, screen, &src);
	
	}
}
/* Function: draw_game_element_monster3
 * -------------------------------
 *Esta funcion dibuja los monstruos en el nivel 2.
 *
 * Arguments:
 *	monsters: Lista de monstruos.
 * Return:
 *	void.
 */
static void draw_game_element_monster3(game_element_t *monsters) {

	SDL_Rect src;
	SDL_Rect dest;
	
	for (int i = 0; i < num_monster; i++) {
	
		src.x = monsters[i].x;
		src.y = monsters[i].y;
		src.w = monsters[i].w;
		src.h = monsters[i].h;
		
		dest.x = 0;
		dest.y = 0;
		dest.w = monsters[i].w;
		dest.h = monsters[i].h;
	
		SDL_BlitSurface(monster2, &dest, screen, &src);
	
	}
}
//sin uso
static void draw_score(){
	
	SDL_Rect src;
	SDL_Rect dest;

	src.x = 0;
	src.y = 0;
	src.w = 200;
	src.h = 79;

	dest.x = -10;
	dest.y = -5;
	dest.w = 200;
	dest.h = 79;


	SDL_BlitSurface(score, &dest, screen, &src);

}
/* Function: draw_numbermap
 * -------------------------------
 *Esta funcion dibuja las unidades, decenas y centenas en pantalla del puntuaje.
 *e
 * Arguments:
 *	none
 * Return:
 *	void.
 */
static void draw_numbermap() {

    
    g_score[0] = puntaje / 100; 
    g_score[1] = (puntaje / 10) % 10; 
    g_score[2] = puntaje % 10; 


    SDL_Rect src;
    SDL_Rect dest;

    src.x = 0;
    src.y = 0;
    src.w = 64; 
    src.h = 64; 


    dest.w = 64; 
    dest.h = 64; 


    for (int i = 0; i < 3; ++i) {
        src.x = src.w * g_score[i]; 
	
        dest.x = i * dest.w+115;
        dest.y = -5; 
        
        
        SDL_BlitSurface(numbermap, &src, screen, &dest);
    }
}
/* Function: draw_clock_image
 * -------------------------------
 *Esta funcion dibuja el temporizador del juego.
 *
 * Arguments:
 *	none
 * Return:
 *	void.
 */
static void draw_clock_image(){
	
	SDL_Rect src;
	SDL_Rect dest;

	src.x = 0;
	src.y = 0;
	src.w = 100;
	src.h = 100;

	dest.x = -500;
	dest.y = 0;
	dest.w = 1000;
	dest.h = 100;


	SDL_BlitSurface(clock_image, &dest, screen, &src);

}
/* Function: draw_clock_image
 * -------------------------------
 *Esta funcion hace la labor de cronometrar el tiempo que utilizara cada nivel.
 *Note que se crea un arreglo de tres, para asi guardar las unidades, decenas y centenas,que indicaran el tiempo transcurrido.
 * Arguments:
 *	none
 * Return:
 *	void.
 */
static void draw_timer_countdown(){




    if (tiempoInicio == 0) {
        tiempoInicio = SDL_GetTicks(); 
    }


    Uint32 tiempoActual = SDL_GetTicks();
    if ((tiempoActual - tiempoInicio) >= 1000) {
        temporizador--; 
        tiempoInicio = tiempoActual; 
    }


    arreglo_temporizador[0] = temporizador / 100; 
    arreglo_temporizador[1] = (temporizador / 10) % 10; 
    arreglo_temporizador[2] = temporizador % 10; 

    SDL_Rect src;
    SDL_Rect dest;

    src.x = 0;
    src.y = 0;
    src.w = 64; 
    src.h = 64; 

    dest.w = 64; 
    dest.h = 64; 



    for (int i = 0; i < 3; ++i) {
        src.x = src.w * arreglo_temporizador[i]; 
        dest.x = i * dest.w + 600; 
        dest.y = -5; 
        
        SDL_BlitSurface(numbermap, &src, screen, &dest);
    }
}

/* Function: draw_powerup_addbomb
 * -------------------------------
 *Esta funcion dibuja el poder de agregar otra bomba en pantalla.
 * Arguments:
 *	powerup_addbomb_object: Poder de agregar otra bomba.
 * Return:
 *	void.
 */
static void draw_powerup_addbomb(game_element_t *powerup_addbomb_object) {



    SDL_Rect src;
    SDL_Rect dest;


    src.x = 0;
    src.y = 0;
    src.w = 70; 
    src.h = 67; 


    dest.x = powerup_addbomb_object -> x; 
    dest.y = powerup_addbomb_object -> y; 
    dest.w = src.w; 
    dest.h = src.h; 

    
    SDL_BlitSurface(powerup_addbomb_image, &src, screen, &dest);
}
/* Function: draw_powerup_speed
 * -------------------------------
 *Esta funcion dibuja el poder de velocidad en pantalla.
 * Arguments:
 *	powerup_speed_object: Poder de correr mas rapido.
 * Return:
 *	void.
 */
static void draw_powerup_speed(game_element_t *powerup_speed_object) {


    SDL_Rect src;
    SDL_Rect dest;

    src.x = 0;
    src.y = 0;
    src.w = 70;
    src.h = 53;

    dest.x = powerup_speed_object->x; 
    dest.y = powerup_speed_object->y;
    dest.w = src.w;
    dest.h = src.h;

    SDL_BlitSurface(powerup_speed_image, &src, screen, &dest);
}
/* Function: draw_powerup_explosion_range
 * -------------------------------
 *Esta funcion dibuja el poder de fuego en pantalla.
 * Arguments:
 *	powerup_speed_object: Rango del poder de fuego.
 * Return:
 *	void.
 */
static void draw_powerup_explosion_range(game_element_t *powerup_explosion_range_object) {


    SDL_Rect src;
    SDL_Rect dest;

    src.x = 0;
    src.y = 0;
    src.w = 70;
    src.h = 70;

    dest.x = powerup_explosion_range_object->x; 
    dest.y = powerup_explosion_range_object->y;
    dest.w = src.w;
    dest.h = src.h;

    SDL_BlitSurface(powerup_explosion_range_image, &src, screen, &dest);
}

// Main function

int main (int argc, char *args[]) {
	int direccion;
	int current_direction = rand() % 4;
	

	// Define the player and the maps
	game_element_t player;
	game_element_t explosion_object;
	game_element_t explosion_object2;
	game_element_t bomb_object;
	game_element_t bomb_object1;
	game_element_t bomb_object2;
	game_element_t bomb_object3;
	
	
	game_element_t *monsters;
	game_element_t portal_object;
	
	// Se definen los objetos powerups
	game_element_t powerup_explosion_range_object;
	game_element_t powerup_addbomb_object;
	game_element_t powerup_speed_object;
	

	monsters = (game_element_t*)malloc(num_monster  * sizeof(game_element_t));


	//SDL Window setup
	if (init_SDL(SCREEN_WIDTH, SCREEN_HEIGHT, argc, args) == FAILURE) {
		
		return FAILURE;
	}
	
	SDL_GetWindowSize(window, &g_width, &g_height);
	
	int sleep = 0;
	int quit = FALSE;
	int state = START_SCREEN;
	Uint32 next_game_tick = SDL_GetTicks();
		
	
	//render loop
	while(quit == FALSE) {
	
		move_monsters(map_elements, lista_destructibles, monsters, &bomb_object, &bomb_object1, &bomb_object2, &bomb_object3);

	
		//check for new events every frame
		SDL_PumpEvents();

		const Uint8 *keystate = SDL_GetKeyboardState(NULL);
		

		
		if (keystate[SDL_SCANCODE_ESCAPE]) {

			quit = TRUE;
		}
		
		if (keystate[SDL_SCANCODE_DOWN]) {
			
			 move_player(DOWN,
			 &player, 
			 lista_destructibles, 
			 NUM_DESTRUCTIBLES, 
			 map_elements, 
			 NUM_MAP_ELEMENTS);


			
			direccion = DOWN;
		}

		if (keystate[SDL_SCANCODE_UP]) {
			
			move_player(UP, 
			&player, 
			lista_destructibles, 
			NUM_DESTRUCTIBLES, 
			map_elements, 
			NUM_MAP_ELEMENTS);


			
			direccion = UP;
		}
		
		if (keystate[SDL_SCANCODE_LEFT]) {
			
			move_player(LEFT, 
			&player, 
			lista_destructibles, 
			NUM_DESTRUCTIBLES, 
			map_elements, 
			NUM_MAP_ELEMENTS);


			
			direccion = LEFT;
		}

		if (keystate[SDL_SCANCODE_RIGHT]) {
			
			move_player(RIGHT, 
			&player, 
			lista_destructibles, 
			NUM_DESTRUCTIBLES, 
			map_elements, 
			NUM_MAP_ELEMENTS);


			
			direccion = RIGHT;
			
		}

		SDL_RenderClear(renderer);

		SDL_FillRect(screen, NULL, BLUE);
		
		
		
		//display main menu
		if (state == START_SCREEN ) {

			number_bombs = 0;
			contador_muerte = num_monster;
			contador_vidas = 3;
			puntaje = 0;
			add_range = 0;
			number_bombs = 0;
			temporizador = 150;
			monster_speed = 5;
			MOVEMENT_DELTA = 3;
		
			if (keystate[SDL_SCANCODE_SPACE]) {

				state = LEVEL_1;
				
			  init_game(&player,
			  map_elements, 
			  NUM_MAP_ELEMENTS,
			  lista_destructibles, 
			  NUM_DESTRUCTIBLES,
			  &explosion_object,
		  	  &explosion_object2,
			  &bomb_object, 
			  &bomb_object1,
		  	  &bomb_object2,
		  	  &bomb_object3,
			  monsters,
			  &portal_object,
			  &powerup_addbomb_object,
			  &powerup_speed_object,
		  	  &powerup_explosion_range_object);
	  
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

		draw_grass();
		draw_vida(contador_vidas);
		draw_numbermap();
		draw_score();
		draw_clock_image();
		draw_timer_countdown();
		draw_powerup_addbomb(&powerup_addbomb_object);
		draw_powerup_speed(&powerup_speed_object);
		draw_powerup_explosion_range(&powerup_explosion_range_object);
		
		
			if (contador_vidas == 0 || temporizador == 0) {	
			draw_vida(contador_vidas);
				contador_vidas = 3;
				state = GAME_OVER;	

			} 		
			
			draw_portal(&portal_object);

		
			
			
			if (keystate[SDL_SCANCODE_DOWN]) {
				draw_skin(DOWN,&player);}
			
			else if (keystate[SDL_SCANCODE_UP]) {
				draw_skin(UP,&player);}
			
			else if (keystate[SDL_SCANCODE_LEFT]) {
				draw_skin(LEFT,&player);}
			
			else if (keystate[SDL_SCANCODE_RIGHT]) {
				draw_skin(RIGHT,&player);}
				
			else{
				draw_skin(DOWN,&player);
			}
			
			
			
			
			if (check_collision(player, powerup_addbomb_object) == TRUE){
				
				powerup_addbomb_object.x = 2000;
				number_bombs += 1;
			
			}	
			
			else if (check_collision(player, powerup_speed_object) == TRUE){
			
				powerup_speed_object.x = 2000;
				MOVEMENT_DELTA =+ 5;
			}
			
			else if (check_collision(player, powerup_explosion_range_object) == TRUE){
				powerup_explosion_range_object.x = 2000;
				add_range += 1;
			
			}
			
			else {}
				
				
			
			
			for (int i = 0; i < 25; i++) { 
   				 if (i < 4) {
        			
       				draw_game_elementLimites(&map_elements[i]);}
       				 
			    	 else {
				
					draw_game_element(&map_elements[i]);
			    }
		    }
			

			for(int i=0;i<16;i++){
			draw_game_element_des(&lista_destructibles[i]);
			}
				
			
					
			time_bomb_countdown(
				&bomb_object,
				&bomb_object1,
				&bomb_object2,
				&bomb_object3,
				&explosion_object,
				&explosion_object2,
				lista_destructibles,
				monsters,
				&player,
				map_elements);

			printf("Time remaining: %d",temporizador);
			printf("\n");
			kill_player(&explosion_object, &player, monsters, state);
			kill_player(&explosion_object2, &player, monsters, state);

				

			for (int i = 0; i < num_monster; i++) {
			    draw_game_element_monster(&monsters[i]);
			}						
			//draw a bomb
			draw_bomb(&bomb_object,0);
			draw_bomb(&bomb_object1,1);
			draw_bomb(&bomb_object2,2);
			draw_bomb(&bomb_object3,3);



			if (keystate[SDL_SCANCODE_1] && bomb_placed==0 && number_bombs>=0) {

				bomb_object.x = player.x;
				bomb_object.y = player.y;
				
				draw_bomb(&bomb_object,0);

				
    				bomb_timer = SDL_GetTicks();
    				bomb_placed = 1; 
    				
    					

			}
			
			else if (keystate[SDL_SCANCODE_2] && bomb_placed_1==0 && number_bombs>=1) {

				bomb_object1.x = player.x;
				bomb_object1.y = player.y;
				
				draw_bomb(&bomb_object1,1);
				
				
    				bomb_timer = SDL_GetTicks();
    				bomb_placed_1 = 1; 
    				
    					

			}
			
			else if (keystate[SDL_SCANCODE_3] && bomb_placed_2==0 && number_bombs>=2) {

				bomb_object2.x = player.x;
				bomb_object2.y = player.y;
				
				draw_bomb(&bomb_object2,2);
				
				
    				bomb_timer = SDL_GetTicks();
    				bomb_placed_2 = 1; 
    				
    					

			}
			
			else if (keystate[SDL_SCANCODE_4] && bomb_placed_3==0 && number_bombs>=3) {

				bomb_object3.x = player.x;
				bomb_object3.y = player.y;
				
				draw_bomb(&bomb_object3,3);
				
				
    				bomb_timer = SDL_GetTicks();
    				bomb_placed_3 = 1; 
    				
    					

			}

			if(contador_muerte == 0){
				if (check_collision(player, portal_object) == TRUE) {
							
				contador_muerte = num_monster;	
				monster_speed = 5*1.5;
				temporizador = 150;

				state = LEVEL_2;
				  init_game(&player,
				  map_elements, 
				  NUM_MAP_ELEMENTS,
				  lista_destructibles, 
				  NUM_DESTRUCTIBLES,
				  &explosion_object,
			  	  &explosion_object2,
				  &bomb_object, 
				  &bomb_object1,
			  	  &bomb_object2,
			  	  &bomb_object3,
				  monsters,
				  &portal_object,
				  &powerup_addbomb_object,
				  &powerup_speed_object,
			  	  &powerup_explosion_range_object);
	  
	  			
					}
		}

}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////		
else if (state == LEVEL_2) {
		
		
		draw_lava();
		draw_vida(contador_vidas);
		draw_numbermap();
		draw_score();
		draw_clock_image();
		draw_timer_countdown();
		
		draw_powerup_addbomb(&powerup_addbomb_object);
		draw_powerup_speed(&powerup_speed_object);
		draw_powerup_explosion_range(&powerup_explosion_range_object);
		
		
			if (contador_vidas == 0 || temporizador == 0) {	
			draw_vida(contador_vidas);
				contador_vidas = 3;
				state = GAME_OVER;	

			} 			
			draw_portal(&portal_object);

		
			
			
			if (keystate[SDL_SCANCODE_DOWN]) {
				draw_skin(DOWN,&player);}
			
			else if (keystate[SDL_SCANCODE_UP]) {
				draw_skin(UP,&player);}
			
			else if (keystate[SDL_SCANCODE_LEFT]) {
				draw_skin(LEFT,&player);}
			
			else if (keystate[SDL_SCANCODE_RIGHT]) {
				draw_skin(RIGHT,&player);}
				
			else{
				draw_skin(DOWN,&player);
			}
			
		if (check_collision(player, powerup_addbomb_object) == TRUE){
			
			powerup_addbomb_object.x = 2000;
			number_bombs += 1;
		
		}	
		
		else if (check_collision(player, powerup_speed_object) == TRUE){
		
			powerup_speed_object.x = 2000;
			MOVEMENT_DELTA =+ 10;
		}
		
		else if (check_collision(player, powerup_explosion_range_object) == TRUE){
			powerup_explosion_range_object.x = 2000;
			add_range += 1;
		
		}
			
		else {}
		
		
			for (int i = 0; i < 25; i++) { 
   				 if (i < 4) {
        			
       				 draw_game_elementLimites(&map_elements[i]);}
       				 
			    	 else {
				
				draw_game_element(&map_elements[i]);
			    }
			    }
			

			for(int i=0;i<16;i++){
			draw_game_element_des(&lista_destructibles[i]);
			}
				

					
			time_bomb_countdown(
				&bomb_object,
				&bomb_object1,
				&bomb_object2,
				&bomb_object3,
				&explosion_object,
				&explosion_object2,
				lista_destructibles,
				monsters,
				&player,
				map_elements);

			printf("Time remaining: %d",temporizador);
			printf("\n");

			kill_player(&explosion_object, &player, monsters, state);
kill_player(&explosion_object2, &player, monsters, state);



			for (int i = 0; i < num_monster; i++) {
			    draw_game_element_monster2(&monsters[i]);
			}			
			

					
			
			//draw a bomb
			draw_bomb(&bomb_object,0);
			draw_bomb(&bomb_object1,1);
			draw_bomb(&bomb_object2,2);
			draw_bomb(&bomb_object3,3);

			
			if (keystate[SDL_SCANCODE_1] && bomb_placed==0 && number_bombs>=0) {

			bomb_object.x = player.x;
			bomb_object.y = player.y;
			
			draw_bomb(&bomb_object,0);
			
			
			bomb_timer = SDL_GetTicks();
			bomb_placed = 1; 
			
				

		}
		
		else if (keystate[SDL_SCANCODE_2] && bomb_placed_1==0 && number_bombs>=1) {

			bomb_object1.x = player.x;
			bomb_object1.y = player.y;
			
			draw_bomb(&bomb_object1,1);
			
			
			bomb_timer = SDL_GetTicks();
			bomb_placed_1 = 1; 
			
				

		}
		
		else if (keystate[SDL_SCANCODE_3] && bomb_placed_2==0 && number_bombs>=2) {

			bomb_object2.x = player.x;
			bomb_object2.y = player.y;
			
			draw_bomb(&bomb_object2,2);
			
			
			bomb_timer = SDL_GetTicks();
			bomb_placed_2 = 1; 
			
				

		}
		
		else if (keystate[SDL_SCANCODE_4] && bomb_placed_3==0 && number_bombs>=3) {

			bomb_object3.x = player.x;
			bomb_object3.y = player.y;
			
			draw_bomb(&bomb_object3,3);
			
			
			bomb_timer = SDL_GetTicks();
			bomb_placed_3 = 1; 
			
				

		}
			if(contador_muerte == 0){
				if (check_collision(player, portal_object) == TRUE) {
				contador_muerte = num_monster;
				monster_speed = 5 * 1.8;
				temporizador = 150;
					
				state = LEVEL_3;
					
				init_game(&player,
				  map_elements, 
				  NUM_MAP_ELEMENTS,
				  lista_destructibles, 
				  NUM_DESTRUCTIBLES,
				  &explosion_object,
			  	  &explosion_object2,
				  &bomb_object, 
				  &bomb_object1,
			  	  &bomb_object2,
			  	  &bomb_object3,
				  monsters,
				  &portal_object,
				  &powerup_addbomb_object,
				  &powerup_speed_object,
			  	  &powerup_explosion_range_object);
	  			
			}
}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////		
	
		
		else if (state == LEVEL_3) {
		
		
				  	
		draw_agua();
		draw_vida(contador_vidas);
		draw_numbermap();
		draw_score();
		draw_clock_image();
		draw_timer_countdown();
		
		draw_powerup_addbomb(&powerup_addbomb_object);
		draw_powerup_speed(&powerup_speed_object);
		draw_powerup_explosion_range(&powerup_explosion_range_object);
		
			//if either player wins, change to game over state
			if (contador_vidas == 0 || temporizador == 0) {	
			draw_vida(contador_vidas);
				contador_vidas = 3;
				state = GAME_OVER;	

			} 			
						
			draw_portal(&portal_object);

		
			
			
			if (keystate[SDL_SCANCODE_DOWN]) {
				draw_skin(DOWN,&player);}
			
			else if (keystate[SDL_SCANCODE_UP]) {
				draw_skin(UP,&player);}
			
			else if (keystate[SDL_SCANCODE_LEFT]) {
				draw_skin(LEFT,&player);}
			
			else if (keystate[SDL_SCANCODE_RIGHT]) {
				draw_skin(RIGHT,&player);}
				
			else{
				draw_skin(DOWN,&player);
			}
			
		if (check_collision(player, powerup_addbomb_object) == TRUE){
			
			powerup_addbomb_object.x = 2000;
			number_bombs += 1;
		
		}	
		
		else if (check_collision(player, powerup_speed_object) == TRUE){
		
			powerup_speed_object.x = 2000;
			MOVEMENT_DELTA =+ 10;
		}
		
		else if (check_collision(player, powerup_explosion_range_object) == TRUE){
			powerup_explosion_range_object.x = 2000;
			add_range += 1;
		
		}
		
		else {}
	
			
			for (int i = 0; i < 25; i++) { 
   				 if (i < 4) {
   				 
        			
       				 draw_game_elementLimites(&map_elements[i]);}
       				 
			    	 else {
				
				draw_game_element(&map_elements[i]);
			    }
			    }
			

			for(int i=0;i<16;i++){
			draw_game_element_des(&lista_destructibles[i]);
			}				
					
			time_bomb_countdown(
				&bomb_object,
				&bomb_object1,
				&bomb_object2,
				&bomb_object3,
				&explosion_object,	 
			 	&explosion_object2,
			 	lista_destructibles,
			 	monsters,
			 	&player,
			 	map_elements);	
			
			printf("Time remaining: %d",temporizador);
			printf("\n");				
			kill_player(&explosion_object, &player, monsters, state);
kill_player(&explosion_object2, &player, monsters, state);


			for (int i = 0; i < num_monster; i++) {
			    draw_game_element_monster3(&monsters[i]);
			}		
			
			for (int i = 0; i < num_monster; i++) {
			}			
			
			
			//draw a bomb
			draw_bomb(&bomb_object,0);
			draw_bomb(&bomb_object1,1);
			draw_bomb(&bomb_object2,2);
			draw_bomb(&bomb_object3,3);

		if (keystate[SDL_SCANCODE_1] && bomb_placed==0 && number_bombs>=0) {

			bomb_object.x = player.x;
			bomb_object.y = player.y;
			
			draw_bomb(&bomb_object,0);
			
			
			bomb_timer = SDL_GetTicks();
			bomb_placed = 1; 
			
				

		}
		
		else if (keystate[SDL_SCANCODE_2] && bomb_placed_1==0 && number_bombs>=1) {

			bomb_object1.x = player.x;
			bomb_object1.y = player.y;
			
			draw_bomb(&bomb_object1,1);
			
			
			bomb_timer = SDL_GetTicks();
			bomb_placed_1 = 1; 
			
				

		}
		
		else if (keystate[SDL_SCANCODE_3] && bomb_placed_2==0 && number_bombs>=2) {

			bomb_object2.x = player.x;
			bomb_object2.y = player.y;
			
			draw_bomb(&bomb_object2,2);
			
			
			bomb_timer = SDL_GetTicks();
			bomb_placed_2 = 1; 
			
				

		}
		
		else if (keystate[SDL_SCANCODE_4] && bomb_placed_3==0 && number_bombs>=3) {

			bomb_object3.x = player.x;
			bomb_object3.y = player.y;
			
			draw_bomb(&bomb_object3,3);
			
			
			bomb_timer = SDL_GetTicks();
			bomb_placed_3 = 1; 
			
				

		}
			if(contador_muerte == 0){
				if (check_collision(player, portal_object) == TRUE) {
					
					state = WIN;
					
			  init_game(&player,
			  map_elements, 
			  NUM_MAP_ELEMENTS,
			  lista_destructibles, 
			  NUM_DESTRUCTIBLES,
			  &explosion_object,
		  	  &explosion_object2,
			  &bomb_object, 
			  &bomb_object1,
		  	  &bomb_object2,
		  	  &bomb_object3,
			  monsters,
			  &portal_object,
			  &powerup_addbomb_object,
			  &powerup_speed_object,
		  	  &powerup_explosion_range_object);
	  		  contador_muerte = 3;
					}
				}
			}
		else if (state == WIN) {
			contador_vidas = 3;
			draw_win(); 
			if (keystate[SDL_SCANCODE_SPACE]) {
				state = START_SCREEN;
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
	SDL_FreeSurface(monster2);
	SDL_FreeSurface(portal);
	SDL_FreeSurface(grass);
	SDL_FreeSurface(score);	
	SDL_FreeSurface(clock_image);
	
	SDL_FreeSurface(powerup_addbomb_image);
	SDL_FreeSurface(powerup_speed_image);
	SDL_FreeSurface(powerup_explosion_range_image);	
	
	SDL_FreeSurface(lava);
	SDL_FreeSurface(win);
	SDL_FreeSurface(agua);
	
	free(monsters);
	monsters = NULL; 
	
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
	monster2 = SDL_LoadBMP("monster2.bmp");
	monster3 = SDL_LoadBMP("monster3.bmp");
	portal = SDL_LoadBMP("portal.bmp");	
	grass = SDL_LoadBMP("grass.bmp");
	vida = SDL_LoadBMP("vida.bmp");
	score = SDL_LoadBMP("score.bmp");
	clock_image = SDL_LoadBMP("clock_image.bmp");
	
	powerup_addbomb_image = SDL_LoadBMP("powerup_addbomb_image.bmp");
	powerup_speed_image = SDL_LoadBMP("powerup_speed_image.bmp");
	powerup_explosion_range_image = SDL_LoadBMP("powerup_explosion_range_image.bmp");
	
	lava = SDL_LoadBMP("lava.bmp");
	win = SDL_LoadBMP("win.bmp");
	end = SDL_LoadBMP("end.bmp");
	agua = SDL_LoadBMP("agua.bmp");



if (portal == NULL) {
    printf("No se pudo cargar la imagen portal.bmp: %s\n", SDL_GetError());
}


	

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




