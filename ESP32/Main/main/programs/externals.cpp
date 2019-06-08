/*
 * externals.cpp
 *
 *  Created on: 21 May 2019
 *      Author: xasin
 */

#include "../program/Program.h"
#include "../core/core.h"

#include "../core/segments.h"

#include "xasin/BME680.h"

#include "xasin/xirr/Transmitter.h"

using namespace DSKY;
using namespace DSKY::Prog;

namespace Programs {

Xasin::I2C::BME680 *bme_ptr = nullptr;

TaskHandle_t monitor_task;
void background_monitor_task(void *arg) {
	while(true) {
		vTaskDelay(3000);

		if(ESP_OK == XaI2C::MasterAction::poke(0b1110111)) {
			auto BME = Xasin::I2C::BME680(0b1110111);
			bme_ptr = &BME;

			BME.init_quickstart();

			auto &bulb = DSKY::Seg::bulbs[14];

			bulb.mode = DSKY::Seg::HFLASH;
			bulb.flash_fill = 8;

			DSKY::console.printf("BME connected\n");
			Xasin::Trek::play(Xasin::Trek::INPUT_OK);


			TickType_t lastMQTTPush = 0;
			while(true) {
				if(ESP_OK != XaI2C::MasterAction::poke(0b1110111)) {
					bulb.mode = DSKY::Seg::HFLASH;
					bulb.flash_fill = 2;
					bulb.target = Material::PURPLE;

					DSKY::console.printf("BME disconnected\n");
					Xasin::Trek::play(Xasin::Trek::PROG_DONE);

					bme_ptr = nullptr;

					break;
				}

				bulb.mode = DSKY::Seg::DFLASH;

				BME.force_measurement();
				vTaskDelay(130);
				BME.fetch_data();

				bulb.mode = DSKY::Seg::IDLE;
				bulb.target = Peripheral::Color::HSV(BME.get_air_quality()*120);

#define MQ_PUB(name, mName) sprintf(buff, "%.3f", BME.get_ ## name ()); DSKY::mqtt.publish_to(std::string("Xasin/BME680/") + mName, buff, strlen(buff))
				if((DSKY::mqtt.is_disconnected() == 0) && (lastMQTTPush + 30000 < xTaskGetTickCount())) {
					char buff[20] = {0};

					MQ_PUB(temp, "Temperature");
					MQ_PUB(humidity, "Humidity");
					MQ_PUB(pressure, "Pressure");
					MQ_PUB(gas_res, "VOC Resistance");
					MQ_PUB(air_quality, "AirQ Rating");

					lastMQTTPush = xTaskGetTickCount();
				}

				vTaskDelay(2870);
			}
		}
	}
}


program_exit_t bme_print_func(const DSKY::Prog::CommandChunk &cmd) {
	DSKY::console.printf("Poking BME680...\n");

	if(bme_ptr == nullptr) {
		DSKY::console.printf("BME not available!\n");
		return DSKY::Prog::FAIL;
	}

	Xasin::Trek::play(Xasin::Trek::PROG_BUSY);

	DSKY::BTN::last_btn_event = {};
	do {
		if(bme_ptr == nullptr)
			return DSKY::Prog::FAIL;

		DSKY::console.printf("Got data: %2.2f %2.3f %2.3f %2.3f\r",
				bme_ptr->get_temp(), bme_ptr->get_humidity(),
				bme_ptr->get_pressure(), bme_ptr->get_gas_res());

		Program::wait_for_button(1000);
	} while(!DSKY::BTN::last_btn_event.enter);

	return DSKY::Prog::OK;
}


#pragma pack(1)
struct bacn_mode_t {
	uint8_t ID:4;
	uint8_t func:4;
};
#pragma pack(0)

program_exit_t ir_test_func(const DSKY::Prog::CommandChunk &cmd) {
	auto ir_tx = Xasin::XIRR::Transmitter(GPIO_NUM_16, RMT_CHANNEL_4);
	ir_tx.init();

	bacn_mode_t out = {};

	do {
		for(uint8_t i=0; i<3; i++) {
			ir_tx.send<bacn_mode_t>(out, 128);
			vTaskDelay(50);
		}
		printf("Booping out!\n");

		Program::wait_for_button(2000);

		char tChar = DSKY::BTN::last_btn_event.typed_char;
		if(tChar >= '0' && tChar <= '9') {
			out.ID   = 0;
			out.func = tChar - '0';
		}
	} while((!DSKY::BTN::last_btn_event.escape));

	return DSKY::Prog::OK;
}

void init_externals() {

	new Program("bme", bme_print_func, false);
	new Program("ir", ir_test_func, true);

	xTaskCreate(background_monitor_task, "DSKY:EXT", 4096, nullptr, 1, &monitor_task);
}

}
