/*
 * buttons.cpp
 *
 *  Created on: 6 Apr 2019
 *      Author: xasin
 */


#include "buttons.h"
#include "core.h"
#include "pins.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <string>

using namespace Xasin;

namespace DSKY {
namespace BTN {

volatile unsigned int current_buttons = 0;
std::string current_typing = "";
btn_restrict_t button_restrict = HEX_ONLY;

TaskHandle_t btn_thread_handle = nullptr;

std::function<void (const std::string &input)> on_enter = nullptr;

void btn_isr_handler(void *_) {
	xTaskNotify(btn_thread_handle, 0, eNoAction);
}

uint8_t get_mode_pins() {
	uint8_t out = 0;

	for(uint8_t i=0; i<PINS_MODE.size(); i++)
		out |= !gpio_get_level(PINS_MODE[i])<<i;

	return out;
}
bool valid_char(char c) {
	switch(button_restrict) {
	case ALL: return true;

	case HEX_ONLY:
		if((c >= 'a') && (c <= 'f'))
			return true;
		if((c >= 'A') && (c <= 'F'))
			return true;
		if((c >= '0') && (c <= '9'))
			return true;
		return false;

	case NUM_ONLY:
		return ((c >= '0') && (c <= '9')) || (c == '.');
	}

	return false;
}

void btn_read_thread(void *_) {
	while(true) {
		xTaskNotifyWait(0, 0, nullptr, portMAX_DELAY);

		uint16_t newButtons = segmentCTRL.get_buttons();
		uint16_t buttonDiff = newButtons ^ current_buttons;

		for(uint8_t i=0; i<16; i++) {
			if(((buttonDiff >> i) & 1) == 0)
				continue;

			if(((newButtons >> i) & 1) != 0)
				continue;

			uint8_t btnNum = btn_remap[i];
			char currentChar = buttonMap[btnNum][get_mode_pins()];

			printf("Button num %d is: %c (%s)\n",
					btnNum, currentChar,
					valid_char(currentChar) ? "OK" : "FALSE");

			switch(currentChar) {
			default:
				if(valid_char(currentChar))
					current_typing.push_back(currentChar);
				else
					Trek::play(Trek::INPUT_BAD);
			break;
			case 26: break;
			case 27:
				puts("ESCAPE'd");
				current_typing.clear();
			break;
			case 8:
				current_typing.pop_back();
			break;
			case '\n':
				printf("ENTER'd, you typed: %s\n", current_typing.data());
				if(on_enter != nullptr)
					on_enter(current_typing);

				current_typing.clear();
			break;
			}
		}

		current_buttons = newButtons;
	}
}

void setup() {
	gpio_isr_handler_add(PIN_BTN_IRQ, btn_isr_handler, nullptr);
	xTaskCreate(btn_read_thread, "BTN Updater", 2048, nullptr, 20, &btn_thread_handle);
}

}
}
