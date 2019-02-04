#pragma once

#include <cstdint>
#include <memory>
#include <ostream>
#include <utility>
#include <vector>

#include "lexer.h"
#include "type-t.h"
#include "var-data.h"

class FunctionData {
public:
	typedef std::unique_ptr<FunctionData> UPtr;

	const TypeT &returnT;
	std::string name;
	std::vector<VarData> args;

	FunctionData(
		const TypeT &returnT,
		std::string name,
		std::vector<VarData> args
	);

	void c_name(std::ostream &out) const;
	void c_prototype(std::ostream &out) const;
};
