#include "function-call-ast.h"

#include <cassert>

void FunctionCallAST::gen_arg_pass(std::ostream &out, uint32_t argI) const {
	const ExprAST* argV = args[argI].get();

	const auto *npT = dynamic_cast<const NonPrimitiveType*>(
		argV->get_result_type()
	);
	if (npT) {
		npT->c_arg_pass_name(out, argV->is_lvalue());
		out << "(";
		if (npT->weak()) {
			npT->c_norm_wval_name(out);
			out << "(";
		}
	}

	argV->generate_expr(out);
	if (npT) {
		if (npT->weak()) {
			out << ")";
		}
		out << ")";
	}
}

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
		assert(i.type.assignable(*a->get_result_type()));
		args.push_back(std::move(a));
	}
	lvalue = false;
	resultType = &func->returnT;
}

void FunctionCallAST::generate_expr(std::ostream &out) const {
	func->c_name(out);
	out << "(";
	if (!args.empty()) {
		gen_arg_pass(out, 0);
		for (uint32_t i = 1;i < args.size();i++) {
			out << ", ";
			gen_arg_pass(out, i);
		}
	}
	out << ")";
}
