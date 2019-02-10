#include "member-ast.h"

#include <cassert>

MemberAST::MemberAST(
	Token::ConstIt &begin,
	Token::ConstIt end,
	const Context &context
) {
	obj = ExprAST::parse(++begin, end, context);
	assert(obj->is_lvalue());

	const auto *structT = dynamic_cast<const StructType*> (
		obj->get_result_type()
	);
	assert(structT);

	assert(begin != end);
	assert(begin->type == TokenType::Identifier);
	memberName = begin->s;
	begin++;

	auto it = structT->data.members.find(memberName);
	assert (it != structT->data.members.end());

	lvalue = true;
	resultType = it->second;
}

void MemberAST::generate_expr(std::ostream &out) const {
	out << "(";
	obj->generate_expr(out);
	out << ")->";
	StructType::c_member_name(out, memberName);
}
