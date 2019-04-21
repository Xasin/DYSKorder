/*
 * core_adc.cpp
 *
 *  Created on: 19 Apr 2019
 *      Author: xasin
 */


#include "core.h"
#include "segments.h"

#include "BatteryIndicator.h"

namespace DSKY {
namespace ADC {

auto bat_ind = Peripheral::OLED::BatteryIndicator();

void setup() {
	bat_ind.set_head(&DSKY::display);

	bat_ind.offsetX = DSKY::display.width - 9;
}

uint8_t adc_buffer_i=0;
std::array<std::array<float, 8>, 4> adc_buffer;

float get_averaged_adc(uint8_t channel) {
	if(channel > adc_buffer.size())
		return 0;

	float adc_sum = 0;
	for(uint8_t i=0; i<adc_buffer[channel].size(); i++)
		adc_sum += adc_buffer[channel][i];

	return adc_sum / adc_buffer[channel].size();
}

void tick() {
	adc.update();
	for(uint8_t i=0; i<adc_buffer.size(); i++) {
		adc_buffer[i][adc_buffer_i] = adc.get_reading(i);
	}
	if(++adc_buffer_i >= adc_buffer[0].size())
		adc_buffer_i = 0;


	battery.set_voltage(3000 * ADC::get_averaged_adc(0));
	bat_ind.set_fill(battery.current_capacity());

	auto &pwrBulb = Seg::bulbs[13];
	if(battery.current_mv() > 4300) {
		pwrBulb.target = Material::CYAN;
		pwrBulb.mode   = Seg::IDLE;
	}
	else {
		pwrBulb.target = Peripheral::Color::HSV(1.2 * battery.current_capacity());
		pwrBulb.mode = (battery.current_capacity() < 30) ? Seg::FLASH : Seg::IDLE;
	}
}

float get_extern_vcc() {
	return get_averaged_adc(3) * 5;
}
float get_extern_vsup() {
	return get_averaged_adc(2) / 0.085;
}

float get_extern_vi() {
	return get_averaged_adc(1) / 20;
}

}
}
