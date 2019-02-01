#include "variable-ast.h"

#include <cassert>

VariableAST::VariableAST(const Token &tok, const Context &context) {
	assert(tok.type == TokenType::Identifier);
	varData = context.get_variable(tok.s);
	assert(varData);

	lvalue = true;
	resultType = &varData->type;
}
void VariableAST::generate_expr(std::ostream &out) const {
	out << varData->name;
}

