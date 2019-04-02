
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

#include "NeoController.h"

#include "MasterAction.h"

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

    XaI2C::MasterAction::init(GPIO_NUM_4, GPIO_NUM_5);

    auto testAction = XaI2C::MasterAction(0);

    uint8_t dummy = 1;
    testAction.write(0b1100, &dummy, 1);

    uint8_t dummyC = 0x07;
    testAction.write(0x0B, &dummyC, 1);

    uint8_t dummyB = 0b11111111;
    for(uint8_t i=1; i<=8; i++)
    	testAction.write(i, &dummyB, 1);

    printf("ErrCode is: %d\n", testAction.execute());

    uint32_t i = 0;
    while (true) {
    	i++;

    	auto testActionB = XaI2C::MasterAction(0);
    	testActionB.write(1, &i, sizeof(i));
    	testActionB.execute();

    	vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

