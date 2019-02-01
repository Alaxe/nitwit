#pragma once

#include "expr-ast.h"
#include <string>

class LiteralAST : public ExprAST {
private:
	std::string val;
public:
	LiteralAST(const Token &t, const Context &context);

	virtual void generate_expr(std::ostream &out) const;
};
