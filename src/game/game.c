#include "game/level.h"
#include "game/game.h"

static struct level level;

void game_init(void)
{
	level = level_init();
}

int game_update(const joypad_buttons_t btn_press,
		const joypad_buttons_t btn_held, const float dt)
{
	if (btn_press.start) {
		return SCENE_TITLE;
	}

	level_update(&level, btn_press, btn_held, dt);

	return SCENE_GAME;
}

void game_render(void)
{
	rdpq_clear(RGBA32(0xFF, 0x7F, 0x64, 0xFF));
	level_render(&level);
}

void game_free(void)
{
	level_free(&level);
}
