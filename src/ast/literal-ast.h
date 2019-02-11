#pragma once

#include "expr-ast.h"
#include <string>

class LiteralAST : public ExprAST {
private:
	static const std::string maxInt;

	std::string val;
	bool int_lit_fits() const;
public:
	LiteralAST(const Token &t, const Context &context);

	virtual void generate_expr(std::ostream &out) const;
};
