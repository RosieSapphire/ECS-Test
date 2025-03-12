#include "game/player.h"

#define PLAYER_ENT_INDEX 0
#define PLAYER_FLOOR_HEIGHT 184.f
#define PLAYER_JUMP_VEL 2.f
#define PLAYER_ACCEL 4.f
#define PLAYER_DECEL 6.f
#define PLAYER_MOVE_VEL_MAX 3.f
#define PLAYER_GRAVITY 4.f
#define PLAYER_WIDTH_PXLS 29

static sprite_t *spr_player;

struct player player_init(struct ecs *ecs)
{
	spr_player = sprite_load("rom:/SPR.Player.ci8.sprite");

	struct player p;
	p.is_facing_left = false;
	p.ent_id = ecs_entity_add(
		ecs, ENT_FLAG_IS_ACTIVE | ENT_FLAG_COMP_POS |
			     ENT_FLAG_COMP_VEL | ENT_FLAG_COMP_GRAV |
			     ENT_FLAG_COMP_FLOOR_COLL | ENT_FLAG_COMP_JUMP |
			     ENT_FLAG_COMP_XCLAMP | ENT_FLAG_COMP_MOVE);
	ecs_entity_set_position(ecs, p.ent_id,
				(float[2]){ 100.f, PLAYER_FLOOR_HEIGHT });
	ecs_entity_set_velocity(ecs, p.ent_id, (float[2]){ 0.f, 0.f });
	ecs_entity_set_gravity(ecs, p.ent_id, PLAYER_GRAVITY);
	ecs_entity_set_floor_coll(ecs, p.ent_id, PLAYER_FLOOR_HEIGHT);
	ecs_entity_set_jump_force(ecs, p.ent_id, PLAYER_JUMP_VEL);
	ecs_entity_set_xclamp(ecs, p.ent_id, 0.f, 320.f - PLAYER_WIDTH_PXLS);
	ecs_entity_set_move(ecs, p.ent_id, PLAYER_ACCEL, PLAYER_DECEL,
			    PLAYER_MOVE_VEL_MAX, 0);

	return p;
}

void player_update(struct player *p, struct ecs *ecs,
		   const joypad_buttons_t btn_press,
		   const joypad_buttons_t btn_held)
{
	ecs_entity_set_jump_condition(ecs, p->ent_id, btn_press.a);
	ecs_entity_set_move_dir(ecs, p->ent_id,
				btn_held.d_right - btn_held.d_left);

	/* facing dir */
	const struct comp_velocity *vel = ecs->velocities + p->ent_id;
	if (vel->v[0] > 0.f) {
		p->is_facing_left = false;
	} else if (vel->v[0] < 0.f) {
		p->is_facing_left = true;
	}
}

void player_render(const struct player *p, const struct ecs *ecs)
{
	const struct comp_position *pos = ecs->positions + p->ent_id;
	rdpq_sprite_blit(
		spr_player, pos->v[0], pos->v[1],
		&(const rdpq_blitparms_t){ .flip_x = p->is_facing_left });
}

void player_free(struct player *p)
{
	sprite_free(spr_player);
	spr_player = NULL;
	p->ent_id = -1;
}
