#pragma once

#include <stdint.h>

enum {
	ENT_FLAG_IS_ACTIVE = (1 << 0),
	ENT_FLAG_COMP_POS = (1 << 1),
	ENT_FLAG_COMP_VEL = (1 << 2),
	ENT_FLAG_COMP_GRAV = (1 << 3),
	ENT_FLAG_COMP_FLOOR_COLL = (1 << 4),
	ENT_FLAG_COMP_JUMP = (1 << 5),
};

#define ENTITY_MAX_COUNT 256

struct comp_position {
	float v[2];
};

struct comp_velocity {
	float v[2];
};

struct comp_gravity {
	float force;
};

struct comp_floor_coll {
	float height;
};

struct comp_jump {
	float force;
	uint8_t cond;
};

struct ecs {
	uint8_t count;
	uint8_t flags[ENTITY_MAX_COUNT];
	struct comp_position positions[ENTITY_MAX_COUNT];
	struct comp_velocity velocities[ENTITY_MAX_COUNT];
	struct comp_gravity gravities[ENTITY_MAX_COUNT];
	struct comp_floor_coll floor_colls[ENTITY_MAX_COUNT];
	struct comp_jump jumps[ENTITY_MAX_COUNT];
};

struct ecs ecs_init(void);

uint8_t ecs_entity_add(struct ecs *ecs, const uint8_t flags);
void ecs_entity_set_position(struct ecs *ecs, const uint8_t id,
			     const float pos[2]);
void ecs_entity_set_velocity(struct ecs *ecs, const uint8_t id,
			     const float vel[2]);
void ecs_entity_set_gravity(struct ecs *ecs, const uint8_t id,
			    const float gravity_force);
void ecs_entity_set_floor_coll(struct ecs *ecs, const uint8_t id,
			       const float floor_coll_height);
void ecs_entity_set_jump_force(struct ecs *ecs, const uint8_t id,
			       const float jump_force);

void ecs_entity_update_jump_cond(struct ecs *ecs, const uint8_t id,
				 const uint8_t jump_cond);
void ecs_update(struct ecs *ecs, const float dt);
/* TODO: There is currently no way to remove entities from the list */
void ecs_free(struct ecs *ecs);
