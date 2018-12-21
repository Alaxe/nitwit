#include "context.h"

#include <iostream>
#include <cassert>

#include "prim-type-data.h"

void GlobalContext::parse_func_declaration(const Line &l) {
	declare_function(FunctionT(l));
}
void GlobalContext::parse_var_declaration(const Line &l) {
	std::cerr << l << "\n";
	if (l.tokens.size() != 3) {
		std::cerr << "Invalid number of args for var ";
		std::cerr << "def on line " << l.lineInd;
		std::cerr << "\n";
		assert(false);
	}
	const Token &typeTok = l.tokens[1];
	const Token &nameTok = l.tokens[2];

	assert(typeTok.type == TokenType::Identifier);
	assert(PrimTypeData::get(typeTok.s));
	assert(nameTok.type == TokenType::Identifier);

	declare_variable(
		nameTok.s,
		TypeT(TypeT::Category::Primitive, typeTok.s)
	);
	//std::cerr << nameTok.s << "\n";

	//std::cerr << "gvar " << l.tokens[2].s << "\n";
}

GlobalContext::GlobalContext() {}
GlobalContext::GlobalContext(const std::vector<Line> &lines) {
	for (const Line &l : lines) {
		if (l.indent > 0) {
			continue;
		}
		const TokenType &tt = l.tokens[0].type;
		if (tt == TokenType::DefFunc) {
			parse_func_declaration(l);
		} else if ((tt == TokenType::DefVar) || (tt == TokenType::DefWeak)) {
			parse_var_declaration(l);
		}
	}
}

void GlobalContext::declare_function(const FunctionT &func) {
	assert(functions.find(func.name) == functions.end());
	functions[func.name] = func;
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
void GlobalContext::generate_c(std::ostream &out) const {
	for (const auto &i : functions) {
		i.second.generate_c(out);
		out << ";\n";
	}

	for (const auto &i : variables) {
		out << i.second.get_c_name() << " " << i.first << ";\n";
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
void Context::generate_c(std::ostream &out) const {
	for (const auto &i : variables) {
		out << "    " << i.second.get_c_name() << " " << i.first << ";\n";
	}
}
