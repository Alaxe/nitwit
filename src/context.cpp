#include "context.h"

#include <iostream>
#include <cassert>

void Context::declare_function(const std::string &name,
	const FunctionT &funcT) {

	assert(functions.find(name) == functions.end());
	functions[name] = funcT;
}

void Context::declare_global_var(const std::string &name, const TypeT &type) {
	assert(globalVar.find(name) == globalVar.end());
	globalVar[name] = type;
}

void Context::declare_local_var(const std::string &name, const TypeT &type) {
	assert(localVar.find(name) == localVar.end());
	localVar[name] = type;
}
void Context::remove_locals() {
	localVar.clear();
}

const FunctionT* Context::get_function(const std::string &name) const {
	auto it = functions.find(name);

	if (it != functions.end()) {
		return &it->second;
	} else {
		return nullptr;
	}
}
const TypeT* Context::get_variable(const std::string &name) const {
	auto itLocal = localVar.find(name);

	if (itLocal != localVar.end()) {
		return &itLocal->second;
	} else {
		auto itGlobal = globalVar.find(name);
		if (itGlobal != globalVar.end()) {
			return &itGlobal->second;
		} else {
			return nullptr;
		}
	}
}

Context Context::generate_global(const std::vector<Line> &lines) {
	Context context;
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
			context.declare_function(
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

			context.declare_global_var(l.tokens[2].s, TypeT());

			std::cerr << "gvar " << l.tokens[2].s << "\n";
		}
	}
	return context;
}
