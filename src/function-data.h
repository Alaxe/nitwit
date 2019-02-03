#pragma once

#include <cstdint>
#include <memory>
#include <ostream>
#include <utility>
#include <vector>

#include "lexer.h"
#include "type-t.h"

class FunctionData {
public:
	class Argument {
	public:
		const TypeT &type;
		std::string name;
		Argument(const TypeT &type, std::string name);
	};
	typedef std::pair<const TypeT&, std::string> ArgT;
	typedef std::unique_ptr<FunctionData> UPtr;

	const TypeT &returnT;
	std::string name;
	std::vector<ArgT> args;

	FunctionData(
		const TypeT &returnT,
		const std::string &name,
		const std::vector<ArgT> &args
	);

	void c_name(std::ostream &out) const;
	void generate_c(std::ostream &out) const;
};
