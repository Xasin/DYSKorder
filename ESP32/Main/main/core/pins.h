/*
 * pins.h
 *
 *  Created on: 6 Apr 2019
 *      Author: xasin
 */

#ifndef MAIN_CORE_PINS_H_
#define MAIN_CORE_PINS_H_

#include <array>

#include "driver/gpio.h"

#define PIN_WS2812 	GPIO_NUM_23

#define PIN_I2C_SDA	GPIO_NUM_4
#define PIN_I2C_SCL	GPIO_NUM_5

#define PIN_AUDIO_DATA GPIO_NUM_12
#define PIN_AUDIO_LRCK GPIO_NUM_13
#define PIN_AUDIO_BCK  GPIO_NUM_14

#define PIN_BTN_IRQ	GPIO_NUM_15
const std::array<gpio_num_t, 3> PINS_MODE = {
		GPIO_NUM_27,
		GPIO_NUM_33,
		GPIO_NUM_32,
};

const std::array<gpio_num_t, 4> PINS_PIO = {
		GPIO_NUM_16,
		GPIO_NUM_17,
		GPIO_NUM_18,
		GPIO_NUM_19
};

#endif /* MAIN_CORE_PINS_H_ */
