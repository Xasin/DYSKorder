/*
 * Program.cpp
 *
 *  Created on: 22 Apr 2019
 *      Author: xasin
 */

#include "Program.h"

#include "../core/core.h"
#include "xasin/TrekAudio.h"

namespace DSKY {
namespace Prog {

Program *Program::headProgram = nullptr;

Peripheral::OLED::StringPrimitive *Program::inputPrimitive = nullptr;
Seg::IndicatorBulb * Program::statusBulb = nullptr;

using namespace Xasin;

Program::Program(std::string name, program_function_t prog, bool longRunning)
	: nextProgram(headProgram),
	  programCode(prog),
	  progName(name), longRunning(longRunning) {

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
	if(programCode != nullptr) {
		if(longRunning)
			Trek::play(Trek::PROG_BUSY);

		if(statusBulb != nullptr) {
			statusBulb->mode = Seg::IDLE;
			statusBulb->target = Material::GREEN;
		}

		auto execCode = programCode(cmd);

		if(execCode == OK) {
			Trek::play(Trek::PROG_DONE);

			DSKY::console.printf("Prog. completed\n");

			if(statusBulb)
				statusBulb->target = Seg::OFF;
		}
		else if(execCode == MAJOR_FAIL) {
			Trek::play(Trek::ERROR_MAJOR);

			DSKY::console.printf("MAJOR ERROR\n");

			if(statusBulb) {
				statusBulb->target = Material::RED;
				statusBulb->mode   = Seg::DFLASH;
				statusBulb->flash_fill = 12;
			}
		}
		else {
			Trek::play(Trek::PROG_FAILED);

			DSKY::console.printf("Prog failed: %d\n", execCode);

			if(statusBulb) {
				statusBulb->target = Material::AMBER;
				statusBulb->mode   = Seg::FLASH;
				statusBulb->flash_fill = 10;
			}
		}

		return execCode;
	}

	Trek::play(Trek::INPUT_BAD);
	return NO_CODE;
}

void Program::wait_for_button(TickType_t timeout) {
	uint32_t notifyVal = 0;

	TickType_t endTime = timeout;
	if(endTime != portMAX_DELAY)
		endTime += xTaskGetTickCount();

	while((1 & notifyVal) == 0 && (xTaskGetTickCount() < endTime))
		xTaskNotifyWait(1, 1, &notifyVal, endTime - xTaskGetTickCount());
}

void Program::wait_for_esc() {
	while(!DSKY::BTN::last_btn_event.escape)
		wait_for_button();
}
void Program::wait_for_enter() {
	while(!DSKY::BTN::last_btn_event.enter)
		wait_for_button();
}

std::string Program::get_input(std::string marker, BTN::btn_restrict_t restriction) {
	BTN::button_restrict = restriction;
	BTN::last_btn_event = {};

	if(inputPrimitive != nullptr) {
		std::string displayString = "";
		std::string cutDisplayString = "";
		while(!BTN::last_btn_event.enter) {
			displayString = marker + DSKY::BTN::current_typing;
			displayString += (DSKY::get_flashcycle_count() & 1) ? ' ' : 179;

			cutDisplayString = displayString;

			auto lineWidth = inputPrimitive->get_line_width() - 1;

			if(cutDisplayString.size() > lineWidth) {
				cutDisplayString = "";
				cutDisplayString += 174;
				cutDisplayString += displayString.substr(displayString.size()-lineWidth+1);
			}

			inputPrimitive->set(cutDisplayString);

			wait_for_button(100);
		}
	}
	else {
		while(!BTN::last_btn_event.enter)
			wait_for_button();
	}


	DSKY::console.printf("%s%s\n", marker.data(), BTN::last_btn_event.typed.data());
	return BTN::last_btn_event.typed;
}

std::string Program::request_input(std::string marker, BTN::btn_restrict_t restriction) {
	Seg::IndicatorBulb outBulb;

	if(statusBulb != nullptr) {
		outBulb = *statusBulb;

		statusBulb->mode = Seg::HFLASH;
		statusBulb->flash_fill = 8;
		statusBulb->target = Material::BLUE;
	}

	Trek::play(Trek::INPUT_REQ);
	std::string outStr = get_input(marker, restriction);

	if(statusBulb != nullptr)
		(*statusBulb) = outBulb;

	return outStr;
}

} /* namespace Prog */
} /* namespace DSKY */
