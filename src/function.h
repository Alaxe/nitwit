#pragma once

#include <vector>
#include <memory>
#include <string>

#include "ast.h"

class Function {
private:
	std::vector<std::string> args;
	std::vector<std::unique_ptr<StatementAST>> statements;
public:
	Function(
		std::vector<Line>::const_iterator begin,
		std::vector<Line>::const_iterator end,
		Context context
	);

	static std::vector<Function> parse_all(
		const std::vector<Line> &code, 
		const Context &context
	);
};
