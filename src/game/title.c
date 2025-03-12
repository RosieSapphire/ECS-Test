#include "game/title.h"

#define TITLE_MUSIC_BPM 105
#define TITLE_LOGO_POS_X 30
#define TITLE_LOGO_POS_Y 16
#define TITLE_OPT_START_POS_X 150
#define TITLE_OPT_START_POS_Y 117
#define TITLE_OPT_CONTROLS_POS_X 150
#define TITLE_OPT_CONTROLS_POS_Y 139
#define TITLE_OPT_SELECTOR_POS_X 128
#define TITLE_OPT_SELECTOR_POS_Y_A 119
#define TITLE_OPT_SELECTOR_POS_Y_B 141

static float flash_timer;
static float fadeout_timer;
static int title_opt_cur;
static int title_state;

static wav64_t mus_title_loop;
static wav64_t sfx_title_move;
static wav64_t sfx_title_pick;
static wav64_t sfx_title_start;
static wav64_t sfx_title_voice;

static sprite_t *spr_title_logo;
static sprite_t *spr_title_opt_start;
static sprite_t *spr_title_opt_controls;
static sprite_t *spr_title_opt_selector;

void title_init(void)
{
	flash_timer = 0.f;
	fadeout_timer = 0.f;
	title_opt_cur = 0;
	title_state = 0;

	wav64_open(&mus_title_loop, "rom:/MUS.TitleLoop.wav64");
	wav64_set_loop(&mus_title_loop, true);
	wav64_open(&sfx_title_move, "rom:/SFX.TitleMove.wav64");
	wav64_open(&sfx_title_pick, "rom:/SFX.TitlePick.wav64");
	wav64_open(&sfx_title_start, "rom:/SFX.TitleStart.wav64");
	wav64_open(&sfx_title_voice, "rom:/SFX.TitleVoice.wav64");

	spr_title_logo = sprite_load("rom:/SPR.TitleLogo.ci8.sprite");
	spr_title_opt_start =
		sprite_load("rom:/SPR.TitleOptionStart.ci8.sprite");
	spr_title_opt_controls =
		sprite_load("rom:/SPR.TitleOptionControls.ci8.sprite");
	spr_title_opt_selector =
		sprite_load("rom:/SPR.TitleOptionSelector.ci8.sprite");

	mixer_ch_set_vol(0, .5f, .5f);
	wav64_play(&mus_title_loop, 0);
}

int title_update(const joypad_buttons_t btn_press,
		 __attribute__((unused)) const joypad_buttons_t btn_held,
		 const float dt)
{
	switch (title_state) {
	case 0:
		fadeout_timer = 0.f;
		const int title_opt_old = title_opt_cur;
		title_opt_cur ^= (btn_press.c_down | btn_press.c_up);
		if (title_opt_old ^ title_opt_cur) {
			mixer_ch_set_vol(1, .5f, .5f);
			wav64_play(&sfx_title_move, 1);
		}

		bool has_picked = (btn_press.a | btn_press.start);
		if (has_picked) {
			mixer_ch_set_vol(1, .5f, .5f);
			wav64_play(&sfx_title_pick, 1);
			switch (title_opt_cur) {
			case 0:
				mixer_ch_stop(0);
				wav64_play(&sfx_title_start, 1);
				mixer_ch_set_vol(2, .5f, .5f);
				wav64_play(&sfx_title_voice, 2);
				title_state = 1;
				break;

			case 1:
				debugf("Open Control Menu\n");
				break;
			}
		}

		flash_timer += (((float)TITLE_MUSIC_BPM / 60.f) * dt) * 0.125f;
		break;

	case 1:
		fadeout_timer += dt;
		if (fadeout_timer >= 3.f) {
			return SCENE_GAME;
		}
		break;
	}

	return SCENE_TITLE;
}

void title_render(void)
{
	float flash_val = 0.f;

	if (title_state == 0) {
		flash_val = 1.f - (flash_timer - (int)flash_timer);
		flash_val *= flash_val;
		flash_val *= flash_val;
	}

	rdpq_clear(RGBA32(flash_val * 0xFF, flash_val * 0xFF, flash_val * 0xFF,
			  0xFF));

	rdpq_set_mode_standard();
	rdpq_mode_alphacompare(1);
	rdpq_sprite_blit(spr_title_logo, TITLE_LOGO_POS_X, TITLE_LOGO_POS_Y,
			 NULL);
	rdpq_sprite_blit(spr_title_opt_selector, TITLE_OPT_SELECTOR_POS_X,
			 title_opt_cur ? TITLE_OPT_SELECTOR_POS_Y_B :
					 TITLE_OPT_SELECTOR_POS_Y_A,
			 NULL);
	rdpq_sprite_blit(spr_title_opt_start, TITLE_OPT_START_POS_X,
			 TITLE_OPT_START_POS_Y, NULL);
	rdpq_sprite_blit(spr_title_opt_start, TITLE_OPT_START_POS_X,
			 TITLE_OPT_START_POS_Y, NULL);
	rdpq_sprite_blit(spr_title_opt_controls, TITLE_OPT_CONTROLS_POS_X,
			 TITLE_OPT_CONTROLS_POS_Y, NULL);
	rdpq_sprite_blit(spr_title_opt_controls, TITLE_OPT_CONTROLS_POS_X,
			 TITLE_OPT_CONTROLS_POS_Y, NULL);

	if (title_state == 1) {
		rdpq_set_mode_standard();
		rdpq_set_prim_color(RGBA32(0x0, 0x0, 0x0, fadeout_timer * 85));
		rdpq_mode_combiner(RDPQ_COMBINER_FLAT);
		rdpq_mode_blender(RDPQ_BLENDER_MULTIPLY);
		rdpq_fill_rectangle(0, 0, 320, 240);
	}
}

void title_free(void)
{
	wav64_close(&sfx_title_voice);
	wav64_close(&sfx_title_start);
	wav64_close(&sfx_title_pick);
	wav64_close(&sfx_title_move);
	wav64_close(&mus_title_loop);

	sprite_free(spr_title_opt_selector);
	sprite_free(spr_title_opt_controls);
	sprite_free(spr_title_opt_start);
	sprite_free(spr_title_logo);
}
