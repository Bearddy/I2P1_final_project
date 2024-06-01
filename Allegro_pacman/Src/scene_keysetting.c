// TODO-HACKATHON 3-9: Create scene_settings.h and scene_settings.c.
// No need to do anything for this part. We've already done it for
// you, so this 2 files is like the default scene template.
#include "scene_settings.h"
#include "scene_game.h"
#include "scene_menu_object.h"
#include "utility.h"
#include <allegro5/allegro_primitives.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>





static void init(void) {
	
}



static void draw(void) {
	

}


static void on_key_down(int keycode) {
	
	game_change_scene(scene_settings_create());

}

// The only function that is shared across files.
Scene scene_keysetting_create(void) {
	Scene scene;
	memset(&scene, 0, sizeof(Scene));
	scene.initialize = &init;
	scene.name = "Settings";
	scene.draw = &draw;
	scene.on_key_down = &on_key_down;
	game_log("Key Settings scene created");
	return scene;
}



char return_key_name(int keycode) {
	game_log("%d", keycode);

	if (keycode == ALLEGRO_KEY_UP)
		return 't';
	else if (keycode == ALLEGRO_KEY_DOWN)
		return 'a';
	else if (keycode == ALLEGRO_KEY_LEFT)
		return 'a';
	else if (keycode == ALLEGRO_KEY_RIGHT)
		return 'a';
	else if (keycode == ALLEGRO_KEY_W)
		return 'W';
	else if (keycode == ALLEGRO_KEY_S)
		return 'S';
	else if (keycode == ALLEGRO_KEY_D)
		return 'D';
	else if (keycode == ALLEGRO_KEY_A)
		return 'A';
	else if (keycode == ALLEGRO_KEY_Q)
		return 'Q';
	else if (keycode == ALLEGRO_KEY_E)
		return 'E';
	else if (keycode == ALLEGRO_KEY_R)
		return 'R';
	else if (keycode == ALLEGRO_KEY_T)
		return 'T';
	else if (keycode == ALLEGRO_KEY_Y)
		return 'Y';
	else if (keycode == ALLEGRO_KEY_U)
		return 'U';
	else if (keycode == ALLEGRO_KEY_I)
		return 'I';
	else if (keycode == ALLEGRO_KEY_O)
		return 'O';
	else if (keycode == ALLEGRO_KEY_P)
		return 'P';
	else if (keycode == ALLEGRO_KEY_F)
		return 'F';
	else if (keycode == ALLEGRO_KEY_H)
		return 'H';
	else if (keycode == ALLEGRO_KEY_J)
		return 'J';
	else if (keycode == ALLEGRO_KEY_K)
		return 'K';
	else if (keycode == ALLEGRO_KEY_L)
		return 'L';
	else if (keycode == ALLEGRO_KEY_Z)
		return 'Z';
	else if (keycode == ALLEGRO_KEY_X)
		return 'X';
	else if (keycode == ALLEGRO_KEY_V)
		return 'V';
	else if (keycode == ALLEGRO_KEY_B)
		return 'B';
	else if (keycode == ALLEGRO_KEY_N)
		return 'N';
	else if (keycode == ALLEGRO_KEY_M)
		return 'M';

}

