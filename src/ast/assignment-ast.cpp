#include "assignment-ast.h"

#include <cassert>

AssignmentAST::AssignmentAST(
	Token::ConstIt &begin,
	Token::ConstIt end,
	const Context &context
) {
	begin++;
	lhs = ExprAST::parse_value(begin, end, context);
	rhs = ExprAST::parse_value(begin, end, context);

	assert(lhs->get_result_category() == ResultCategory::LValue);
	assert(lhs->get_result_type()->assignable(*rhs->get_result_type()));

	resultCat = ResultCategory::LValue;
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
