#include "index-ast.h"

#include <cassert>

const ArrayType* IndexAST::arr_t() const {
	return dynamic_cast<const ArrayType*>(arr->get_result_type());
}

IndexAST::IndexAST(
	Token::ConstIt &begin,
	Token::ConstIt end,
	const Context &context
) {
	begin++;
	assert(begin != end);
	arr = ExprAST::parse(begin, end, context);
	assert(arr_t());

	assert(begin != end);
	ind = ExprAST::parse(begin, end, context);
	const auto *indT = dynamic_cast<const PrimitiveType*>(
		ind->get_result_type()
	);
	assert(indT);
	assert(!indT->isFloat);

	resultType = &arr_t()->elementT;
	lvalue = arr->is_lvalue();
}
void IndexAST::generate_expr(std::ostream &out) const {
	arr_t()->c_index_name(out, arr->is_lvalue());
	out << "(";
	arr->generate_expr(out);
	out << ", ";
	ind->generate_expr(out);
	out << ")";
}
