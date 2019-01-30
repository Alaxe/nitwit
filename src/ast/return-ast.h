#pragma once
#include "expr-ast.h"
#include "statement-ast.h"

class ReturnAST : public StatementAST {
private:
	ExprAST::UPtr returnVal;
public:
	ReturnAST(const Line &l, const Context &context);
	virtual void debug_print() const;
	virtual void generate_c(std::ostream &out) const;
};
