#include "variable-ast.h"

#include <cassert>

VariableAST::VariableAST(const Token &tok, const Context &context) {
	assert(tok.type == TokenType::Identifier);
	varData = VarData::UPtr(new VarData(*context.get_variable(tok.s)));
	assert(varData.get());

	lvalue = true;
	resultType = &varData->type;
}
void VariableAST::generate_expr(std::ostream &out) const {
	varData->c_name(out);
}

