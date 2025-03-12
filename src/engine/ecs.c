#include <string.h>

#include "engine/ecs.h"

struct ecs ecs_init(void)
{
	struct ecs ecs;
	ecs.count = 0;
	memset(ecs.flags, 0, ENTITY_MAX_COUNT * sizeof(*ecs.flags));
	memset(ecs.positions, 0, ENTITY_MAX_COUNT * sizeof(*ecs.positions));
	memset(ecs.velocities, 0, ENTITY_MAX_COUNT * sizeof(*ecs.velocities));

	return ecs;
}

uint8_t ecs_entity_add(struct ecs *ecs, const uint8_t flags)
{
	uint8_t id = ecs->count++;
	ecs->flags[id] = flags;
	if (flags & ENT_FLAG_COMP_POS) {
		ecs->positions[id].v[0] = 0.f;
		ecs->positions[id].v[1] = 0.f;
	}

	if (flags & ENT_FLAG_COMP_VEL) {
		ecs->velocities[id].v[0] = 0.f;
		ecs->velocities[id].v[1] = 0.f;
	}

	return id;
}

void ecs_entity_set_position(struct ecs *ecs, const uint8_t id,
			     const float pos[2])
{
	struct comp_position *pos_out = ecs->positions + id;
	pos_out->v[0] = pos[0];
	pos_out->v[1] = pos[1];
}

void ecs_entity_set_velocity(struct ecs *ecs, const uint8_t id,
			     const float vel[2])
{
	struct comp_velocity *vel_out = ecs->velocities + id;
	vel_out->v[0] = vel[0];
	vel_out->v[1] = vel[1];
}

void ecs_entity_set_gravity(struct ecs *ecs, const uint8_t id,
			    const float gravity_force)
{
	ecs->gravities[id].force = gravity_force;
}

void ecs_entity_set_floor_coll(struct ecs *ecs, const uint8_t id,
			       const float floor_coll_height)
{
	ecs->floor_colls[id].height = floor_coll_height;
}

void ecs_entity_set_jump_force(struct ecs *ecs, const uint8_t id,
			       const float jump_force)
{
	ecs->jumps[id].force = jump_force;
}

void ecs_entity_update_jump_cond(struct ecs *ecs, const uint8_t id,
				 const uint8_t jump_cond)
{
	ecs->jumps[id].cond = jump_cond;
}

void ecs_update(struct ecs *ecs, const float dt)
{
	for (uint8_t i = 0; i < ecs->count; i++) {
		uint8_t *flags = ecs->flags + i;
		if (!(*flags & ENT_FLAG_IS_ACTIVE)) {
			continue;
		}

		if ((*flags & ENT_FLAG_COMP_VEL) &&
		    (*flags & ENT_FLAG_COMP_POS)) {
			struct comp_velocity *vel = ecs->velocities + i;
			const struct comp_gravity *grav = ecs->gravities + i;
			if (*flags & ENT_FLAG_COMP_GRAV) {
				vel->v[1] += grav->force * dt;
			}

			struct comp_position *pos = ecs->positions + i;
			pos->v[0] += vel->v[0];
			pos->v[1] += vel->v[1];

			const struct comp_floor_coll *fcoll =
				ecs->floor_colls + i;
			if (*flags & ENT_FLAG_COMP_FLOOR_COLL) {
				if (pos->v[1] > fcoll->height) {
					pos->v[1] = fcoll->height;
					vel->v[1] = 0.f;
				}
			}

			if (*flags & ENT_FLAG_COMP_JUMP) {
				const struct comp_jump *jump = ecs->jumps + i;
				if (jump->cond & 1 &&
				    (pos->v[1] >= fcoll->height)) {
					vel->v[1] = -jump->force;
				}
			}
		}
	}
}

void ecs_free(struct ecs *ecs)
{
	ecs->count = 0;
	memset(ecs->flags, 0, ENTITY_MAX_COUNT * sizeof(*ecs->flags));
	memset(ecs->positions, 0, ENTITY_MAX_COUNT * sizeof(*ecs->positions));
	memset(ecs->velocities, 0, ENTITY_MAX_COUNT * sizeof(*ecs->velocities));
}
