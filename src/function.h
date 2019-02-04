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
	std::unique_ptr<StatementAST> body;
	const FunctionData *proto;

public:
	Function(
		Line::ConstIt &begin,
		Line::ConstIt end,
		const GlobalContext &globalContext
	);
	void generate_c(std::ostream &out) const;

	static std::vector<Function> parse_all(
		Line::ConstIt begin,
		Line::ConstIt end,
		const GlobalContext &globalContext
	);
};
