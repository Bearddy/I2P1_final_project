// TODO-HACKATHON 3-9: Create scene_settings.h and scene_settings.c.
// No need to do anything for this part. We've already done it for
// you, so this 2 files is like the default scene template.
#include "scene_settings.h"
#include "scene_game.h"
#include "pacman_obj.h"
#include "scene_menu_object.h"
#include "scene_keysettings.h"
#include "utility.h"
#include <allegro5/allegro_primitives.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Variables and functions with 'static' prefix at the top level of a
// source file is only accessible in that file ("file scope", also
// known as "internal linkage"). If other files has the same variable
// name, they'll be different variables.

/* Define your static vars / function prototypes below. */

// TODO-IF: More variables and functions that will only be accessed
// inside this scene. They should all have the 'static' prefix.

char char_preview_name[4][100] = {
	"Assets/my/pacman_preview.png",
	"Assets/my/realman_preview.png",
	"Assets/my/ghostman_preview.png"
};

char bgm_name[4][100] = {
	"original",
	"silent hill",
	"idiot"
};

char key_name[100][20] = { "nothing", "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "NUMPAD 0", "NUMPAD 1", "NUMPAD 2", "NUMPAD 3", "NUMPAD 4", "NUMPAD 5", "NUMPAD 6", "NUMPAD 7", "NUMPAD 8", "NUMPAD 9", "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12", "ESC", "TILDE", "-", "=", "BACKSPACE", "TAB", "[", "]", "ENTER", ";", "|", "||", "|||", ",", ".", "/", "SPACEBAR", "INSERT", "DELETE", "HOME", "END", "PAGE UP", "PAGE DOWN", "LEFT ARROW", "RIGHT ARROW", "UP ARROW", "DOWN ARROW", "NUMPAD /", "NUMPAD *", "NUMPAD -", "NUMPAD +", "NUM .", "NUM ENTER", "PRINTSCREEN", "PAUSE" };
int bgm_muted = 0;
int eff_muted = 0;
int bgm_index = 0;
int click_dropdown = 0;
int dy = 90;

ALLEGRO_FONT* myFont = NULL;
static Button btnNextPacArrow;
static Button btnPrevPacArrow;
static Button btnUpKey;
static Button btnDownKey;
static Button btnLeftKey;
static Button btnRightKey;
static Button btnMuteBgm;
static Button btnMuteBgmAfter;
static Button btnMuteEff;
static Button btnMuteEffAfter;
static Button btnDropDown;
static Button btnBgmOriginal;
static Button btnBgmSilent;
static Button btnBgmIdiot;
static ALLEGRO_BITMAP* char_preview;
ALLEGRO_TIMER *show_char_timer = NULL;
bool get_keys = false;



static void init(void) {
	click_dropdown = 0;
	show_char_timer = al_create_timer(1.0f / 60.0f);
	al_set_timer_count(show_char_timer, 0);
	al_start_timer(show_char_timer);
	myFont = load_font("Assets/OpenSans-Regular.ttf", 18);
	char_preview = load_bitmap(char_preview_name[get_char_index()]);
	
	btnPrevPacArrow = button_create(300, 70, 38, 38, "Assets/my/prev_pac.png", "Assets/my/prev_pac2.png");
	btnNextPacArrow = button_create(500-38, 70, 38, 38, "Assets/my/next_pac.png", "Assets/my/next_pac2.png");

	btnMuteBgm = button_create(150 - 25, 360 + dy, 50, 50, "Assets/my/checkbox.png", "Assets/my/checkbox.png");
	btnMuteEff = button_create(150 - 25, 430 + dy, 50, 50, "Assets/my/checkbox.png", "Assets/my/checkbox.png");
	btnMuteBgmAfter = button_create(150-25, 360 + dy, 50, 50, "Assets/my/checkbox2.png", "Assets/my/checkbox2.png");
	btnMuteEffAfter = button_create(150 - 25, 430 + dy, 50, 50, "Assets/my/checkbox2.png", "Assets/my/checkbox2.png");

	btnDropDown = button_create(500 - 16, 365+40 + dy, 160, 32, "Assets/my/dropdown.png", "Assets/my/dropdown.png");
	btnBgmOriginal = button_create(500 - 16, 365+72 + dy, 160, 32, "Assets/my/bgm_original1.png", "Assets/my/bgm_original2.png");
	btnBgmSilent = button_create(500 - 16, 365+104 + dy, 160, 32, "Assets/my/bgm_silent1.png", "Assets/my/bgm_silent2.png");
	btnBgmIdiot = button_create(500 - 16, 365+136 + dy, 160, 32, "Assets/my/bgm_idiot1.png", "Assets/my/bgm_idiot2.png");

	btnUpKey = button_create(250-128, 150 + dy, 256, 48, "Assets/my/btn_outline.png", "Assets/my/btn_outline2.png");
	btnDownKey = button_create(650-128, 150 + dy, 256, 48, "Assets/my/btn_outline.png", "Assets/my/btn_outline2.png");
	btnLeftKey = button_create(250-128, 220 + dy, 256, 48, "Assets/my/btn_outline.png", "Assets/my/btn_outline2.png");
	btnRightKey = button_create(650-128, 220 + dy, 256, 48, "Assets/my/btn_outline.png", "Assets/my/btn_outline2.png");

	game_log("sdfsdfsdfs");
}


int offset = 0;
static void draw(void ){
	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 255),
		SCREEN_W/2,
		SCREEN_H - 150,
		ALLEGRO_ALIGN_CENTER,
		"<ENTER> Back to menu"
	);
	//-----------------------------------------for change charactor-----------------------------------------
	drawButton(btnNextPacArrow);
	drawButton(btnPrevPacArrow);

	if ((al_get_timer_count(show_char_timer) >> 5)&1 == 1) {
		offset = 32;
	}
	else {
		offset = 0;
	}
	if (get_char_index() == 1) {
		al_draw_scaled_bitmap(
			char_preview,
			offset, 0,
			32, 32,
			SCREEN_W / 2 - 25, 60,
			50, 50,
			0
		);
	}
	else if(get_char_index() == 0){
		al_draw_scaled_bitmap(
			char_preview,
			offset, 0,
			32, 32,
			SCREEN_W / 2 - 16, 70,
			32, 32,
			0
		);
	}
	else if (get_char_index() == 2) {
		al_draw_scaled_bitmap(
			char_preview,
			offset/2, 0,
			16, 16,
			SCREEN_W / 2 - 16, 70,
			32, 32,
			0
		);
	}
	
	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 255),
		SCREEN_W / 2,
		30,
		ALLEGRO_ALIGN_CENTER,
		"Select Character"
	);

	char* char_title = (char*)malloc(sizeof(char) * 50);
	strcpy(char_title, &char_preview_name[get_char_index()][10]);
	char_title[strlen(char_title) - 12] = '\0';
	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 255),
		SCREEN_W / 2,
		120,
		ALLEGRO_ALIGN_CENTER,
		char_title
	);
	//---------------------------------------------------------------------------------------------

	//------------------------------------------for custom keys------------------------------------
	drawButton(btnDownKey);
	drawButton(btnUpKey);
	drawButton(btnLeftKey);
	drawButton(btnRightKey);

	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 255),
		SCREEN_W / 2,
		190,
		ALLEGRO_ALIGN_CENTER,
		"Key Settings | Click to Change"
	);

	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 255),
		250, 160 + dy,
		ALLEGRO_ALIGN_CENTER,
		key_name[get_custom_key("up")]
	);
	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 255),
		250, 230 + dy,
		ALLEGRO_ALIGN_CENTER,
		key_name[get_custom_key("left")]
	);
	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 255),
		650, 160 + dy,
		ALLEGRO_ALIGN_CENTER,
		key_name[get_custom_key("down")]
	);
	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 255),
		650, 230 + dy,
		ALLEGRO_ALIGN_CENTER,
		key_name[get_custom_key("right")]
	);

	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 255),
		80, 160 + dy,
		ALLEGRO_ALIGN_CENTER,
		"UP :"
	);
	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 255),
		60, 230 + dy,
		ALLEGRO_ALIGN_CENTER,
		"LEFT :"
	);
	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 255),
		460, 160 + dy,
		ALLEGRO_ALIGN_CENTER,
		"DOWN :"
	);
	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 255),
		450, 230 + dy,
		ALLEGRO_ALIGN_CENTER,
		"RIGHT :"
	);
	//---------------------------------------------------------------------------------------------

	if (!bgm_muted) {
		drawButton(btnMuteBgm);
	}
	else {
		drawButton(btnMuteBgmAfter);
	}
	if (!eff_muted) drawButton(btnMuteEff);
	else drawButton(btnMuteEffAfter);
	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 255),
		190, 370 + dy,
		ALLEGRO_ALIGN_LEFT,
		"Mute BGM"
	);
	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 255),
		190, 440 + dy,
		ALLEGRO_ALIGN_LEFT,
		"Mute EFFECT"
	);


	drawButton(btnDropDown);
	if (click_dropdown) {
		drawButton(btnBgmOriginal);
		drawButton(btnBgmSilent);
		drawButton(btnBgmIdiot);
	}
	

	al_draw_text(
		myFont,
		al_map_rgb(255, 255, 255),
		562, 407 + dy,
		ALLEGRO_ALIGN_CENTER,
		bgm_name[bgm_index]
	);
	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 255),
		500-16, 365 + dy,
		ALLEGRO_ALIGN_LEFT,
		"Select BGM"
	);
	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 255),
		SCREEN_W / 2, 400,
		ALLEGRO_ALIGN_CENTER,
		"Sound Settings"
	);

}

static void on_mouse_move(int a, int mouse_x, int mouse_y, int f) {
	btnNextPacArrow.hovered = buttonHover(btnNextPacArrow, mouse_x, mouse_y);
	btnPrevPacArrow.hovered = buttonHover(btnPrevPacArrow, mouse_x, mouse_y);
	btnUpKey.hovered = buttonHover(btnUpKey, mouse_x, mouse_y);
	btnDownKey.hovered = buttonHover(btnDownKey, mouse_x, mouse_y);
	btnLeftKey.hovered = buttonHover(btnLeftKey, mouse_x, mouse_y);
	btnRightKey.hovered = buttonHover(btnRightKey, mouse_x, mouse_y);
	btnMuteBgm.hovered = buttonHover(btnMuteBgm, mouse_x, mouse_y);
	btnMuteBgmAfter.hovered = buttonHover(btnMuteBgmAfter, mouse_x, mouse_y);
	btnMuteEff.hovered = buttonHover(btnMuteEff, mouse_x, mouse_y);
	btnMuteEffAfter.hovered = buttonHover(btnMuteEffAfter, mouse_x, mouse_y);
	btnDropDown.hovered = buttonHover(btnDropDown, mouse_x, mouse_y);
	btnBgmOriginal.hovered = buttonHover(btnBgmOriginal, mouse_x, mouse_y);
	btnBgmSilent.hovered = buttonHover(btnBgmSilent, mouse_x, mouse_y);
	btnBgmIdiot.hovered = buttonHover(btnBgmIdiot, mouse_x, mouse_y);
}

static void on_mouse_down() {
	if (btnNextPacArrow.hovered) {
		if (get_char_index() == 2)
			set_char_index(0);
		else
			set_char_index(get_char_index() + 1);
		al_set_timer_count(show_char_timer, 0);
		al_start_timer(show_char_timer);
		char_preview = load_bitmap(char_preview_name[get_char_index()]);
	}
	
	if (btnPrevPacArrow.hovered) {
		if (get_char_index() == 0)
			set_char_index(2);
		else
			set_char_index(get_char_index() - 1);
		al_set_timer_count(show_char_timer, 0);
		al_start_timer(show_char_timer);
		char_preview = load_bitmap(char_preview_name[get_char_index()]);
	}
	
	if (btnUpKey.hovered) {
		game_change_scene(scene_keysettings_create(0));
	}
	
	if (btnDownKey.hovered) {
		game_change_scene(scene_keysettings_create(1));
	}
	
	if (btnLeftKey.hovered) {
		game_change_scene(scene_keysettings_create(2));
	}
	
	if (btnRightKey.hovered) {
		game_change_scene(scene_keysettings_create(3));
	}
	
	if (btnMuteBgmAfter.hovered || btnMuteBgm.hovered) {
		
		bgm_muted = !bgm_muted;
	}
		
	if (btnMuteEff.hovered || btnMuteEffAfter.hovered) {
		eff_muted = !eff_muted;
	}

	if (btnDropDown.hovered) {
		click_dropdown = !click_dropdown;
	}

	if(btnBgmOriginal.hovered) {
		bgm_index = 0;
		click_dropdown = !click_dropdown;
	}
	if (btnBgmSilent.hovered) {
		bgm_index = 1;
		click_dropdown = !click_dropdown;
	}
	if (btnBgmIdiot.hovered) {
		bgm_index = 2;
		click_dropdown = !click_dropdown;
	}
	
}

static void on_key_down(int keycode) {
	switch (keycode) {
		case ALLEGRO_KEY_ENTER:
			game_change_scene(scene_menu_create());
			al_stop_timer(show_char_timer);
			break;
		default:
			break;
	}
}

static void destroy(void) {
	al_destroy_font(myFont);
	al_destroy_bitmap(char_preview);
	al_destroy_bitmap(btnBgmOriginal.default_img);
	al_destroy_bitmap(btnBgmOriginal.hovered_img);
	al_destroy_bitmap(btnBgmSilent.default_img);
	al_destroy_bitmap(btnBgmSilent.hovered_img);
	al_destroy_bitmap(btnDropDown.default_img);
	al_destroy_bitmap(btnDropDown.hovered_img);
	al_destroy_bitmap(btnMuteBgm.default_img);
	al_destroy_bitmap(btnMuteBgm.hovered_img);
	al_destroy_bitmap(btnMuteBgmAfter.default_img);
	al_destroy_bitmap(btnMuteBgmAfter.hovered_img);
	al_destroy_bitmap(btnMuteEff.default_img);
	al_destroy_bitmap(btnMuteEff.hovered_img);
	al_destroy_bitmap(btnMuteEffAfter.default_img);
	al_destroy_bitmap(btnMuteEffAfter.hovered_img);
	al_destroy_bitmap(btnNextPacArrow.default_img);
	al_destroy_bitmap(btnNextPacArrow.hovered_img);
	al_destroy_bitmap(btnPrevPacArrow.default_img);
	al_destroy_bitmap(btnPrevPacArrow.hovered_img);
	al_destroy_bitmap(btnUpKey.default_img);
	al_destroy_bitmap(btnUpKey.hovered_img);
	al_destroy_bitmap(btnDownKey.default_img);
	al_destroy_bitmap(btnDownKey.hovered_img);
	al_destroy_bitmap(btnLeftKey.default_img);
	al_destroy_bitmap(btnLeftKey.hovered_img);
	al_destroy_bitmap(btnRightKey.default_img);
	al_destroy_bitmap(btnRightKey.hovered_img);
	al_destroy_bitmap(btnBgmIdiot.default_img);
	al_destroy_bitmap(btnBgmIdiot.hovered_img);


	al_destroy_timer(show_char_timer);
}

// The only function that is shared across files.
Scene scene_settings_create(void) {
	Scene scene;
	memset(&scene, 0, sizeof(Scene));
	scene.initialize = &init;
	scene.name = "Settings";
	scene.draw = &draw;
	scene.on_key_down = &on_key_down;
	scene.on_mouse_move = &on_mouse_move;
	scene.on_mouse_down = &on_mouse_down;
	scene.destroy = &destroy;
	game_log("Settings scene created");
	return scene;
}

static void change_to_next_pac(void) {

}

static void change_to_prev_pac(void) {

}

static void change_to_next_map(void) {
	if (return_map_index() < return_max_map_index())
		set_map_index(return_map_index() + 1);
	else
		set_map_index(return_min_map_index());

}

static void change_to_prev_map(void) {
	if (return_map_index() > return_min_map_index())
		set_map_index(return_map_index() - 1);
	else
		set_map_index(return_max_map_index());
}

int get_bgm_muted() {
	return bgm_muted;
}
int get_eff_muted() {
	return eff_muted;
}
int get_bgm_index() {
	return bgm_index;
}
const char* return_key_name(int keycode) {
	game_log("%d", keycode);
	
	if (keycode == ALLEGRO_KEY_UP)
		return "ARROW UP";
	else if (keycode == ALLEGRO_KEY_DOWN)
		return "ARROW DOWN";
	else if (keycode == ALLEGRO_KEY_LEFT)
		return "ARROW LEFT";
	else if (keycode == ALLEGRO_KEY_RIGHT)
		return "ARROW RIGHT";
	else if (keycode == ALLEGRO_KEY_W)
		return "W";
	else if (keycode == ALLEGRO_KEY_S)
		return "S";
	else if (keycode == ALLEGRO_KEY_D)
		return "D";
	else if (keycode == ALLEGRO_KEY_A)
		return "A";
	else if (keycode == ALLEGRO_KEY_Q)
		return "Q";
	else if (keycode == ALLEGRO_KEY_E)
		return "E";
	else if (keycode == ALLEGRO_KEY_R)
		return "R";
	else if (keycode == ALLEGRO_KEY_T)
		return "T";
	else if (keycode == ALLEGRO_KEY_Y)
		return "Y";
	else if (keycode == ALLEGRO_KEY_U)
		return "U";
	else if (keycode == ALLEGRO_KEY_I)
		return "I";
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

const char* return_test_tttt(int keycode) {
	if (keycode == ALLEGRO_KEY_S)
		return "fsrdknlsdfgknlsdfgnjkldfgnkldfgnkldfgklndfglnk";
	else if (keycode == ALLEGRO_KEY_D)
		return "sdjfbklnblfksdsdfbklsdfblksdfbklsdfbklsdflbk";
	else if (keycode == ALLEGRO_KEY_A)
		return "sdbjflskdbfklsdbflksdbfklsdbfklsdbflkds";
}



// TODO-Graphical_Widget:
// Just suggestions, you can create your own usage.
	// Selecting BGM:
	// 1. Declare global variables for storing the BGM. (see `shared.h`, `shared.c`)
	// 2. Load the BGM in `shared.c`.
	// 3. Create dropdown menu for selecting BGM in setting scene, involving `scene_settings.c` and `scene_setting.h.
	// 4. Switch and play the BGM if the corresponding option is selected.

	// Adjusting Volume:
	// 1. Declare global variables for storing the volume. (see `shared.h`, `shared.c`)
	// 2. Create a slider for adjusting volume in setting scene, involving `scene_settings.c` and `scene_setting.h.
		// 2.1. You may use checkbox to switch between mute and unmute.
	// 3. Adjust the volume and play when the button is pressed.

	// Selecting Map:
	// 1. Preload the map to `shared.c`.
	// 2. Create buttons(or dropdown menu) for selecting map in setting scene, involving `scene_settings.c` and `scene_setting.h.
		// 2.1. For player experience, you may also create another scene between menu scene and game scene for selecting map.
	// 3. Create buttons(or dropdown menu) for selecting map in setting scene, involving `scene_settings.c` and `scene_setting.h.
	// 4. Switch and draw the map if the corresponding option is selected.
		// 4.1. Suggestions: You may use `al_draw_bitmap` to draw the map for previewing. 
							// But the map is too large to be drawn in original size. 
							// You might want to modify the function to allow scaling.