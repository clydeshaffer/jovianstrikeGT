#include "fighter.h"
#include "gt/gfx/draw_queue.h"
#include "gt/gfx/sprites.h"
#include "gt/input.h"

#include "gen/assets/chars_marin.h"

extern setup_character_marin(char player_idx);

fighter_t fighters[MAX_CHARACTERS];

fighter_t temp_fighter;
char prev_state;

int temp_buttons;
int temp_old_buttons;
int temp_new_buttons;

CharVoidFunction get_fighter_anim_offset[MAX_CHARACTERS];
VoidFunction next_frame[MAX_CHARACTERS];

void init_fighter(char player_idx, char character_idx) {
	if(player_idx >= MAX_CHARACTERS) return;

	temp_fighter.x.b.msb = player_idx ? 96 : 32;
	temp_fighter.y.b.msb = 96;
	
	temp_fighter.x.b.lsb = 128;
	temp_fighter.y.b.lsb = 0;

	temp_fighter.vx.u = 0;
	temp_fighter.vy.u = 0;

	temp_fighter.health = 48;

	temp_fighter.super_meter = 0;

	temp_fighter.anim_frame = 0;

	temp_fighter.anim_subframe = 0;

	temp_fighter.state = 0;
	temp_fighter.flags = FIGHTER_FLAG_GROUNDED;

	switch(character_idx) {
		case 0:
			temp_fighter.sprite = allocate_sprite(&ASSET__chars_marin__marin_bmp_load_list);
			set_sprite_frametable(temp_fighter.sprite, &ASSET__chars_marin__marin_json);
			setup_character_marin(player_idx);
			break;
	}
	temp_fighter.flip = player_idx ? SPRITE_FLIP_X : 0;

	fighters[player_idx] = temp_fighter;
}

void update_fighter(char fighter_idx) {
	temp_fighter = fighters[fighter_idx];
	prev_state = temp_fighter.state;

	if(fighter_idx) {
		temp_buttons = player2_buttons;
		temp_old_buttons = player2_old_buttons;
		temp_new_buttons = player2_new_buttons;
	} else {
		temp_buttons = player1_buttons;
		temp_old_buttons = player1_old_buttons;
		temp_new_buttons = player1_new_buttons;
	}

	++temp_fighter.anim_subframe;
	if(temp_fighter.anim_subframe == 7) {
		temp_fighter.anim_subframe = 0;
		//++temp_fighter.anim_frame;
		//if(temp_fighter.anim_frame == 8) {
		//		temp_fighter.anim_frame = 0;
		//}
		next_frame[fighter_idx]();
		if(temp_fighter.state != prev_state) {
			temp_fighter.flags &= ~FIGHTER_FLAG_MOVEMENT_LOCK;
		}
	}

	if(temp_fighter.flags & FIGHTER_FLAG_GROUNDED) {
		if(~temp_fighter.flags & FIGHTER_FLAG_MOVEMENT_LOCK) {
			if(temp_new_buttons & INPUT_MASK_UP) {
					temp_fighter.vy.i = FIGHTER_JUMP_STRENGTH;
					temp_fighter.flags &= ~FIGHTER_FLAG_GROUNDED;
					--temp_fighter.y.b.msb;
					temp_fighter.anim_frame = 0;
					temp_fighter.anim_subframe = 0;
					temp_fighter.state = 3; //STATE_JUMP
			} else if(temp_new_buttons & INPUT_MASK_A) {
				temp_fighter.state = 5; //STATE_ATTACK1
				temp_fighter.vx.i = 0;
				temp_fighter.anim_frame = 0;
				temp_fighter.anim_subframe = 0;
				temp_fighter.flags |= FIGHTER_FLAG_MOVEMENT_LOCK;
			}
			else if(temp_buttons & INPUT_MASK_RIGHT) {
				temp_fighter.state = 1;//STATE_WALK 
			}
			else if(temp_buttons & INPUT_MASK_LEFT) {
				temp_fighter.state = 1;//STATE_WALK
			} else {
				temp_fighter.vx.i = 0;
				temp_fighter.state = 0;//STATE_IDLE
			}
		}
	} else {
		temp_fighter.vy.i += FIGHTER_GRAVITY;
		if(~temp_fighter.flags & FIGHTER_FLAG_MOVEMENT_LOCK) {
			if(temp_new_buttons & INPUT_MASK_A) {
				temp_fighter.anim_frame = 0;
				temp_fighter.anim_subframe = 0;
				temp_fighter.state = 7; //STATE_AIRATTACK 
			}
		}
	}
	if(temp_buttons & INPUT_MASK_LEFT) {
		temp_fighter.vx.i = -256;
	}
	if(temp_buttons & INPUT_MASK_RIGHT) {
		temp_fighter.vx.i = 256;
	}
	
	if(~temp_fighter.flags & FIGHTER_FLAG_MOVEMENT_LOCK) {
		temp_fighter.x.i += temp_fighter.vx.i;
		temp_fighter.y.i += temp_fighter.vy.i;
	}

	if(temp_fighter.y.b.msb >= 96) {
			temp_fighter.y.b.msb = 96;
			temp_fighter.y.b.lsb = 0;
			temp_fighter.vy.i = 0;
			if(~temp_fighter.flags & FIGHTER_FLAG_GROUNDED) {
				temp_fighter.flags |= FIGHTER_FLAG_GROUNDED;
				if(temp_fighter.state == 7) { //STATE_AIRATTACK
					temp_fighter.state = 5; //STATE_ATTACK1
					temp_fighter.flags |= FIGHTER_FLAG_MOVEMENT_LOCK;
				} else {
					temp_fighter.state = 0; //STATE_IDLE
				}
			}
	}

	fighters[fighter_idx] = temp_fighter;
}

void update_fighters() {
	update_fighter(0);
	update_fighter(1);	
}

void draw_fighter(char fighter_idx) {
	temp_fighter = fighters[fighter_idx];

	queue_draw_sprite_frame(temp_fighter.sprite, temp_fighter.x.b.msb, temp_fighter.y.b.msb, (temp_fighter.anim_frame + get_fighter_anim_offset[fighter_idx]()), temp_fighter.flip);
}

void draw_fighters() {
	draw_fighter(0);
	draw_fighter(1);
}
