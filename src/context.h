#pragma once

#include <ostream>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>

#include "function-t.h"
#include "lexer.h"
#include "type-t.h"

class VarData {
public:
	TypeT type;
	std::string name;
	VarData();
	VarData(const TypeT &type, const std::string &name);

	void generate_c(std::ostream &out) const;
};

class GlobalContext {
private:
	static std::string get_c_name(const std::string &name);

	std::unordered_map<std::string, FunctionT> functions;
	std::unordered_map<std::string, VarData> variables;

	void parse_func_declaration(const Line &l);
	void parse_var_declaration(const Line &l);

public:
	GlobalContext();
	GlobalContext(const std::vector<Line> &lines);

	void declare_function(const FunctionT &func);
	void declare_variable(const std::string &name, const TypeT &type);

	const FunctionT* get_function(const std::string &name) const;
	const VarData* get_variable(const std::string &name) const;

	void generate_c(std::ostream &out) const;
};

class Context {
private:
	class VarInst {
	public:
		VarData var;
		uint32_t indent;

		VarInst();
		VarInst(const VarData &var, const uint32_t &indent);
	};
	class VarStack {
	public:
		uint32_t nextId;
		std::stack<VarInst> st;
		VarStack();
	};

	static std::string get_c_name(
		const std::string &name,
		const uint32_t& id
	);

	std::stack<std::pair<uint32_t, std::string>> jointSt;
	std::unordered_map<std::string, VarStack> variables;
	std::vector<VarData> declarations;

	uint32_t curIndent;
	const GlobalContext &gc;
public:


	Context(const GlobalContext &gc);
	const FunctionT* get_function(const std::string &name) const;

	void declare_variable(const std::string &name, const TypeT &type);
	void update_indent(uint32_t indent);
	const VarData* get_variable(const std::string &name) const;

	std::vector<VarData> get_declarations();
};
