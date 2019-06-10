/*
 * event.h
 *
 *  Created on: 20 мая 2019 г.
 *      Author: Serhii
 */

#ifndef EVENT_H_
#define EVENT_H_

typedef enum {
	event_z,
	event_enc_rotation_left,
	event_enc_rotation_right,
	event_enc_push_quick,
	event_enc_push_long,
	event_rendering
}event_t;

event_t event_get();
void event_set(event_t event);

#endif /* EVENT_H_ */
