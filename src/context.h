#pragma once

#include <ostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "function-t.h"
#include "lexer.h"
#include "type-t.h"

class GlobalContext {
private:
	std::unordered_map<std::string, FunctionT> functions;
	std::unordered_map<std::string, TypeT> variables;

public:
	GlobalContext();
	GlobalContext(const std::vector<Line> &lines);

	void declare_function(const std::string &name, const FunctionT &proto);
	void declare_variable(const std::string &name, const TypeT &type);

	const FunctionT* get_function(const std::string &name) const;
	const TypeT* get_variable(const std::string &name) const;

	void generate_c(std::ostream &out) const;
};

class Context {
private:
	std::unordered_map<std::string, TypeT> variables;
	const GlobalContext &gc;

public:
	Context(const GlobalContext &gc);
	void declare_variable(const std::string &name, const TypeT &type);

	const FunctionT* get_function(const std::string &name) const;
	const TypeT* get_variable(const std::string &name) const;

	void generate_c(std::ostream &out) const;
};
