/*
 * Program.h
 *
 *  Created on: 22 Apr 2019
 *      Author: xasin
 */

#ifndef MAIN_PROGRAM_PROGRAM_H_
#define MAIN_PROGRAM_PROGRAM_H_

#include "../core/buttons.h"
#include "../core/IndicatorBulb.h"
#include "CommandChunk.h"

#include "StringPrimitive.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <functional>

namespace DSKY {
namespace Prog {

enum program_exit_t {
	OK,
	FAIL		= -1,
	MAJOR_FAIL	= -2,
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
	static Peripheral::OLED::StringPrimitive *inputPrimitive;
	static Seg::IndicatorBulb	*statusBulb;

	const std::string progName;
	const bool 		  longRunning;

	Program(std::string name, program_function_t prog, bool longRunning = true);
	Program *next();

	static  Program * find(const CommandChunk &cmd);

	virtual program_exit_t run(const CommandChunk &cmd);

	static void wait_for_button(TickType_t timeout = portMAX_DELAY);
	static void wait_for_esc();
	static void wait_for_enter();

	static std::string get_input(std::string marker, BTN::btn_restrict_t restriction = BTN::ALL);
	static std::string request_input(std::string marker, BTN::btn_restrict_t restriction = BTN::ALL);
};

} /* namespace Prog */
} /* namespace DSKY */

#endif /* MAIN_PROGRAM_PROGRAM_H_ */
