/*
 * bullshit.cpp
 *
 *  Created on: 13 Jun 2019
 *      Author: xasin
 */

#include "../program/Program.h"
#include "../core/core.h"

#include "../core/segments.h"

#include "../testing/QuickServo.h"
#include "xasin/DShot.h"

#include "xasin/socks/UDP.h"

#include "cJSON.h"

#include <cmath>

using namespace DSKY;
using namespace DSKY::Prog;
using namespace DSKY::Seg;

#include "output.espmidi"
#include "Portal2.espmidi"

namespace Programs {

program_exit_t send_hovercraft(const CommandChunk &cmd) {
	cJSON *axis_obj = cJSON_CreateObject();

	cJSON *fwd_val = cJSON_CreateNumber(0);
	cJSON *turn_val = cJSON_CreateNumber(0);

	cJSON_AddItemToObject(axis_obj, "T", turn_val);
	cJSON_AddItemToObject(axis_obj, "F", fwd_val);

	cJSON *out_obj = cJSON_CreateObject();
	cJSON_AddItemToObject(out_obj, "Axis", axis_obj);

	auto hvr_sender = Xasin::Socks::UDP("192.168.6.14", 456);
	hvr_sender.open_remote();

	while(!DSKY::BTN::last_btn_event.escape) {
		DSKY::gyro.update();

		cJSON_SetNumberValue(fwd_val, DSKY::gyro[0] * 0.4);
		cJSON_SetNumberValue(turn_val, DSKY::gyro[1] * 0.4);

		char *outJSON = cJSON_PrintUnformatted(out_obj);
		hvr_sender.send(outJSON, strlen(outJSON));
		delete outJSON;

		vTaskDelay(10);
	}

	cJSON_SetNumberValue(fwd_val, 0);
	cJSON_SetNumberValue(turn_val, 0);

	char *outJSON = cJSON_PrintUnformatted(out_obj);
	hvr_sender.send(outJSON, strlen(outJSON));
	delete outJSON;

	return DSKY::Prog::OK;
}

program_exit_t test_servo(const CommandChunk &cmd) {
	auto tServ = Xasin::Drone::DShot(RMT_CHANNEL_4, 2, PINS_PIO[0]);
	tServ.init();
	vTaskDelay(100);

	DSKY::console.printf("Ready...");

	Program::wait_for_button();

	DSKY::console.printf("Sending reset signal!\n");

	for(int i=5000; i!=0; i--) {
		tServ.set_motor_power(1, 0.1 + 0.05*sin(xTaskGetTickCount()/70.0));
		tServ.set_motor_power(0, 0.2 + 0.05*sin(xTaskGetTickCount()/70.0));
		vTaskDelay(1);
	}

	vTaskDelay(20);
	for(int i=0; i<50; i++) {
		tServ.send_cmd(0, tServ.STOP);
		vTaskDelay(2);
	}
	vTaskDelay(10);
	for(int i=0; i<50; i++) {
		tServ.send_cmd(0, tServ.SPIN_3D);
		vTaskDelay(2);
	}

	vTaskDelay(10);
	for(int i=0; i<50; i++) {
		tServ.send_cmd(0, tServ.SAVE_SETTING);
		vTaskDelay(2);
	}
	tServ.send_cmd(1, tServ.SAVE_SETTING);

	DSKY::console.printf("Attempting beacon\n");

	vTaskDelay(10);
	for(int i=0; i<40; i++) {
		tServ.send_cmd(0, Xasin::Drone::DShot::BEACON3);
		vTaskDelay(20);
	}

	for(int i=1000; i!=0; i--) {
		tServ.set_motor_power(1, 0.1 + 0.05*sin(xTaskGetTickCount()/70.0));
		tServ.set_motor_power(0, 0.2 + 0.05*sin(xTaskGetTickCount()/70.0));
		vTaskDelay(1);
	}

	float tSpeed = 0;
	float cSpeed = 0;

	while(true) {
		vTaskDelay(2);

		auto pressedChar = BTN::last_btn_event.typed_char;
		if((pressedChar >= '0') && (pressedChar <= '4'))
			tSpeed = static_cast<float>(pressedChar - '0') / 9.0;
		else if((pressedChar >= '5') && (pressedChar <= '9'))
			tSpeed = ('4' - static_cast<float>(pressedChar)) / 9.0;
		else if(BTN::last_btn_event.escape)
			break;
		else if(pressedChar == '.') {
			vTaskDelay(10);
			for(int i=0; i<10; i++) {
				tServ.send_cmd(0, tServ.SPIN_3D);
				vTaskDelay(2);
			}
			tServ.send_cmd(0, tServ.SPIN_3D);
		}

		tServ.set_motor_power(1, 0.2 * sin(xTaskGetTickCount()/5000.0));

		cSpeed = (tSpeed*0.01 + cSpeed*0.99);
		tServ.set_motor_power(0, cSpeed);
	}

	return DSKY::Prog::OK;
}

program_exit_t playPortal(const CommandChunk &cmd) {
	int i=0;

	while(i < (sizeof(Portal2_midi_data)/sizeof(esp_midi_byte_t))) {
		auto samp = Portal2_midi_data[i];

		samp.duration *= 0.5;

		if(samp.type == 0)
			vTaskDelay(samp.duration);
		else {
			float freq = 440.0 * pow(2.0, (samp.type-69)/12.0);
			printf("Playing: %f\n", freq);
			DSKY::audio.insert_sample(new Xasin::Peripheral::SawtoothWave(freq, samp.vol, samp.duration));
		}

		i++;
	}

	return DSKY::Prog::OK;
}

program_exit_t voice_control_button(const CommandChunk &cmd) {
	bool old_state = false;

	while(!DSKY::BTN::last_btn_event.escape) {
		xTaskNotifyWait(0, 0, nullptr, 500);

		bool new_state = (DSKY::BTN::current_buttons & (1<<7)) != 0;

		if(new_state != old_state)
			DSKY::mqtt.publish_to("Test/VCTRL", new_state ? "1" : "0", 1);

		old_state = new_state;
	}

	return DSKY::Prog::OK;
}

void init_bullshit() {
	new Program("portal", playPortal, true);

	new Program("servo", test_servo, true);

	new Program("hover", send_hovercraft, true);

	new Program("vctrl", voice_control_button, true);
}

}
