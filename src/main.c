#include "gt/gametank.h"
#include "gt/gfx/draw_queue.h"
#include "gt/input.h"
#include "fighter.h"

char box_x = 30, box_y = 20;
char dx = 1, dy = 1;

void main () {


    init_fighter(0, 0);
	init_fighter(1, 0);

    while (1) {                                     //  Run forever
        queue_clear_screen(3);
        
		
		update_fighters();

		draw_fighters();
		
		
		
		queue_clear_border(0);
        
 
        await_draw_queue();
        await_vsync(1);
        flip_pages();
		update_inputs();
 
    }
}
