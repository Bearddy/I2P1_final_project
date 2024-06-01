// Functions without 'static', 'extern' prefixes is just a normal
// function, they can be accessed by other files using 'extern'.
// Define your normal function prototypes below.

#ifndef SCENE_GAME_H
#define SCENE_GAME_H
#include "game.h"

Scene scene_main_create(void);
const int get_power_up_dur(void);
ALLEGRO_TIMER* get_power_up_timer(void);


int return_map_index(void);
char* return_map_names(int);
void set_map_index(int);
const int return_min_map_index(void);
const int return_max_map_index(void);
int get_custom_key(char*);
void set_custom_key(char*, int);
int get_wall_mode(void);
/*
int64_t get_power_up_timer_tick();
int64_t get_power_up_duration();
*/

#endif
