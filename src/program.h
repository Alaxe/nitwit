#pragma once

#include "lexer.h"
#include "context.h"
#include "function.h"

class Program {
protected:
	GlobalContext globalContext;
	std::vector<Function> functions;

public:
	Program(std::istream &s);
	void generate_c(std::ostream &s) const;
};
