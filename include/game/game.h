#pragma once

#include <libdragon.h>

#include "game/scene.h"

void game_init(void);
int game_update(const joypad_buttons_t btn_press,
		const joypad_buttons_t btn_held, const float dt);
void game_render(void);
void game_free(void);
