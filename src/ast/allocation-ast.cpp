#include "allocation-ast.h"

#include <cassert>

AllocationAST::AllocationAST(
	Token::ConstIt &begin,
	Token::ConstIt end,
	const Context &context
) {
	begin++;
	assert(begin != end);
	assert(begin->type == TokenType::Identifier);

	lvalue = false;
	resultType = context.get_type(begin->s);
	assert(resultType);
	assert(dynamic_cast<const NonPrimitiveType*> (resultType));
	begin++;

	const auto *arrPtr = dynamic_cast<const ArrayType*> (resultType);
	if (arrPtr != nullptr) {
		sizeArg = ExprAST::parse(begin, end, context);
		const auto *argT = sizeArg->get_result_type();
		const auto *argPrim = dynamic_cast<const PrimitiveType*>(argT);
		assert(argPrim);
		assert(!argPrim->isFloat);
	} else {
		sizeArg = nullptr;
	}
}

void AllocationAST::generate_expr(std::ostream &out) const {
	const auto *tPtr = dynamic_cast<const NonPrimitiveType*>(resultType);
	tPtr->c_alloc_name(out);
	out << "(";
	if (sizeArg) {
		sizeArg->generate_expr(out);
	}
	out << ")";
}
