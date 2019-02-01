#include "read-ast.h"
#include <cassert>

const PrimitiveType* ReadAST::operand_type() const {
	return dynamic_cast<const PrimitiveType*> (operand.get());
}
ReadAST::ReadAST(
	Token::ConstIt &begin,
	Token::ConstIt end,
	const Context &context
) {
	begin++;
	operand = ExprAST::parse(begin, end, context);
	assert(operand_type());
	assert(operand->is_lvalue());

	lvalue = false;
	resultType = context.get_type(PrimitiveType::defaultInt);
}
void ReadAST::generate_expr(std::ostream &out) const {
	operand_type()->c_read_function(out);
	out << "(";
	operand->generate_expr(out);
	out << ")";
}
