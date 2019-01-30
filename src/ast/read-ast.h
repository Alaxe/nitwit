#pragma once

#include "expr-ast.h"

class ReadAST : public ExprAST {
protected:
	ExprAST::UPtr operand;
public:
	ReadAST(
		Token::ConstIt &begin,
		Token::ConstIt end,
		const Context &context
	);
	virtual void debug_print() const;
	virtual void generate_expr(std::ostream &out) const;
};
