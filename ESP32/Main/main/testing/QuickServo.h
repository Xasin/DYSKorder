/*
 * QuickServo.h
 *
 *  Created on: 20 Jun 2019
 *      Author: xasin
 */

#ifndef MAIN_TESTING_QUICKSERVO_H_
#define MAIN_TESTING_QUICKSERVO_H_

#include "driver/gpio.h"
#include "driver/ledc.h"

namespace DSKY {

class QuickServo {
public:
	const gpio_num_t 	pin;
	const ledc_timer_t	led_timer;
	const ledc_channel_t led_channel;

	QuickServo(gpio_num_t servo_pin, ledc_timer_t timer = LEDC_TIMER_3, ledc_channel_t led_channel = LEDC_CHANNEL_7);
	~QuickServo();

	void set_to(float degs);
	void set_programming();
};

} /* namespace DSKY */

#endif /* MAIN_TESTING_QUICKSERVO_H_ */
