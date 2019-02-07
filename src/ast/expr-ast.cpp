#include "../ast.h"

#include <cassert>

bool ExprAST::is_lvalue() const {
	return lvalue;
}
const TypeT* ExprAST::get_result_type() const {
	return resultType;
}

void ExprAST::generate_c(std::ostream &out) const {
	generate_expr(out);
	out << ";\n";
}
ExprAST::UPtr ExprAST::parse(
	Token::ConstIt &begin,
	Token::ConstIt end,
	const Context &context
) {
	assert(begin != end);
	const Token &token = *begin;
	const TokenType &tt = token.type;

	ExprAST* ans = nullptr;

	if (tt & TokenType::Literal) {
		ans = new LiteralAST(token, context);
		begin++;
	} else if (tt == TokenType::Identifier) {
		ans = new VariableAST(token, context);
		begin++;
	} else if (tt & TokenType::Operator) {
		if (tt & TokenType::BinOpr) {
			ans = new BinaryOperatorAST(begin, end, context);
		} else if (tt & TokenType::UnOpr) {
			ans = new UnaryOperatorAST(begin, end, context);
		} else if (tt & TokenType::AugOpr) {
			assert((tt & TokenType::AugOpr) == 0);
		} else if (tt == TokenType::Assign) {
			ans = new AssignmentAST(begin, end, context);
		} else if (tt == TokenType::FuncCall) {
			ans = new FunctionCallAST(begin, end, context);
		} else if (tt == TokenType::Read) {
			ans = new ReadAST(begin, end, context);
		} else if (tt == TokenType::Write) {
			ans = new WriteAST(begin, end, context);
		} else if (tt == TokenType::Alloc) {
			ans = new AllocationAST(begin, end, context);
		} else if (tt == TokenType::Index) {
			ans = new IndexAST(begin, end, context);
		} else if (
			(tt == TokenType::Equal)
			|| (tt == TokenType::NotEqual)
		) {
			ans = new ComparisonAST(begin, end, context);
		} else if (tt == TokenType::ArrSize) {
			ans = new ArraySizeAST(begin, end, context);
		}
	} else {
		assert(false);
	}
	return ExprAST::UPtr(ans);
}

ExprAST::UPtr ExprAST::parse_condition(
	Token::ConstIt &begin,
	Token::ConstIt end,
	const Context &context
) {
	auto ans = parse(begin, end, context);

	const PrimitiveType *pt = dynamic_cast<const PrimitiveType*>(
		ans->resultType
	);
	assert(pt);
	assert(!pt->isFloat);

	return std::move(ans);
}
