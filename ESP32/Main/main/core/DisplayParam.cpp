/*
 * DisplayParam.cpp
 *
 *  Created on: 9 Apr 2019
 *      Author: xasin
 */

#include "DisplayParam.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <cmath>

namespace DSKY {
namespace Seg {

DisplayParam::DisplayParam() {
	clear();
}

void DisplayParam::clear() {
	value =0;
	fixComma = -1;
	param_type = NONE;

	error = false;

	blink = false;
	blinkInv = false;
}

uint32_t DisplayParam::get_numeric_code(int number, uint8_t base, uint8_t minDigits) {
	uint32_t outBuffer = 0;

	int printedNumber = abs(number);

	uint8_t digitPos = 0;
	for(digitPos=0; digitPos<4; digitPos++) {
		outBuffer |= char_templates[printedNumber % base] << (8*digitPos);
		printedNumber /= base;

		if((printedNumber == 0) && (digitPos>=minDigits)) {
			digitPos += 2;
			break;
		}
	}
	digitPos--;

	if(number < 0) {
		outBuffer &= ~(0xFF<<(digitPos*8));
		outBuffer |= 1<<(6 + digitPos*8);
	}

	return outBuffer;
}

uint32_t DisplayParam::get_signal_code(const uint8_t *signal, uint8_t len) {
	uint32_t outBuffer = 0;

	for(uint8_t i=0; i<len; i++)
		outBuffer |= char_templates[signal[i]] << (8*(3-i));

	return outBuffer;
}

uint32_t DisplayParam::get_current_display() {
	if(error) {
		if(xTaskGetTickCount()/50 & 1)
			return get_signal_code(signal_err, 3);
	}
	else if(blink)
		if((0==(xTaskGetTickCount()/25 & 3)) ^ blinkInv)
			return 0;

	uint32_t outBuffer = 0;

	switch(param_type) {
	case NONE:
		return 0;
	case IDLE:
		outBuffer = get_signal_code(signal_idle, 4);
		outBuffer |= 1<<(31 - 8*(xTaskGetTickCount()/100 & 0b11));
		return outBuffer;
	case LOADING:
		outBuffer = get_signal_code(signal_load, 4);
		for(uint8_t i=0; i<=(xTaskGetTickCount()/50 & 0b11); i++)
			outBuffer |= 1<<(31 - 8*i);
		return outBuffer;
	case RUNNING:
		outBuffer = get_signal_code(signal_run, 3);
		outBuffer |= 1<<(xTaskGetTickCount()/50 % 6);
		return outBuffer;
	case DONE:
		outBuffer = get_signal_code(signal_done, 4);
		if(xTaskGetTickCount()/100 & 1) {
			for(uint8_t i=7; i<32; i+=8)
				outBuffer |= 1<<i;
		}
		return outBuffer;

	case INT:
		if(fixComma > 0) {
			outBuffer = get_numeric_code(round(value * pow(10, fixComma)), 10, fixComma);
			outBuffer |= 1<<(7 + fixComma*8);

			return outBuffer;
		}
		else
			return get_numeric_code(value);
	break;

	case HEX:
		return get_numeric_code(value, 16);
	}

	return 0;
}

} /* namespace Seg */
} /* namespace DSKY */
