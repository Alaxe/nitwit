#pragma once

#include "expr-ast.h"
#include "../operator-data.h"

class BinaryOperatorAST : public ExprAST {
protected:
	const OperatorData *oprData;
	ExprAST::UPtr lhs, rhs;

public:
	BinaryOperatorAST(
		Token::ConstIt &begin,
		Token::ConstIt end, 
		const Context &context
	);

	virtual void generate_expr(std::ostream &out) const;
};
