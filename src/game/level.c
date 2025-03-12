#include <libdragon.h>

#include "game/level.h"

struct level level_init(void)
{
	struct level lvl;

	lvl.ecs = ecs_init();
	lvl.player = player_init(&lvl.ecs);

	return lvl;
}

void level_update(struct level *lvl, const joypad_buttons_t btn_press,
		  joypad_buttons_t btn_held, const float dt)
{
	player_update(&lvl->player, &lvl->ecs, btn_press, btn_held);
	ecs_update(&lvl->ecs, dt);
}

void level_render(const struct level *lvl)
{
	rdpq_set_mode_standard();
	rdpq_mode_alphacompare(1);
	player_render(&lvl->player, &lvl->ecs);
}

void level_free(struct level *lvl)
{
	player_free(&lvl->player);
	ecs_free(&lvl->ecs);
}
