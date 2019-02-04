#include "write-ast.h"
#include <cassert>

const PrimitiveType* WriteAST::operand_type() const {
	return dynamic_cast<const PrimitiveType*> (operand->get_result_type());
}
WriteAST::WriteAST(
	Token::ConstIt &begin,
	Token::ConstIt end,
	const Context &context
) {
	begin++;
	operand = ExprAST::parse(begin, end, context);
	assert(operand_type());

	lvalue = false;
	resultType = context.get_type(PrimitiveType::defaultInt);
}
void WriteAST::generate_expr(std::ostream &out) const {
	operand_type()->c_write_function(out);
	out << "(";
	operand->generate_expr(out);
	out << ")";
}
