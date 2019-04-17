/*
 * segments.h
 *
 *  Created on: 8 Apr 2019
 *      Author: xasin
 */

#ifndef MAIN_CORE_SEGMENTS_H_
#define MAIN_CORE_SEGMENTS_H_

#include "IndicatorBulb.h"
#include "DisplayParam.h"

#include <stdint.h>
#include <array>

namespace DSKY {
namespace Seg {

extern std::array<IndicatorBulb, 14> bulbs;

extern DisplayParam seg_a;
extern DisplayParam seg_b;

void setup();

}
}


#endif /* MAIN_CORE_SEGMENTS_H_ */
