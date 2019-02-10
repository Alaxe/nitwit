#include "while-loop-ast.h"

#include <cassert>

WhileLoopAST::WhileLoopAST(
	Line::ConstIt &begin,
	Line::ConstIt end,
	Context &context
) {
	auto exprIt = begin->tokens.begin() + 1;
	condition = ExprAST::parse_condition(exprIt, begin->tokens.end(), context);
	assert(exprIt == begin->tokens.end());

	uint32_t indent = begin->indent;
	body = BlockAST::UPtr(new BlockAST(
		++begin,
		end,
		context,
		indent + 1
	));
}

void WhileLoopAST::generate_c(std::ostream &out) const {
	generate_c(out, 0);
}
void WhileLoopAST::generate_c(std::ostream &out, uint32_t indent) const {
	out << std::string(indent, ' ') << "while (";
	condition->generate_expr(out);
	out << ")\n";
	body->generate_c(out, indent);
}

