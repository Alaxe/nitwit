#include "context.h"

#include <iostream>
#include <cassert>


FunctionData GlobalContext::parse_func_declaration(const Line &l) const {
	if ((l.tokens.size() < 3) || (l.tokens.size() % 2 == 0)) {
		std::cerr << "Invalid number of tokens for ";
		std::cerr << "func def on line ";
		std::cerr << l.lineInd << "\n";

		assert(false);
	}

	const Token &retTok = l.tokens[1];
	const Token &nameTok = l.tokens[2];

	assert(retTok.type == TokenType::Identifier);
	assert(nameTok.type == TokenType::Identifier);

	const TypeT *retType = get_type(retTok.s);
	if (retType == nullptr) {
		std::cerr << "Invalid function type " << retTok.s << "\n";
	}
	assert(retType != nullptr);
	assert(l.tokens.size() % 2 == 1);

	std::vector<FunctionData::ArgT> args;
	for (uint32_t i = 3;i < l.tokens.size();i += 2) {
		const Token &argTypeTok = l.tokens[i];
		const Token &argNameTok = l.tokens[i + 1];

		assert(argTypeTok.type == TokenType::Identifier);
		assert(argNameTok.type == TokenType::Identifier);
		const TypeT* argType = get_type(argTypeTok.s);

		assert(argType);
		assert(argType->is_declarable());
		args.push_back({*argType, argNameTok.s});
	}

	return FunctionData(*retType, nameTok.s, std::move(args));
}


VarData GlobalContext::parse_var_declaration(const Line &l) const {
	if (l.tokens.size() != 3) {
		std::cerr << "Invalid number of args for var ";
		std::cerr << "def on line " << l.lineInd;
		std::cerr << "\n";
		assert(false);
	}
	const Token &typeTok = l.tokens[1];
	const Token &nameTok = l.tokens[2];

	assert(typeTok.type == TokenType::Identifier);
	assert(nameTok.type == TokenType::Identifier);

	const TypeT *retType = get_type(typeTok.s);
	assert(retType);
	assert(retType->is_declarable());

	return VarData(*retType, nameTok.s);
}

std::string GlobalContext::parse_struct_declaration(const Line &l) const {
	assert(l.tokens.size() == 2);
	assert(l.tokens[1].type == TokenType::Identifier);
	return l.tokens[1].s;
}


StructData GlobalContext::parse_struct_definition(
	Line::ConstIt &begin,
	Line::ConstIt end
) const {
	StructData s;
	s.name = begin->tokens[1].s;
	for (begin++;(begin->indent > 0) && begin != end;begin++) {
		const Line &l = *begin;

		assert(l.tokens.size() == 3);
		assert(l.tokens[0].type == TokenType::VarDef);
		assert(l.tokens[1].type == TokenType::Identifier);
		assert(l.tokens[2].type == TokenType::Identifier);
		assert(s.members.find(l.tokens[2].s) == s.members.end());

		const TypeT *type = get_type(l.tokens[1].s);
		assert(type);
		assert(type->is_declarable());

		s.members[l.tokens[2].s] = type;
	}
	return s;
}

void GlobalContext::add_primitive_types() {
	for (const PrimitiveType &i : PrimitiveType::types) {
		types[i.name] = TypeT::UPtr(new PrimitiveType(i));
	}
	TypeT::UPtr voidT(new VoidType());
	types[voidT->get_name()] = std::move(voidT);
}


GlobalContext::GlobalContext() {
	add_primitive_types();
}
GlobalContext::GlobalContext(Line::ConstIt begin, Line::ConstIt end):
	GlobalContext()
{
	parse(begin, end);
}

void GlobalContext::parse(Line::ConstIt begin, Line::ConstIt end) {
	for (auto i = begin;i != end;i++) {
		if (i->indent > 0) {
			continue;
		}
		if (i->tokens[0].type == TokenType::StructDef) {
			assert(i->tokens.size() == 2);
			assert(i->tokens[1].type == TokenType::Identifier);
			declare_struct(i->tokens[1].s);
		}
	}
	auto i = begin;
	while (i != end) {
		if (i->indent > 0) {
			i++;
			continue;
		}
		const TokenType &tt = i->tokens[0].type;
		if (tt == TokenType::FuncDef) {
			declare_function(std::move(
				parse_func_declaration(*i++)
			));
		} else if (tt == TokenType::VarDef) {
			declare_variable(std::move(
				parse_var_declaration(*i++)
			));
		} else if (tt == TokenType::StructDef) {
			define_struct(std::move(
				parse_struct_definition(i, end)
			));
		} else {
			std::cerr << "invalid top level construct" << std::endl;
			assert(false);
		}
	}
}



void GlobalContext::declare_function(FunctionData func) {
	assert(get_function(func.name) == nullptr);
	std::string funcName = func.name;
	functions.emplace(
		std::move(funcName),
		new FunctionData(std::move(func))
	);
}

void GlobalContext::declare_variable(VarData var) {
	std::string name = var.name;
	assert(get_variable(name) == nullptr);

	variables.emplace(std::move(name), new VarData(std::move(var)));
}



void GlobalContext::declare_struct(std::string name) {
	assert(structs.find(name) == structs.end());
	auto it = structs.emplace(name, new StructData()).first;

	auto p = StructType::make_pair(*it->second.get());
	types.emplace("~" + name, std::move(p.second));
	types.emplace(std::move(name), std::move(p.first));
}

void GlobalContext::define_struct(StructData structData) {
	auto it = structs.find(structData.name);
	*it->second = std::move(structData);
}

const FunctionData* GlobalContext::get_function(const std::string &name) const {
	auto it = functions.find(name);

	if (it != functions.end()) {
		return it->second.get();
	} else {
		return nullptr;
	}
}
const VarData* GlobalContext::get_variable(const std::string &name) const {
	auto it = variables.find(name);

	if (it != variables.end()) {
		return it->second.get();
	} else {
		return nullptr;
	}
}

const TypeT* GlobalContext::get_type(const std::string &name, uint32_t i)
const {
	assert(name.size() > i);
	auto it = types.find(name);
	if (it != types.end()) {
		return it->second.get();
	}
	bool weak;
	if (name[i] == '#') {
		weak = false;
	} else if (
		(name.size() > i + 1)
		&& (name[i] == '~')
		&& (name[i + 1] == '#')
	) {
		weak = true;
	} else {
		assert(false);
	}
	const TypeT *elT = get_type(name, i + 1 + weak);
	assert(elT != nullptr);
	assert(elT->is_declarable());
	auto p = ArrayType::make_pair(*elT);
	auto nameP = std::make_pair(p.first->get_name(), p.second->get_name());

	auto s = types.emplace(std::move(nameP.first), std::move(p.first));
	auto w = types.emplace(std::move(nameP.second), std::move(p.second));

	return (weak ? w : s).first->second.get();
}
const TypeT* GlobalContext::get_null_type() const {
	return &nullType;
}

void GlobalContext::generate_c(std::ostream &out) const {
	for (const auto &i : types) {
		i.second->c_declare_type(out);
	}
	for (const auto &i : types) {
		i.second->c_define_type(out);
	}
	for (const auto &i : functions) {
		i.second->generate_c(out);
		out << ";\n";
	}

	for (const auto &i : variables) {
		i.second->c_declaration(out);
	}
}

Context::Block::Block(uint32_t id): id(id) {}

Context::Context(const GlobalContext &gc, const FunctionData &functionData):
	nextBlockId(0), gc(gc), functionData(functionData) {}


uint32_t Context::get_block_id() const {
	return blockSt.top().id;
}
void Context::start_block() {
	blockSt.emplace(nextBlockId++);
}
std::vector<VarData::UPtr> Context::end_block() {
	std::vector<VarData::UPtr> ans;
	for (const std::string &s : blockSt.top().vars) {
		ans.push_back(std::move(varSt[s].top()));
	}
	blockSt.pop();
	return ans;
}

void Context::declare_variable(std::string name, const TypeT &type) {
	assert(!blockSt.empty()); // this shouldn't trigger
	Block &bl = blockSt.top();
	assert(bl.vars.find(name) == bl.vars.end());

	varSt[name].emplace(new VarData(type, name));
	bl.vars.insert(std::move(name));
}


const VarData* Context::get_variable(const std::string &name) const {
	auto stIt = varSt.find(name);
	if (stIt == varSt.end()) {
		return gc.get_variable(name);
	}
	const auto &curVarSt = stIt->second;
	if (curVarSt.empty()) {
		return gc.get_variable(name);
	} else {
		return curVarSt.top().get();
	}
}
const FunctionData* Context::get_function(const std::string &name) const {
	return gc.get_function(name);
}
const TypeT* Context::get_type(const std::string &name) const {
	return gc.get_type(name);
}
const TypeT* Context::get_null_type() const {
	return gc.get_null_type();
}
