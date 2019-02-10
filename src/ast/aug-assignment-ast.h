#pragma once
#include "expr-ast.h"
#include "../operator-data.h"

class AugAssignmentAST : public ExprAST {
protected:
	const OperatorData *oprData;
	ExprAST::UPtr lhs;
	ExprAST::UPtr rhs;

public:
	AugAssignmentAST(
		Token::ConstIt &begin,
		Token::ConstIt end,
		const Context &context
	);

	void generate_expr(std::ostream &out) const override;
};
