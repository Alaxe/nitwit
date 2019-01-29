#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "ast.h"
#include "context.h"
#include "function-data.h"

class Function {
private:
	std::vector<VarData> declarations;
	std::unique_ptr<StatementAST> body;
	const FunctionData *proto;

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
