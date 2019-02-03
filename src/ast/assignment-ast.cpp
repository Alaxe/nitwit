#include "assignment-ast.h"
#include "literal-ast.h"
#include "variable-ast.h"

#include <cassert>

AssignmentAST::AssignmentAST() {
	lvalue = true;
}
AssignmentAST::AssignmentAST(
	Token::ConstIt &begin,
	Token::ConstIt end,
	const Context &context
) {
	begin++;
	lhs = ExprAST::parse(begin, end, context);
	rhs = ExprAST::parse(begin, end, context);

	assert(lhs->is_lvalue());
	assert(lhs->get_result_type()->assignable(*rhs->get_result_type()));

	lvalue = true;
	resultType = lhs->get_result_type();
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
		assert(false);
	} else {
		assert(false);
	}
}

AssignmentAST::UPtr AssignmentAST::parse_declaration(
	const Line &l,
	Context &context
) {
	const auto &tok = l.tokens;
	AssignmentAST::UPtr ans(new AssignmentAST());
	assert(tok.size() >= 3);

	assert(tok[1].type == TokenType::Identifier);
	assert(tok[2].type == TokenType::Identifier);

	const TypeT *varT = context.get_type(tok[1].s);
	assert(varT);
	assert(varT->is_declarable());

	if (tok.size() > 3) {
		auto it = tok.begin() + 3;
		ans->rhs = ExprAST::parse(it, tok.end(), context);
		assert(varT->assignable(*ans->rhs->get_result_type()));
		assert(it == tok.end());
	} else {
		Token t;
		if (dynamic_cast<const PrimitiveType*>(varT)) {
			t.type = TokenType::LitInt;
			t.s = "0";
		} else if (dynamic_cast<const NonPrimitiveType*>(varT)) {
			t.type = TokenType::LitNull;
			t.s = ".";
		} else {
			assert(false);
		}
		ans->rhs = ExprAST::UPtr(new LiteralAST(t, context));
	}

	context.declare_variable(tok[2].s, *varT);
	ans->lhs = ExprAST::UPtr(new VariableAST(tok[2], context));
	ans->resultType = varT;
	return ans;
}
