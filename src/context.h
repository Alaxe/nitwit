#pragma once

#include <ostream>
#include <stack>
#include <string>
#include <unordered_map>
#include <memory>
#include <vector>

#include "function-data.h"
#include "lexer.h"
#include "type-t.h"

class VarData {
public:
	typedef std::unique_ptr<VarData> UPtr;

	const TypeT &type;
	std::string name;
	VarData(const TypeT &type, std::string name);

	std::ostream& generate_c(std::ostream &out) const;
};

class GlobalContext {
private:
	static std::string get_var_c_name(const std::string &name);

	std::unordered_map<std::string, FunctionData::UPtr> functions;
	std::unordered_map<std::string, VarData::UPtr> variables;

	std::unordered_map<std::string, StructData::UPtr> structs;
	//get_type creates array types, which are addded here
	mutable std::unordered_map<std::string, TypeT::UPtr> types;

	void add_primitive_types();

	FunctionData parse_func_declaration(const Line &l) const;
	VarData parse_var_declaration(const Line &l) const;
	std::string parse_struct_declaration(const Line &l) const;

	StructData parse_struct_definition(
		Line::ConstIt &begin,
		Line::ConstIt end
	) const;

public:
	GlobalContext();
	GlobalContext(Line::ConstIt begin, Line::ConstIt end);

	void parse(Line::ConstIt begin, Line::ConstIt end);

	void declare_function(FunctionData func);
	void declare_variable(VarData var);
	void declare_struct(std::string name);
	void define_struct(StructData structData);

	const FunctionData* get_function(const std::string &name) const;
	const VarData* get_variable(const std::string &name) const;
	const TypeT* get_type(const std::string &name) const;

	void generate_c(std::ostream &out) const;
};

class Context {
private:
	class VarInst {
	public:
		std::unique_ptr<VarData> var;
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
public:
	const GlobalContext &gc;

	Context(const GlobalContext &gc);

	void declare_variable(const std::string &name, const TypeT &type);
	void update_indent(uint32_t indent);

	const FunctionData* get_function(const std::string &name) const;
	const VarData* get_variable(const std::string &name) const;
	const TypeT* get_type(const std::string &name) const;

	std::vector<VarData> get_declarations();
};
