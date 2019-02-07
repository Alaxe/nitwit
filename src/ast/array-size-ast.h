#pragma once
#include "expr-ast.h"

class ArraySizeAST : public ExprAST {
protected:
	ExprAST::UPtr arr;
	const ArrayType* get_arr_type() const;

public:
	ArraySizeAST(
		Token::ConstIt &begin,
		Token::ConstIt end,
		const Context &context
	);

	void generate_expr(std::ostream &out) const override;
};
