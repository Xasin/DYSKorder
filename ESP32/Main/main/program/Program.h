/*
 * Program.h
 *
 *  Created on: 22 Apr 2019
 *      Author: xasin
 */

#ifndef MAIN_PROGRAM_PROGRAM_H_
#define MAIN_PROGRAM_PROGRAM_H_

#include "../core/buttons.h"
#include "CommandChunk.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <functional>

namespace DSKY {
namespace Prog {

enum program_exit_t {
	OK,
	NOT_FOUND 	= -1000,
	NO_CODE		= -1001,
};

typedef std::function<program_exit_t(const CommandChunk&)> program_function_t;

class Program {
protected:
	static Program *headProgram;
	Program *nextProgram;

	program_function_t programCode;

public:
	const std::string progName;

	Program(std::string name, program_function_t prog);
	Program *next();

	static  Program * find(const CommandChunk &cmd);

	virtual program_exit_t run(const CommandChunk &cmd);

	static void wait_for_button(TickType_t timeout = portMAX_DELAY);
	static void wait_for_esc();
};

} /* namespace Prog */
} /* namespace DSKY */

#endif /* MAIN_PROGRAM_PROGRAM_H_ */
