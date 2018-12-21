#include "function.h"

#include <cassert>
#include <iostream>

Function::Function(
	std::vector<Line>::const_iterator begin,
	std::vector<Line>::const_iterator end,
	const GlobalContext &globalContext
): proto(*begin), context(globalContext) {

	for (const auto &i : proto.args) {
		context.declare_variable(i.second, i.first);
	}

	bool bodyStarted = false;
	for (auto lineIt = begin + 1;lineIt != end;lineIt++) {
		auto tok = lineIt[0].tokens;
		if (tok[0].type == TokenType::DefVar) {
			assert(tok.size() == 3);
			assert(!bodyStarted);

			std::unique_ptr<VarDefAST> ptr(
				new VarDefAST(tok[1], tok[2])
			);

			ptr->add_to_context(context);
			statements.push_back(std::move(ptr));
		} else if (tok[0].type == TokenType::Return) {
			auto expr = ExprAST::parse(
				tok.begin() + 1,
				tok.end(),
				context
			);

			assert(expr.size() == 1);

			std::unique_ptr<ReturnAST> ptr(
				new ReturnAST(std::move(expr[0]))
			);

			statements.push_back(std::move(ptr));
			bodyStarted = true;
		} else {
			auto expr = ExprAST::parse(
				tok.begin(),
				tok.end(),
				context
			);

			for (auto &i : expr) {
				std::unique_ptr<StatementAST> ptr(
					static_cast<StatementAST*>(i.release())
				);
				statements.push_back(std::move(ptr));
			}

			bodyStarted = true;
		}
		//statements.back()->debug_print();
		//std::cerr << "-----------------" << std::endl;
	}
}

void Function::generate_c(std::ostream &out) const {
	proto.generate_c(out);
	out << " {\n";
	context.generate_c(out);

	for (const auto &i : statements) {
		i->generate_c(out);
	}
	out << "}\n";
}

std::vector<Function> Function::parse_all(
		const std::vector<Line> &code,
		const GlobalContext &globalContext) {

	std::vector<Function> functions;
	for (uint32_t i = 0;i < code.size();i++) {
		assert(code[i].indent == 0);
		if (code[i].tokens[0].type == TokenType::DefVar) {
			continue;
		}
		assert(code[i].tokens[0].type == TokenType::DefFunc);
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
