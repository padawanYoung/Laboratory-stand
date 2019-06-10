/*
 * view_main_menu.c
 *
 *  Created on: 27 мая 2019 г.
 *      Author: Serhii
 */
#include "view_main_menu.h"

#include "lcd_hd44780_i2c.h"
#include "event.h"
#include "menu.h"
#include "string.h"


void show_title (char * title) {
	char row0 [16];
	char row1 [16];

	sscanf(title, "%s %s", row0, row1);
	uint8_t col0 = (16-strlen(row0))/2;
	uint8_t col1 = (16-strlen(row1))/2;

	lcdSetCursorPosition(col0,0);
	lcdPrintStr((uint8_t*) row0, strlen(row0));

	if (strlen(row1) != 0) {
		lcdSetCursorPosition(col1, 1);
		lcdPrintStr((uint8_t*) row1, strlen(row1));
	}

}

void vm_generator_cb (){
	lcdDisplayClear();
	show_title ("GENERATOR");

}

void vm_frequency_meter_cb (){
	lcdDisplayClear();
	show_title ("FREQUENCY METER");
}

void vm_FSK_cb (){
	lcdDisplayClear();
	show_title ("FSK MODULATION");
}

void vm_PSK_cb (){
	lcdDisplayClear();
	show_title ("PSK MODULATION");
}
