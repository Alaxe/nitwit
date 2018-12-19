#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "ast.h"

class Function {
private:
	std::vector<std::string> args;
	std::vector<std::unique_ptr<StatementAST>> statements;

	std::string name;
	Context context;

public:
	Function(
		std::vector<Line>::const_iterator begin,
		std::vector<Line>::const_iterator end,
		const GlobalContext &globalContext
	);
	void generate_c(std::ostream &out) const;

	static std::vector<Function> parse_all(
		const std::vector<Line> &code, 
		const GlobalContext &globalContext
	);
};
