#include <allegro5/allegro_primitives.h>
#include "pacman_obj.h"
#include "map.h"
#include "scene_settings.h"
#include "scene_game.h"
/* Static variables */
static const int start_grid_x = 25, start_grid_y = 25;		// where to put pacman at the beginning
static const int fix_draw_pixel_offset_x = -3, fix_draw_pixel_offset_y = -3;  // draw offset 
static const int draw_region = 30;							// pacman bitmap draw region
static ALLEGRO_SAMPLE_ID PACMAN_MOVESOUND_ID;
// [ NOTE - speed ]
// If you want to implement something regarding speed.
// You may have to modify the basic_speed here.
// But before you modify this, make sure you are 
// totally understand the meaning of speed and function
// `step()` in `scene_game.c`, also the relationship between
// `speed`, `GAME_TICK`, `GAME_TICK_CD`, `objData->moveCD`.
static const int basic_speed = 2;
//----------------------------------------my code------------------------------
ALLEGRO_BITMAP* pman_move = NULL;
ALLEGRO_BITMAP* pman_die = NULL;
int char_index = 0;

char char_name[4][2][100] = {
	{"Assets/my/pacman_move.png", "Assets/my/pacman_die.png"},
	{"Assets/my/realman_move.png", "Assets/my/realman_die.png"},
	{"Assets/my/ghostman_move.png", "Assets/my/ghostman_die.png"}
};
//-------------------------------------------------------------------------


/* Shared variables */
extern ALLEGRO_SAMPLE* PACMAN_MOVESOUND;
extern ALLEGRO_SAMPLE* PACMAN_DEATH_SOUND;
extern ALLEGRO_SAMPLE* PACMAN_FRUITSOUND;
extern uint32_t GAME_TICK;
extern uint32_t GAME_TICK_CD;
extern bool game_over;
extern float effect_volume;


//----------------------------------------my code------------------------------
void set_pman_bitmap(char* move_path, char* die_path) {
	pman_move = load_bitmap(move_path);
	pman_die = load_bitmap(die_path);
}

void set_char_index(int index) {
	char_index = index;
}

int get_char_index(void) {
	return char_index;
}
//-------------------------------------------------------------------------
/* Declare static function */
static bool pacman_movable(const Pacman* pacman, const Map* M, Directions targetDirec) {
	// TODO-HACKATHON 1-2: Determine if the current direction is movable.
	// That is to say, your pacman shouldn't penetrate 'wall' and 'room'
	// , where room is reserved for ghost to set up.
	// 1) For the current direction `targetDirec`, use pre-implemented function
	// `is_wall_block` and `is_room_block` to check if the block is wall or room. (they are both defined in map.c)
	// 2) the coordinate data of pacman is stored in pacman->objData.Coord
	// it is a self-defined pair IntInt type. Trace the code and utilize it.

	int pac_x = pacman->objData.Coord.x;

	int pac_y = pacman->objData.Coord.y;

	switch (targetDirec)
	{
	case UP:
		if ((!get_wall_mode() && is_wall_block(M, pac_x, pac_y - 1)) || is_room_block(M, pac_x, pac_y - 1))
			return false;
		break;
	case DOWN:
		if ((!get_wall_mode() && is_wall_block(M, pac_x, pac_y + 1)) || is_room_block(M, pac_x, pac_y + 1))
			return false;
		break;
	case LEFT:
		if ((!get_wall_mode() && is_wall_block(M, pac_x - 1, pac_y)) || is_room_block(M, pac_x - 1, pac_y))
			return false;
		break;
	case RIGHT:
		if ((!get_wall_mode() && is_wall_block(M, pac_x + 1, pac_y)) || is_room_block(M, pac_x + 1, pac_y))
			return false;
		break;
	default:
		// for none UP, DOWN, LEFT, RIGHT direction u should return false.
		return false;
		break;
	}
	return true;
}

Pacman* pacman_create() {
	// Allocate dynamic memory for pman pointer;
	Pacman* pman = (Pacman*)malloc(sizeof(Pacman));
	if (!pman)
		return NULL;
	pman->objData.Coord.x = 24;
	pman->objData.Coord.y = 24;
	pman->objData.Size.x = block_width;
	pman->objData.Size.y = block_height;

	pman->objData.preMove = NONE;
	pman->objData.nextTryMove = NONE;
	pman->speed = basic_speed;

	pman->death_anim_counter = al_create_timer(1.0f / 8.0f);
	pman->powerUp = false;
	/* load sprites */
	//default
	game_log("pacman_create loading1");
	pman->move_sprite = load_bitmap(char_name[char_index][0]);
	pman->die_sprite = load_bitmap(char_name[char_index][1]);
	game_log("pacman_create loading2");

	return pman;

}

void pacman_destroy(Pacman* pman) {	
	// TODO-GC-memory: free pacman resource
	//homework6666

	al_destroy_bitmap(pman->die_sprite);
	al_destroy_bitmap(pman->move_sprite);
	al_destroy_timer(pman->death_anim_counter);
		
	free(pman);

	
}

int pac_create_check = 0;

void pacman_draw(Pacman* pman) {
	// TODO-GC-animation: Draw Pacman and animations
	// hint: use pman->objData.moveCD to determine which frame of the animation to draw
	//?????
	RecArea drawArea = getDrawArea((object *)pman, GAME_TICK_CD);

	//Draw default image

	/*al_draw_scaled_bitmap(pman->move_sprite, 0, 0,
		16, 16,
		drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
		draw_region, draw_region, 0
	);*/
	
	int offset = 0;
	if (!game_over) {
		//homework4444
		// TODO-GC-animation: We have two frames for each direction. You can use the value of pman->objData.moveCD to determine which frame of the animation to draw.
		// For example, if the value if odd, draw 1st frame. Otherwise, draw 2nd frame.
		// But this frame rate may be a little bit too high. We can use % 32 and draw 1st frame if value is 0~15, and 2nd frame if value is 16~31.
		switch (char_index) {
			case 0:
				if ((pman->objData.moveCD >> 4) & 1 == 0) {
					offset = 0;
				}	
				else if ((pman->objData.moveCD >> 4) & 1 == 1) {
					offset = 16;
				}

				break;
			case 1:
				if ((pman->objData.moveCD >> 5) & 1 == 0) {
					offset = 0;
				}
				else if ((pman->objData.moveCD >> 5) & 1 == 1) {
					offset = 16;
				}
				break;
			case 2:
				if ((pman->objData.moveCD >> 4) & 1 == 0) {
					offset = 0;
				}
				else if ((pman->objData.moveCD >> 4) & 1 == 1) {
					offset = 16;
				}

				break;
		}
		
		
		/*
		NOTE: since modulo operation is expensive in clock cycle perspective (reference: https://stackoverflow.com/questions/27977834/why-is-modulus-operator-slow)
			, you can use & (bitwise and) operator to determine a value is odd or even.
			e.g. If (val & 1 == 1) is true then `val` is odd. If (val & 1 == 0) is false then `val` is even.
			e.g. Similarly, if ((val>>4) & 1 == 0) is true then `val % 32` is 0~15, if ((val>>4) & 1 == 1) is true then `val % 32` is 16~31. 
		*/
		
		switch (pman->objData.facing)
		{
		case LEFT:
			al_draw_scaled_bitmap(pman->move_sprite, 32 + offset, 0,
				16, 16,
				drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
				draw_region, draw_region, 0
			);
			break;
		case RIGHT:
			al_draw_scaled_bitmap(pman->move_sprite, 0 + offset, 0,
				16, 16,
				drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
				draw_region, draw_region, 0
			);
			break;
		case UP:
			al_draw_scaled_bitmap(pman->move_sprite, 64 + offset, 0,
				16, 16,
				drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
				draw_region, draw_region, 0
			);
			break;
		case DOWN:
			al_draw_scaled_bitmap(pman->move_sprite, 96 + offset, 0,
				16, 16,
				drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
				draw_region, draw_region, 0
			);
			break;
		default:
			al_draw_scaled_bitmap(pman->move_sprite, 0 + offset, 0,
				16, 16,
				drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
				draw_region, draw_region, 0
			);
			break;
		}
		
	}
	else {
		// TODO-GC-animation: Draw die animation(pman->die_sprite)
		// hint: instead of using pman->objData.moveCD, use pman->death_anim_counter to create animation.
		// refer al_get_timer_count and al_draw_scaled_bitmap. Suggestion frame rate: 8fps.
		//homework9999
		al_draw_scaled_bitmap(pman->die_sprite, al_get_timer_count(pman->death_anim_counter)*16, 0,
			16, 16,
			drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
			draw_region, draw_region, 0
		);
		
	}
}
void pacman_move(Pacman* pacman, Map* M) {
	if (!movetime(pacman->speed))
		return;
	if (game_over)
		return;

	int probe_x = pacman->objData.Coord.x, probe_y = pacman->objData.Coord.y;
	if (pacman_movable(pacman, M, pacman->objData.nextTryMove)) 
		pacman->objData.preMove = pacman->objData.nextTryMove;
	else if (!pacman_movable(pacman, M, pacman->objData.preMove)) 
		return;

	switch (pacman->objData.preMove)
	{
	case UP:
		pacman->objData.Coord.y -= 1;
		pacman->objData.preMove = UP;
		break;
	case DOWN:
		pacman->objData.Coord.y += 1;
		pacman->objData.preMove = DOWN;
		break;
	case LEFT:
		pacman->objData.Coord.x -= 1;
		pacman->objData.preMove = LEFT;
		break;
	case RIGHT:
		pacman->objData.Coord.x += 1;
		pacman->objData.preMove = RIGHT;
		break;
	default:
		break;
	}
	pacman->objData.facing = pacman->objData.preMove;
	pacman->objData.moveCD = GAME_TICK_CD;
}
void pacman_eatItem(Pacman* pacman, const char Item) {
	switch (Item)
	{
	case '.':
		stop_bgm(PACMAN_MOVESOUND_ID);
		PACMAN_MOVESOUND_ID = play_audio(PACMAN_MOVESOUND, get_eff_muted() ? 0 : effect_volume);
		break;
	// TODO-GC-PB: set pacman powerUp mode
	case 'P':
		pacman->powerUp = true;
		stop_bgm(PACMAN_MOVESOUND_ID);
		PACMAN_MOVESOUND_ID = play_audio(PACMAN_FRUITSOUND, get_eff_muted() ? 0 : effect_volume);
		game_log("ate power up bean");
		break;
	/*
	case 'P':
		...
		break;
	*/
	default:
		break;
	}
}

void pacman_NextMove(Pacman* pacman, Directions next) {
	pacman->objData.nextTryMove = next;
}

void pacman_die() {
	// TODO-GC-game_over: play sound of pacman's death! see shared.c
	// hint: check pacman_eatItem(...) above.
	stop_bgm(PACMAN_MOVESOUND_ID);
	PACMAN_MOVESOUND_ID = play_audio(PACMAN_DEATH_SOUND, get_eff_muted() ? 0 : effect_volume);
}
