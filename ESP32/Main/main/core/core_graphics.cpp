/*
 * core_graphics.cpp
 *
 *  Created on: 30 Apr 2019
 *      Author: xasin
 */

#include "core.h"
#include "segments.h"

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

void tick() {
	auto &connBulb = Seg::bulbs[12];

	switch(mqtt.is_disconnected()) {
	default:
		connBulb.mode = Seg::IDLE;
		connBulb.target = Material::GREEN;
	break;

	case 255:
		connBulb.mode = Seg::OFF;
	break;

	case 1:
		connBulb.mode = Seg::HFLASH;
		connBulb.target = Material::AMBER;
	break;

	case 2:
		connBulb.mode = Seg::HFLASH;
		connBulb.target = Material::RED;
	break;
	}
}

}
}
