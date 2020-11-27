/*
 * IndicatorBulb.h
 *
 *  Created on: 16 Apr 2019
 *      Author: xasin
 */

#ifndef MAIN_CORE_INDICATORBULB_H_
#define MAIN_CORE_INDICATORBULB_H_

#include "xasin/NeoController.h"

namespace DSKY {
namespace Seg {

enum bulb_mode_t {
	OFF,
	IDLE,
	HFLASH,
	FLASH,
	DFLASH,
	VAL_RISING,
	VAL_FALLING,
};

class IndicatorBulb {
protected:
	Peripheral::Color current;
	TickType_t deactivateAfter;

public:
	Peripheral::Color target;

	bulb_mode_t mode;
	uint8_t flash_fill;

	IndicatorBulb();
	IndicatorBulb& operator=(const IndicatorBulb &other);

	void set(Peripheral::Color target, bulb_mode_t mode, uint8_t fill = 8, TickType_t deactivateTicks = 0);

	Peripheral::Color tick();
	Peripheral::Color get_color();

	void deactivate_after(TickType_t ticks);
};

} /* namespace Seg */
} /* namespace DSKY */

#endif /* MAIN_CORE_INDICATORBULB_H_ */
