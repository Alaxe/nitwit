#include "literal-ast.h"

#include <cassert>

const std::string LiteralAST::maxInt = std::to_string(INT64_MAX);

bool LiteralAST::int_lit_fits() const {
	if (val.size() != maxInt.size()) {
		return val.size() < maxInt.size();
	} else {
		return val <= maxInt;
	}
}
LiteralAST::LiteralAST(const Token &t, const Context &context) {
	val = t.s;
	lvalue = false;

	if (t.type == TokenType::LitInt) {
		assert(int_lit_fits());
		resultType = context.get_type(PrimitiveType::maxInt);
	} else if (t.type == TokenType::LitFloat) {
		resultType = context.get_type(PrimitiveType::maxFloat);
	} else if (t.type == TokenType::LitNull) {
		resultType = context.get_null_type();
	} else {
		assert(false);
	}
}

void LiteralAST::generate_expr(std::ostream &out) const {
	if (dynamic_cast<const PrimitiveType*>(resultType)) {
		out << val << "LL";
	} else if (dynamic_cast<const NullType*>(resultType)) {
		out << "NULL";
	}
}
