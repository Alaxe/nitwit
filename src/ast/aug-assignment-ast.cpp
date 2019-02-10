#include "aug-assignment-ast.h"

#include <cassert>

AugAssignmentAST::AugAssignmentAST(
	Token::ConstIt &begin,
	Token::ConstIt end,
	const Context &context
) {
	oprData = OperatorData::get(begin->type);
	assert(oprData);
	begin++;

	lhs = ExprAST::parse(begin, end, context);
	rhs = ExprAST::parse(begin, end, context);

	assert(lhs->is_lvalue());
	const auto *lType = dynamic_cast<const PrimitiveType*>(
		lhs->get_result_type()
	);
	const auto *rType = dynamic_cast<const PrimitiveType*>(
		rhs->get_result_type()
	);

	assert(lType);
	assert(rType);

	if (!oprData->takeFloat) {
		assert(!lType->isFloat);
		assert(!rType->isFloat);
	}

	lvalue = true;
	resultType = lhs->get_result_type();
}

void AugAssignmentAST::generate_expr(std::ostream &out) const {
	out << "(";
	lhs->generate_expr(out);
	out << ")" << oprData->cInfix << "(";
	rhs->generate_expr(out);
	out << ")";
}
