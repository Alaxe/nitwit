#include "unary-operator-ast.h"

#include <cassert>

const PrimitiveType* UnaryOperatorAST::operand_type() const {
	return dynamic_cast<const PrimitiveType*>(operand->get_result_type());
}

UnaryOperatorAST::UnaryOperatorAST(
	Token::ConstIt &begin,
	Token::ConstIt end, 
	const Context &context
) {
	oprData = OperatorData::get(begin->type);
	assert(oprData != nullptr); // shouldn't trigger
	begin++;

	operand = ExprAST::parse_value(begin, end, context);
	assert(operand_type() != nullptr);

	if (!oprData->takeFloat) {
		assert(!operand_type()->isFloat);
	}

	resultCat = ResultCategory::LValue;
	if (oprData->resCommon) {
		resultType = operand_type();
	} else {
		resultType = context.get_type(PrimitiveType::defaultInt);
	}
}
void UnaryOperatorAST::generate_expr(std::ostream &out) const {
	out << "(";
	resultType->c_name(out);
	out << ") (";

	out << oprData->cPrefix << "(";
	operand->generate_expr(out);
	out << ")" << oprData->cSuffix;
	
	out << ")";
}
