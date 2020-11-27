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

/*program_exit_t playPortal(const CommandChunk &cmd) {
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
}*/

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
	// new Program("portal", playPortal, true);

	new Program("hover", send_hovercraft, true);
	new Program("vctrl", voice_control_button, true);
}

}
