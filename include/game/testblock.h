#pragma once

#include "engine/ecs.h"

struct testblock {
	uint8_t ent_id;
};

struct testblock testblock_init(struct ecs *ecs);
void testblock_render(const struct testblock *tb, const struct ecs *ecs);
void testblock_free(struct testblock *tb);
