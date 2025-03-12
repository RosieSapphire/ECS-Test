#pragma once

#include <libdragon.h>

#include "engine/ecs.h"

#include "game/player.h"
#include "game/testblock.h"

struct level {
	struct ecs ecs;
	struct player player;
	struct testblock testblock;
};

struct level level_init(void);
void level_update(struct level *lvl, const joypad_buttons_t btn_press,
		  joypad_buttons_t btn_held, const float dt);
void level_render(const struct level *lvl);
void level_free(struct level *lvl);
