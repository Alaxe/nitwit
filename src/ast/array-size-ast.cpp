#include "array-size-ast.h"

#include <cassert>

const ArrayType* ArraySizeAST::get_arr_type() const {
	return dynamic_cast<const ArrayType*> (arr->get_result_type());
}

ArraySizeAST::ArraySizeAST(
	Token::ConstIt &begin,
	Token::ConstIt end,
	const Context &context
) {
	begin++;
	assert(begin != end);
	arr = ExprAST::parse(begin, end, context);
	assert(get_arr_type());

	resultType = context.get_type(PrimitiveType::defaultInt);
	lvalue = false;
}

void ArraySizeAST::generate_expr(std::ostream &out) const {
	out << "(";
	arr->generate_expr(out);
	out << ")->size";
}

