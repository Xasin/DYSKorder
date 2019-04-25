/*
 * IndicatorBulb.h
 *
 *  Created on: 16 Apr 2019
 *      Author: xasin
 */

#ifndef MAIN_CORE_INDICATORBULB_H_
#define MAIN_CORE_INDICATORBULB_H_

#include "NeoController.h"

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
private:
	Peripheral::Color current;

public:
	Peripheral::Color target;

	bulb_mode_t mode;
	uint8_t flash_fill;

	IndicatorBulb();
	IndicatorBulb& operator=(const IndicatorBulb &other);

	Peripheral::Color tick();
	Peripheral::Color get_color();
};

} /* namespace Seg */
} /* namespace DSKY */

#endif /* MAIN_CORE_INDICATORBULB_H_ */
