/*
 * DisplayParam.cpp
 *
 *  Created on: 9 Apr 2019
 *      Author: xasin
 */

#include "DisplayParam.h"

#include "freertos/FreeRTOS.h"

namespace DSKY {
namespace Seg {

DisplayParam::DisplayParam() {
	clear();
}

void DisplayParam::clear() {
	value =0;
	fixComma = -1;
	param_type = NONE;
	blink = false;
	blinkInv = false;
}

uint32_t DisplayParam::get_signal_code(const uint8_t *signal, uint8_t len) {
	uint32_t outBuffer = 0;

	for(uint8_t i=0; i<len; i++)
		outBuffer |= char_templates[signal[i]] << (8*(3-i));

	return outBuffer;
}

uint32_t DisplayParam::get_current_display() {
	if(blink)
		if((0==(xTaskGetTickCount()/40 & 0b11)) ^ blinkInv)
			return 0;

	uint32_t outBuffer = 0;

	switch(param_type) {
	case NONE:
		return 0;
	case IDLE:
		outBuffer = get_signal_code(signal_idle, 4);
		outBuffer |= 1<<(31 - 8*(xTaskGetTickCount()/80 & 0b11));
		return outBuffer;
	case LOADING:
		outBuffer = get_signal_code(signal_load, 4);
		for(uint8_t i=0; i<(xTaskGetTickCount()/40 & 0b11); i++)
			outBuffer |= 1<<(31 - 8*i);
		return outBuffer;
	case RUNNING:
		outBuffer = get_signal_code(signal_run, 3);
		outBuffer |= 1<<(xTaskGetTickCount()/40 % 6);
		return outBuffer;
	case DONE:
		outBuffer = get_signal_code(signal_done, 4);
		if(xTaskGetTickCount()/80 & 1) {
			for(uint8_t i=7; i<32; i+=8)
				outBuffer |= 1<<i;
		}
		return outBuffer;
	case ERROR:
		if(xTaskGetTickCount()/50 & 1)
			outBuffer = get_signal_code(signal_err, 3);
		else
			outBuffer = get_numeric_code(value); // FIXME
		return outBuffer;
	case INT:

	}

	return 0;
}

} /* namespace Seg */
} /* namespace DSKY */
