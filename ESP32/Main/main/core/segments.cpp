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

TaskHandle_t update_handle = nullptr;

DisplayParam seg_a;
DisplayParam seg_b;

void set_line(uint8_t lNum, uint32_t data) {
	memcpy(current_segments.data()+lNum*4, &data, 4);
}

void update_segments() {
	uint8_t rawSegments[8] = {};

	for(uint8_t seg=0; seg<8; seg++)
		for(uint8_t i=0; i<8; i++)
			rawSegments[i] |= ((current_segments[seg]>>i) & 1) << ((seg+7) & 0b111);

	for(uint8_t i=0; i<8; i++)
		segmentCTRL.set_segment(i, rawSegments[i]);

	segmentCTRL.update_segments();
}

void update_task(void *_) {
	while(true) {
		set_line(0, seg_a.get_current_display());
		set_line(1, seg_b.get_current_display());

		update_segments();
		vTaskDelay(50);
		xTaskNotifyWait(0, 0, nullptr, 70);
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
