#include "result-ast.h"
#include "return-ast.h"
#include <cassert>

ReturnAST::ReturnAST(const Line &l, const Context &context) {
	const auto& tok = l.tokens;
	const TypeT& returnT = context.functionData.returnT;
	blockId = context.get_block_id();

	if (dynamic_cast<const VoidType*>(&returnT)) {
		assignment = nullptr;
		return;
	}

	auto it = tok.begin() + 1;
	assert(it != tok.end());
	ExprAST::UPtr returnVal = ExprAST::parse(it, tok.end(), context);
	assert(it == tok.end());
	assert(returnT.assignable(*returnVal->get_result_type()));

	ResultAST::UPtr result = ResultAST::UPtr(new ResultAST(context));
	assignment = AssignmentAST::UPtr(new AssignmentAST(
		std::move(result),
		std::move(returnVal)
	));
}
void ReturnAST::generate_c(std::ostream &out) const {
	generate_c(out, 0);
}
void ReturnAST::generate_c(std::ostream &out, uint32_t indent) const {
	if (assignment != nullptr) {
		assignment->generate_c(out, indent);
	}
	out << std::string(indent, ' ');
	out << "goto ";
	Context::c_return_label(out, blockId);
	out << ";\n";
}
