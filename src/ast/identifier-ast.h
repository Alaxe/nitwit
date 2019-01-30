#pragma once

#include "expr-ast.h"

class IdentifierAST : public ExprAST {
protected:
	std::string name;

public:
	IdentifierAST(const Token &tok);
	const std::string& get_name() const;
	virtual void debug_print() const;
	virtual void generate_expr(std::ostream &out) const;
};

