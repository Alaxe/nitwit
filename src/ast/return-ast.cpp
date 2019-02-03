#include "return-ast.h"
#include <cassert>

ReturnAST::ReturnAST(const Line &l, const Context &context) {
	const auto& tok = l.tokens;
	const TypeT& returnT = context.functionData.returnT;

	if (dynamic_cast<const VoidType*>(&returnT)) {
		returnVal = nullptr;
		return;
	}

	auto it = tok.begin() + 1;
	assert(it != tok.end());
	returnVal = ExprAST::parse(it, tok.end(), context);
	assert(it == tok.end());
	assert(returnT.assignable(*returnVal->get_result_type()));
}
void ReturnAST::generate_c(std::ostream &out) const {
	out << "return";
	if (returnVal != nullptr) {
		out << " ";
		returnVal->generate_expr(out);
		out << ";\n";
	}
}

