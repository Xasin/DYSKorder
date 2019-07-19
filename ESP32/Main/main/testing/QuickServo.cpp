/*
 * QuickServo.cpp
 *
 *  Created on: 20 Jun 2019
 *      Author: xasin
 */

#include "QuickServo.h"

namespace DSKY {

QuickServo::QuickServo(gpio_num_t servo_pin, ledc_timer_t timer, ledc_channel_t channel)
	: pin(servo_pin), led_timer(timer), led_channel(channel) {

	gpio_reset_pin(pin);

	ledc_timer_config_t ledCFG = {};
	ledCFG.speed_mode = LEDC_LOW_SPEED_MODE;
	ledCFG.duty_resolution = LEDC_TIMER_15_BIT;
	ledCFG.timer_num = timer;
	ledCFG.freq_hz   = 500;

	ledc_timer_config(&ledCFG);

	ledc_channel_config_t ledChannel = {};
	ledChannel.gpio_num = servo_pin;
	ledChannel.speed_mode = LEDC_LOW_SPEED_MODE;
	ledChannel.channel = channel;
	ledChannel.intr_type = LEDC_INTR_DISABLE;
	ledChannel.timer_sel = timer;
	ledChannel.duty = 0;
	ledc_channel_config(&ledChannel);

	ledc_fade_func_install(0);

}

QuickServo::~QuickServo() {
	gpio_reset_pin(pin);
}

void QuickServo::set_to(float pwr) {
	ledc_set_duty(LEDC_LOW_SPEED_MODE, led_channel, (125.0 + pwr*125.0)/2000.0 * (32767));
	ledc_update_duty(LEDC_LOW_SPEED_MODE, led_channel);
}
void QuickServo::set_programming() {
	ledc_set_duty(LEDC_LOW_SPEED_MODE, led_channel, 32767);
	ledc_update_duty(LEDC_LOW_SPEED_MODE, led_channel);
}

} /* namespace DSKY */
