#include "game/player.h"

#define PLAYER_ENT_INDEX 0
#define PLAYER_FLOOR_HEIGHT 184.f
#define PLAYER_JUMP_VEL 2.f
#define PLAYER_ACCEL 4.f
#define PLAYER_DECEL 6.f
#define PLAYER_MOVE_VEL_MAX 3.f
#define PLAYER_GRAVITY 4.f

static sprite_t *spr_player;

struct player player_init(struct ecs *ecs)
{
	spr_player = sprite_load("rom:/SPR.Player.ci8.sprite");

	struct player p;
	p.ent_id = ecs_entity_add(
		ecs, ENT_FLAG_IS_ACTIVE | ENT_FLAG_COMP_POS |
			     ENT_FLAG_COMP_VEL | ENT_FLAG_COMP_GRAV |
			     ENT_FLAG_COMP_FLOOR_COLL | ENT_FLAG_COMP_JUMP);
	ecs_entity_set_position(ecs, p.ent_id,
				(float[2]){ 100.f, PLAYER_FLOOR_HEIGHT });
	ecs_entity_set_velocity(ecs, p.ent_id, (float[2]){ 0.f, 0.f });
	ecs_entity_set_gravity(ecs, p.ent_id, PLAYER_GRAVITY);
	ecs_entity_set_floor_coll(ecs, p.ent_id, PLAYER_FLOOR_HEIGHT);
	ecs_entity_set_jump_force(ecs, p.ent_id, PLAYER_JUMP_VEL);

	return p;
}

void player_update(struct player *p, struct ecs *ecs,
		   const joypad_buttons_t btn_press,
		   const joypad_buttons_t btn_held, const float dt)
{
	/* floor move */
	const int move_dir = (btn_held.d_right - btn_held.d_left);
	struct comp_velocity *vel = ecs->velocities + p->ent_id;
	vel->v[0] += move_dir * PLAYER_ACCEL * dt;
	if (vel->v[0] > PLAYER_MOVE_VEL_MAX) {
		vel->v[0] = PLAYER_MOVE_VEL_MAX;
	}
	if (vel->v[0] < -PLAYER_MOVE_VEL_MAX) {
		vel->v[0] = -PLAYER_MOVE_VEL_MAX;
	}
	if (!move_dir) {
		if (vel->v[0] > 0.f) {
			vel->v[0] -= PLAYER_DECEL * dt;
		}
		if (vel->v[0] < 0.f) {
			vel->v[0] += PLAYER_DECEL * dt;
		}
		if (fabsf(vel->v[0]) < 0.1f) {
			vel->v[0] = 0.f;
		}
	}

	ecs_entity_update_jump_cond(ecs, p->ent_id, btn_press.a);
}

void player_render(const struct player *p, const struct ecs *ecs)
{
	const struct comp_position *pos = ecs->positions + p->ent_id;
	const struct comp_velocity *vel = ecs->velocities + p->ent_id;
	rdpq_sprite_blit(
		spr_player, pos->v[0], pos->v[1],
		&(const rdpq_blitparms_t){ .flip_x = (vel->v[0] < 0.f) });
}

void player_free(struct player *p)
{
	sprite_free(spr_player);
	spr_player = NULL;
	p->ent_id = -1;
}
