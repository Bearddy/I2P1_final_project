
#include "scene_settings.h"
#include "scene_game.h"
#include "scene_menu_object.h"
#include "utility.h"
#include <allegro5/allegro_primitives.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


int change = 0;
int error_msg = 0;


static void init(void) {

}



static void draw(void) {

	al_draw_text(menuFont, 
		al_map_rgb(255, 255, 255), 
		SCREEN_W / 2, 240, 
		ALLEGRO_ALIGN_CENTER, 
		"Press any key to change"
	);
	al_draw_text(menuFont,
		al_map_rgb(255, 255, 255),
		SCREEN_W / 2, 300,
		ALLEGRO_ALIGN_CENTER,
		"Any key can't be included Esc, C, G, K"
	);
	al_draw_text(menuFont,
		al_map_rgb(255, 255, 255),
		SCREEN_W / 2, 700,
		ALLEGRO_ALIGN_CENTER,
		"<Enter> back to settings"
	);

	if (error_msg == 1) {
		al_draw_text(menuFont,
			al_map_rgb(255, 0, 50),
			SCREEN_W / 2, 400,
			ALLEGRO_ALIGN_CENTER,
			"Please do not press Esc, C, G, K"
		);
	}
	else if (error_msg == 2) {
		al_draw_text(menuFont,
			al_map_rgb(255, 0, 50),
			SCREEN_W / 2, 500,
			ALLEGRO_ALIGN_CENTER,
			"Please do not press the duplicated key"
		);
	}
	else if (error_msg == 3) {
		al_draw_text(menuFont,
			al_map_rgb(255, 0, 50),
			SCREEN_W / 2, 500,
			ALLEGRO_ALIGN_CENTER,
			"That key is not supported"
		);
	}

}



static void on_key_down(int keycode) {
	//0 : up, 1 : down, 2 : left, 3 : right
	if (!(keycode != ALLEGRO_KEY_C && keycode != ALLEGRO_KEY_ESCAPE && keycode != ALLEGRO_KEY_G && keycode != ALLEGRO_KEY_K)) error_msg = 1;
	else if ((change == 0 && (keycode == get_custom_key("down") || keycode == get_custom_key("left") || keycode == get_custom_key("right")))
		|| (change == 1 && (keycode == get_custom_key("up") || keycode == get_custom_key("left") || keycode == get_custom_key("right"))) 
		|| (change == 2 && (keycode == get_custom_key("up") || keycode == get_custom_key("down") || keycode == get_custom_key("right")))
		|| (change == 3 && (keycode == get_custom_key("up") || keycode == get_custom_key("down") || keycode == get_custom_key("left")))) error_msg = 2;
	else if (keycode < 1 || keycode > 94) error_msg = 3;
	else if(keycode == ALLEGRO_KEY_ENTER) game_change_scene(scene_settings_create());
	else {
		set_custom_key(change == 0 ? "up" : change == 1 ? "down" : change == 2 ? "left" : change == 3 ? "right" : "dd", keycode);
		error_msg = 0;
		game_change_scene(scene_settings_create());
	}
	

}

// The only function that is shared across files.
Scene scene_keysettings_create(int key) {
	Scene scene;
	memset(&scene, 0, sizeof(Scene));
	scene.initialize = &init;
	scene.name = "Settings";
	scene.draw = &draw;
	scene.on_key_down = &on_key_down;
	game_log("Key Settings scene created");
	change = key;
	return scene;
}




