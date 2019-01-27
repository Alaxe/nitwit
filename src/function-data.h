#pragma once

#include <cstdint>
#include <ostream>
#include <vector>
#include <utility>

#include "lexer.h"
#include "type-t.h"

class FunctionData {
public:
	typedef std::pair<TypeT, std::string> ArgT;

	TypeT returnT;
	std::string name;
	std::vector<ArgT> args;

	FunctionData();
	FunctionData(
		const TypeT &returnT,
		const std::string &name,
		const std::vector<ArgT> &args
	);
	FunctionData(const Line &l);
	void generate_c(std::ostream &out) const;
};
