#include "if-ast.h"
#include <cassert>

IfAST::IfAST(Line::ConstIt &begin, Line::ConstIt end, Context &context) {
	TokenType nextIf = TokenType::If;
	uint32_t indent = begin->indent;
	while (begin != end) {
		const auto &tok = begin->tokens;
		if ((tok[0].type != nextIf) || (begin->indent < indent)) {
			break;
		}
		auto it = tok.begin() + 1;
		ExprAST::UPtr condition = ExprAST::parse_condition(
			it,
			tok.end(),
			context
		);
		assert(it == tok.end());
		uint32_t indent = begin->indent + 1;
		begin++;
		BlockAST::UPtr body(new BlockAST(
			begin,
			end,
			context,
			indent
		));
		ifPairs.push_back({std::move(condition), std::move(body)});
		nextIf = TokenType::Elif;
	}
	if (
		(begin != end)
		&& (begin->tokens[0].type == TokenType::Else)
		&& (begin->indent == indent)
	) {
		uint32_t indent = begin->indent + 1;
		begin++;
		elseBody = BlockAST::UPtr(new BlockAST(
			begin,
			end,
			context,
			indent
		));
	}

}

void IfAST::generate_c(std::ostream &out) const {
	generate_c(out, 0);
}
void IfAST::generate_c(std::ostream &out, uint32_t indent) const {
	std::string indentS = std::string(indent, ' ');
	bool first = true;
	for (const IfPair &p : ifPairs) {
		out << indentS;
		if (!first) {
			out << "else ";
		} 
		out << "if (";
		p.first->generate_expr(out);
		out << ")\n";
		p.second->generate_c(out, indent);

		first = false;
	}
	if (elseBody) {
		out << indentS;
		out << "else\n";
		elseBody->generate_c(out, indent);
	}
	out << "\n";
}
