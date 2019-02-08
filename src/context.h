#pragma once

#include <memory>
#include <ostream>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "function-data.h"
#include "lexer.h"
#include "type-t.h"
#include "var-data.h"

class GlobalContext {
private:
	std::unordered_map<std::string, FunctionData::UPtr> functions;
	std::unordered_map<std::string, VarData::UPtr> variables;

	std::unordered_map<std::string, StructData::UPtr> structs;
	//get_type creates array types, which are addded here
	mutable std::unordered_map<std::string, TypeT::UPtr> types;
	NullType nullType;

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
	const TypeT* get_type(std::string name) const;
	const TypeT* get_null_type() const;

	void generate_c(std::ostream &out) const;
};

class Context {
private:
	class Block {
	public:
		uint32_t id;
		std::unordered_set<std::string> vars;

		Block(uint32_t id);
	};

	std::stack<Block> blockSt;
	std::unordered_map<std::string, std::stack<VarData::UPtr>> varSt;

	uint32_t nextBlockId;
public:
	const GlobalContext &gc;
	const FunctionData &functionData;

	Context(const GlobalContext &gc, const FunctionData &functionData);

	void start_block();
	std::vector<VarData> end_block();
	uint32_t get_block_id() const;

	void declare_variable(std::string name, const TypeT &type);

	const FunctionData* get_function(const std::string &name) const;
	const VarData* get_variable(const std::string &name) const;
	const TypeT* get_type(const std::string &name) const;
	const TypeT* get_null_type() const;

	static void c_end_label(std::ostream &out, uint32_t id);
	static void c_return_label(std::ostream &out, uint32_t id);
};
