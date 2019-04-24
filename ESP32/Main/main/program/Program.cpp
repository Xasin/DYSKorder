/*
 * Program.cpp
 *
 *  Created on: 22 Apr 2019
 *      Author: xasin
 */

#include "Program.h"

namespace DSKY {
namespace Prog {

Program *Program::headProgram = nullptr;

Program::Program(std::string name, program_function_t prog)
	: nextProgram(headProgram),
	  programCode(prog),
	  progName(name) {

	headProgram = this;
}

Program *Program::next() {
	return nextProgram;
}

Program *Program::find(const CommandChunk &cmd) {
	Program *progPTR = headProgram;

	while(progPTR != nullptr) {
		if(progPTR->progName == cmd.get_cmd())
			return progPTR;

		progPTR = progPTR->next();
	}

	return nullptr;
}

program_exit_t Program::run(const CommandChunk &cmd) {
	if(programCode != nullptr)
		return programCode(cmd);

	return NO_CODE;
}

void Program::wait_for_button(TickType_t timeout) {
	xTaskNotifyWait(0, 0, nullptr, timeout);
}

void Program::wait_for_esc() {
	while(!DSKY::BTN::last_btn_event.escape)
		wait_for_button();
}

} /* namespace Prog */
} /* namespace DSKY */
