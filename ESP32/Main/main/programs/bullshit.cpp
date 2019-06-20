/*
 * bullshit.cpp
 *
 *  Created on: 13 Jun 2019
 *      Author: xasin
 */

#include "../program/Program.h"
#include "../core/core.h"

#include "../core/segments.h"

#include <cmath>

using namespace DSKY;
using namespace DSKY::Prog;
using namespace DSKY::Seg;

#include "output.espmidi"
#include "Portal2.espmidi"

namespace Programs {

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
}

}
