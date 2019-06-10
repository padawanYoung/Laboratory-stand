/*
 * node.h
 *
 *  Created on: 7 мая 2019 г.
 *      Author: Serhii
 */

#ifndef NODE_H_
#define NODE_H_

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "event.h"

typedef void (*callback_t) ();

typedef struct node {
	callback_t rendering_cb;
	callback_t enc_push_long_cb;
	callback_t enc_push_quick_cb;
	callback_t enc_rot_right_cb;
	callback_t enc_rot_left_cb;
	struct node * child;
	struct node * parent;
	struct node * next;
	struct node * prev;
}node_t;

node_t* node_get_ptr ();

void node_set_ptr (node_t* node);

bool node_has_child ();

bool node_has_parent ();

bool node_has_next ();

bool node_has_prev ();

void node_get_child ();

void node_get_parent ();

void node_get_next ();

void node_get_prev ();

void node_execute (event_t event);

void node_init(node_t * node,
		callback_t rendering_cb,
		callback_t enc_push_long_cb,
		callback_t enc_push_quick_cb,
		callback_t enc_rot_right_cb,
		callback_t enc_rot_left_cb,
		node_t * next,
		node_t * prev,
		node_t * child,
		node_t * parent);

#endif /* NODE_H_ */
