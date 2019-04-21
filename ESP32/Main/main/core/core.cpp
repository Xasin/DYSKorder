/*
 * core.cpp
 *
 *  Created on: 6 Apr 2019
 *      Author: xasin
 */


#include "core.h"
#include "pins.h"
#include "segments.h"

#include "buttons.h"

#include <math.h>

namespace DSKY {

Xasin::Peripheral::AudioHandler audio(44100);


Xasin::I2C::AS1115 			segmentCTRL = Xasin::I2C::AS1115();
Peripheral::NeoController	RGBCTRL(PIN_WS2812, RMT_CHANNEL_0, 14);

Peripheral::OLED::SSD1327	display = Peripheral::OLED::SSD1327();
Peripheral::OLED::LittleConsole console = Peripheral::OLED::LittleConsole(display);

Xasin::I2C::MAX11613 adc = Xasin::I2C::MAX11613();
Housekeeping::BatteryManager battery = Housekeeping::BatteryManager();

void housekeep_thread(void *_) {
	while(true) {
		ADC::tick();

		vTaskDelay(100);
	}
}

void pin_setup() {
	// OUTPUT CONFIGURATION
    gpio_config_t pinCFG = {
			1<<PIN_AUDIO_DATA | 1<<PIN_AUDIO_LRCK | 1<<PIN_AUDIO_BCK,
			GPIO_MODE_OUTPUT,
			GPIO_PULLUP_DISABLE,
			GPIO_PULLDOWN_DISABLE,
			GPIO_INTR_DISABLE
	};
    gpio_config(&pinCFG);

    uint64_t newMask = 0;
    for(int p : PINS_MODE)
    	newMask += pow(2, p);

    pinCFG.pin_bit_mask = newMask;
    pinCFG.mode = GPIO_MODE_INPUT;
    pinCFG.pull_up_en = GPIO_PULLUP_ENABLE;
    gpio_config(&pinCFG);

    pinCFG.pin_bit_mask = 1<<PIN_BTN_IRQ;
    pinCFG.intr_type	= GPIO_INTR_NEGEDGE;
    gpio_config(&pinCFG);
    gpio_intr_enable(PIN_BTN_IRQ);

    gpio_install_isr_service(0);
}

void setup() {
	pin_setup();

	XaI2C::MasterAction::init(PIN_I2C_SDA, PIN_I2C_SCL);

	Seg::setup();

    adc.init();
    ADC::setup();

    i2s_pin_config_t pinCFG = {
    	PIN_AUDIO_BCK,
		PIN_AUDIO_LRCK,
		PIN_AUDIO_DATA,
		I2S_PIN_NO_CHANGE
    };
    audio.start_thread(pinCFG);
    Xasin::Trek::init(audio);

    display.initialize();

    xTaskCreate(housekeep_thread, "DSKY::House", 2048, nullptr, 10, nullptr);

    Xasin::Trek::play(Xasin::Trek::PROG_DONE);

    BTN::setup();
}

int get_flashcycle_ticks() {
	return xTaskGetTickCount()/100;
}
int get_flashcycle_count() {
	return get_flashcycle_ticks()/8;
}

}
