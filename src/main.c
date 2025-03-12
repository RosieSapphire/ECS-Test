#include <libdragon.h>

#include "game/title.h"
#include "game/game.h"

static void (*init_funcs[SCENE_COUNT])(void) = { title_init, game_init };

static int (*update_funcs[SCENE_COUNT])(const joypad_buttons_t,
					const joypad_buttons_t,
					const float) = { title_update,
							 game_update };

static void (*render_funcs[SCENE_COUNT])(void) = { title_render, game_render };

static void (*free_funcs[SCENE_COUNT])(void) = { title_free, game_free };

int main(void)
{
	debug_init_isviewer();
	debug_init_usblog();

	display_init(RESOLUTION_320x240, DEPTH_16_BPP, 2, GAMMA_NONE,
		     FILTERS_RESAMPLE);
	rdpq_init();
	rdpq_debug_start();

	joypad_init();

	audio_init(44100, 2);
	mixer_init(3);

	asset_init_compression(1);
	int dfs_handle = dfs_init(DFS_DEFAULT_LOCATION);

	(*init_funcs[scene_index])();

	while (1) {
		joypad_poll();
		joypad_buttons_t btn_press =
			joypad_get_buttons_pressed(JOYPAD_PORT_1);
		joypad_buttons_t btn_held =
			joypad_get_buttons_held(JOYPAD_PORT_1);
		const float dt = display_get_delta_time();

		int scene_index_old = scene_index;
		scene_index =
			(*update_funcs[scene_index])(btn_press, btn_held, dt);
		if (scene_index ^ scene_index_old) {
			rspq_wait(); /* possibly unnessesary */
			(*free_funcs[scene_index_old])();
			(*init_funcs[scene_index])();
		}

		rdpq_attach(display_get(), NULL);
		(*render_funcs[scene_index])();
		rdpq_detach_show();

		if (audio_can_write()) {
			short *audio_buf = audio_write_begin();
			mixer_poll(audio_buf, audio_get_buffer_length());
			audio_write_end();
		}
	}

	(*free_funcs[scene_index])();

	dfs_close(dfs_handle);

	joypad_close();

	mixer_close();
	audio_close();

	rdpq_debug_stop();
	rdpq_close();
	display_close();

	return 0;
}
