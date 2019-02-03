#include "function.h"

#include <cassert>
#include <iostream>

Function::Function(
	std::vector<Line>::const_iterator begin,
	std::vector<Line>::const_iterator end,
	const GlobalContext &globalContext
) {
	assert(begin->tokens.size() >= 3);
	proto = globalContext.get_function(begin->tokens[2].s);

	Context context(globalContext, *proto);

	for (auto &i : proto->args) {
		context.declare_variable(i.second, i.first);
		i.second = context.get_variable(i.second)->name;
	}

	begin++;
	body = std::unique_ptr<StatementAST>(
		new BlockAST(1, begin, end, context, proto)
	);

	declarations = context.get_declarations();
}

void Function::generate_c(std::ostream &out) const {
	proto.generate_c(out);
	out << " {\n";
	for (const VarData &i : declarations) {
		out << "    ";
		i.generate_c(out);
	}

	body->generate_c(out);
	out << "}\n";
}

std::vector<Function> Function::parse_all(
		const std::vector<Line> &code,
		const GlobalContext &globalContext) {

	std::vector<Function> functions;
	for (uint32_t i = 0;i < code.size();i++) {
		assert(code[i].indent == 0);
		if (code[i].tokens[0].type == TokenType::VarDef) {
			continue;
		}
		assert(code[i].tokens[0].type == TokenType::FuncDef);
		uint32_t j;
		for (j = i + 1;j < code.size();j++) {
			if (code[j].indent == 0) {
				break;
			}
		}
		functions.push_back(
			Function(code.begin() + i, code.begin() + j, globalContext)
		);
		i = j - 1;
	}
	return functions;
}
