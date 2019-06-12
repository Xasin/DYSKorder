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
#include "xasin/xirr/Receiver.h"

#include <cmath>

using namespace DSKY;
using namespace DSKY::Prog;
using namespace DSKY::Seg;

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

TickType_t stateTimeout = 0;
uint8_t    winnerID;
enum bacn_state_t {
	NORMAL,
	ATTENTION,
	STOP,

	BTN_GAME_PREP,
	BTN_GAME_GO,
	BTN_GAME_WINNER,
};
bacn_state_t current_bacn_mode = NORMAL;
bacn_state_t old_bacn_mode = NORMAL;

program_exit_t ir_test_func(const DSKY::Prog::CommandChunk &cmd) {
	auto ir_tx = Xasin::XIRR::Transmitter(GPIO_NUM_16, RMT_CHANNEL_4);
	ir_tx.init();

    auto testRX = Xasin::XIRR::Receiver(GPIO_NUM_17, RMT_CHANNEL_5);

    testRX.init();
    testRX.on_rx = [](const uint8_t *data, uint8_t len, uint8_t channel) {
    	if(channel != 128)
    		return;

    	const bacn_mode_t *bacnData = reinterpret_cast<const bacn_mode_t *>(data);

    	for(uint8_t i=0; i<len; i++) {
    		if(current_bacn_mode == BTN_GAME_GO) {
    			if(bacnData[i].func == 10) {
    				winnerID = bacnData[i].ID;
    				current_bacn_mode = BTN_GAME_WINNER;
    				Program::send_notify();
    			}
    		}
    	}
    };

	auto ir_sub = Xasin::MQTT::Subscription(DSKY::mqtt, "DSKorder/BACN/Set", 1);

	ir_sub.on_received = [](const Xasin::MQTT::MQTT_Packet data) {
		current_bacn_mode = static_cast<bacn_state_t>(atoi(data.data.data()));

		if(current_bacn_mode == BTN_GAME_PREP)
			stateTimeout = xTaskGetTickCount() + 2000;

		Program::send_notify();
	};


	TickType_t nextUpdate = 0;
	do {
		if(nextUpdate < xTaskGetTickCount() || current_bacn_mode != old_bacn_mode) {
			bacn_mode_t out[2] = {};
			nextUpdate = xTaskGetTickCount() + 3000;

			switch(current_bacn_mode) {
			default:
				out[0].ID = 0;
				out[0].func = static_cast<uint8_t>(current_bacn_mode)+2;
				out[1] = out[0];
			break;

			case BTN_GAME_WINNER:
				out[0].ID = 0;
				out[0].func = 8;
				out[1].ID = winnerID;
				out[1].func = 7;
			break;
			}

			for(uint8_t i=0; i<3; i++) {
				ir_tx.send(out, sizeof(out), 128);
				vTaskDelay(50);
			}

			old_bacn_mode = current_bacn_mode;
		}

		if(current_bacn_mode == BTN_GAME_PREP) {
			if(stateTimeout < xTaskGetTickCount())
				current_bacn_mode = BTN_GAME_GO;
		}

		DSKY::Prog::Program::wait_for_notify(100);
	} while((!DSKY::BTN::last_btn_event.escape));

	return DSKY::Prog::OK;
}

program_exit_t simon_says(const DSKY::Prog::CommandChunk &cmd) {
	int pressed_button = 0;
	std::vector<uint8_t> buttons;

	auto ir_tx = Xasin::XIRR::Transmitter(GPIO_NUM_16, RMT_CHANNEL_4);
	ir_tx.init();

    auto testRX = Xasin::XIRR::Receiver(GPIO_NUM_17, RMT_CHANNEL_5);
    testRX.init();
    testRX.on_rx = [&pressed_button](const uint8_t *data, uint8_t len, uint8_t channel) {
    	if(channel != 128)
    		return;

    	pressed_button = *data;
    	Program::send_notify();
    };


    while(!DSKY::BTN::last_btn_event.escape) {
    	buttons.push_back(esp_random() & 3);

    	for(auto c : buttons) {
    		ir_tx.send<uint8_t>(c, 128);
    		vTaskDelay(800);
    	}

    	Program::wait_for_notify();
    }

	return Prog::OK;
}

void display_accell(int axisA, int axisB) {
	DSKY::gyro.update();

	seg_a.value = DSKY::gyro[axisA];
	seg_a.blink = fabs(seg_a.value) > 1;
	seg_b.value = DSKY::gyro[axisB];
	seg_b.blink = fabs(seg_b.value) > 1;
}
program_exit_t gyro_test_func(const DSKY::Prog::CommandChunk &cmd) {
	seg_a.param_type = DisplayParam::INT;
	seg_a.fixComma = 2;
	seg_b.param_type = DisplayParam::INT;
	seg_b.fixComma = 2;

	if(cmd.get_arg_str(0) == "spin") {

		seg_a.fixComma = 0;

		float rotCount = 0;
		while(!DSKY::BTN::last_btn_event.escape) {
			DSKY::gyro.update();
			rotCount += DSKY::gyro[3] * 0.02;

			seg_a.value = rotCount;

			vTaskDelay(20);
		}

		return DSKY::Prog::OK;
	}

	while(!DSKY::BTN::last_btn_event.escape) {
		display_accell(cmd.get_arg_flt(0, 0), cmd.get_arg_flt(1, 1));
		vTaskDelay(30 / portTICK_PERIOD_MS);
	}

	return DSKY::Prog::OK;
}

void init_externals() {
	new Program("bme", bme_print_func, false);
	new Program("ir", ir_test_func, true);
	new Program("gyro", gyro_test_func, true);

	new Program("simon", simon_says, true);

	xTaskCreate(background_monitor_task, "DSKY:EXT", 4096, nullptr, 1, &monitor_task);
}

}
