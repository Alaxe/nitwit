#include "literal-ast.h"

#include <cassert>

LiteralAST::LiteralAST(const Token &t, const Context &context) {
	val = t.s;
	lvalue = false;

	if (t.type == TokenType::LitInt) {
		resultType = context.get_type(PrimitiveType::maxInt);
	} else if (t.type == TokenType::LitFloat) {
		resultType = context.get_type(PrimitiveType::maxFloat);
	} else if (t.type == TokenType::LitNull) {
		assert(false);
	} else {
		assert(false);
	}
}

void LiteralAST::generate_expr(std::ostream &out) const {
}

