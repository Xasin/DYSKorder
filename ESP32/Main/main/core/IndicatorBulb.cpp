/*
 * IndicatorBulb.cpp
 *
 *  Created on: 16 Apr 2019
 *      Author: xasin
 */

#include "IndicatorBulb.h"
#include "core.h"

namespace DSKY {
namespace Seg {

IndicatorBulb::IndicatorBulb() : current(),
		target(), mode(OFF), flash_fill(125) {
}

Peripheral::Color IndicatorBulb::tick() {
	switch(mode) {
	case OFF: current = Peripheral::Color(); break;

	case IDLE:
		current.merge_overlay(
				Peripheral::Color(target).bMod(70 + (get_flashcycle_count()&1) ? 20 : 0)
				, 3);
	break;

	case FLASH:
		current.merge_overlay(
				Peripheral::Color(target).bMod(70 + (get_flashcycle_ticks()&0b111) < flash_fill ? 20 : 0)
				, 15);
	break;

	case DFLASH:
		current.merge_overlay(
				Peripheral::Color(target).bMod(70 + (get_flashcycle_ticks()&0b11) < flash_fill/2 ? 20 : 0)
				, 15);
	break;

	case VAL_RISING:
		current.merge_overlay(Peripheral::Color(target).bMod(90), 1);
		if((get_flashcycle_ticks()&0b111) == 0)
			current.merge_overlay(0, 20);
	break;

	case VAL_FALLING:
		current.merge_overlay(0, 1);
		if((get_flashcycle_ticks()&0b111) == 0)
			current.merge_overlay(Peripheral::Color(target).bMod(90), 20);
	break;
	}


	return current;
}

} /* namespace Seg */
} /* namespace DSKY */
