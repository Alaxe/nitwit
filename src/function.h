#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "ast.h"
#include "function-t.h"

class Function {
private:
	std::vector<std::unique_ptr<StatementAST>> statements;
	FunctionT proto;

	Context context; //shouldn't keep a context here, but just the declared variables
	//this way we won't be risking dangling references

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
