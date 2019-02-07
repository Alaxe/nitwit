#pragma once

#include "expr-ast.h"

class IndexAST : public ExprAST {
protected:
	ExprAST::UPtr arr;
	ExprAST::UPtr ind;

	const ArrayType* arr_t() const;
public:
	IndexAST(
		Token::ConstIt &begin,
		Token::ConstIt end,
		const Context &context
	);
	void generate_expr(std::ostream &out) const override;
};
