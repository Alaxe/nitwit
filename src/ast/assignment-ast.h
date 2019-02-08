#pragma once
#include "expr-ast.h"

class AssignmentAST : public ExprAST {
protected:
	ExprAST::UPtr lhs;
	ExprAST::UPtr rhs;

	void init();
public:
	typedef std::unique_ptr<AssignmentAST> UPtr;

	AssignmentAST(ExprAST::UPtr lhs, ExprAST::UPtr rhs);
	AssignmentAST(
		Token::ConstIt &begin,
		Token::ConstIt end,
		const Context &context
	);

	virtual void generate_expr(std::ostream &out) const;

	static UPtr parse_declaration(const Line &l, Context &context);
};
