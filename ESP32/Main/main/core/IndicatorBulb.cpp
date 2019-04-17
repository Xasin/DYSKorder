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
		target(), mode(OFF), flash_fill(4) {
}

Peripheral::Color IndicatorBulb::tick() {

	auto bufferedTarget = target;
	bool onBuffer = false;

	switch(mode) {
	case OFF: current = Peripheral::Color(); break;

	case IDLE:
		current.merge_overlay(
				bufferedTarget.bMod((get_flashcycle_count()&1) ? 87 : 100)
				, 14);
	break;

	case HFLASH:
		onBuffer = ((get_flashcycle_ticks()&0b1111) < flash_fill*2);

		current.merge_overlay(
				bufferedTarget.bMod(onBuffer ? 150 : 100)
				, onBuffer ? 110 : 85);
	break;

	case FLASH:
		onBuffer = ((get_flashcycle_ticks()&0b111) < flash_fill);
		current.merge_overlay(
				bufferedTarget.bMod(onBuffer ? 180 : 0)
				, onBuffer ? 120 : 90);
	break;

	case DFLASH:
		onBuffer = ((get_flashcycle_ticks()&0b11) < flash_fill/2);
		current.merge_overlay(
				bufferedTarget.bMod(onBuffer ? 255 : 0)
				, onBuffer ? 180 : 130);
	break;

	case VAL_RISING:

		if((get_flashcycle_ticks()&0b111) == 0)
			current.merge_overlay(bufferedTarget.bMod(60), 100);
		else
			current.merge_overlay(bufferedTarget.bMod(130), 10);
	break;

	case VAL_FALLING:
		if((get_flashcycle_ticks()&0b111) == 0)
			current.merge_overlay(bufferedTarget.bMod(130), 100);
		else
			current.merge_overlay(bufferedTarget.bMod(60), 10);
	break;
	}


	return current;
}

} /* namespace Seg */
} /* namespace DSKY */
