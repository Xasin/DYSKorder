/*
 * segments.h
 *
 *  Created on: 8 Apr 2019
 *      Author: xasin
 */

#ifndef MAIN_CORE_SEGMENTS_H_
#define MAIN_CORE_SEGMENTS_H_

#include "DisplayParam.h"
#include <stdint.h>

namespace DSKY {
namespace Seg {

extern DisplayParam seg_a;
extern DisplayParam seg_b;

void setup();
void update();

}
}


#endif /* MAIN_CORE_SEGMENTS_H_ */
