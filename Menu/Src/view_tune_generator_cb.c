#include "view_tune_generator_cb.h"

#include "AD9834.h"
#include "lcd_hd44780_i2c.h"
#include "menu.h"
#include "event.h"
#include "rendering.h"

#include <string.h>
#include <math.h>
#include <stdbool.h>

#define CHAR_EMPTY				((uint8_t)0x20)			//make field empty
#define CHAR_POINTER			((uint8_t)0x3e)			//pointer for menu selection

typedef enum
{
	opt_form = 0,
	opt_amplitude_val,
	opt_amplitude_pref,
	opt_frequency_val,
	opt_frequency_pref
} option_t;

typedef enum
{
	voc_order_low = 10,
	voc_order_skip = 9,
	voc_order_amplitude_hight = 5,
	voc_order_frequency_hight = 1
} value_order_column_t;

typedef enum
{
	vor_amplitude,
	var_frequency
}value_order_row_t;

uint8_t generator_select_option[5][2] =
{
	{ 0, 0 }, /*tune signal form*/
	{ 0, 0 }, /*tune amplitude value*/
	{ 0, 12 }, /*tune amplitude prefix*/
	{ 1, 0 }, /*tune frequency value*/
	{ 1, 12 } /*tune frequency prefix*/
};

prefix_CI_unit_t frequency_prefix[] =
{ empty, prefix_CI_unit_kilo, prefix_CI_unit_mega };

prefix_CI_unit_t amplitude_prefix[] =
{ empty, prefix_CI_unit_mili };

option_t option_current = opt_form;

uint8_t current_frequency_prefix = 0;
uint8_t current_amplitude_prefix = 0;

uint8_t frequency_val_length = 0;
uint8_t amplitude_val_length = 0;

int8_t value_order = -1;			// setting order to 10^-1
uint8_t value_order_column = voc_order_low;
uint8_t value_order_row = vor_amplitude;

parameter_t TEMP_FREQ;
parameter_t TEMP_AMPLITUDE;
AD9834_form_t TEMP_FORM;

char prefix_get_symbol(prefix_CI_unit_t prefix)
{
	switch (prefix)
	{
	case prefix_CI_unit_mili:
		return 'm';
		break;
	case prefix_CI_unit_kilo:
		return 'k';
		break;
	case prefix_CI_unit_mega:
		return 'M';
		break;
	default:
		return CHAR_EMPTY;
		break;
	}
}

char signal_form_get_symbol()
{
	switch (TEMP_FORM)
	{
	case AD9834_SINE:
		return 'S';
		break;
	case AD9834_TRIANGLE:
		return 'T';
		break;
	default:
		return 0;
		break;
	}

}

uint8_t parameter_show(char* FORMAT, parameter_t param)
{
	char buffer[16];
	char value_buffer [11];
	char prefix_char = prefix_get_symbol(param.prefix);
	float value = param.value;
	value /= pow(10, param.prefix);

	uint32_t whole = (uint32_t) value;
	float temp_fract = value - whole;
	uint32_t fract = (uint32_t) (temp_fract * 10);

	snprintf(value_buffer, sizeof(value_buffer), "%d.%d", (int) whole, (int) fract);
	snprintf(buffer, sizeof(buffer), FORMAT, value_buffer, prefix_char);
	lcdPrintStr((uint8_t*) buffer, strlen(buffer));

	return strlen(value_buffer);
}

void cursor_hide()
{
	uint8_t col_correction = 0;
	for (option_t i = 0; i < 5; i++)
	{
		switch (i) {
				case opt_amplitude_val:
					col_correction = voc_order_low - amplitude_val_length;
					break;
				case opt_frequency_val:
					col_correction = voc_order_low - frequency_val_length;
					break;
				default:
					col_correction = 0;
					break;
			}
		lcdSetCursorPosition((col_correction + generator_select_option[i][1]), generator_select_option[i][0]);
		lcdPrintChar(CHAR_EMPTY);
	}
}

void cursor_show()
{
	uint8_t col_correction = 0;
	switch (option_current) {
		case opt_amplitude_val:
			col_correction = voc_order_low - amplitude_val_length;
			break;
		case opt_frequency_val:
			col_correction = voc_order_low - frequency_val_length;
			break;
		default:

			break;
	}
	lcdSetCursorPosition((col_correction + generator_select_option[option_current][1]), generator_select_option[option_current][0]);
		lcdPrintChar(CHAR_POINTER);
}

void vtg_init()
{
	lcdDisplayClear();
	lcdCursorOff();

	TEMP_FORM = AD9834_form_get();
	TEMP_AMPLITUDE.value = AD9834_amplitude_get();
	TEMP_AMPLITUDE.prefix = AD9834_amplitude_prefix_get();
	TEMP_FREQ.value = AD9834_frequency_get(AD9834_REG0);
	TEMP_FREQ.prefix = AD9834_frequency_prefix_get(AD9834_REG0);

	lcdSetCursorPosition(1, 0);
	lcdPrintChar((uint8_t) signal_form_get_symbol());

	lcdSetCursorPosition(5, 0);
	amplitude_val_length = parameter_show("%6s%3cV", TEMP_AMPLITUDE);

	lcdSetCursorPosition(1, 1);
	frequency_val_length = parameter_show("%10s%3cHz", TEMP_FREQ);

	cursor_show();
}

void form_changing()
{
	event_t event = event_get();
	if ((event == event_enc_rotation_left) || (event == event_enc_rotation_right))
	{
		TEMP_FORM = (TEMP_FORM++ == 1) ? 0 : TEMP_FORM;
	}
	lcdSetCursorPosition(1, 0);
	lcdPrintChar((uint8_t) signal_form_get_symbol());
}

void frequency_prefix_changing()
{
	event_t event = event_get();
	if (event == event_enc_rotation_left)
	{
		current_frequency_prefix = (current_frequency_prefix--) == 0 ? sizeof(frequency_prefix)-1 : current_frequency_prefix;
	} else if (event == event_enc_rotation_right)
	{
		current_frequency_prefix = (current_frequency_prefix++) == (sizeof(frequency_prefix)-1) ? 0 : current_frequency_prefix;
	}

	lcdSetCursorPosition(1, 1);
	TEMP_FREQ.prefix = frequency_prefix[current_frequency_prefix];
	frequency_val_length = parameter_show("%10s%3cHz", TEMP_FREQ);
}

void amplitude_prefix_changing()
{
	event_t event = event_get();
	if ((event == event_enc_rotation_left) || (event == event_enc_rotation_right))
	{
		current_amplitude_prefix = (current_amplitude_prefix++ == (sizeof(amplitude_prefix) - 1)) ?
						0 : current_amplitude_prefix;
		TEMP_AMPLITUDE.prefix = amplitude_prefix[current_amplitude_prefix];
	}

	lcdSetCursorPosition(5, 0);
	amplitude_val_length = parameter_show("%6s%3cV", TEMP_AMPLITUDE);
}

void val_changing (AD9834_parameter_t param, parameter_t * TEMP)
{
	uint32_t limit_max = 0, limit_min = 0;
	float step = 0;
	event_t event = event_get();

	switch (param)
	{
	case AD9834_FREQUENCY:
		limit_max = AD9834_FREQUENCY_MAX;
		limit_min = AD9834_FREQUENCY_MIN;
		break;
	case AD9834_AMPLITUDE:
		limit_max = AD9834_AMPLITUDE_MAX;
		limit_min = AD9834_AMPLITUDE_MIN;
		break;
	default:
		break;
	}

	switch (event)
	{
	case event_enc_rotation_left:
		step = (float) pow(10, value_order) * (float) pow(10, TEMP->prefix);
		TEMP->value += step;
		TEMP->value = (TEMP->value > limit_max) ? limit_max : TEMP->value;
		break;
	case event_enc_rotation_right:
		step = (float) pow(10, value_order) * (float) pow(10, TEMP->prefix);
		TEMP->value -= step;
		TEMP->value = (TEMP->value < limit_min) ? limit_min : TEMP->value;
		break;
	default:
		break;
	}

}

void vtg_parameter_change()
{
	switch (option_current)
	{
	case opt_form:
	{
		form_changing();
		break;
	}
	case opt_amplitude_val:
	{
		val_changing(AD9834_AMPLITUDE,&TEMP_AMPLITUDE);
		lcdSetCursorPosition(5, 0);
		amplitude_val_length = parameter_show("%6s%3cV", TEMP_AMPLITUDE);

		break;
	}
	case opt_amplitude_pref:
	{
		amplitude_prefix_changing();
		break;
	}
	case opt_frequency_val:
	{
		val_changing(AD9834_FREQUENCY, &TEMP_FREQ);
		lcdSetCursorPosition(1, 1);
		frequency_val_length = parameter_show("%10s%3cHz", TEMP_FREQ);
		break;
	}
	case opt_frequency_pref:
	{
		frequency_prefix_changing();
		break;
	}
	default:
	{
		break;
	}
	}
}

void vtg_parameter_cancel()
{
	rendering_parent();
}

void vtg_parameter_apply()
{
	switch (option_current)
	{
	case opt_form:
		AD9834_form_set(TEMP_FORM);
		break;
	case opt_amplitude_val:
		AD9834_amplitude_set(TEMP_AMPLITUDE.value);
		break;
	case opt_amplitude_pref:
		AD9834_amplitude_prefix_set(TEMP_AMPLITUDE.prefix);
		break;
	case opt_frequency_val:
		AD9834_frequency_set(AD9834_REG0, TEMP_FREQ.value);
		break;
	case opt_frequency_pref:
		AD9834_frequency_prefix_set(AD9834_REG0, TEMP_FREQ.prefix);
		break;
	default:
		break;
	}
	rendering_parent();
}

void vtg_select_param()
{
	cursor_hide();
	switch (event_get())
	{
	case event_enc_rotation_right:
		option_current = ((option_current++) == 4 ? 0 : option_current);
		break;
	case event_enc_rotation_left:
		option_current = ((option_current--) == 0 ? 4 : option_current);
		break;
	default:
		break;
	}

	cursor_show();
}

void vtg_select_order()
{
	cursor_hide();

	if ((option_current == opt_form) || (option_current == opt_amplitude_pref) || (option_current == opt_frequency_pref))
	{
		rendering_next();
		return;
	} else if (option_current == opt_amplitude_val)
	{
		value_order_row = vor_amplitude;
	} else if (option_current == opt_frequency_val)
	{
		value_order_row = var_frequency;
	}
	value_order_column = voc_order_low;
	value_order = -1;		// setting order to 10^-1
	lcdSetCursorPosition(value_order_column, value_order_row);
	lcdCursorOn();
}

void  order_changing (uint8_t col_order_low, uint8_t col_order_hight)
{
	event_t event = event_get();

	switch (event)
	{
	case event_enc_rotation_left:
		value_order_column--;
		if (value_order_column < col_order_hight)
		{
			value_order_column = col_order_low;
			value_order = -1;				// setting order to 1^-1
		} else if (value_order_column == voc_order_skip)
		{
			value_order_column--;
			value_order = 0;

		} else if (value_order_column >= col_order_hight)
		{
			value_order++;
		}
		break;
	case event_enc_rotation_right:
		value_order_column++;
		if (value_order_column > col_order_low)
		{
			value_order_column = col_order_hight;
			value_order = (col_order_low - col_order_hight) - 2;	// given that value has 10^-1 order
		} else if (value_order_column == voc_order_skip)
		{
			value_order_column++;
			value_order = 0;
		} else if (value_order_column <= col_order_low)
		{
			value_order--;
		}
		break;
	default:
		break;
	}
		lcdSetCursorPosition(value_order_column, value_order_row);

}

void vtg_order_change()
{
	switch (option_current)
	{
	case opt_amplitude_val:
		order_changing (voc_order_low, (voc_order_low-amplitude_val_length)+1);
		break;
	case opt_frequency_val:
		order_changing (voc_order_low, (voc_order_low-frequency_val_length)+1);
		break;
	default:
		break;
	}

}
