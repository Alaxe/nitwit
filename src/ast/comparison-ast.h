#pragma once
#include "expr-ast.h"

class ComparisonAST : public ExprAST {
protected:
	ExprAST::UPtr a1, a2;
	bool negate;

	void generate_arg(std::ostream &out, const ExprAST *arg) const;
public:
	ComparisonAST(
		Token::ConstIt &begin,
		Token::ConstIt end,
		const Context &context
	);

	void generate_expr(std::ostream &out) const override;
};
