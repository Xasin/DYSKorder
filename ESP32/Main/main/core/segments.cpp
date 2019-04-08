/*
 * segments.c
 *
 *  Created on: 8 Apr 2019
 *      Author: xasin
 */

#include "segments.h"
#include "core.h"

#include <array>

namespace DSKY {
namespace Seg {

std::array<uint8_t, 8> current_raw_segments = {};
bool segA_flashing = false;
bool segB_flashing = false;

void update_segments() {
	std::array<uint8_t, 8> blanked_segments = current_raw_segments;
	if(segA_flashing )
}

void print_number(float num, uint8_t segment) {

}

}
}
