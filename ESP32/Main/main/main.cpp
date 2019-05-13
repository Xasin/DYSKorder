
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_event_loop.h"
#include "esp_pm.h"
#include "esp_timer.h"
#include "esp32/pm.h"
#include "nvs_flash.h"
#include "driver/gpio.h"
#include "driver/rtc_io.h"

#include <vector>
#include <cstring>
#include <cmath>

#include "xasin/mqtt/Subscription.h"

#include "core/core.h"
#include "core/pins.h"
#include "core/segments.h"

#include "ValueBox.h"

#include "core/IndicatorBulb.h"
#include "core/buttons.h"

#include "program/Program.h"
#include "programs/programs.h"

#include "xasin/BME680.h"

esp_err_t event_handler(void *ctx, system_event_t *event)
{
	Xasin::MQTT::Handler::try_wifi_reconnect(event);
	DSKY::mqtt.wifi_handler(event);

    return ESP_OK;
}

using namespace DSKY::Seg;

void display_accell(int axisA, int axisB) {
	DSKY::gyro.update();

	seg_a.value = DSKY::gyro[axisA];
	seg_a.blink = fabs(seg_a.value) > 0.5;
	seg_b.value = DSKY::gyro[axisB];
	seg_b.blink = fabs(seg_b.value) > 0.5;
}

DSKY::BTN::btn_event_t lastButtonPress = {};
TaskHandle_t main_thread = nullptr;

void reset_interfaces() {
	lastButtonPress = {};
	seg_a.clear();
	seg_b.clear();
	seg_a.param_type = DisplayParam::IDLE;
	seg_b.param_type = DisplayParam::NONE;

	for(uint8_t i=0; i<10; i++) {
		bulbs[i].mode = OFF;
	}
}

auto gyroProg = DSKY::Prog::Program("gyro", [](const DSKY::Prog::CommandChunk &cmd) {
	seg_a.param_type = DisplayParam::INT;
	seg_a.fixComma = 2;
	seg_b.param_type = DisplayParam::INT;
	seg_b.fixComma = 2;

	if(cmd.get_arg_str(0) == "spin") {

		seg_a.fixComma = 0;

		float rotCount = 0;
		while(!lastButtonPress.escape) {
			DSKY::gyro.update();
			rotCount += DSKY::gyro[3] * 0.02;

			seg_a.value = rotCount;

			vTaskDelay(20);
		}

		return DSKY::Prog::OK;
	}

	while(!lastButtonPress.escape) {
		display_accell(cmd.get_arg_flt(0, 0), cmd.get_arg_flt(1, 1));
		vTaskDelay(30 / portTICK_PERIOD_MS);
	}

	return DSKY::Prog::OK;
});

auto cuteProg = DSKY::Prog::Program("cute", [](const DSKY::Prog::CommandChunk &cmd) {
	DSKY::inputArea.set("Reading cuteness...");
	bulbs[12].mode = DFLASH;
	bulbs[12].target = Material::PURPLE;

	seg_b.param_type = DisplayParam::INT;

	for(uint8_t i = 0; i<100; i++) {
		vTaskDelay(50);
		seg_b.value = i;
	}

	bulbs[12].target = 0;

	bulbs[7].mode = FLASH;
	bulbs[7].flash_fill = 6;
	bulbs[6].mode = HFLASH;
	bulbs[7].target = Material::AMBER;
	bulbs[6].target = Material::GREEN;

	seg_a.param_type = DisplayParam::DONE;

	Xasin::Trek::play(Xasin::Trek::INPUT_REQ);

	DSKY::inputArea.set("Hugs needed!");
	while(!lastButtonPress.escape) {
		DSKY::inputArea.set_invert((DSKY::get_flashcycle_ticks() & 7) < 4);

		xTaskNotifyWait(0, 0, nullptr, 40);
	}

	return DSKY::Prog::MAJOR_FAIL;
});

auto flauschProg = DSKY::Prog::Program("greet", [](const DSKY::Prog::CommandChunk &cmd) {
	DSKY::console.printf_style("New person found...\n");
	vTaskDelay(300);

	DSKY::console.printf_style("Assessing... ");
	seg_b.param_type = DisplayParam::INT;
	for(uint8_t i=0; i<=100; i++) {
		seg_b.value = i;
		vTaskDelay(30);
	}
	seg_a.param_type = DisplayParam::DONE;

	DSKY::console.printf_style("Done!\n");
	DSKY::console.printf_style("Output: \n");
	DSKY::console.printf_style("Hewoooo!!!\n");

	return DSKY::Prog::OK;
}, false);

auto bme_test = DSKY::Prog::Program("bme", [](const DSKY::Prog::CommandChunk &cmd) {
	DSKY::console.printf_style("Poking BME680...\n");

	auto testSensor = Xasin::I2C::BME680(0b1110111);
	auto ret = testSensor.init_quickstart();
	if(ret != ESP_OK) {
		DSKY::console.printf_style("BME not available!\n");
		return DSKY::Prog::FAIL;
	}

	testSensor.force_measurement();

	vTaskDelay(1000);
	auto data = testSensor.fetch_data();

	while(!DSKY::BTN::last_btn_event.escape) {
		DSKY::console.printf("Got data: %5d %5d %5d %5d\r",
				data.raw_temp, data.raw_humidity,
				data.raw_pressure, data.raw_voc);

		vTaskDelay(500);
	}

	return DSKY::Prog::OK;
});

extern "C" void app_main(void)
{
    nvs_flash_init();
    tcpip_adapter_init();
    ESP_ERROR_CHECK( esp_event_loop_init(event_handler, NULL) );

    esp_timer_init();

    esp_pm_config_esp32_t power_config = {};
    power_config.max_freq_mhz = 80;
	power_config.min_freq_mhz = 80;
	power_config.light_sleep_enable = false;
    esp_pm_configure(&power_config);

    DSKY::setup();

    DSKY::audio.volumeMod = 130;

    bulbs[11].mode = DFLASH;
    bulbs[11].target = Material::RED;
    bulbs[11].flash_fill = 8;

    seg_a.param_type = DisplayParam::LOADING;
    seg_b.param_type = DisplayParam::INT;
    for(uint8_t i=0; i<100; i++) {
    	seg_b.value = i;
    	vTaskDelay(13);

    }

    bulbs[11].mode = OFF;

    Programs::lzr_init();
    Programs::util_init();

    main_thread = xTaskGetCurrentTaskHandle();
    DSKY::BTN::on_event = [](DSKY::BTN::btn_event_t event) {
    	lastButtonPress = event;
    	xTaskNotify(main_thread, 1, eSetBits);
    };

    bulbs[12].mode = IDLE;
    bulbs[12].target = Peripheral::Color(Material::LIME, 160);
    bulbs[13].mode = IDLE;

    DSKY::Prog::Program::inputPrimitive = &DSKY::inputArea;
    DSKY::Prog::Program::statusBulb 	= &bulbs[10];

    while(true) {
    	break;

    	vTaskDelay(2000);

    	do {
    		auto testBox = Peripheral::OLED::ValueBox(64, 12, &DSKY::display);
    		vTaskDelay(2000);
    	} while(false);
    }

    while (true) {
    	reset_interfaces();
    	auto chunk = DSKY::Prog::CommandChunk(DSKY::Prog::Program::get_input(">"));

    	auto nextProg = DSKY::Prog::Program::find(chunk);
    	if(nextProg == nullptr) {
    		Xasin::Trek::play(Xasin::Trek::INPUT_BAD);

    		bulbs[10].set(Material::RED, DFLASH, 8, 2000);
    	}
    	else {
        	DSKY::inputArea.set("Running...");
        	seg_a.param_type = DisplayParam::RUNNING;

    		auto progRes = nextProg->run(chunk);

    		seg_a.clear();
    		seg_b.clear();

    		seg_a.param_type = DisplayParam::DONE;
    		seg_b.param_type = DisplayParam::INT;
    		seg_b.value 	 = progRes;

    		DSKY::Prog::Program::wait_for_enter();
    	}

    	if(lastButtonPress.typed == "adc") {
    		while(!lastButtonPress.escape) {
    			DSKY::adc.update();
    			DSKY::inputArea.printf("Reads: %1.3f %1.3f",
    					DSKY::adc.get_reading(2), DSKY::adc.get_reading(3));

    			xTaskNotifyWait(0, 0, nullptr, 500);
    		}
    	}
    	else if(lastButtonPress.typed == "whos a good boi") {
    		DSKY::inputArea.printf("Duh: Yubutt >~<");

    		while(!lastButtonPress.escape) {
    			DSKY::inputArea.set_invert((DSKY::get_flashcycle_ticks() & 7) < 4);

    			xTaskNotifyWait(0, 0, nullptr, 40);
    		}
    	}
    }
}

