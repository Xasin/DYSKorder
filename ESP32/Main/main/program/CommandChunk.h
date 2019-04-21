/*
 * CommandChunk.h
 *
 *  Created on: 19 Apr 2019
 *      Author: xasin
 */

#ifndef MAIN_PROGRAM_COMMANDCHUNK_H_
#define MAIN_PROGRAM_COMMANDCHUNK_H_

#include <string>
#include <vector>
#include <map>

namespace DSKY {
namespace Prog {

class CommandChunk {
public:
	const std::string command;
	std::vector<std::string> args;
	std::map<std::string, std::string> keys;

	CommandChunk(std::string command);

	std::string get_cmd() const;

	int get_arg_count() const;

	bool has_arg(const std::string key) const;

	const std::string get_arg_str(const std::string key, const std::string def = "") const;
	const std::string get_arg_str(int i, const std::string def = "") const;
	float		get_arg_flt(const std::string key, float def = 0) const;
	float 		get_arg_flt(int i, float def = 0) const;
};

} /* namespace Prog */
} /* namespace DSKY */

#endif /* MAIN_PROGRAM_COMMANDCHUNK_H_ */
