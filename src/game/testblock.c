#include <libdragon.h>

#include "game/testblock.h"

static sprite_t *spr_testblock;

struct testblock testblock_init(struct ecs *ecs)
{
	spr_testblock = sprite_load("rom:/SPR.TestBlock.ci8.sprite");

	struct testblock tb;
	tb.ent_id = ecs_entity_add(ecs, ENT_FLAG_IS_ACTIVE | ENT_FLAG_COMP_POS |
						ENT_FLAG_COMP_VEL |
						ENT_FLAG_COMP_GRAV |
						ENT_FLAG_COMP_FLOOR_COLL);
	ecs_entity_set_position(ecs, tb.ent_id, (float[2]){ 198, 53 });
	ecs_entity_set_gravity(ecs, tb.ent_id, 3.f);
	ecs_entity_set_floor_coll(ecs, tb.ent_id, 184.f, 16.f);

	return tb;
}

void testblock_render(const struct testblock *tb, const struct ecs *ecs)
{
	const struct comp_position *pos = ecs->positions + tb->ent_id;
	rdpq_sprite_blit(spr_testblock, pos->v[0], pos->v[1], NULL);
}

void testblock_free(struct testblock *tb)
{
	sprite_free(spr_testblock);
	spr_testblock = NULL;
}
