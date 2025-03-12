#pragma once

#include <libdragon.h>

#include "game/scene.h"

void title_init(void);
int title_update(const joypad_buttons_t btn_press,
		 const joypad_buttons_t btn_held, const float dt);
void title_render(void);
void title_free(void);
