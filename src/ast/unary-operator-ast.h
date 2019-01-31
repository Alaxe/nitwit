#pragma once

#include "expr-ast.h"
#include "../operator-data.h"

class UnaryOperatorAST : public ExprAST {
protected:
	const OperatorData *oprData;
	ExprAST::UPtr operand;

	const PrimitiveType* operand_type() const;
public:
	UnaryOperatorAST(
		Token::ConstIt &begin,
		Token::ConstIt end, 
		const Context &context
	);

	virtual void generate_expr(std::ostream &out) const;
};
