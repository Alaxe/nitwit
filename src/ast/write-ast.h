#pragma once

#include "expr-ast.h"

class WriteAST : public ExprAST {
protected:
	ExprAST::UPtr operand;
public:
	WriteAST(
		Token::ConstIt &begin,
		Token::ConstIt end,
		const Context &context
	);

	virtual void debug_print() const;
	virtual void generate_expr(std::ostream &out) const;
};


