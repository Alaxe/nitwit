#pragma once

#include "expr-ast.h"
#include "../operator-data.h"

class BinaryOperatorAST : public ExprAST {
protected:
	const OperatorData *oprData;
	ExprAST::UPtr lhs, rhs;

	const PrimitiveType *l_type() const;
	const PrimitiveType *r_type() const;
	const PrimitiveType *common_type() const;

public:
	BinaryOperatorAST(
		Token::ConstIt &begin,
		Token::ConstIt end, 
		const Context &context
	);

	virtual void generate_expr(std::ostream &out) const;
};
