#include "function-call-ast.h"

#include <cassert>

FunctionCallAST::FunctionCallAST(
	Token::ConstIt &begin,
	Token::ConstIt end,
	const Context &context
) {
	begin++; //eat @ token

	assert (begin != end);
	const Token &nameTok = *begin;
	assert(nameTok.type == TokenType::Identifier);
	func = context.get_function(nameTok.s);
	assert(func != nullptr);

	begin++;

	for (const auto &i : func->args) {
		ExprAST::UPtr a = ExprAST::parse(begin, end, context);
		assert(i.first.assignable(*a->get_result_type()));
		args.push_back(std::move(a));
	}
	lvalue = false;
	resultType = &func->returnT;
}

void FunctionCallAST::generate_expr(std::ostream &out) const {
	func->c_name(out);
	out << "(";
	if (!args.empty()) {
		args[0]->generate_expr(out);
		for (uint32_t i = 1;i < args.size();i++) {
			out << ", ";
			args[i]->generate_expr(out);
		}
	}
	out << ")";
}
