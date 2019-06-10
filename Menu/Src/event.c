/*
 * event.c
 *
 *  Created on: 20 мая 2019 г.
 *      Author: Serhii
 */
#include "event.h"
#include "main.h"

#include "lcd_hd44780_i2c.h"

#include "stm32f4xx_hal.h"
#include "tim.h"
#include "gpio.h"
#include <stdbool.h>
#include <math.h>
#include <string.h>

#define BTN_HOLD_TIME 			1500U 		//msec
#define BTN_PUSH_TIME_MAX		250U		//msec
#define BTN_PUSH_TIME_MIN		100U		//msec

uint16_t pulse_width; // measured pulse width

int32_t encoder = 0;
int32_t temp = 0;
event_t event_handler = event_z;

event_t event_get()
{
	return event_handler;
}

void event_set(event_t event_new)
{
	event_handler = event_new;
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{

	if (htim->Instance == TIM1)
	{
		// =FALLING= EDGE DETECTED AT FIRST
		if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
		{
			// Get =FALLING= EDGE Capture value
			pulse_width = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);

			if ((pulse_width <= BTN_PUSH_TIME_MAX)
					&& (pulse_width >= BTN_PUSH_TIME_MIN))
			{
				event_handler = event_enc_push_quick;
			} else if (pulse_width <= BTN_HOLD_TIME)
			{
				event_handler = event_enc_push_long;
			}
		}
		// =RISING= EDGE DETECTED
		if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
		{
			__HAL_TIM_SET_COUNTER(&htim1, 0x0);
		}

	} else if (htim->Instance == TIM2)
	{
//		encoder = __HAL_TIM_GET_COUNTER(&htim2);

		if (__HAL_TIM_IS_TIM_COUNTING_DOWN(htim))
		{
			event_handler = event_enc_rotation_left;
			temp++;
		} else
		{
			event_handler = event_enc_rotation_right;
			temp--;
		}
	}

}

