#include "./types.h"
#include "gt/gfx/sprites.h" 
#define MAX_CHARACTERS 2

#define FIGHTER_GRAVITY 80
#define FIGHTER_JUMP_STRENGTH -1000

#define FIGHTER_FLAG_GROUNDED 1
#define FIGHTER_FLAG_MOVEMENT_LOCK 2

//Using array of structs here instead of struct of arrays
//They'll be copied into a temp memory location before being processed
//and then back to the array
typedef struct fighter {
	fixedp x;
	fixedp y;
	fixedp vx;
	fixedp vy;
	char health;
	char super_meter;
	char anim_frame;
	char anim_subframe;
	char state;
	char flags;
	SpriteSlot sprite;
	char flip;
} fighter_t;
//whee exactly 16 bytes for easy array indexing

extern fighter_t temp_fighter;

extern CharVoidFunction get_fighter_anim_offset[MAX_CHARACTERS];
extern VoidFunction next_frame[MAX_CHARACTERS];

void init_fighter(char player_idx, char character_idx);
void update_fighters();
void draw_fighters();
