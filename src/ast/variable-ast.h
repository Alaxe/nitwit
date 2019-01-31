#pragma once

#include "expr-ast.h"

class VariableAST : public ExprAST {
private:
	const VarData *varData;
	std::string cName;
public:
	VariableAST(const Token &tok, const Context &context);
	void generate_expr(std::ostream &out) const;
};
