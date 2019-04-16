/*
 * core.h
 *
 *  Created on: 6 Apr 2019
 *      Author: xasin
 */

#ifndef MAIN_CORE_CORE_H_
#define MAIN_CORE_CORE_H_

#include "NeoController.h"

#include "AudioHandler.h"
#include "xasin/TrekAudio.h"

#include "MasterAction.h"
#include "xasin/AS1115.h"

#include "SSD1327.h"
#include "LittleConsole.h"

#include <functional>

namespace DSKY {

extern Xasin::Peripheral::AudioHandler audio;

extern Xasin::I2C::AS1115 			segmentCTRL;
extern Peripheral::NeoController	RGBCTRL;

extern Peripheral::OLED::SSD1327	display;
extern Peripheral::OLED::LittleConsole console;

void setup();

int get_flashcycle_ticks();
int get_flashcycle_count();

}

#endif /* MAIN_CORE_CORE_H_ */
