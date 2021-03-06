#include "block-ast.h"
#include "../ast.h"
#include <iostream>
#include <cassert>

BlockAST::BlockAST(
	std::vector<Line>::const_iterator& begin,
	std::vector<Line>::const_iterator end,
	Context &context,
	uint32_t indent
): indent(indent) {
	parId = context.get_block_id();
	context.start_block();
	id = context.get_block_id();

	while (begin != end) {
		const auto &tok = begin->tokens;
		if (begin->indent < indent) {
			break;
		} else if (begin->indent > indent) {
			statements.emplace_back(new BlockAST(
				begin,
				end,
				context,
				begin->indent
			));
			continue;
		} else if (tok[0].type & TokenType::Operator) {
			auto it = tok.begin();
			while (it != tok.end()) {
				statements.push_back(
					ExprAST::parse(it, tok.end(), context)
				);
			}
			begin++;
		} else if (tok[0].type == TokenType::Return) {
			statements.emplace_back(new ReturnAST(*begin, context));
			begin++;
		} else if (tok[0].type == TokenType::If) {
			statements.emplace_back(new IfAST(begin, end, context));
		} else if (tok[0].type == TokenType::While) {
			statements.emplace_back(
				new WhileLoopAST(begin, end, context)
			);
		} else if (tok[0].type == TokenType::VarDef) {
			auto sPtr = AssignmentAST::parse_declaration(
				*begin,
				context
			);
			if (sPtr.get()) {
				statements.push_back(std::move(sPtr));
			}
			begin++;
		} else {
			assert(false);
		}
	}
	declarations = context.end_block();
}

void BlockAST::c_generate_gc(std::ostream &out) const {
	std::string indentS = std::string(indent, ' ');

	VarData::c_generate_destructors(out, declarations, indent);

	out << indentS << "goto "; 
	Context::c_end_label(out, id);
	out << ";\n";

	out << indentS;
	Context::c_return_label(out, id);
	out << ":\n";

	VarData::c_generate_destructors(out, declarations, indent);

	out << indentS << "goto "; 
	Context::c_return_label(out, parId);
	out << ";\n";

	out << indentS;
	Context::c_end_label(out, id);
	out << ":;\n";
}

void BlockAST::generate_c(std::ostream &out, uint32_t parIndent) const {
	std::string indentS = std::string(parIndent, ' ');
	out << indentS << "{\n";

	for (const auto &i : declarations) {
		i.c_declaration(out, indent);
	}
	for (const auto &i : statements) {
		i->generate_c(out, indent);
	}
	c_generate_gc(out);

	out << indentS << "}\n";
}
void BlockAST::generate_c(std::ostream &out) const {
	generate_c(out, 0);
}
