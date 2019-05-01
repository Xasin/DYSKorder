/*
 * core_graphics.cpp
 *
 *  Created on: 30 Apr 2019
 *      Author: xasin
 */

#include "core.h"

namespace DSKY {

using namespace Peripheral::OLED;

SSD1327		display = SSD1327();

auto borderDrawer = DrawBox(127, 127, &display);

StringPrimitive inputArea = StringPrimitive(128, 10, &display);

auto consoleArea = DrawBox(128, 128 - 3*8, &display);
LittleConsole console = LittleConsole(consoleArea);

namespace Graphics {

void setup() {

	borderDrawer.onRedraw = []() {
		borderDrawer.draw_line(0, 8, 127, 0);
		borderDrawer.draw_line(0, 116, 127, 0);

		borderDrawer.write_string(36, 0, "DSKorder");
	};

    inputArea.offsetY = 119;

    consoleArea.offsetY = 11;

    display.initialize();
}

}
}
