#include "binary-operator-ast.h"

#include <cassert>

const PrimitiveType* BinaryOperatorAST::l_type() const {
	return dynamic_cast<const PrimitiveType*>(lhs->get_result_type());
}
const PrimitiveType* BinaryOperatorAST::r_type() const {
	return dynamic_cast<const PrimitiveType*>(rhs->get_result_type());
}
const PrimitiveType* BinaryOperatorAST::common_type() const {
	return &l_type()->common_type(*r_type());
}


BinaryOperatorAST::BinaryOperatorAST(
	Token::ConstIt &begin,
	Token::ConstIt end, 
	const Context &context
) {
	oprData = OperatorData::get(begin->type);
	assert(oprData != nullptr); // shouldn't trigger
	begin++;

	lhs = ExprAST::parse(begin, end, context);
	assert(l_type() != nullptr);

	rhs = ExprAST::parse(begin, end, context);
	assert(r_type() != nullptr);

	if (!oprData->takeFloat) {
		assert(!l_type()->isFloat);
		assert(!r_type()->isFloat);
	}

	lvalue = false;
	if (oprData->resCommon) {
		resultType = common_type();
	} else {
		resultType = context.get_type(PrimitiveType::defaultInt);
	}
}

void BinaryOperatorAST::generate_expr(std::ostream &out) const {
	out << "(";
	resultType->c_name(out);
	out << ") (";

	out << oprData->cPrefix << "(";
	lhs->generate_expr(out);
	out << ")" << oprData->cInfix << "(";
	rhs->generate_expr(out);
	out << ")" << oprData->cSuffix;
	
	out << ")";
}
