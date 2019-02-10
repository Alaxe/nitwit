#pragma once

#include "block-ast.h"
#include "expr-ast.h"
#include "statement-ast.h"

class WhileLoopAST : public StatementAST {
protected:
	ExprAST::UPtr condition;
	BlockAST::UPtr body;
public:
	WhileLoopAST(Line::ConstIt &begin, Line::ConstIt end, Context &context);

	void generate_c(std::ostream &out) const override;
	void generate_c(std::ostream &out, uint32_t indent) const override;
};
