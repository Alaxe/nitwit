#include "variable-ast.h"

#include <cassert>
#include <iostream>

VariableAST::VariableAST(const Token &tok, const Context &context) {
	std::cerr << "var ast: " << tok << "\n";
	assert(tok.type == TokenType::Identifier);

	const auto *varDataRaw = context.get_variable(tok.s);
	assert(varDataRaw);
	varData = VarData::UPtr(new VarData(*varDataRaw));
	assert(varData.get());

	lvalue = true;
	resultType = &varData->type;
}
void VariableAST::generate_expr(std::ostream &out) const {
	varData->c_name(out);
}
