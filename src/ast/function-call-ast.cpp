#include "function-call-ast.h"

#include <cassert>

FunctionCallAST::FunctionCallAST(	
	Token::ConstIt &begin,
	Token::ConstIt end,
	const Context &context
) {
	begin++; //eat @ token
	assert (begin != end);
}

virtual void generate_expr(std::ostream &out) const;

