#include "context.h"

#include <iostream>
#include <cassert>

#include "prim-type-data.h"

VarData::VarData() {}
VarData::VarData(const TypeT &type, const std::string &name):
	type(type), name(name) {}
void VarData::generate_c(std::ostream &out) const {
	out << type.get_c_name() << " ";
	out << name << ";\n";
}

std::string GlobalContext::get_c_name(const std::string &name) {
	return "g_" + name;
}
void GlobalContext::parse_func_declaration(const Line &l) {
	declare_function(FunctionT(l));
}
void GlobalContext::parse_var_declaration(const Line &l) {
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
	assert(get_function(func.name) == nullptr);
	assert(get_variable(func.name) == nullptr);
	functions[func.name] = func;
}

void GlobalContext::declare_variable(
	const std::string &name,
	const TypeT &type
) {
	assert(get_function(name) == nullptr);
	assert(get_variable(name) == nullptr);

	variables[name] = VarData(type, get_c_name(name));
}
const FunctionT* GlobalContext::get_function(const std::string &name) const {
	auto it = functions.find(name);

	if (it != functions.end()) {
		return &it->second;
	} else {
		return nullptr;
	}
}
const VarData* GlobalContext::get_variable(const std::string &name) const {
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
		i.second.generate_c(out);
	}
}


Context::VarInst::VarInst() {}
Context::VarInst::VarInst(const VarData &var, const uint32_t &indent):
	var(var), indent(indent) {}

Context::VarStack::VarStack(): nextId(0) {}

std::string Context::get_c_name(const std::string &name, const uint32_t &id) {
	return "l_" + std::to_string(id) + "_" + name;
}

Context::Context(const GlobalContext &gc): curIndent(0), gc(gc) {}
const FunctionT* Context::get_function(const std::string &name) const {
	return gc.get_function(name);
}
void Context::declare_variable(const std::string &name, const TypeT &type) {
	VarStack &vStack = variables[name];
	assert((vStack.st.empty()) || (vStack.st.top().indent < curIndent));
	assert(gc.get_function(name) == nullptr);

	VarData data(type, get_c_name(name, vStack.nextId++));
	if (curIndent > 0) {
		declarations.push_back(data);
	}

	vStack.st.push(VarInst(std::move(data), curIndent));
	jointSt.push({curIndent, name});

}
void Context::update_indent(uint32_t indent) {
	curIndent = indent;
	while ((!jointSt.empty()) && (jointSt.top().first > curIndent)) {
		variables[jointSt.top().second].st.pop();
		jointSt.pop();
	}
}
const VarData* Context::get_variable(const std::string &name) const {
	auto it = variables.find(name);
	if (it == variables.end()) {
		return gc.get_variable(name);
	}
	const VarStack &vStack = it->second;
	if (vStack.st.empty()) {
		return gc.get_variable(name);
	}
	return &vStack.st.top().var;
}
std::vector<VarData> Context::get_declarations() {
	return std::move(declarations);
}
