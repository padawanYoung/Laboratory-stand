/*
 * node.c
 *
 *  Created on: 7 мая 2019 г.
 *      Author: Serhii
 */

#include <node.h>
#include "event.h"
#include <stdbool.h>

node_t * node_ptr;

node_t* node_get_ptr()
{
	return node_ptr;
}

void node_set_ptr(node_t* ptr)
{
	node_ptr = ptr;
}

bool node_has_child()
{
	return (node_ptr->child != NULL);
}

bool node_has_parent()
{
	return (node_ptr->parent != NULL);
}

bool node_has_next()
{
	return (node_ptr->next != NULL);
}

bool node_has_prev()
{
	return (node_ptr->prev != NULL);
}

void node_get_child()
{
	node_ptr = node_has_child(node_ptr) ? node_ptr->child : node_ptr;
}

void node_get_parent()
{
	node_ptr = node_has_parent(node_ptr) ? node_ptr->parent : node_ptr;
}

void node_get_next()
{
	node_ptr = node_has_next(node_ptr) ? node_ptr->next : node_ptr;
}

void node_get_prev()
{
	node_ptr = node_has_prev(node_ptr) ? node_ptr->prev : node_ptr;
}

void node_execute(event_t event)
{

	switch (event)
	{
	case event_enc_rotation_left:
		if (node_ptr->enc_rot_left_cb != NULL)
		{
			(node_ptr->enc_rot_left_cb)();
		}
		break;
	case event_enc_rotation_right:
		if (node_ptr->enc_rot_right_cb != NULL)
		{
			(node_ptr->enc_rot_right_cb)();
		}
		break;
	case event_enc_push_quick:
		if (node_ptr->enc_push_quick_cb != NULL)
		{
			(node_ptr->enc_push_quick_cb)();
		}
		break;
	case event_enc_push_long:
		if (node_ptr->enc_push_long_cb != NULL)
		{
			(node_ptr->enc_push_long_cb)();
		}
		break;

	case event_rendering:
		if (node_ptr->rendering_cb != NULL)
		{
			(node_ptr->rendering_cb)();
		}
		break;
	default:

		break;
	}
}

void node_init(node_t * node, callback_t rendering_cb,
		callback_t enc_push_long_cb, callback_t enc_push_quick_cb,
		callback_t enc_rot_right_cb, callback_t enc_rot_left_cb, node_t * next,
		node_t * prev, node_t * child, node_t * parent)
{

	node->rendering_cb = rendering_cb;
	node->enc_push_long_cb = enc_push_long_cb;
	node->enc_push_quick_cb = enc_push_quick_cb;
	node->enc_rot_right_cb = enc_rot_right_cb;
	node->enc_rot_left_cb = enc_rot_left_cb;
	node->next = next;
	node->prev = prev;
	node->child = child;
	node->parent = parent;
}

