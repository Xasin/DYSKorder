/*
 * buttons.h
 *
 *  Created on: 6 Apr 2019
 *      Author: xasin
 */

#ifndef MAIN_CORE_BUTTONS_H_
#define MAIN_CORE_BUTTONS_H_

#include <functional>
#include <string>

namespace DSKY {
namespace BTN  {

enum btn_restrict_t {
	ALL,
	NUM_ONLY,
	HEX_ONLY,
};

struct btn_event_t {
	bool enter;
	bool escape;
	std::string typed;
	char typed_char;
	uint8_t  btn_pressed_no;
	uint8_t  btn_released_no;
	uint8_t mode_btns;
};

extern volatile unsigned int current_buttons;
extern std::string current_typing;
extern btn_event_t last_btn_event;

extern btn_restrict_t button_restrict;

extern std::function<void (const btn_event_t)> on_event;

void setup();

const char btn_remap[] = {
	0, 0, 0, 5,
	4, 3, 2, 1,
	12, 11, 10, 9,
	8, 7, 6, 13,
};

#define BTN_NUMCHAR(num, capNum, lChar, rChar) {num, rChar, capNum, rChar - 'a' + 'A', lChar, 26, lChar + 'A' - 'a', 26}
const char buttonMap[][8] = {
	{27, 27, 27, 27, 27, 27, 27, 27},
	{'\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n'},
	BTN_NUMCHAR('1', '!', 'q', 't'),
	BTN_NUMCHAR('2', '"', 'w', 'z'),
	BTN_NUMCHAR('3', '+', 'e', 'u'),
	{8, 'i', 26, 'I', 'r', 'o', 'R', 'O'},
	BTN_NUMCHAR('4', '-', 'a', 'g'),
	BTN_NUMCHAR('5', '*', 's', 'h'),
	BTN_NUMCHAR('6', '/', 'd', 'j'),
	{'0', 'k', ' ', 'K', 'f', 'p', 'F', 'P'},
	BTN_NUMCHAR('7', '=', 'y', 'b'),
	BTN_NUMCHAR('8', '(', 'x', 'n'),
	BTN_NUMCHAR('9', ')', 'c', 'm'),
	{'.', ' ', ':', ',', 'v', 'l', 'V', 'L'}
};

}
}

#endif /* MAIN_CORE_BUTTONS_H_ */
