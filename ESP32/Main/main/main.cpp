
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

#include <cstring>

#include "core/core.h"
#include "core/pins.h"

auto segmentCTRL = Xasin::I2C::AS1115();

uint8_t segments[8] = {};

void set_number(uint8_t p, uint8_t val, bool dot = false) {
	if(val < sizeof(numberTemplates))
		segments[(p+7)&0b111] = numberTemplates[val];
	if(dot)
		segments[(p+7)&0b111] |= 1<<7;
}

void update_segments() {
	uint8_t rawSegments[8] = {};

	for(uint8_t seg=0; seg<8; seg++)
		for(uint8_t i=0; i<8; i++)
			rawSegments[i] |= ((segments[seg]>>i) & 1) << seg;

	for(uint8_t i=0; i<8; i++)
		segmentCTRL.set_segment(i, rawSegments[i]);

	segmentCTRL.update_segments();
}

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
	int16_t OUTG_X;
	int16_t OUTG_Y;
	int16_t OUTG_Z;
	int16_t OUTXL_X;
	int16_t OUTXL_Y;
	int16_t OUTXL_Z;
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

tap_data_t oldGyroReg = {};
void display_accell() {
	 auto gyroData = read_accell();

	int16_t iTemp = gyroData.OUTXL_X/100;
	if(iTemp < 0) {
		segments[2] = 1<<6;
		iTemp *= -1;
	}
	else
		segments[2] = 0;
	for(uint8_t j=0; j<3; j++) {
		set_number(j, iTemp%10);
		iTemp /= 10;
	}

	iTemp = gyroData.OUTXL_Y/100;
	if(iTemp < 0) {
		segments[6] = 1<<6;
		iTemp *= -1;
	}
	else
		segments[6] = 0;
	for(uint8_t j=0; j<3; j++) {
		set_number(j+4, iTemp%10);
		iTemp /= 10;
	}

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

		printf("Tap: TRIG:%1d D: %c%c DOUBLE:%1d\n",
				oldGyroReg.TRG, oldGyroReg.DIR >= 8 ? '-' : '+', axis,
				oldGyroReg.CNT);
	}
	oldGyroReg = tapData;
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
    init_gyro();

    while (true) {
    	display_accell();

    	update_segments();

    	vTaskDelay(30 / portTICK_PERIOD_MS);
    }
}

