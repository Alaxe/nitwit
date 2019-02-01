#pragma once

#include "expr-ast.h"

class WriteAST : public ExprAST {
protected:
	ExprAST::UPtr operand;
	const PrimitiveType* operand_type() const;
public:
	WriteAST(
		Token::ConstIt &begin,
		Token::ConstIt end,
		const Context &context
	);

	virtual void generate_expr(std::ostream &out) const;
};
