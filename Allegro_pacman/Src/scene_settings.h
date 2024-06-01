#ifndef SCENE_SETTINGS_H
#define SCENE_SETTINGS_H
#include "game.h"
#include "shared.h"
#include "scene_menu.h"

Scene scene_settings_create(void);
const char* return_key_name(int);
const char* return_test_tttt(int);
int get_bgm_muted();
int get_eff_muted();
int get_bgm_index();
#endif
