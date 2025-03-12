#pragma once

#include <libdragon.h>

#include "engine/ecs.h"

struct player {
	uint32_t ent_id;
	bool is_facing_left;
};

struct player player_init(struct ecs *ecs);
void player_update(struct player *p, struct ecs *ecs,
		   const joypad_buttons_t btn_press,
		   const joypad_buttons_t btn_held);
void player_render(const struct player *p, const struct ecs *ecs);
void player_free(struct player *p);
