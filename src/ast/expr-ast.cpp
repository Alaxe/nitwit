#include "../ast.h"

#include <cassert>

void ExprAST::generate_c(std::ostream &out) const {
	generate_expr(out);
	out << ";\n";
}
ExprAST::UPtr ExprAST::parse(
	Token::ConstIt &begin,
	Token::ConstIt end,
	const Context &context
) {
	if (begin == end) {
		return nullptr;
	}
	const Token &tok = *begin;
	begin++;

	if (tok.type & TokenType::Literal) {
		return new LitAST(begin, end, context);
	}

	std::vector<std::unique_ptr<ExprAST>> stack;
	for (auto it = end - 1;it + 1 != begin;it--) {
		const Token &tok = *it;
		switch (tok.type) {
		case TokenType::LitInt:
		case TokenType::LitFloat:
			stack.emplace_back(new LitAST(tok));
			break;
		case TokenType::Identifier:
			stack.emplace_back(new IdentifierAST(tok, context));
			break;
		default:
			assert(tok.type & TokenType::Operator);
			const OperatorData* oprData = OperatorData::get(tok.s);

			if (oprData != nullptr) {
				if (oprData->arity == 2) {
					stack.emplace_back(new BinOperatorAST(oprData, stack));
				} else {
					assert(oprData->arity == 2);
				}
			} else if (tok.s == "@") {
				stack.emplace_back(new FunctionCallAST(context, stack));
			} else if (tok.s == "=") {
				stack.emplace_back(new AssignmentAST(stack));
			} else if (tok.s == ">>") {
				stack.emplace_back(new InputAST(stack));
			} else if (tok.s == "<<") {
				stack.emplace_back(new OutputAST(stack));
			}
		}
	}

	std::reverse(stack.begin(), stack.end());
	return stack;
}
