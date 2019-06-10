/*
 * view_tune_generator_cb.h
 *
 *  Created on: 27 мая 2019 г.
 *      Author: Serhii
 */

#ifndef VIEW_TUNE_GENERATOR_CB_H_
#define VIEW_TUNE_GENERATOR_CB_H_

void vtg_init ();

void vtg_select_param();

void vtg_select_order();
void vtg_order_change();

void vtg_parameter_change();
void vtg_parameter_cancel();
void vtg_parameter_apply();

#endif /* VIEW_TUNE_GENERATOR_CB_H_ */
