#include "../fighter.h"
#include "../gen/assets/chars_marin/marin.json.h"

#define STATE_IDLE 0
#define STATE_WALK 1
#define STATE_RUN  2
#define STATE_JUMP 3
#define STATE_DIE 4
#define STATE_ATTACK1 5
#define STATE_ATTACK2 6
#define STATE_AIRATTACK 7
#define STATE_BLOCK 8

static const char anim_offsets[] = {
	MARIN_TAG_IDLE_START,
	MARIN_TAG_WALK_START,
	MARIN_TAG_RUN_START,
	MARIN_TAG_JUMP_START,
	MARIN_TAG_DEATH_FROM_RIGHT_START,
	MARIN_TAG_SLASH1_START,
	MARIN_TAG_SLASH2_START,
	MARIN_TAG_JUMP_SLASH_START,
	MARIN_TAG_GUARD_START
};

static const char anim_lengths[] = {
	MARIN_TAG_IDLE_LENGTH,
	MARIN_TAG_WALK_LENGTH,
	MARIN_TAG_RUN_LENGTH,
	MARIN_TAG_JUMP_LENGTH,
	MARIN_TAG_DEATH_FROM_RIGHT_LENGTH,
	MARIN_TAG_SLASH1_LENGTH,
	MARIN_TAG_SLASH2_LENGTH,
	MARIN_TAG_JUMP_SLASH_LENGTH,
	MARIN_TAG_GUARD_LENGTH
};

//assuming nothing else happens go to this state
//when the animation ends
static const char next_state[] = {
	STATE_IDLE,
	STATE_WALK,
	STATE_RUN,
	STATE_JUMP,
	STATE_DIE,
	STATE_IDLE,
	STATE_IDLE,
	STATE_JUMP,
	STATE_BLOCK
};

static char get_anim_offset_marin() {
	return anim_offsets[temp_fighter.state];
}

static void next_frame_marin() {
	++temp_fighter.anim_frame;
	if(temp_fighter.anim_frame == anim_lengths[temp_fighter.state]) {
		temp_fighter.anim_frame = 0;
		temp_fighter.state = next_state[temp_fighter.state];
	}
}

void setup_character_marin(char player_idx) {
	get_fighter_anim_offset[player_idx] = get_anim_offset_marin;
	next_frame[player_idx] = next_frame_marin;
}
