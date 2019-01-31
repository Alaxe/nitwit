#pragma once
#include "expr-ast.h"

class AssignmentAST : public ExprAST {
protected:
	ExprAST::UPtr lhs, rhs;
public:
	AssignmentAST(
		Token::ConstIt &begin,
		Token::ConstIt end,
		const Context &context
	);

	virtual void debug_print() const;
	virtual void generate_expr(std::ostream &out) const;
};
