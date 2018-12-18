#include "context.h"

#include <iostream>
#include <cassert>

GlobalContext::GlobalContext() {}
GlobalContext::GlobalContext(const std::vector<Line> &lines) {
	for (const Line &l : lines) {
		if (l.indent > 0) {
			continue;
		}
		const TokenType &tt = l.tokens[0].type;
		if (tt == TokenType::DefFunc) {
			if ((l.tokens.size() < 3) || (l.tokens.size() % 2 == 0)) {
				std::cerr << "Invalid number of tokens for ";
				std::cerr << "func def on line ";
				std::cerr << l.lineInd << "\n";

				assert(false);
			}
			declare_function(
				l.tokens[2].s,
				FunctionT((l.tokens.size() - 3) / 2)
			);

			std::cerr << "gfunc " << l.tokens[2].s << " ";
			std::cerr << (l.tokens.size() - 3) / 2 << "\n";

		} else if ((tt == TokenType::DefVar) || (tt == TokenType::DefWeak)) {
			if (l.tokens.size() < 3) {
				std::cerr << "Invalid number of args for var ";
				std::cerr << "def on line " << l.lineInd;
				std::cerr << "\n"; 

				assert(false);
			} else if (l.tokens.size() > 3) {
				std::cerr << "Trailing tokens after var ";
				std::cerr << "declaration on line ";
				std::cerr << l.lineInd << "\n";

				assert(false);
			}
			assert(l.tokens[2].type == TokenType::Identifier);

			declare_variable(l.tokens[2].s, TypeT());

			std::cerr << "gvar " << l.tokens[2].s << "\n";
		}
	}
}
void GlobalContext::declare_function(const std::string &name,
	const FunctionT &funcT) {

	assert(functions.find(name) == functions.end());
	functions[name] = funcT;
}
void GlobalContext::declare_variable(const std::string &name, const TypeT &type) {
	assert(variables.find(name) == variables.end());
	variables[name] = type;
}
const FunctionT* GlobalContext::get_function(const std::string &name) const {
	auto it = functions.find(name);

	if (it != functions.end()) {
		return &it->second;
	} else {
		return nullptr;
	}
}
const TypeT* GlobalContext::get_variable(const std::string &name) const {
	auto it = variables.find(name);

	if (it != variables.end()) {
		return &it->second;
	} else {
		return nullptr;
	}
}

Context::Context(const GlobalContext &gc): gc(gc) {}
void Context::declare_variable(const std::string &name, const TypeT &type) {
	assert(variables.find(name) == variables.end());
	variables[name] = type;
}

const FunctionT* Context::get_function(const std::string &name) const {
	return gc.get_function(name);
}
const TypeT* Context::get_variable(const std::string &name) const {
	auto it = variables.find(name);

	if (it != variables.end()) {
		return &it->second;
	} else {
		return gc.get_variable(name);
	}
}
