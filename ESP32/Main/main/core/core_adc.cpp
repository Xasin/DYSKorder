/*
 * core_adc.cpp
 *
 *  Created on: 19 Apr 2019
 *      Author: xasin
 */


#include "core.h"
#include "segments.h"

#include "BatteryIndicator.h"

#include <cmath>

namespace DSKY {
namespace ADC {

auto bat_ind = Peripheral::OLED::BatteryIndicator();

vcc_warn_mode_t vcc_warn_mode = AUTO_OFF;
TickType_t 		vcc_last_warned = 0;

void setup() {
	bat_ind.set_head(&DSKY::display);

	bat_ind.offsetX = DSKY::display.width - 9;

	vcc_warn_mode = AUTO_OFF;
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


float get_extern_vcc() {
	return get_averaged_adc(3) * 5;
}
float get_extern_vsup() {
	return get_averaged_adc(2) / 0.085;
}

float get_extern_vi() {
	return get_averaged_adc(1) / 20;
}

void update_internal_bat() {
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

void update_external_vcc() {
	auto &vccBulb = Seg::bulbs[7];

	auto nextMode = vcc_warn_mode;
	if(vcc_warn_mode >= AUTO_OFF && vcc_warn_mode <= AUTO_50V) {
		if(get_extern_vcc() > 4.3)
			nextMode = AUTO_50V;
		else if(get_extern_vcc() > 2.7)
			nextMode = AUTO_33V;
		else
			nextMode = AUTO_OFF;
	}

	if(nextMode != vcc_warn_mode) {
		vcc_warn_mode = nextMode;
		Xasin::Trek::play(Xasin::Trek::INPUT_OK);

		const std::vector<float> voltages = {0, 3.3, 5};
		DSKY::console.printf("Ext. V target at %fV\n", voltages[vcc_warn_mode]);
	}

	float vTarget = 0;
	float vOKSpread = 0.1;

	switch(vcc_warn_mode) {
	case AUTO_OFF:
	case FIX_OFF:
		return;

	case AUTO_33V:
	case FIX_33V:
		vTarget = 3.3;
		vOKSpread = 0.25;
	break;

	case AUTO_50V:
	case FIX_50V:
		vTarget = 5;
	break;
	}

	float vDev = fabs((get_extern_vcc() - vTarget) / vOKSpread);

	vccBulb.target = Peripheral::Color::HSV(120 - 60 * vDev);
	if(vDev < 0.3)
		vccBulb.mode = Seg::IDLE;
	if(vDev < 0.5)
		vccBulb.mode = Seg::HFLASH;
	if(vDev < 1.2) {
		vccBulb.mode = Seg::FLASH;
		if(xTaskGetTickCount() > vcc_last_warned + 20000) {
			vcc_last_warned = xTaskGetTickCount();
			Xasin::Trek::play(Xasin::Trek::INPUT_BAD);
		}
	}
	else {
		vccBulb.mode = Seg::DFLASH;
		if(xTaskGetTickCount() > vcc_last_warned + 10000) {
			vcc_last_warned = xTaskGetTickCount();
			Xasin::Trek::play(Xasin::Trek::ERROR_MAJOR);
		}
	}

}

void tick() {
	adc.update();

	for(uint8_t i=0; i<adc_buffer.size(); i++) {
		adc_buffer[i][adc_buffer_i] = adc.get_reading(i);
	}
	if(++adc_buffer_i >= adc_buffer[0].size())
		adc_buffer_i = 0;

	update_internal_bat();
	update_external_vcc();
}

}
}
