#define _CRT_SECURE_NO_WARNINGS

#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include <string.h>
#include "game.h"
#include "shared.h"
#include "utility.h"
#include "scene_game.h"
#include "scene_menu.h"
#include "pacman_obj.h"
#include "ghost.h"
#include "map.h"




// TODO-HACKATHON 2-0: Create one ghost
// Just modify the GHOST_NUM to 1
#define GHOST_NUM 4
// TODO-GC-ghost: create a least FOUR ghost!
/* global variables*/
extern const uint32_t GAME_TICK_CD;
extern uint32_t GAME_TICK;
extern ALLEGRO_TIMER* game_tick_timer;
int game_main_Score = 0;
bool game_over = false;

/* Internal variables*/
static ALLEGRO_TIMER* power_up_timer;
static const int power_up_duration = 10;
static Pacman* pman;
static Map* basic_map;
static Ghost** ghosts;
bool debug_mode = false;
bool cheat_mode = false;

int key_right = 4;
int key_left = 1;
int key_up = 23;
int key_down = 19;

//-------------------------my code---------------------------------
static char map_names[100][100] = {"Assets/map_nthu.txt", "Assets/test.txt"};
static int map_index = 0;
static const int max_map_index = 1;
static const int min_map_index = 0;
int pressed_k = 0;
int pressed_s_ghost = 0;
int pressed_l = 0;
int pressed_ctrl = 0;
//-----------------------------------------------------------------

/* Declare static function prototypes */
static void init(void);
static void step(void);
static void checkItem(void);
static void status_update(void);
static void update(void);
static void draw(void);
static void printinfo(void);
static void destroy(void);
static void on_key_down(int key_code);
static void on_mouse_down(int btn, int x, int y, int dz);
static void render_init_screen(void);
static void draw_hitboxes(void);

//-----------------------------my code---------------------------------
int return_map_index(void){
	return map_index;
}

const int return_max_map_index(void){
	return max_map_index;
}

const int return_min_map_index(void){
	return min_map_index;
}

char* return_map_names(int index){
	return map_names[index];
}

void set_map_index(int index) {
	map_index = index;
	//game_log("map_index = %d", map_index);
	//game_log("map_name = %s", map_names[map_index]);
}

void set_custom_key(char *s, int key_code) {
	if (strcmp(s, "right") == 0) key_right = key_code;
	else if (strcmp(s, "left") == 0) key_left = key_code;
	else if (strcmp(s, "up") == 0) key_up = key_code;
	else if (strcmp(s, "down") == 0) key_down = key_code;
}

int get_custom_key(char* s) {
	if (strcmp(s, "right") == 0) return key_right;
	else if (strcmp(s, "left") == 0) return key_left;
	else if (strcmp(s, "up") == 0) return key_up;
	else if (strcmp(s, "down") == 0) return key_down;

}

int get_wall_mode() {
	return pressed_l;
}
//----------------------------------------------------------------------

static void init(void) {
	game_over = false;
	game_main_Score = 0;
	// create map
	basic_map = create_map(NULL);
	// TODO-GC-read_txt: Create map from .txt file so that you can design your own map!!
	//basic_map = create_map("Assets/map_nthu.txt");
	basic_map = create_map("Assets/test.txt");
	if (!basic_map) {
		game_abort("error on creating map");
	}	
	// create pacman
	pman = pacman_create();
	if (!pman) {
		game_abort("error on creating pacamn\n");
	}
	
	// allocate ghost memory
	// TODO-HACKATHON 2-1: Allocate dynamic memory for ghosts array.
	
	ghosts = (Ghost**)malloc(sizeof(Ghost*) * GHOST_NUM);
	
	if(!ghosts){
		game_log("We haven't create any ghosts!\n");
	}
	else {
		// TODO-HACKATHON 2-2: create a ghost.
		// Try to look the definition of ghost_create in ghost.h and figure out what should be placed here.
		for (int i = 0; i < GHOST_NUM; i++) {
			game_log("creating ghost %d\n", i);
			ghosts[i] = ghost_create(i);
			if (!ghosts[i])
				game_abort("error creating ghost\n");

		}
	}
	GAME_TICK = 0;

	render_init_screen();
	power_up_timer = al_create_timer(1.0f); // 1 tick per second
	if (!power_up_timer)
		game_abort("Error on create timer\n");


	

	return ;
}

static void step(void) {
	if (pman->objData.moveCD > 0)
		pman->objData.moveCD -= pman->speed;
	for (int i = 0; i < GHOST_NUM; i++) {
		// important for movement
		if (ghosts[i]->objData.moveCD > 0)
			ghosts[i]->objData.moveCD -= ghosts[i]->speed;
	}
}
static void checkItem(void) {
	int Grid_x = pman->objData.Coord.x, Grid_y = pman->objData.Coord.y;
	if (Grid_y >= basic_map->row_num - 1 || Grid_y <= 0 || Grid_x >= basic_map->col_num - 1 || Grid_x <= 0)
		return;
	// TODO-HACKATHON 1-3: check which item you are going to eat and use `pacman_eatItem` to deal with it.
	switch (basic_map->map[Grid_y][Grid_x])
	{
	case '.':
		game_main_Score+=10;
		pacman_eatItem(pman, '.');
		basic_map->map[Grid_y][Grid_x] = ' ';
		break;
	case 'P':
		// TODO-GC-PB: ease power bean
		// pacman_eatItem(...);
		// stop and reset power_up_timer value
		// start the timer
		pacman_eatItem(pman, 'P');
		al_stop_timer(power_up_timer);
		al_set_timer_count(power_up_timer, 0);
		al_start_timer(power_up_timer);
		basic_map->map[Grid_y][Grid_x] = ' ';
		break;
	default:
		break;
	}

	// TODO-HACKATHON 1-4: erase the item you eat from map
	// Be careful, don't erase the wall block.
	
	
}
static void status_update(void) {
	// TODO-PB: check powerUp duration
	/*
	if (pman->powerUp)
	{
		// Check the value of power_up_timer
		// If runs out of time reset all relevant variables and ghost's status
		// hint: ghost_toggle_FLEE
	}
	*/
	if (pman->powerUp) {
		if(al_get_timer_count(power_up_timer) > power_up_duration){
			pman->powerUp = false;
			for(int i = 0; i < GHOST_NUM; i++){
				ghost_toggle_FLEE(ghosts[i], false);
			}
		}
		else {
			for(int i = 0; i < GHOST_NUM; i++){
				ghost_toggle_FLEE(ghosts[i], true);
			}
		}
	}


	for (int i = 0; i < GHOST_NUM; i++) {
		if (ghosts[i]->status == GO_IN){
			continue;
		}
		else if (ghosts[i]->status == FREEDOM)
		{
			// TODO-GC-game_over: use `getDrawArea(..., GAME_TICK_CD)` and `RecAreaOverlap(..., GAME_TICK_CD)` functions to detect if pacman and ghosts collide with each other.
			// And perform corresponding operations.
			// [NOTE] You should have some if-else branch here if you want to implement power bean mode.
			// Uncomment Following Code
			
			
			RecArea pmanHitbox = getDrawArea((object *)pman, GAME_TICK_CD);
			RecArea ghostHitbox = getDrawArea((object *)ghosts[i], GAME_TICK_CD);
			if(!cheat_mode && RecAreaOverlap(&pmanHitbox, &ghostHitbox))
			{
					game_log("collide with ghost\n");
					al_rest(1.0);
					pacman_die();
					game_over = true;
					break; // animation shouldn't be trigger twice.
			}
			
			
		}
		else if (ghosts[i]->status == FLEE)
		{
			// TODO-GC-PB: if ghost is collided by pacman, it should go back to the cage immediately and come out after a period.
			/*
			if(!cheat_mode and collision of pacman and ghost)
			{
				ghost_collided(...)
			}
			*/
			RecArea pmanHitbox = getDrawArea((object*)pman, GAME_TICK_CD);
			RecArea ghostHitbox = getDrawArea((object*)ghosts[i], GAME_TICK_CD);
			if (!cheat_mode && RecAreaOverlap(&pmanHitbox, &ghostHitbox)){
				ghost_collided(ghosts[i]);
			}
		}
	}
}

static void update(void) {

	if (game_over) {
		// TODO-GC-game_over: start pman->death_anim_counter and schedule a game-over event (e.g change scene to menu) after Pacman's death animation finished
		// hint: refer al_get_timer_started(...), al_get_timer_count(...), al_stop_timer(...), al_rest(...)
		//go backkkkk
		if (!al_get_timer_started(pman->death_anim_counter)) {
			al_set_timer_count(pman->death_anim_counter, 0);
			al_start_timer(pman->death_anim_counter);
		}
		
		if (al_get_timer_count(pman->death_anim_counter) > 12) {
			game_change_scene(scene_menu_create());
		}

		/*
			// start the timer if it hasn't been started.
			// check timer counter.
			// stop the timer if counter reach desired time.
			game_change_scene(...);
		*/
		return;
	}


	step();
	checkItem();
	status_update();
	pacman_move(pman, basic_map);
	for (int i = 0; i < GHOST_NUM; i++) 
		ghosts[i]->move_script(ghosts[i], basic_map, pman);
}

static void draw(void) {

	al_clear_to_color(al_map_rgb(0, 0, 0));

	
	// TODO-GC-scoring: Draw scoreboard, something your may need is sprinf();

	char *score = (char*)malloc(sizeof(char)*50);
	sprintf_s(score, 50, "SCORE : %d", game_main_Score);
	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 255),
		30, 10,
		ALLEGRO_ALIGN_LEFT,
		score
	);
	free(score);

	draw_map(basic_map);

	pacman_draw(pman);
	if (game_over)
		return;
	// no drawing below when game over
	for (int i = 0; i < GHOST_NUM; i++)
		ghost_draw(ghosts[i]);
	
	//debugging mode
	if (debug_mode) {
		draw_hitboxes();
	}

}

static void draw_hitboxes(void) {
	RecArea pmanHB = getDrawArea((object *)pman, GAME_TICK_CD);
	al_draw_rectangle(
		pmanHB.x, pmanHB.y,
		pmanHB.x + pmanHB.w, pmanHB.y + pmanHB.h,
		al_map_rgb_f(1.0, 0.0, 0.0), 2
	);

	for (int i = 0; i < GHOST_NUM; i++) {
		RecArea ghostHB = getDrawArea((object *)ghosts[i], GAME_TICK_CD);
		al_draw_rectangle(
			ghostHB.x, ghostHB.y,
			ghostHB.x + ghostHB.w, ghostHB.y + ghostHB.h,
			al_map_rgb_f(1.0, 0.0, 0.0), 2
		);
	}

}

static void printinfo(void) {
	game_log("pacman:\n");
	game_log("coord: %d, %d\n", pman->objData.Coord.x, pman->objData.Coord.y);
	game_log("PreMove: %d\n", pman->objData.preMove);
	game_log("NextTryMove: %d\n", pman->objData.nextTryMove);
	game_log("Speed: %f\n", pman->speed);
}


static void destroy(void) {
	//homework1111
	// 
	// 
	// 
	// -memory: free ghost resource
	for(int i = 0; i < GHOST_NUM; i++){
		ghost_destory(ghosts[i]);
	}

	pacman_destroy(pman);
	delete_map(basic_map);
	

	
	

	
}

static void on_key_up(int key_code) {
	if (key_code == ALLEGRO_KEY_LCTRL || key_code == ALLEGRO_KEY_RCTRL) {
		pressed_ctrl = 0;
	}
}
static void on_key_down(int key_code) {
	/*switch (key_code)
	{
		// TODO-HACKATHON 1-1: Use allegro pre-defined enum ALLEGRO_KEY_<KEYNAME> to controll pacman movement
		// we provided you a function `pacman_NextMove` to set the pacman's next move direction.
		case ALLEGRO_KEY_W:
			pacman_NextMove(pman, UP);
			break;
		case ALLEGRO_KEY_A:
			pacman_NextMove(pman, LEFT);
			break;
		case ALLEGRO_KEY_S:
			pacman_NextMove(pman, DOWN);
			break;
		case ALLEGRO_KEY_D:
			pacman_NextMove(pman, RIGHT);
			break;
		case ALLEGRO_KEY_C:
			cheat_mode = !cheat_mode;
			if (cheat_mode)
				printf("cheat mode on\n");
			else
				printf("cheat mode off\n");
			break;
		case ALLEGRO_KEY_G:
			debug_mode = !debug_mode;
			break;
		default:
			break;
	}*/
	if (pressed_ctrl) {
		if (cheat_mode) {
			if (key_code == ALLEGRO_KEY_S) {
				for (int i = 0; i < 4; i++) {
					ghosts[i]->stop = !ghosts[i]->stop;
				}
			}
			else if (key_code == ALLEGRO_KEY_L) {
				pressed_l = !pressed_l;
			}

		}
	}

	
	if (key_code == key_up) {
		pacman_NextMove(pman, UP);
	}
	else if (key_code == key_left) {
		pacman_NextMove(pman, LEFT);
	}
	else if (key_code == key_down) {
		pacman_NextMove(pman, DOWN);
	}
	else if (key_code == key_right) {
		pacman_NextMove(pman, RIGHT);
	}
	else if (key_code == ALLEGRO_KEY_C) {
		cheat_mode = !cheat_mode;
		if (cheat_mode)
			printf("cheat mode on\n");
		else
			printf("cheat mode off\n");
	}
	else if (key_code == ALLEGRO_KEY_G) {
		debug_mode = !debug_mode;
	}
	else if (key_code == ALLEGRO_KEY_K) {
		if (!pressed_k) {
			for (int i = 0; i < 4; i++) {
				ghosts[i]->status = GO_IN;
			}
		}
		else {
			for (int i = 0; i < 4; i++) {
				if (ghosts[i]->status == GO_IN)
					ghosts[i]->status = FREEDOM;
			}
		}
		pressed_k = !pressed_k;
	}
	else if (key_code == ALLEGRO_KEY_LCTRL || key_code == ALLEGRO_KEY_RCTRL) {
		pressed_ctrl = 1;
	}
	
	

}

static void on_mouse_down(int btn, int x, int y, int dz) {

	// nothing here

}

static void render_init_screen(void) {
	al_clear_to_color(al_map_rgb(0, 0, 0));
	//delete it later
	basic_map = create_map(map_names[map_index]);
	draw_map(basic_map);
	pacman_draw(pman);
	for (int i = 0; i < GHOST_NUM; i++) {
		ghost_draw(ghosts[i]);
	}

	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 0),
		400, 400,
		ALLEGRO_ALIGN_CENTER,
		"READY!"
	);

	al_flip_display();
	al_rest(2.0);

}

ALLEGRO_TIMER* get_power_up_timer(void) {
	return power_up_timer;
}

const int get_power_up_dur(void) {
	return power_up_duration;
}
// Functions without 'static', 'extern' prefixes is just a normal
// function, they can be accessed by other files using 'extern'.
// Define your normal function prototypes below.

// The only function that is shared across files.
Scene scene_main_create(void) {
	Scene scene;
	memset(&scene, 0, sizeof(Scene));
	scene.name = "Start";
	scene.initialize = &init;
	scene.update = &update;
	scene.draw = &draw;
	scene.destroy = &destroy;
	scene.on_key_down = &on_key_down;
	scene.on_key_up = &on_key_up;
	scene.on_mouse_down = &on_mouse_down;
	// TODO-IF: Register more event callback functions such as keyboard, mouse, ...
	game_log("Start scene created");
	return scene;
}
