#pragma once

#include "expr-ast.h"
#include "../function-data.h"

#include <vector>

class FunctionCallAST : public ExprAST {
protected:
	const FunctionData *func;
	std::vector<ExprAST::UPtr> args;

public:
	FunctionCallAST(	
		Token::ConstIt &begin,
		Token::ConstIt end,
		const Context &context
	);
	virtual void generate_expr(std::ostream &out) const;
};
