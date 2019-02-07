#include "comparison-ast.h"

#include <cassert>

void ComparisonAST::generate_arg(std::ostream &out, const ExprAST *arg) const {
	out << "(";
	const auto *npT = dynamic_cast<const NonPrimitiveType*> (
		arg->get_result_type()
	);
	if ((npT) && (npT->weak())) {
		npT->c_norm_wval_name(out);
		out << "(";
	}
	arg->generate_expr(out);

	if ((npT) && (npT->weak())) {
		out << ")";
	}
	out << ")";
}

ComparisonAST::ComparisonAST(
	Token::ConstIt &begin,
	Token::ConstIt end,
	const Context &context
) {
	if (begin->type == TokenType::Equal) {
		negate = false;
	} else if (begin->type == TokenType::NotEqual) {
		negate = true;
	} else {
		assert(false);
	}

	begin++;
	assert(begin != end);
	a1 = ExprAST::parse(begin, end, context);
	assert(begin != end);
	a2 = ExprAST::parse(begin, end, context);
	assert (a1->get_result_type()->assignable(*a2->get_result_type()));

	resultType = context.get_type(PrimitiveType::defaultInt);
	lvalue = false;
}
void ComparisonAST::generate_expr(std::ostream &out) const {
	generate_arg(out, a1.get());
	if (negate) {
		out << "!=";
	} else {
		out << "==";
	}
	generate_arg(out, a2.get());
}


