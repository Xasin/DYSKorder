/*
 * segments.h
 *
 *  Created on: 8 Apr 2019
 *      Author: xasin
 */

#ifndef MAIN_CORE_SEGMENTS_H_
#define MAIN_CORE_SEGMENTS_H_

#include <stdint.h>

namespace DSKY {
namespace Seg {

enum segment_modes_t {
	IDLE,
	LOADING,
	DONE,
	ERROR,
	FLOATS,
	HEX
};

extern segment_modes_t segment_mode;
extern float seg_a_value;
extern bool  seg_a_blink;
extern float seg_b_value;
extern bool  seg_b_blink;

void setup();
void update();

const uint8_t char_templates[] = {
		0b0111111,	// 0
		0b0000110,	// 1
		0b1011011,	// 2
		0b1001111,	// 3
		0b1100110,	// 4
		0b1101101,	// 5
		0b1111101,	// 6
		0b0000111,	// 7
		0b1111111,	// 8
		0b1101111,	// 9
		0b1110111,	// A (10)
		0b1111100,	// b (11)
		0b0111001,	// C (12)
		0b1011110,	// d (13)
		0b1111001,	// E (14)
		0b1110001,	// F (15)
		0b1010000,	// r (16)
		0b1010100,	// n (17)
		0b1011100,	// o (18)
		0b0111000,	// L (19)
		0b0110000,	// I (20)
};

const uint8_t signal_done[] = {13, 18, 17, 14};
const uint8_t signal_idle[] = {20, 13, 19, 14};
const uint8_t signal_load[] = {19, 18, 10, 13};
const uint8_t signal_err[]  = {14, 16, 16};
const uint8_t signal_nan[]  = {17, 10, 17};

}
}


#endif /* MAIN_CORE_SEGMENTS_H_ */
