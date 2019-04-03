
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

#include "NeoController.h"

#include "MasterAction.h"

uint8_t segments[8] = {};

const uint8_t numberTemplates[] = {
		0b0111111,
		0b0000110,
		0b1011011,
		0b1001111,
		0b1100110,
		0b1101101,
		0b1111101,
		0b0000111,
		0b1111111,
		0b1101111,
		0b1110111,
		0b1111100,
		0b0111001,
		0b1011110,
		0b1111001,
		0b1110001,
};

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

	auto i2cSend = XaI2C::MasterAction(0);
	i2cSend.write(1, &rawSegments, sizeof(rawSegments));
	i2cSend.execute();
}

esp_err_t event_handler(void *ctx, system_event_t *event)
{
    return ESP_OK;
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

    gpio_config_t pinCFG = {
    		1<<23,
			GPIO_MODE_OUTPUT,
			GPIO_PULLUP_DISABLE,
			GPIO_PULLDOWN_DISABLE,
			GPIO_INTR_DISABLE
	};
    gpio_config(&pinCFG);

    auto RGBCore = Peripheral::NeoController(GPIO_NUM_23, RMT_CHANNEL_0, 3);
    RGBCore.colors[0] = Material::RED;
    RGBCore.nextColors[0] = Material::RED;
    RGBCore.update();

    XaI2C::MasterAction::init(GPIO_NUM_4, GPIO_NUM_5);

    auto testAction = XaI2C::MasterAction(0);

    uint8_t dummy = 1;
    testAction.write(0b1100, &dummy, 1);

    uint8_t dummyC = 0x07;
    testAction.write(0x0B, &dummyC, 1);

    uint8_t brightness = 0x0F;
    testAction.write(0x0A, &brightness, 1);

    uint8_t dummyB = 0b11111111;
    for(uint8_t i=1; i<=8; i++)
    	testAction.write(i, &dummyB, 1);

    printf("ErrCode is: %d\n", testAction.execute());

    uint32_t i = 0;
    while (true) {
    	uint16_t iTemp = i;
    	for(uint8_t j=0; j<4; j++) {
    		set_number(j, iTemp%16);
    		iTemp /= 16;
    	}

    	i++;

    	update_segments();

    	vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

