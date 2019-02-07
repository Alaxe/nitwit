#pragma once

#include "expr-ast.h"
#include "../function-data.h"

#include <vector>

class FunctionCallAST : public ExprAST {
protected:
	const FunctionData *func;
	std::vector<ExprAST::UPtr> args;

	void gen_arg_pass(std::ostream &out, uint32_t argI) const;

public:
	FunctionCallAST(	
		Token::ConstIt &begin,
		Token::ConstIt end,
		const Context &context
	);
	void generate_expr(std::ostream &out) const;
};
