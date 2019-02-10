#pragma once

#include "expr-ast.h"

class MemberAST : public ExprAST {
protected:
	ExprAST::UPtr obj;
	std::string memberName;
public:
	MemberAST(
		Token::ConstIt &begin,
		Token::ConstIt end,
		const Context &context
	);

	void generate_expr(std::ostream &out) const override;
};
