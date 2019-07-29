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

#include <cmath>

using namespace DSKY;
using namespace DSKY::Prog;
using namespace DSKY::Seg;

#include "output.espmidi"
#include "Portal2.espmidi"

namespace Programs {

program_exit_t test_servo(const CommandChunk &cmd) {
	auto tServ = Xasin::OneShot125::DShot(PINS_PIO[0], RMT_CHANNEL_6);

	vTaskDelay(1000);

	tServ.init();

	tServ.send(0);

	tServ.send_cmd(Xasin::OneShot125::DShot::SPIN_NORMAL);

	float tSpeed = 0;
	float cSpeed = 0;

	while(true) {
		vTaskDelay(10);

		auto pressedChar = BTN::last_btn_event.typed_char;
		if((pressedChar >= '0') && (pressedChar <= '9')) {
			tSpeed = static_cast<float>(pressedChar - '0') / 9;
		}
		else if(BTN::last_btn_event.escape)
			break;
		else if(pressedChar == '.')
			tServ.send_cmd(Xasin::OneShot125::DShot::BEACON4);

		cSpeed = (tSpeed*0.01 + cSpeed*0.99);
		tServ.send(48 + 2000*tSpeed);
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

void init_bullshit() {
	new Program("portal", playPortal, true);

	new Program("servo", test_servo, true);
}

}
