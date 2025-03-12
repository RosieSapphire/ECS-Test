#pragma once

#include <stdint.h>
#include <stdbool.h>

enum {
	ENT_FLAG_IS_ACTIVE = (1 << 0),
	ENT_FLAG_COMP_POS = (1 << 1),
	ENT_FLAG_COMP_VEL = (1 << 2),
	ENT_FLAG_COMP_GRAV = (1 << 3),
	ENT_FLAG_COMP_FLOOR_COLL = (1 << 4),
	ENT_FLAG_COMP_JUMP = (1 << 5),
	ENT_FLAG_COMP_XCLAMP = (1 << 6),
	ENT_FLAG_COMP_MOVE = (1 << 7),
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
	float floor_height;
	float ent_height;
};

struct comp_jump {
	float force;
	bool cond;
};

struct comp_xclamp {
	float min;
	float max;
};

struct comp_move {
	float accel;
	float decel;
	float max_vel;
	int8_t dir;
};

struct ecs {
	uint32_t flags[ENTITY_MAX_COUNT];
	struct comp_position positions[ENTITY_MAX_COUNT];
	struct comp_velocity velocities[ENTITY_MAX_COUNT];
	struct comp_gravity gravities[ENTITY_MAX_COUNT];
	struct comp_floor_coll floor_colls[ENTITY_MAX_COUNT];
	struct comp_jump jumps[ENTITY_MAX_COUNT];
	struct comp_xclamp xclamps[ENTITY_MAX_COUNT];
	struct comp_move moves[ENTITY_MAX_COUNT];
	uint8_t count;
};

struct ecs ecs_init(void);

uint8_t ecs_entity_add(struct ecs *ecs, const uint32_t flags);
void ecs_entity_set_position(struct ecs *ecs, const uint8_t id,
			     const float pos[2]);
void ecs_entity_set_velocity(struct ecs *ecs, const uint8_t id,
			     const float vel[2]);
void ecs_entity_set_gravity(struct ecs *ecs, const uint8_t id,
			    const float gravity_force);
void ecs_entity_set_floor_coll(struct ecs *ecs, const uint8_t id,
			       const float floor_coll_height,
			       const float entity_height);
void ecs_entity_set_jump_force(struct ecs *ecs, const uint8_t id,
			       const float jump_force);
void ecs_entity_set_jump_condition(struct ecs *ecs, const uint8_t id,
				   const bool jump_cond);
void ecs_entity_set_xclamp(struct ecs *ecs, const uint8_t id, const float min,
			   const float max);
void ecs_entity_set_move(struct ecs *ecs, const uint8_t id, const float accel,
			 const float decel, const float max_vel,
			 const int8_t dir);
void ecs_entity_set_move_dir(struct ecs *ecs, const uint8_t id,
			     const int8_t dir);
void ecs_update(struct ecs *ecs, const float dt);
/* TODO: There is currently no way to remove entities from the list */
void ecs_free(struct ecs *ecs);
