/*
 * AD9834.h
 *
 *  Created on: 6 мая 2019 г.
 *      Author: Serhii
 */

#ifndef AD9834_H_
#define AD9834_H_

#include "stm32f4xx_hal.h"

#define AD9834_FREQUENCY_MAX	((uint32_t)37500000) //Hz
#define AD9834_FREQUENCY_MIN    ((uint8_t)0)
#define AD9834_AMPLITUDE_MAX	((uint8_t)3)		 //V
#define AD9834_AMPLITUDE_MIN	((uint8_t)0)

typedef enum {
	AD9834_REG0,
	AD9834_REG1
}AD9834_register_t;

typedef enum {
	empty = 0,
	prefix_CI_unit_kilo = 3,
	prefix_CI_unit_mega = 6,
	prefix_CI_unit_mili = -3
}prefix_CI_unit_t;

typedef enum {
	AD9834_SINE,
	AD9834_TRIANGLE
}AD9834_form_t;

typedef enum{
	AD9834_GET,
	AD9834_SET
}AD9834_property_t;

typedef enum{
	AD9834_FREQUENCY,
	AD9834_PHASE,
	AD9834_AMPLITUDE,
	AD9834_FORM
}AD9834_parameter_t;

typedef struct parameter{
	float value;
	prefix_CI_unit_t  prefix;
}parameter_t;

typedef struct {
	parameter_t FREQ0;			//	Hz
	parameter_t FREQ1;			//	Hz
	float PHASE0;				//deg
	float PHASE1;				//deg
	parameter_t amplitude;		//	mV
	AD9834_form_t form;			// signal form
}AD9834_t;

void AD9834_init ();

//	SET FUNCTIONS BEGGIN
void AD9834_frequency_prefix_set(AD9834_register_t reg, prefix_CI_unit_t prefix);

void AD9834_amplitude_prefix_set (prefix_CI_unit_t prefix);

void AD9834_frequency_set(AD9834_register_t reg, float value);

void AD9834_amplitude_set (float value);

void AD9834_phase_set(AD9834_register_t reg, float value);

void AD9834_form_set(AD9834_form_t form);
//	SET FUNCTIONS END
//////////////////////////////////////////////////////////////////////////////////////////
//	GET FUNCTIONS BEGGIN
prefix_CI_unit_t AD9834_frequency_prefix_get (AD9834_register_t reg);

prefix_CI_unit_t AD9834_amplitude_prefix_get ();

float AD9834_amplitude_get ();

float AD9834_frequency_get(AD9834_register_t reg);

float AD9834_phase_get(AD9834_register_t reg);

AD9834_form_t AD9834_form_get();
//	GET FUNCTIONS END

#endif /* AD9834_H_ */
