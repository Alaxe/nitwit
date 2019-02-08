#include "assignment-ast.h"
#include "literal-ast.h"
#include "variable-ast.h"

#include <cassert>
#include <iostream>

void AssignmentAST::init() {
	assert(lhs->is_lvalue());
	assert(lhs->get_result_type()->assignable(*rhs->get_result_type()));

	resultType = lhs->get_result_type();
	lvalue = true;
}

AssignmentAST::AssignmentAST(ExprAST::UPtr lhs, ExprAST::UPtr rhs):
	lhs(std::move(lhs)), rhs(std::move(rhs))
{
	init();
}
AssignmentAST::AssignmentAST(
	Token::ConstIt &begin,
	Token::ConstIt end,
	const Context &context
):
	lhs(ExprAST::parse(++begin, end, context)),
	rhs(ExprAST::parse(begin, end, context))
{
	init();
}

void AssignmentAST::generate_expr(std::ostream &out) const {
	if (dynamic_cast<const PrimitiveType*>(lhs->get_result_type())) {
		out << "(";
		lhs->generate_expr(out);
		out << ") = (";
		rhs->generate_expr(out);
		out << ")";
	} else if (dynamic_cast<const NonPrimitiveType*>(
		lhs->get_result_type()
	)) {
		const auto *lType = dynamic_cast<const NonPrimitiveType*>(
			lhs->get_result_type()
		);
		const auto *rType = dynamic_cast<const NonPrimitiveType*>(
			rhs->get_result_type()
		);


		lType->c_assign_name(out, rhs->is_lvalue());
		out << "(&";
		lhs->generate_expr(out);
		out << ", ";
		if ((rType) && (rType->weak())) {
			rType->c_norm_wval_name(out);
			out << "(";
		}
		rhs->generate_expr(out);
		if ((rType) && (rType->weak())) {
			out << ")";
		}
		out << ")";
	} else {
		assert(false);
	}
}

AssignmentAST::UPtr AssignmentAST::parse_declaration(
	const Line &l,
	Context &context
) {
	const auto &tok = l.tokens;
	assert(tok.size() >= 3);

	assert(tok[1].type == TokenType::Identifier);
	assert(tok[2].type == TokenType::Identifier);

	const TypeT *varT = context.get_type(tok[1].s);
	assert(varT);
	assert(varT->is_declarable());

	ExprAST::UPtr lhs;

	AssignmentAST::UPtr ans;
	if (tok.size() == 3) {
		context.declare_variable(tok[2].s, *varT);
		return nullptr;
	} else {
		auto it = tok.begin() + 3;
		ExprAST::UPtr rhs = ExprAST::parse(it, tok.end(), context);
		assert(it == tok.end());

		context.declare_variable(tok[2].s, *varT);
		return AssignmentAST::UPtr(new AssignmentAST(
			ExprAST::UPtr(new VariableAST(tok[2], context)),
			std::move(rhs)
		));
	}

	context.declare_variable(tok[2].s, *varT);

	if (tok.size() > 3) {
		ans->lhs = ExprAST::UPtr(new VariableAST(tok[2], context));
		ans->resultType = varT;
	}
	return ans;
}
