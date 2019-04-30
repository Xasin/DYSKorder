
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

#include "core/core.h"
#include "core/pins.h"
#include "core/segments.h"

#include "ValueBox.h"

#include "core/IndicatorBulb.h"
#include "core/buttons.h"

#include "program/Program.h"

esp_err_t event_handler(void *ctx, system_event_t *event)
{
    return ESP_OK;
}

void send_gyro_command(uint8_t cmd, uint32_t data, uint8_t len = 1) {
	auto i2c_cmd = XaI2C::MasterAction(0b1101011);
	i2c_cmd.write(cmd, &data, len);
	i2c_cmd.execute();
}

void init_gyro() {
	send_gyro_command(0x10, 0b01000001);
	send_gyro_command(0x11, 0b01000000);

	// Enable functions
	send_gyro_command(0x19, 0b00101110);

	// Setup INT1 as Active Low, OD
	send_gyro_command(0x12, 0b00110100);

	// Setup tap detection
	send_gyro_command(0x58, 0b00011110); // Detect enable
	send_gyro_command(0x59, 0b00001111); // Threshold
	send_gyro_command(0x5A, 0b00010100); // DTap duration
	send_gyro_command(0x5B, 0b10000000); // DTap enable
}

struct tap_data_t {
	uint8_t DIR:4;
	uint8_t CNT:2;
	uint8_t TRG:1;
};

#pragma pack(1)
struct accellerometer_data_t {
	union {
		struct {
			int16_t OUTG_X;
			int16_t OUTG_Y;
			int16_t OUTG_Z;
			int16_t OUTXL_X;
			int16_t OUTXL_Y;
			int16_t OUTXL_Z;
		};
		int16_t axis[6];
	};
	union {
		uint8_t reg;
		tap_data_t bits;
	} TAP_DTECT;
};

accellerometer_data_t read_accell() {
	accellerometer_data_t outData = {};

	auto i2c_cmd = XaI2C::MasterAction(0b1101011);
	i2c_cmd.read(0x22, &outData, 12);
	i2c_cmd.execute();

	auto i2c_cmd_b = XaI2C::MasterAction(0b1101011);
	i2c_cmd_b.read(0x1C, &(outData.TAP_DTECT), 1);
	i2c_cmd_b.execute();

	return outData;
}

using namespace DSKY::Seg;

tap_data_t oldGyroReg = {};
void display_accell(int axisA, int axisB) {
	auto gyroData = read_accell();

	auto tapData = gyroData.TAP_DTECT.bits;
	if(tapData.DIR != oldGyroReg.DIR && tapData.DIR == 0) {
		char axis = ' ';
		if(oldGyroReg.DIR & 0b100)
			axis = 'X';
		else if(oldGyroReg.DIR & 0b010)
			axis = 'Y';
		else if(oldGyroReg.DIR & 0b001)
			axis = 'Z';

		uint32_t outColor = 0;
		switch(axis) {
		case 'X': outColor = Material::ORANGE; break;
		case 'Y': outColor = Material::PINK;   break;
		case 'Z': outColor = Material::CYAN;   break;
		}

		DSKY::console.printf("Tap: TRIG:%1d D: %c%c DOUBLE:%1d\n",
				oldGyroReg.TRG, oldGyroReg.DIR >= 8 ? '-' : '+', axis,
				oldGyroReg.CNT);

		bulbs[0].mode = oldGyroReg.DIR >= 8 ? VAL_RISING : VAL_FALLING;
		bulbs[0].target = outColor;
	}
	oldGyroReg = tapData;

	seg_a.value = gyroData.axis[axisA] / 16384.0;
	seg_a.blink = fabs(seg_a.value) > 0.5;
	seg_b.value = gyroData.axis[axisB] / 16384.0;
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

		float rotCount = 0;
		while(!lastButtonPress.escape) {
			auto gyroData = read_accell();
			rotCount += 0.02 * gyroData.OUTG_Z / 32767.0 * 200 / 360.0;

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
    init_gyro();

    bulbs[12].mode = DFLASH;
    bulbs[12].target = Material::RED;
    bulbs[12].flash_fill = 2;
    bulbs[13].mode = VAL_RISING;
    bulbs[13].target = Material::GREEN;

    seg_a.param_type = DisplayParam::LOADING;
    seg_b.param_type = DisplayParam::INT;
    for(uint8_t i=0; i<100; i++) {
    	seg_b.value = i;
    	vTaskDelay(13);

    }

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

