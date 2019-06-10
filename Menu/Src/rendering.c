#include "rendering.h"

#include "lcd_hd44780_i2c.h"
#include "event.h"
#include "node.h"
#include "stdbool.h"


void rendering_next(){
	node_get_next();
	node_execute(event_rendering);
}

void rendering_prev(){
	node_get_prev();
	node_execute(event_rendering);
}

void rendering_child(){
	node_get_child();
	node_execute(event_rendering);
}

void rendering_parent(){
	node_get_parent();
	node_execute(event_rendering);
}
