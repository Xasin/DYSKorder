
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
void reset_interfaces() {
	seg_a.clear();
	seg_b.clear();
	seg_a.param_type = DisplayParam::IDLE;
	seg_b.param_type = DisplayParam::NONE;

	for(uint8_t i=0; i<17; i++) {
		if((i < 14) && (i>9))
			continue;

		bulbs[i].mode = OFF;

		if(i > 13) {
			bulbs[i].mode = HFLASH;
			bulbs[i].flash_fill = 2;
			bulbs[i].target = Material::PURPLE;
		}
	}
}

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
    Programs::init_externals();

    DSKY::Prog::Program::programTask = xTaskGetCurrentTaskHandle();
    DSKY::BTN::on_event = [](DSKY::BTN::btn_event_t event) {
    	xTaskNotify(DSKY::Prog::Program::programTask, 1, eSetBits);
    };

    bulbs[12].mode = IDLE;
    bulbs[12].target = Peripheral::Color(Material::LIME, 160);
    bulbs[13].mode = IDLE;

    DSKY::Prog::Program::inputPrimitive = &DSKY::inputArea;
    DSKY::Prog::Program::statusBulb 	= &bulbs[10];

    auto wifiEnableChunk = DSKY::Prog::CommandChunk("consoleRelay");
    DSKY::Prog::Program::find(wifiEnableChunk)->run(wifiEnableChunk);

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
    }
}

