#include "function.h"

#include <cassert>
#include <iostream>

Function::Function(
	Line::ConstIt &begin,
	Line::ConstIt end,
	const GlobalContext &globalContext
) {
	assert(begin->tokens.size() >= 3);
	proto = globalContext.get_function(begin->tokens[2].s);

	Context context(globalContext, *proto);

	begin++;
	body = std::unique_ptr<StatementAST>(
		new BlockAST(begin, end, context, 1)
	);
}

void Function::generate_c(std::ostream &out) const {
	proto->c_prototype(out);
	out << "\n{\n";

	body->generate_c(out);
	out << "}\n";
}

std::vector<Function> Function::parse_all(
	Line::ConstIt begin,
	Line::ConstIt end,
	const GlobalContext &globalContext
) {

	std::vector<Function> functions;
	while (begin != end) {
		if (
			(begin->indent > 0)
			|| (begin->tokens[0].type != TokenType::FuncDef)
		) {
			begin++;
			continue;
		}
		functions.emplace_back(begin, end, globalContext);
	}
	return functions;
}
