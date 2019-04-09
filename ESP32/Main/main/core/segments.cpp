/*
 * segments.c
 *
 *  Created on: 8 Apr 2019
 *      Author: xasin
 */

#include "segments.h"
#include "core.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <array>
#include <cstring>
#include <cmath>

namespace DSKY {
namespace Seg {

std::array<uint8_t, 8> current_segments = {};

segment_modes_t segment_mode = LOADING;
float seg_a_value = false;
bool  seg_a_blink = false;
float seg_b_value = false;
bool  seg_b_blink = false;

TaskHandle_t update_handle = nullptr;

void update_segments() {
	uint8_t rawSegments[8] = {};

	for(uint8_t seg=0; seg<8; seg++)
		for(uint8_t i=0; i<8; i++)
			rawSegments[i] |= ((current_segments[seg]>>i) & 1) << ((seg+7) & 0b111);

	for(uint8_t i=0; i<8; i++)
		segmentCTRL.set_segment(i, rawSegments[i]);

	segmentCTRL.update_segments();
}

void write_signal(uint8_t dNum, const uint8_t *start, uint8_t len) {
	for(uint8_t i=0; i<len; i++)
		current_segments[4*dNum + 3 - i] = char_templates[start[i]];
}

void write_number(uint8_t dNum, int16_t number, uint8_t base = 10, bool all=false) {
	uint16_t printedNum = abs(number);

	for(uint8_t i=0; i<4; i++) {
		current_segments[i + dNum*4] = char_templates[printedNum % base];
		printedNum /= base;
		if(printedNum == 0 && !all)
			break;
	}
	if(number < 0)
		current_segments[3+dNum*4] = 1<<6;
}

void write_float(uint8_t dNum, float data) {
	uint8_t dotPos = 0;
	float printedValue = fabs(data);

	while(printedValue < 1000) {
		printedValue *= 10;
		dotPos++;

		if(dotPos >= 3)
			break;
	}

	if((data < 0)) {
		current_segments[dNum*4 + 3] = 1<<6;
		if(dotPos > 0) {
			dotPos--;
			printedValue /= 10;
		}
	}

	write_number(dNum, round(printedValue), 10, true);
	if(dotPos > 0)
		current_segments[dotPos + dNum*4] |= 1<<7;
}

void update_task(void *_) {
	while(true) {
		bool flash_cycle = (xTaskGetTickCount() & 63) < 20;

		switch(segment_mode) {
		case IDLE:
			write_signal(0, signal_idle, 4);
			current_segments[3 - (xTaskGetTickCount()/80 & 0b11)] |= 1<<7;
		break;

		case LOADING:
			write_signal(0, signal_load, 4);
			for(uint8_t i=0; i<(xTaskGetTickCount()/40 & 0b11); i++)
				current_segments[3-i] |= 1<<7;
			write_number(1, seg_a_value);
		break;

		case FLOATS:
			current_segments.fill(0);
			if(!seg_a_blink || !flash_cycle)
				write_float(0, seg_a_value);
			if(!seg_b_blink || !flash_cycle)
				write_float(1, seg_b_value);
		break;

		default: break;
		}

		update_segments();
		vTaskDelay(5);
		xTaskNotifyWait(0, 0, nullptr, 15);
	}
}

void setup() {
    segmentCTRL.send_self_addressing();
    segmentCTRL.init();

	xTaskCreate(update_task, "DSKY:Segments", 2048, nullptr, 20, &update_handle);
}

void update() {
	xTaskNotify(update_handle, 0, eNoAction);
}

}
}
