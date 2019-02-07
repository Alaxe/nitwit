#pragma once

#include "expr-ast.h"

class AllocationAST : public ExprAST {
protected:
	ExprAST::UPtr sizeArg;
public:
	AllocationAST(
		Token::ConstIt &begin,
		Token::ConstIt end,
		const Context &context
	);

	void generate_expr(std::ostream &out) const override;
};
