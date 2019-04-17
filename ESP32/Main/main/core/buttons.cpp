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
btn_restrict_t button_restrict = ALL;

TaskHandle_t btn_thread_handle = nullptr;

std::function<void (const btn_event_t)> on_event = nullptr;

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

void process_button(uint8_t btnNum) {
	char currentChar = buttonMap[btnNum][get_mode_pins()];

	printf("Button num %d is: %c (%s)\n",
			btnNum, currentChar,
			valid_char(currentChar) ? "OK" : "FALSE");

	btn_event_t outEvent = {};
	outEvent.btn_pressed_no = btnNum;
	outEvent.typed_char = currentChar;

	outEvent.typed = current_typing;

	switch(currentChar) {
	default:
		if(valid_char(currentChar)) {
			current_typing.push_back(currentChar);
			outEvent.typed = current_typing;

			Trek::play(Trek::KEYPRESS);
		}
		else
			Trek::play(Trek::INPUT_BAD);
	break;
	case 26: break;
	case 27:
		puts("ESCAPE'd");
		outEvent.escape = true;

		current_typing.clear();
		Trek::play(Trek::PROG_DONE);
	break;
	case 8:
		if(current_typing.length() > 0)
			current_typing.pop_back();

		outEvent.typed = current_typing;
		Trek::play(Trek::KEYPRESS);
	break;
	case '\n':
		printf("ENTER'd, you typed: %s\n", current_typing.data());
		outEvent.enter = true;

		Trek::play(Trek::PROG_BUSY);
		current_typing.clear();
	break;
	}

	if(on_event != nullptr)
		on_event(outEvent);
}

void btn_read_thread(void *_) {
	while(true) {
		uint16_t newButtons = segmentCTRL.get_buttons();
		uint16_t buttonDiff = newButtons ^ current_buttons;

		for(uint8_t i=0; i<16; i++) {
			if(((buttonDiff >> i) & 1) == 0)
				continue;

			uint8_t btnNum = btn_remap[i];

			if(((newButtons >> i) & 1) == 0) {
				btn_event_t outEvent = {};

				outEvent.btn_released_no = i+1;
				outEvent.mode_btns = get_mode_pins();
				continue;
			}

			process_button(btnNum);
		}

		current_buttons = newButtons;
		xTaskNotifyWait(0, 0, nullptr, 500);
	}
}

void setup() {
	gpio_isr_handler_add(PIN_BTN_IRQ, btn_isr_handler, nullptr);
	xTaskCreate(btn_read_thread, "BTN Updater", 2048, nullptr, 20, &btn_thread_handle);
}

}
}
