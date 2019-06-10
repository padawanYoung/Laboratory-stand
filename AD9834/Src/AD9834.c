/*
 * AD9834.c
 *
 *  Created on: 6 мая 2019 г.
 *      Author: Serhii
 */

#include "AD9834.h"

AD9834_t DDS;

void AD9834_init()
{

	AD9834_amplitude_set(3); 												//V
	AD9834_amplitude_prefix_set(prefix_CI_unit_mili);
	AD9834_frequency_set(AD9834_REG0, 138200); 								//Hz
	AD9834_frequency_prefix_set(AD9834_REG0, prefix_CI_unit_mega);
	AD9834_phase_set(AD9834_REG0, 0);										//degrees
	AD9834_form_set(AD9834_SINE);											// set the signa's form
}

void AD9834_frequency_prefix_set(AD9834_register_t reg, prefix_CI_unit_t prefix)
{
	if (reg == AD9834_REG0)
	{
		DDS.FREQ0.prefix = prefix;
	} else
	{
		DDS.FREQ1.prefix = prefix;
	}
}

void AD9834_amplitude_prefix_set(prefix_CI_unit_t prefix)
{
	DDS.amplitude.prefix = prefix;
}

void AD9834_amplitude_set(float value)
{
	DDS.amplitude.value = value;
}

void AD9834_frequency_set(AD9834_register_t reg, float value)
{
	if (value > AD9834_FREQUENCY_MAX)
	{
		value = AD9834_FREQUENCY_MAX;
	} else if (value < 0)
	{
		value = 0;
	}

	if (reg == AD9834_REG0)
	{
		DDS.FREQ0.value = value;
	} else
	{
		DDS.FREQ1.value = value;
	}
	// here is  will be written code, that sends command to AD9834 to write 28 bits into FREQ0 register
}

void AD9834_phase_set(AD9834_register_t reg, float value)
{
	if (reg == AD9834_REG0)
	{
		DDS.PHASE0 = value;
	} else
	{
		DDS.PHASE1 = value;
	}
	// here is  will be written code, that sends command to AD9834 to write 12 bits into PHASE0 register
}

void AD9834_form_set(AD9834_form_t form)
{
	DDS.form = form;
}

prefix_CI_unit_t AD9834_amplitude_prefix_get()
{
	return DDS.amplitude.prefix;
}

prefix_CI_unit_t AD9834_frequency_prefix_get(AD9834_register_t reg)
{
	if (reg == AD9834_REG0)
	{
		return DDS.FREQ0.prefix;
	} else
	{
		return DDS.FREQ1.prefix;
	}
}

float AD9834_amplitude_get()
{
	return DDS.amplitude.value;
}

float AD9834_frequency_get(AD9834_register_t reg)
{

	if (reg == AD9834_REG0)
	{
		return DDS.FREQ0.value;
	} else
	{
		return DDS.FREQ1.value;
	}
}

float AD9834_phase_get(AD9834_register_t reg)
{

	if (reg == AD9834_REG0)
	{
		return DDS.PHASE0;
	} else
	{
		return DDS.PHASE1;
	}
}

AD9834_form_t AD9834_form_get()
{
	return DDS.form;
}

