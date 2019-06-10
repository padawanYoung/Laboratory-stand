/*
 * menu.c
 *
 *  Created on: 3 мая 2019 г.
 *      Author: Serhii
 */
#include <menu.h>
#include <event.h>
#include <node.h>

#include "rendering.h"
#include "view_main_menu.h"
#include "view_tune_generator_cb.h"

#include "AD9834.h"
#include "lcd_hd44780_i2c.h"

#include "i2c.h"
#include "tim.h"

#include <string.h>

node_t list_generator;
node_t list_freq_meter;
node_t list_FSK;
node_t list_PSK;

node_t generator_init;
node_t generator_change_param;
node_t generator_select_order;

uint16_t find_slave()
{
	HAL_StatusTypeDef res;
	uint16_t addr = 0;
	for (uint16_t i = 0; i < 128; i++)
	{
		addr = i << 1;
		res = HAL_I2C_IsDeviceReady(&hi2c1, addr, 1, 10);
		if (res == HAL_OK)
		{
			return addr;
		}
	}
	return 0;
}

void menu_init()
{
	// Set starting point for menu
	node_set_ptr(&list_generator);

	node_init(&list_generator, vm_generator_cb,
	NULL, rendering_child, rendering_next, rendering_prev, &list_freq_meter,
			&list_PSK, &generator_init,
			NULL);

	node_init(&list_freq_meter, vm_frequency_meter_cb,
	NULL, rendering_child, rendering_next, rendering_prev, &list_FSK,
			&list_generator,
			NULL,
			NULL);

	node_init(&list_FSK, vm_FSK_cb,
	NULL, rendering_child, rendering_next, rendering_prev, &list_PSK,
			&list_freq_meter,
			NULL,
			NULL);

	node_init(&list_PSK, vm_PSK_cb,
	NULL, rendering_child, rendering_next, rendering_prev, &list_freq_meter,
			&list_FSK,
			NULL,
			NULL);

	node_init(&generator_init,
			vtg_init,
			rendering_parent,
			rendering_child, // make rendering without clearing display
			vtg_select_param,
			vtg_select_param,
			NULL,
			NULL,
			&generator_select_order,
			&list_generator);

	node_init(&generator_select_order,
			vtg_select_order,
			rendering_parent,
			rendering_next,
			vtg_order_change,
			vtg_order_change,
			&generator_change_param,
			NULL,
			NULL,
			&generator_init);

	node_init(&generator_change_param,
			NULL,
			vtg_parameter_cancel,
			vtg_parameter_apply,
			vtg_parameter_change,
			vtg_parameter_change,
			NULL,
			&generator_select_order,
			NULL,
			&generator_init);

	// LCD Init
	lcdInit(&hi2c1, (uint8_t) find_slave(), (uint8_t) 2, (uint8_t) 16);
	lcdDisplayClear();

	node_execute(event_rendering);
}

void menu_execute()
{
	event_t event = event_get();
	node_execute(event);
	event_set(event_z);
}

void menu_deInit()
{
	lcdDisplayClear();
	lcdDisplayOff();
//	HAL_TIM_Encoder_Stop(&htim2, TIM_CHANNEL_ALL);
}

