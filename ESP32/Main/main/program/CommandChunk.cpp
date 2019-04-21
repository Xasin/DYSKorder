/*
 * CommandChunk.cpp
 *
 *  Created on: 19 Apr 2019
 *      Author: xasin
 */

#include "CommandChunk.h"

#include <cstdlib>
#include <stdio.h>

namespace DSKY {
namespace Prog {

CommandChunk::CommandChunk(std::string command)
	: command(command),
	  args(), keys() {

	int lastPos = command.find(" ");

	while(true) {
		lastPos = command.find(" ", lastPos);
		if(lastPos > command.length())
			break;

		while(command.at(lastPos) == ' ')
			lastPos++;

		if(lastPos >= command.length())
			break;

		if(command.at(lastPos) != '-') {
			auto argStr = 	command.substr(lastPos,
					command.find(" ", lastPos) - lastPos);

			args.emplace_back(argStr);
		}
		else {
			int keyEnd = command.find(" ", lastPos);

			int keyArgStart = keyEnd;
			while(command.at(keyArgStart) == ' ')
				keyArgStart++;

			int keyArgEnd = command.find(" ", keyArgStart);

			if(keyArgStart >= command.length())
				break;

			auto keyStr = command.substr(lastPos, keyEnd-lastPos);
			auto valStr = command.substr(keyArgStart, keyArgEnd-keyArgStart);

			keys.emplace(keyStr, valStr);

			lastPos = keyArgEnd;
		}
	}
}

std::string CommandChunk::get_cmd() const {
	return command.substr(0, command.find(' '));
}

int CommandChunk::get_arg_count() const {
	return args.size();
}

bool CommandChunk::has_arg(const std::string key) const {
	return keys.count(key) == 1;
}

const std::string CommandChunk::get_arg_str(const std::string key, const std::string def) const {
	if(!has_arg(key))
		return def;

	return keys.find(key)->first;
}
const std::string CommandChunk::get_arg_str(int i, const std::string def) const {
	if(args.size() <= i)
		return def;

	return args[i];
}

float CommandChunk::get_arg_flt(const std::string key, float def) const {
	if(!has_arg(key))
		return def;

	return atof((keys.find(key)->first).data());
}

float CommandChunk::get_arg_flt(int i, float def) const {
	if(args.size() <= i)
		return def;

	return atof(args[i].data());
}

} /* namespace Prog */
} /* namespace DSKY */
