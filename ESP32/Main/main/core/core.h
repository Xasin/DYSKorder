/*
 * core.h
 *
 *  Created on: 6 Apr 2019
 *      Author: xasin
 */

#ifndef MAIN_CORE_CORE_H_
#define MAIN_CORE_CORE_H_

#include "pins.h"

#include "NeoController.h"

#include "AudioHandler.h"
#include "xasin/TrekAudio.h"

#include "MasterAction.h"
#include "xasin/AS1115.h"

#include "xasin/mqtt/Subscription.h"

#include "SSD1327.h"
#include "LittleConsole.h"

#include "xasin/MAX11613.h"
#include "BatteryManager.h"
#include "xasin/LSM6DS3.h"

#include <functional>

namespace DSKY {

extern Xasin::Peripheral::AudioHandler audio;

extern Xasin::I2C::AS1115 			segmentCTRL;
extern Peripheral::NeoController	RGBCTRL;

extern Xasin::I2C::MAX11613			adc;
extern Housekeeping::BatteryManager battery;

extern Xasin::I2C::LSM6DS3			gyro;

extern Xasin::MQTT::Handler			mqtt;

extern Peripheral::OLED::SSD1327	display;
extern Peripheral::OLED::LittleConsole console;
extern Peripheral::OLED::StringPrimitive inputArea;

namespace ADC {
	void setup();

	void tick();
	float get_averaged_adc(uint8_t channel);
}

namespace Graphics {
	void setup();

	void tick();
}

void setup();

int get_flashcycle_ticks();
int get_flashcycle_count();

}

#endif /* MAIN_CORE_CORE_H_ */
